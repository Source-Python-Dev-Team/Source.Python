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
bool EdictFromIndex( unsigned int iEntityIndex, edict_t*& output )
{
	if (iEntityIndex >= (unsigned int) gpGlobals->maxEntities)
		return false;

	edict_t* pEdict;
#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS) || defined(ENGINE_GMOD)
	pEdict = engine->PEntityOfEntIndex(iEntityIndex);
#else
	pEdict = (edict_t *)(gpGlobals->pEdicts + iEntityIndex);
#endif
	if (!pEdict || pEdict->IsFree() || !pEdict->GetUnknown())
		return false;

	output = pEdict;
	return true;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given UserID.
//-----------------------------------------------------------------------------
bool EdictFromUserid( unsigned int iUserID, edict_t*& output )
{
	for (int iCurrentIndex = 1; iCurrentIndex <= gpGlobals->maxClients; iCurrentIndex++)
	{
		edict_t* pEdict;
		if (!EdictFromIndex(iCurrentIndex, pEdict))
			continue;

		if (engine->GetPlayerUserId(pEdict) == iUserID) {
			output = pEdict;
			return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, edict_t*& output )
{
	if (!pPlayerInfo)
		return false;

	return EdictFromUserid(pPlayerInfo->GetUserID(), output);
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
bool EdictFromBaseEntity( CBaseEntity *pBaseEntity, edict_t*& output )
{
	if (!pBaseEntity)
		return false;
	
	IServerNetworkable *pServerNetworkable = pBaseEntity->GetNetworkable();
	if (!pServerNetworkable)
		return false;

	edict_t* pEdict = pServerNetworkable->GetEdict();
	if (!pEdict || pEdict->IsFree())
		return false;

	output = pEdict;
	return true;
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
bool EdictFromBaseHandle( CBaseHandle hBaseHandle, edict_t*& output )
{
	unsigned int iIndex;
	if (!IndexFromBaseHandle(hBaseHandle, iIndex))
		return false;

	return EdictFromIndex(iIndex, output);
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given IntHandle.
//-----------------------------------------------------------------------------
bool EdictFromIntHandle( unsigned int iEntityHandle, edict_t*& output )
{
	unsigned int iIndex;
	if (!IndexFromIntHandle(iEntityHandle, iIndex))
		return false;

	return EdictFromIndex(iIndex, output);
}


//-----------------------------------------------------------------------------
// Returns an Edict instance from the given Pointer instance.
//-----------------------------------------------------------------------------
bool EdictFromPointer( CPointer *pEntityPointer, edict_t*& output )
{
	if (!IsValidNetworkedEntityPointer(pEntityPointer))
		return false;

	return EdictFromBaseEntity((CBaseEntity *) pEntityPointer->m_ulAddr, output);
}
