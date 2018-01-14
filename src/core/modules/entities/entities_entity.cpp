/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

// ============================================================================
// >> INCLUDES
// ============================================================================
// Source.Python
#include "utilities/conversions.h"
#include "entities_entity.h"
#include "entities_props.h"
#include "entities_factories.h"
#include "entities_datamaps.h"
#include "modules/physics/physics.h"
#include ENGINE_INCLUDE_PATH(entities_datamaps_wrap.h)


// ============================================================================
// >> External variables
// ============================================================================
extern IMDLCache *modelcache;


// ============================================================================
// >> CBaseEntityWrapper
// ============================================================================
boost::shared_ptr<CBaseEntityWrapper> CBaseEntityWrapper::__init__(unsigned int uiEntityIndex)
{
	return CBaseEntityWrapper::wrap(ExcBaseEntityFromIndex(uiEntityIndex));
}

boost::shared_ptr<CBaseEntityWrapper> CBaseEntityWrapper::wrap(CBaseEntity* pEntity)
{
	return boost::shared_ptr<CBaseEntityWrapper>(
		(CBaseEntityWrapper *) pEntity,
		&NeverDeleteDeleter<CBaseEntityWrapper *>
	);
}

CBaseEntity* CBaseEntityWrapper::create(const char* name)
{
#ifdef ENGINE_CSGO
	void *pItem = servertools->CreateItemEntityByName(name);
	if (pItem)
		return (CBaseEntity *)pItem;
#endif
	IServerNetworkable* pEntity = get_factory(name)->Create(name);
	if (!pEntity)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to create entity with class name '%s'.", name)

	return pEntity->GetBaseEntity();
}

CBaseEntity* CBaseEntityWrapper::find(const char* name)
{
	CBaseEntity* pEntity = (CBaseEntity *) servertools->FirstEntity();
	while (pEntity)
	{
		if (strcmp(IServerUnknownExt::GetClassname(pEntity), name) == 0)
			return pEntity;

		pEntity = (CBaseEntity*) servertools->NextEntity(pEntity);
	}
	return NULL;
}

CBaseEntity* CBaseEntityWrapper::find_or_create(const char* name)
{
	CBaseEntity* entity = find(name);
	if (!entity)
		entity = create(name);

	return entity;
}

IEntityFactory* CBaseEntityWrapper::get_factory(const char* name)
{
	IEntityFactory* pFactory = GetEntityFactoryDictionary()->FindFactory(name);
	if (!pFactory)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "'%s' is not a valid entity class name.", name)

	return pFactory;
}

IEntityFactory* CBaseEntityWrapper::get_factory()
{
	IServerNetworkable* pNetworkable = GetNetworkable();
	if (!pNetworkable)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to get the IServerNetworkable pointer.");

	return get_factory(pNetworkable->GetClassName());
}

void CBaseEntityWrapper::remove()
{
	static inputfunc_t pInputKillFunc = NULL;
	if (!pInputKillFunc)
	{
		datamap_t* datamap = GetDataDescMap();
		if (!datamap)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve the datamap.")

		typedescription_t *pTypeDesc = DataMapSharedExt::find(datamap, "InputKill");
		if (!pTypeDesc)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find the 'InputKill' descriptor.");

		inputfunc_t pInputFunc = pTypeDesc->inputFunc;
		if (!pInputFunc)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Input function is NULL.");

		pInputKillFunc = pInputFunc;
	}

	CBaseEntity *pEntity = GetThis();
	inputdata_t data;
	variant_t value;

	data.pActivator = pEntity;
	data.pCaller = pEntity;
	data.value = value;
	data.nOutputID = 0;

	(pEntity->*pInputKillFunc)(data);
}

int CBaseEntityWrapper::get_size()
{
	return get_factory()->GetEntitySize();
}

void CBaseEntityWrapper::spawn()
{
	servertools->DispatchSpawn(GetThis());
}

int CBaseEntityWrapper::FindDataMapOffset(const char* name)
{
	datamap_t* datamap = GetDataDescMap();
	if (!datamap)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve the datamap.")

	int offset = DataMapSharedExt::find_offset(datamap, name);
	if (offset == -1)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find property '%s'.", name)

	return offset;
}

