/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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
#include "basehandle.h"
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "modules/memory/memory_tools.h"
#include "utilities/baseentity.h"
#include "utilities/wrap_macros.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;


//-----------------------------------------------------------------------------
// Invalid results.
//-----------------------------------------------------------------------------
#define INVALID_ENTITY_INDEX -1
#define INVALID_PLAYER_USERID -1
#define INVALID_ENTITY_INTHANDLE INVALID_EHANDLE_INDEX


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
edict_t *EdictFromIndex( unsigned int uiEntityIndex, bool bRaiseException );
edict_t *EdictFromUserid( unsigned int uiUserID, bool bRaiseException );
edict_t *EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException );
edict_t *EdictFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException );
CBaseHandle BaseHandleFromEdict( edict_t *pEdict, bool bRaiseException );
int IntHandleFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException );
CPointer *PointerFromEdict( edict_t *pEdict, bool bRaiseException );
int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException );
IPlayerInfo *PlayerInfoFromIndex( unsigned int uiEntityIndex, bool bRaiseException );
IPlayerInfo *PlayerInfoFromPointer( CPointer *pEntityPointer, bool bRaiseException );
int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException );


//-----------------------------------------------------------------------------
// TConversion functions.
//-----------------------------------------------------------------------------
inline int IndexFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pEdict && !pEdict->IsFree())
#ifdef ENGINE_ORANGEBOX
		uiEntityIndex = (unsigned int)engine->IndexOfEdict(pEdict);
#else
		uiEntityIndex = (unsigned int)(pEdict - gpGlobals->pEdicts);
#endif

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Edict instance (%x).", pEdict);

	return uiEntityIndex;
}


inline int IndexFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (hBaseHandle.IsValid())
		uiEntityIndex = hBaseHandle.GetEntryIndex();

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseHandle instance.");

	return uiEntityIndex;
}


inline int IndexFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (iEntityHandle != INVALID_ENTITY_INTHANDLE)
	{
		CBaseHandle hBaseHandle(iEntityHandle);
		const CBaseHandle hTestHandle = BaseHandleFromEdict(EdictFromIndex(hBaseHandle.GetEntryIndex(), false), false);
		if (hTestHandle.IsValid() || (hBaseHandle.GetSerialNumber() == hTestHandle.GetSerialNumber()))
			uiEntityIndex = hBaseHandle.GetEntryIndex();
	}

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given IntHandle (%i).", iEntityHandle);

	return uiEntityIndex;
}


inline int IndexFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pEntityPointer && pEntityPointer->IsValid())
		uiEntityIndex = IndexFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr, false);

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Pointer instance.");

	return uiEntityIndex;
}


inline int IndexFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = IndexFromEdict(EdictFromUserid(uiUserID, false));

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given UseriID (%i).", uiUserID);

	return uiEntityIndex;
}


inline int IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = IndexFromEdict(EdictFromPlayerInfo(pPlayerInfo, false));

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given PlayerInfo instance.");

	return uiEntityIndex;
}


inline int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	unsigned int uiEntityIndex = INVALID_ENTITY_INDEX;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				uiEntityIndex = IndexFromEdict(pServerNetworkable->GetEdict());
		}
	}

	if (uiEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseEntity instance.");

	return uiEntityIndex;
}


inline edict_t *EdictFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (uiEntityIndex < (unsigned int)gpGlobals->maxEntities)
	{
#ifdef ENGINE_ORANGEBOX
		edict_t *pTempEdict = engine->PEntityOfEntIndex(uiEntityIndex);
#else
		edict_t *pTempEdict = (edict_t *) (gpGlobals->pEdicts + uiEntityIndex);
#endif
		if (pTempEdict && !pTempEdict->IsFree())
			pEdict = pTempEdict;
	}

	if ((!pEdict || !pEdict->GetUnknown()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given index (%i).", uiEntityIndex);

	return pEdict;
}


inline edict_t *EdictFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	edict_t *pEdict = EdictFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseHandle instance.");

	return pEdict;
}


