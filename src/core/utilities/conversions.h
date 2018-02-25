/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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

#ifndef _CONVERSIONS_H
#define _CONVERSIONS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"
#include "basehandle.h"
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "utilities/baseentity.h"
#include "toolframework/itoolentity.h"
#include "sp_util.h"

BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(CBaseEntity)

//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;
extern IServerTools *servertools;

class CBaseEntityWrapper;

//-----------------------------------------------------------------------------
// Constants.
//-----------------------------------------------------------------------------
#define WORLD_ENTITY_INDEX 0


//-----------------------------------------------------------------------------
// Invalid results.
//-----------------------------------------------------------------------------
#define INVALID_ENTITY_INDEX -1
#define INVALID_PLAYER_USERID -1


//-----------------------------------------------------------------------------
// Helper macro to avoid some redundant typing...
//-----------------------------------------------------------------------------
#define CREATE_EXC_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name) \
	inline to_type Exc##to_name##From##from_name(from_type from) { \
		to_type result; \
		if (!to_name##From##from_name(from, result)) { \
			str str_from = str(from); \
			const char* str_value = extract<const char*>(str_from); \
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, XSTRINGIFY(Conversion from #from_name (%s) to #to_name failed.), str_value); \
		} \
		return result; \
	}

#define CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(to_type, to_name, from_type, from_name) \
	inline to_type Exc##to_name##From##from_name(from_type from) { \
		to_type result; \
		if (!to_name##From##from_name(from, result)) { \
			str str_from = str( \
				boost::shared_ptr<CBaseEntityWrapper>( \
					(CBaseEntityWrapper *) from, \
					&NeverDeleteDeleter<CBaseEntityWrapper *>)); \
			const char* str_value = extract<const char*>(str_from); \
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, XSTRINGIFY(Conversion from #from_name (%s) to #to_name failed.), str_value); \
		} \
		return result; \
	}

#define EXPORT_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name, ...) \
	def(extract<const char *>(str(XSTRINGIFY(to_name##_from_##from_name)).lower().ptr()), \
		&Exc##to_name##From##from_name, \
		XSTRINGIFY(Return the to_name (of type `#to_type`) from the given from_name (of type `#from_type`).), \
		args(XSTRINGIFY(from_name)), \
		##__VA_ARGS__ \
	)


//-----------------------------------------------------------------------------
// EdictFrom* declarations
//-----------------------------------------------------------------------------
bool EdictFromIndex( unsigned int iEntityIndex, edict_t*& output );
bool EdictFromUserid( unsigned int iUserID, edict_t*& output );
bool EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, edict_t*& output );
bool EdictFromBaseEntity( CBaseEntity *pBaseEntity, edict_t*& output );
bool EdictFromBaseHandle( CBaseHandle hBaseHandle, edict_t*& output );
bool EdictFromIntHandle( unsigned int iEntityHandle, edict_t*& output );
bool EdictFromPointer( CPointer *pEntityPointer, edict_t*& output );

CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(edict_t *, Edict, CBaseEntity *, BaseEntity);
CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(edict_t *, Edict, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// IntHandleFrom* declarations
//-----------------------------------------------------------------------------
bool IntHandleFromBaseHandle( CBaseHandle hBaseHandle, unsigned int& output );
bool IntHandleFromIndex( unsigned int iEntityIndex, unsigned int& output );
bool IntHandleFromEdict( edict_t *pEdict, unsigned int& output );
bool IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, unsigned int& output );
bool IntHandleFromPointer( CPointer *pEntityPointer, unsigned int& output );
bool IntHandleFromUserid( unsigned int iUserID, unsigned int& output );
bool IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, unsigned int& output );

CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(unsigned int, IntHandle, CBaseEntity *, BaseEntity);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, IntHandle, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// BaseEntityFrom* declarations
//-----------------------------------------------------------------------------
bool BaseEntityFromEdict( edict_t *pEdict, CBaseEntity*& output );
bool BaseEntityFromPointer( CPointer *pEntityPointer, CBaseEntity*& output );
bool BaseEntityFromIndex( unsigned int iEntityIndex, CBaseEntity*& output );
bool BaseEntityFromIntHandle( unsigned int iEntityHandle, CBaseEntity*& output );
bool BaseEntityFromBaseHandle( CBaseHandle hBaseHandle, CBaseEntity*& output );
bool BaseEntityFromUserid( unsigned int iUserID, CBaseEntity*& output );
bool BaseEntityFromPlayerInfo( IPlayerInfo *pPlayerInfo, CBaseEntity*& output );

CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// UseridFrom* declarations
//-----------------------------------------------------------------------------
bool UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, unsigned int& output );
bool UseridFromIndex( unsigned int iEntityIndex, unsigned int& output );
bool UseridFromEdict( edict_t *pEdict, unsigned int& output );
bool UseridFromBaseHandle( CBaseHandle hBaseHandle, unsigned int& output );
bool UseridFromIntHandle( unsigned int iEntityHandle, unsigned int& output );
bool UseridFromBaseEntity( CBaseEntity *pBaseEntity, unsigned int& output );
bool UseridFromPointer( CPointer *pEntityPointer, unsigned int& output );

CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Userid, IPlayerInfo *, PlayerInfo);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(unsigned int, Userid, CBaseEntity *, BaseEntity);


//-----------------------------------------------------------------------------
// PlayerInfoFrom* declarations
//-----------------------------------------------------------------------------
bool PlayerInfoFromIndex( unsigned int iEntityIndex, IPlayerInfo*& output );
bool PlayerInfoFromBaseEntity( CBaseEntity *pBaseEntity, IPlayerInfo*& output );
bool PlayerInfoFromPointer( CPointer *pEntityPointer, IPlayerInfo*& output );
bool PlayerInfoFromEdict( edict_t *pEdict, IPlayerInfo*& output );
bool PlayerInfoFromBaseHandle( CBaseHandle hBaseHandle, IPlayerInfo*& output );
bool PlayerInfoFromIntHandle( unsigned int iEntityHandle, IPlayerInfo*& output );
bool PlayerInfoFromUserid( unsigned int iUserID, IPlayerInfo*& output );

CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(IPlayerInfo *, PlayerInfo, CBaseEntity *, BaseEntity);


//-----------------------------------------------------------------------------
// IndexFrom* declarations
//-----------------------------------------------------------------------------
bool IndexFromEdict( edict_t *pEdict, unsigned int& output );
bool IndexFromBaseEntity( CBaseEntity *pBaseEntity, unsigned int& output );
bool IndexFromPointer( CPointer *pEntityPointer, unsigned int& output );
bool IndexFromBaseHandle( CBaseHandle hBaseHandle, unsigned int& output );
bool IndexFromIntHandle( unsigned int iEntityHandle, unsigned int& output );
bool IndexFromUserid( unsigned int iUserID, unsigned int& output );
bool IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, unsigned int& output );
bool IndexFromName( const char *szName, unsigned int& output );
bool IndexFromSteamID( const char* szSteamID, unsigned int& output );
bool IndexFromUniqueID( const char* szUniqueID, unsigned int& output );

CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(unsigned int, Index, CBaseEntity *, BaseEntity);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, IPlayerInfo *, PlayerInfo);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, const char *, Name);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, const char *, SteamID);
CREATE_EXC_CONVERSION_FUNCTION(unsigned int, Index, const char *, UniqueID);


//-----------------------------------------------------------------------------
// BaseHandleFrom* declarations
//-----------------------------------------------------------------------------
bool BaseHandleFromIndex( unsigned int iEntityIndex, CBaseHandle& output );
bool BaseHandleFromIntHandle( unsigned int iEntityHandle, CBaseHandle& output );
bool BaseHandleFromBaseEntity( CBaseEntity *pBaseEntity, CBaseHandle& output );
bool BaseHandleFromPointer( CPointer *pEntityPointer, CBaseHandle& output );
bool BaseHandleFromUserid( unsigned int iUserID, CBaseHandle& output );
bool BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, CBaseHandle& output );
bool BaseHandleFromEdict( edict_t *pEdict, CBaseHandle& output );

CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, CPointer *, Pointer);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(CBaseHandle, BaseHandle, CBaseEntity *, BaseEntity);
CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// PointerFrom* declarations
//-----------------------------------------------------------------------------
bool PointerFromIndex( unsigned int iEntityIndex, CPointer& output );
bool PointerFromBaseEntity( CBaseEntity *pBaseEntity, CPointer& output );
bool PointerFromBaseHandle( CBaseHandle hBaseHandle, CPointer& output );
bool PointerFromIntHandle( unsigned int iEntityHandle, CPointer& output );
bool PointerFromUserid( unsigned int iUserID, CPointer& output );
bool PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, CPointer& output );
bool PointerFromEdict( edict_t *pEdict, CPointer& output );

CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, unsigned int, Index);
CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, edict_t *, Edict);
CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, CBaseHandle, BaseHandle);
CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, unsigned int, IntHandle);
CREATE_EXC_CONVERSION_FUNCTION_BASE_ENTITY(CPointer, Pointer, CBaseEntity *, BaseEntity);
CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, unsigned int, Userid);
CREATE_EXC_CONVERSION_FUNCTION(CPointer, Pointer, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// AddressFrom* declarations
//-----------------------------------------------------------------------------
bool AddressFromPlayerInfo2( IPlayerInfo* pInfo, const char*& output );
bool AddressFromPlayerInfo( IPlayerInfo* pInfo, str& output );

CREATE_EXC_CONVERSION_FUNCTION(str, Address, IPlayerInfo *, PlayerInfo);


//-----------------------------------------------------------------------------
// UniqueIDFrom* declarations
//-----------------------------------------------------------------------------
#define UNIQUE_ID_SIZE 128

bool UniqueIDFromPlayerInfo2( IPlayerInfo* pInfo, char*& output );
bool UniqueIDFromPlayerInfo( IPlayerInfo* pInfo, str& output );
bool UniqueIDFromIndex( unsigned int iIndex, str& output );

CREATE_EXC_CONVERSION_FUNCTION(str, UniqueID, IPlayerInfo *, PlayerInfo);
CREATE_EXC_CONVERSION_FUNCTION(str, UniqueID, unsigned int, Index);


//-----------------------------------------------------------------------------
// Helper functions
//-----------------------------------------------------------------------------
inline bool IsValidBaseEntityPointer(void* ptr)
{
	if (!ptr)
		return false;

	CBaseEntity* pEntity = (CBaseEntity*) servertools->FirstEntity();
	while (pEntity) {
		if (pEntity == ptr)
			return true;

		pEntity = (CBaseEntity*) servertools->NextEntity(pEntity);
	}
	return false;
}

inline bool IsValidBaseEntityPointer(CPointer* pPtr)
{
	return pPtr && IsValidBaseEntityPointer((void*) pPtr->m_ulAddr);
}

inline bool IsValidNetworkedEntityPointer(void* ptr)
{
	if (!ptr)
		return false;

    for (int i=0; i < gpGlobals->maxEntities; ++i)
    {
		edict_t* pEdict = NULL;
		if (!EdictFromIndex(i, pEdict))
			continue;

		if (pEdict->GetUnknown()->GetBaseEntity() == ptr)
			return true;
	}
	return false;
}

inline bool IsValidNetworkedEntityPointer(CPointer* pPtr)
{
	return pPtr && IsValidNetworkedEntityPointer((void*) pPtr->m_ulAddr);
}

inline bool IsValidPlayerPointer(void* ptr)
{
	if (!ptr)
		return false;

	for (int i=1; i <= gpGlobals->maxClients; ++i)
	{
		edict_t* pEdict = NULL;
		if (!EdictFromIndex(i, pEdict))
			continue;

		if (pEdict->GetUnknown()->GetBaseEntity() == ptr)
			return true;
	}
	return false;
}

inline bool IsValidPlayerPointer(CPointer* pPtr)
{
	return pPtr && IsValidPlayerPointer((void*) pPtr->m_ulAddr);
}

#endif // _CONVERSIONS_H