int CBaseEntityWrapper::FindNetworkPropertyOffset(const char* name)
{
	ServerClass* pServerClass = GetServerClass();
	if (!pServerClass)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve the server class.")

	int offset = SendTableSharedExt::find_offset(pServerClass->m_pTable, name);
	if (offset == -1)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find property '%s'.", name)

	return offset;
}

CBaseEntity* CBaseEntityWrapper::GetThis()
{
	return (CBaseEntity *) this;
}

bool CBaseEntityWrapper::operator==(object other)
{
	CBaseEntity* pOther = NULL;
	try {
		pOther = extract<CBaseEntity*>(other);
	}
	catch(...) {
		PyErr_Clear();
		return false;
	}
	return GetThis() == pOther;
}

void CBaseEntityWrapper::GetKeyValueStringRaw(const char* szName, char* szOut, int iLength)
{
	if (!servertools->GetKeyValue(GetThis(), szName, szOut, iLength))
		BOOST_RAISE_EXCEPTION(PyExc_NameError, "\"%s\" is not a valid KeyValue for entity class \"%s\".",
			szName, GetDataDescMap()->dataClassName);
}

str CBaseEntityWrapper::GetKeyValueString(const char* szName)
{
	char szResult[MAX_KEY_VALUE_LENGTH];
	GetKeyValueStringRaw(szName, szResult, MAX_KEY_VALUE_LENGTH);

	// TODO: Don't hardcode this
	// Fix for field name "model". I think a string_t object is copied to szResult.
	// https://developer.valvesoftware.com/wiki/Team_train_watcher
	if (strcmp(szName, "model") == 0 || strcmp(szName, "train") == 0)
		return str(*(char **) szResult);

	return str(szResult);
}

long CBaseEntityWrapper::GetKeyValueInt(const char* szName)
{
	char szResult[128];
	GetKeyValueStringRaw(szName, szResult, 128);
		
	long iResult;
	if (!sputils::UTIL_StringToLong(&iResult, szResult))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be an integer: '%s'.", szResult);

	return iResult;
}

double CBaseEntityWrapper::GetKeyValueFloat(const char* szName)
{
	char szResult[128];
	GetKeyValueStringRaw(szName, szResult, 128);
		
	double dResult;
	if (!sputils::UTIL_StringToDouble(&dResult, szResult))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a float: '%s'.", szResult);

	return dResult;
}

Vector CBaseEntityWrapper::GetKeyValueVector(const char* szName)
{
	char szResult[128];
	GetKeyValueStringRaw(szName, szResult, 128);

	float fResult[3];
	if (!sputils::UTIL_StringToFloatArray(fResult, 3, szResult))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a vector: '%s'.", szResult);

	return Vector(fResult[0], fResult[1], fResult[2]);
}

QAngle CBaseEntityWrapper::GetKeyValueQAngle(const char* szName)
{
	char szResult[128];
	GetKeyValueStringRaw(szName, szResult, 128);

	float fResult[3];
	if (!sputils::UTIL_StringToFloatArray(fResult, 3, szResult))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be an angle: '%s'.", szResult);

	return QAngle(fResult[0], fResult[1], fResult[2]);
}

bool CBaseEntityWrapper::GetKeyValueBool(const char* szName)
{
	char szResult[3];
	GetKeyValueStringRaw(szName, szResult, 3);
	if (szResult[1] != '\0')
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a boolean: '%s'.", szResult);

	if (szResult[0] == '1')
		return true;
	else if (szResult[0] == '0')
		return false;
	else
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid boolean value: '%c'.", szResult[0]);

	return false; // to fix a warning.
}

Color CBaseEntityWrapper::GetKeyValueColor(const char* szName)
{
	char szResult[128];
	GetKeyValueStringRaw(szName, szResult, 128);

	int iResult[4];
	if (!sputils::UTIL_StringToIntArray(iResult, 4, szResult))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a color: '%s'.", szResult);

	return Color(iResult[0], iResult[1], iResult[2], iResult[3]);
}

void CBaseEntityWrapper::SetKeyValueColor(const char* szName, Color& color)
{
	char string[16];
	Q_snprintf(string, sizeof(string), "%i %i %i %i", color.r(), color.g(), color.b(), color.a());
	SetKeyValue(szName, string);
}

void CBaseEntityWrapper::SetKeyValueQAngle(const char* szName, QAngle& angles)
{
	SetKeyValue(szName, (Vector&) angles);
}