inline edict_t *EdictFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	edict_t *pEdict = EdictFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given IntHandle (%i).", iEntityHandle);

	return pEdict;
}


inline edict_t *EdictFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pEntityPointer && pEntityPointer->IsValid())
		pEdict = EdictFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr, false);

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given Pointer instance.");

	return pEdict;
}


inline edict_t *EdictFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	for (int iCurrentIndex = 1; iCurrentIndex <= gpGlobals->maxClients; iCurrentIndex++)
	{
		edict_t *pCurrentEdict = EdictFromIndex(iCurrentIndex);
		if (engine->GetPlayerUserId(pCurrentEdict) == uiUserID)
			pEdict = pCurrentEdict;
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given UseriID (%i).", uiUserID);

	return pEdict;
}


inline edict_t *EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pPlayerInfo)
	{
		pEdict = EdictFromUserid(pPlayerInfo->GetUserID());
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given PlayerInfo instance.");

	return pEdict;
}


inline edict_t *EdictFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false )
{
	edict_t *pEdict = NULL;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				pEdict = pServerNetworkable->GetEdict();
		}
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseEntity instance.");

	return pEdict;
}


inline CBaseHandle BaseHandleFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromIndex(uiEntityIndex), false);

	if (hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given index (%i).", uiEntityIndex);

	return hBaseHandle;
}


inline CBaseHandle BaseHandleFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerNetworkable *pServerNetworkable = pEdict->GetNetworkable();
		if (pServerNetworkable)
		{
			IHandleEntity *pHandleEntity = pServerNetworkable->GetEntityHandle();
			if (pHandleEntity)
				hBaseHandle = pHandleEntity->GetRefEHandle();
		}
	}

	if (!hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Edict instance.");

	return hBaseHandle;
}


inline CBaseHandle BaseHandleFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = CBaseHandle(iEntityHandle);

	if (!hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given IntHandle (%i).", iEntityHandle);

	return hBaseHandle;
}


inline CBaseHandle BaseHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromPointer(pEntityPointer));

	if (!hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given Pointer instance.");

	return hBaseHandle;
}


inline CBaseHandle BaseHandleFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromUserid(uiUserID));

	if (!hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given UseriID (%i).", uiUserID);

	return hBaseHandle;
}


inline CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	CBaseHandle hBaseHandle = BaseHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!hBaseHandle.IsValid() && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a BaseHandle instance from the given PlayerInfo instance.");

	return hBaseHandle;
}


inline int IntHandleFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromIndex(uiEntityIndex), false);

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given index (%i).", uiEntityIndex);

	return iEntityHandle;
}


inline int IntHandleFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromEdict(pEdict), false);

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Edict instance.");

	return iEntityHandle;
}


inline int IntHandleFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	int iEntityHandle = INVALID_ENTITY_INTHANDLE;

	if (hBaseHandle.IsValid())
		iEntityHandle = hBaseHandle.ToInt();

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseHandle instance.");

	return iEntityHandle;
}


inline int IntHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromPointer(pEntityPointer));

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Pointer instance.");

	return iEntityHandle;
}


inline int IntHandleFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	int iEntityHandle =  IntHandleFromEdict(EdictFromUserid(uiUserID));

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given UserID (%i).", uiUserID);

	return iEntityHandle;
}

inline int IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityHandle == INVALID_ENTITY_INTHANDLE && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given PlayerInfo instance.");

	return iEntityHandle;
}


inline CPointer *PointerFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromEdict(EdictFromIndex(uiEntityIndex), false);

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given index (%i).", uiEntityIndex);

	return pEntityPointer;
}


inline CPointer *PointerFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	CPointer *pEntityPointer = NULL;

	if (pEdict && !pEdict->IsFree())
	{
		IServerUnknown *pServerUnknown = pEdict->GetUnknown();
		if (pServerUnknown)
		{
			CBaseEntity *pBaseEntity = pServerUnknown->GetBaseEntity();
			if (pBaseEntity)
				pEntityPointer = new CPointer((unsigned long)pBaseEntity);
		}
	}

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given Edict instance.");

	return pEntityPointer;
}


