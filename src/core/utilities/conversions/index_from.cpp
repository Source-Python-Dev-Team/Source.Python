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
// Returns an index from the given Edict instance.
//-----------------------------------------------------------------------------
int IndexFromEdict( edict_t *pEdict, bool bRaiseException )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pEdict && !pEdict->IsFree())
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS)
		iEntityIndex = engine->IndexOfEdict(pEdict);
#else
		iEntityIndex = pEdict - gpGlobals->pEdicts;
#endif

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Edict instance (%x).", pEdict);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given BaseEntity instance.
//-----------------------------------------------------------------------------
int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pBaseEntity)
	{
		IServerUnknown *pServerUnknown = (IServerUnknown *)pBaseEntity;
		if (pServerUnknown)
		{
			IServerNetworkable *pServerNetworkable = pServerUnknown->GetNetworkable();
			if (pServerNetworkable)
				iEntityIndex = IndexFromEdict(pServerNetworkable->GetEdict());
		}
	}

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseEntity instance (%x).", pBaseEntity);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given Pointer instance.
//-----------------------------------------------------------------------------
int IndexFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (pEntityPointer && pEntityPointer->IsValid())
		iEntityIndex = IndexFromBaseEntity((CBaseEntity *)pEntityPointer->m_ulAddr);

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given BaseHandle instance.
//-----------------------------------------------------------------------------
int IndexFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	int iEntityIndex = INVALID_ENTITY_INDEX;

	if (hBaseHandle.IsValid())
		iEntityIndex = hBaseHandle.GetEntryIndex();

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given IntHandle.
//-----------------------------------------------------------------------------
int IndexFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	int iEntityIndex = (int) INVALID_ENTITY_INDEX;

	if (iEntityHandle != (int) INVALID_EHANDLE_INDEX)
	{
		CBaseHandle hBaseHandle(iEntityHandle);
		const CBaseHandle hTestHandle = BaseHandleFromEdict(EdictFromIndex(hBaseHandle.GetEntryIndex()));
		if (hTestHandle.IsValid() || (hBaseHandle.GetSerialNumber() == hTestHandle.GetSerialNumber()))
			iEntityIndex = hBaseHandle.GetEntryIndex();
	}

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given IntHandle (%i).", iEntityHandle);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index from the given UserID.
//-----------------------------------------------------------------------------
int IndexFromUserid( int iUserID, bool bRaiseException )
{
	int iEntityIndex = IndexFromEdict(EdictFromUserid(iUserID));

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given UserID (%i).", iUserID);

	return iEntityIndex;
}


//-----------------------------------------------------------------------------
// Returns an index instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
int IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	int iEntityIndex = IndexFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityIndex == INVALID_ENTITY_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an index from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iEntityIndex;
}