edict_t* CBaseEntityWrapper::GetEdict()
{
	return ExcEdictFromBaseEntity(GetThis());
}
	
unsigned int CBaseEntityWrapper::GetIndex()
{
	return ExcIndexFromBaseEntity(GetThis());
}
	
CPointer CBaseEntityWrapper::GetPointer()
{
	return ExcPointerFromBaseEntity(GetThis());
}
	
unsigned int CBaseEntityWrapper::GetIntHandle()
{
	return ExcIntHandleFromBaseEntity(GetThis());
}

bool CBaseEntityWrapper::IsPlayer()
{
	if (!IServerUnknownExt::IsNetworked(GetThis()))
		return false;

	unsigned int iEntityIndex;
	if (!IndexFromBaseEntity(GetThis(), iEntityIndex)) 
		return false;

	return iEntityIndex > WORLD_ENTITY_INDEX && iEntityIndex <= (unsigned int) gpGlobals->maxClients;
}

IPhysicsObjectWrapper* CBaseEntityWrapper::GetPhysicsObject()
{
	return Wrap<IPhysicsObjectWrapper>(GetDatamapProperty<IPhysicsObject*>("m_pPhysicsObject"));
}


Vector CBaseEntityWrapper::GetOrigin()
{
	static int offset = FindDataMapOffset("m_vecOrigin");
	return GetDatamapPropertyByOffset<Vector>(offset);
}

void CBaseEntityWrapper::SetOrigin(Vector& vec)
{
	static int offset = FindDataMapOffset("m_vecOrigin");
	SetDatamapPropertyByOffset<Vector>(offset, vec);
}


Vector CBaseEntityWrapper::GetMaxs()
{
	static int offset = FindDataMapOffset("m_Collision.m_vecMaxs");
	return GetDatamapPropertyByOffset<Vector>(offset);
}

void CBaseEntityWrapper::SetMaxs(Vector& vec)
{
	static int offset = FindDataMapOffset("m_Collision.m_vecMaxs");
	SetDatamapPropertyByOffset<Vector>(offset, vec);
}


Vector CBaseEntityWrapper::GetMins()
{
	static int offset = FindDataMapOffset("m_Collision.m_vecMins");
	return GetDatamapPropertyByOffset<Vector>(offset);
}

void CBaseEntityWrapper::SetMins(Vector& vec)
{
	static int offset = FindDataMapOffset("m_Collision.m_vecMins");
	SetDatamapPropertyByOffset<Vector>(offset, vec);
}


SolidType_t CBaseEntityWrapper::GetSolidType()
{
	static int offset = FindDataMapOffset("m_Collision.m_nSolidType");
	return (SolidType_t) GetDatamapPropertyByOffset<unsigned char>(offset);
}

void CBaseEntityWrapper::SetSolidType(SolidType_t type)
{
	static int offset = FindDataMapOffset("m_Collision.m_nSolidType");
	SetDatamapPropertyByOffset<unsigned char>(offset, type);
}


SolidFlags_t CBaseEntityWrapper::GetSolidFlags()
{
	static int offset = FindDataMapOffset("m_Collision.m_usSolidFlags");
	return (SolidFlags_t) GetDatamapPropertyByOffset<unsigned short>(offset);
}

void CBaseEntityWrapper::SetSolidFlags(SolidFlags_t flags)
{
	static int offset = FindDataMapOffset("m_Collision.m_usSolidFlags");
	SetDatamapPropertyByOffset<unsigned short>(offset, flags);
}


Collision_Group_t CBaseEntityWrapper::GetCollisionGroup()
{
	static int offset = FindDataMapOffset("m_CollisionGroup");
	return GetDatamapPropertyByOffset<Collision_Group_t>(offset);
}

void CBaseEntityWrapper::SetCollisionGroup(Collision_Group_t group)
{
	static int offset = FindDataMapOffset("m_CollisionGroup");
	SetDatamapPropertyByOffset<Collision_Group_t>(offset, group);
}


Color CBaseEntityWrapper::GetRenderColor()
{
	static int offset = FindDataMapOffset("m_clrRender");
	return GetDatamapPropertyByOffset<Color>(offset);
}

void CBaseEntityWrapper::SetRenderColor(Color& color)
{
	static int offset = FindDataMapOffset("m_clrRender");
	SetDatamapPropertyByOffset<Color>(offset, color);
}