inline CPointer *PointerFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromEdict(EdictFromBaseHandle(hBaseHandle));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseHandle instance.");

	return pEntityPointer;
}


inline CPointer *PointerFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromEdict(EdictFromIntHandle(iEntityHandle));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given IntHandle (%i).", iEntityHandle);

	return pEntityPointer;
}


inline CPointer *PointerFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromEdict(EdictFromUserid(uiUserID));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given UserID (%i).", uiUserID);

	return pEntityPointer;
}


inline CPointer *PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	CPointer *pEntityPointer = PointerFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (!(pEntityPointer && pEntityPointer->IsValid()) && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given PlayerInfo instance.");

	return pEntityPointer;
}


inline int UseridFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromPlayerInfo(PlayerInfoFromIndex(uiEntityIndex, false), false);

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given index (%i).", uiEntityIndex);

	return uiUserID;
}


inline int UseridFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromIndex(IndexFromEdict(pEdict));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given Edict instance.");

	return uiUserID;
}


inline int UseridFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseHandle instance.");

	return uiUserID;
}


inline int UseridFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	unsigned int uiUserID = INVALID_PLAYER_USERID;

	if (iEntityHandle != INVALID_ENTITY_INTHANDLE)
		uiUserID = UseridFromBaseHandle(BaseHandleFromIntHandle(iEntityHandle));

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given IntHandle (%i).", iEntityHandle);

	return uiUserID;
}


inline int UseridFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	unsigned int uiUserID = UseridFromPlayerInfo(PlayerInfoFromPointer(pEntityPointer, false), false);

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given CPointer instance.");

	return uiUserID;
}


inline int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false )
{
	unsigned int uiUserID = INVALID_PLAYER_USERID;

	if (pPlayerInfo)
		uiUserID = pPlayerInfo->GetUserID();

	if (uiUserID == INVALID_PLAYER_USERID && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given PlayerInfo instance.");

	return uiUserID;
}


inline IPlayerInfo *PlayerInfoFromIndex( unsigned int uiEntityIndex, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = NULL;

	if (uiEntityIndex > 0 || (int)uiEntityIndex < gpGlobals->maxClients)
	{
		edict_t *pEdict = EdictFromIndex(uiEntityIndex);
		if (pEdict)
			pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);
	}

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given index (%i).", uiEntityIndex);

	return pPlayerInfo;
}


inline IPlayerInfo *PlayerInfoFromEdict( edict_t *pEdict, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = NULL;

	if (pEdict && !pEdict->IsFree())
		pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Edict instance.");

	return pPlayerInfo;
}


inline IPlayerInfo *PlayerInfoFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(hBaseHandle.GetEntryIndex());

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given BaseHandle instance.");

	return pPlayerInfo;
}


inline IPlayerInfo *PlayerInfoFromIntHandle( int iEntityHandle, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given IntHandle (%i).", iEntityHandle);

	return pPlayerInfo;
}


inline IPlayerInfo *PlayerInfoFromPointer( CPointer *pEntityPointer, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromIndex(IndexFromPointer(pEntityPointer));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given Pointer instance.");

	return pPlayerInfo;
}


inline IPlayerInfo *PlayerInfoFromUserid( unsigned int uiUserID, bool bRaiseException = false )
{
	IPlayerInfo *pPlayerInfo = PlayerInfoFromEdict(EdictFromUserid(uiUserID));

	if (!pPlayerInfo && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a PlayerInfo instance from the given UserID (%i).", uiUserID);

	return pPlayerInfo;
}


//-----------------------------------------------------------------------------
// Helper template to avoid some redundant typing...
//-----------------------------------------------------------------------------
#define EXPORT_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name, ...) \
	def(extract<const char *>(str(XSTRINGIFY(to_name##_from_##from_name)).lower().ptr()), \
		&to_name##From##from_name, \
		XSTRINGIFY(Returns the to_name (of type #to_type) from the given from_name (of type #from_type).), \
		(XSTRINGIFY(from_name), arg("raise_exception")=true), \
		##__VA_ARGS__ \
	)


#endif // _CONVERSIONS_H
