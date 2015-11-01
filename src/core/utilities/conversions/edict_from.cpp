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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "../conversions.h"


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given index.
//-----------------------------------------------------------------------------
edict_t *EdictFromIndex( int iEntityIndex, bool bRaiseException )
{
	edict_t *pEdict = NULL;

	if (iEntityIndex > INVALID_ENTITY_INDEX && iEntityIndex < gpGlobals->maxEntities)
	{
		edict_t *pTempEdict;
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		pTempEdict = engine->PEntityOfEntIndex(iEntityIndex);
#else
		pTempEdict = (edict_t *)(gpGlobals->pEdicts + iEntityIndex);
#endif
		if (pTempEdict && !pTempEdict->IsFree() && pTempEdict->GetUnknown())
			pEdict = pTempEdict;
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given index (%i).", iEntityIndex);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given UserID.
//-----------------------------------------------------------------------------
edict_t *EdictFromUserid( int iUserID, bool bRaiseException )
{
	edict_t *pEdict = NULL;

	if (iUserID > INVALID_PLAYER_USERID)
	{
		for (int iCurrentIndex = 1; iCurrentIndex <= gpGlobals->maxClients; iCurrentIndex++)
		{
			edict_t *pCurrentEdict = EdictFromIndex(iCurrentIndex);
			if (engine->GetPlayerUserId(pCurrentEdict) == iUserID)
				pEdict = pCurrentEdict;
		}
	}

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given UserID (%i).", iUserID);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
edict_t *EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	edict_t *pEdict = NULL;

	if (pPlayerInfo)
		pEdict = EdictFromUserid(pPlayerInfo->GetUserID());

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
edict_t *EdictFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
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
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseEntity instance (%x).", pBaseEntity);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
edict_t *EdictFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	edict_t *pEdict = EdictFromIndex(IndexFromBaseHandle(hBaseHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given IntHandle.
//-----------------------------------------------------------------------------
edict_t *EdictFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	edict_t *pEdict = EdictFromIndex(IndexFromIntHandle(iEntityHandle));

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given IntHandle (%i).", iEntityHandle);

	return pEdict;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given Pointer instance.
//-----------------------------------------------------------------------------
edict_t *EdictFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	edict_t *pEdict = NULL;

	if (pEntityPointer && pEntityPointer->IsValid())
		pEdict = EdictFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr);

	if (!pEdict && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an Edict instance from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return pEdict;
}