float CBaseEntityWrapper::GetElasticity()
{
	static int offset = FindDataMapOffset("m_flElasticity");
	return GetDatamapPropertyByOffset<float>(offset);
}

void CBaseEntityWrapper::SetElasticity(float elasticity)
{
	static int offset = FindDataMapOffset("m_flElasticity");
	SetDatamapPropertyByOffset<float>(offset, elasticity);
}


int CBaseEntityWrapper::GetGroundEntity()
{
	static int offset = FindDataMapOffset("m_hGroundEntity");
	return GetDatamapPropertyByOffset<int>(offset);
}

void CBaseEntityWrapper::SetGroundEntity(int entity)
{
	static int offset = FindDataMapOffset("m_hGroundEntity");
	SetDatamapPropertyByOffset<int>(offset, entity);
}


int CBaseEntityWrapper::GetTeamIndex()
{
	static int offset = FindDataMapOffset("m_iTeamNum");
	return GetDatamapPropertyByOffset<int>(offset);
}

void CBaseEntityWrapper::SetTeamIndex(int team)
{
	static int offset = FindDataMapOffset("m_iTeamNum");
	SetDatamapPropertyByOffset<int>(offset, team);
}


RenderFx_t CBaseEntityWrapper::GetRenderFx()
{
	static int offset = FindDataMapOffset("m_nRenderFX");
	return (RenderFx_t) GetDatamapPropertyByOffset<unsigned char>(offset);
}

void CBaseEntityWrapper::SetRenderFx(RenderFx_t fx)
{
	static int offset = FindDataMapOffset("m_nRenderFX");
	SetDatamapPropertyByOffset<unsigned char>(offset, fx);
}


RenderMode_t CBaseEntityWrapper::GetRenderMode()
{
	static int offset = FindDataMapOffset("m_nRenderMode");
	return (RenderMode_t) GetDatamapPropertyByOffset<unsigned char>(offset);
}

void CBaseEntityWrapper::SetRenderMode(RenderMode_t mode)
{
	static int offset = FindDataMapOffset("m_nRenderMode");
	SetDatamapPropertyByOffset<unsigned char>(offset, mode);
}


MoveType_t CBaseEntityWrapper::GetMoveType()
{
	static int offset = FindDataMapOffset("m_MoveType");
	return (MoveType_t) GetDatamapPropertyByOffset<unsigned char>(offset);
}

void CBaseEntityWrapper::SetMoveType(MoveType_t type)
{
	static int offset = FindDataMapOffset("m_MoveType");
	SetDatamapPropertyByOffset<unsigned char>(offset, type);
}


int CBaseEntityWrapper::GetParentHandle()
{
	static int offset = FindDataMapOffset("m_pParent");
	return GetDatamapPropertyByOffset<int>(offset);
}

void CBaseEntityWrapper::SetParentHandle(int entity)
{
	static int offset = FindDataMapOffset("m_pParent");
	SetDatamapPropertyByOffset<int>(offset, entity);
}


QAngle CBaseEntityWrapper::GetAngles()
{
	return GetKeyValueQAngle("angles");
}

void CBaseEntityWrapper::SetAngles(QAngle& angles)
{
	SetKeyValueQAngle("angles", angles);
}


str CBaseEntityWrapper::GetTargetName()
{
	return GetKeyValueString("targetname");
}

void CBaseEntityWrapper::SetTargetName(const char* name)
{
	SetKeyValue("targetname", name);
}



int CBaseEntityWrapper::GetOwnerHandle()
{
	static int offset_fallback = FindDataMapOffset("m_hOwnerEntity");
	int offset = offset_fallback;

	try {
		// TODO:
		// Might use something that doesn't throw Python exceptions.
		// That will speed things up again.
		offset = FindDataMapOffset("m_hOwner");
	} catch( ... ) {
		PyErr_Clear();
	}

	return GetDatamapPropertyByOffset<int>(offset);
}

void CBaseEntityWrapper::SetOwnerHandle(int entity)
{
	static int offset_fallback = FindDataMapOffset("m_hOwnerEntity");
	int offset = offset_fallback;

	try {
		offset = FindDataMapOffset("m_hOwner");
	} catch( ... ) {
		PyErr_Clear();
	}

	return SetDatamapPropertyByOffset<int>(offset, entity);
}
