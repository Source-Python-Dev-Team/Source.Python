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
// Returns an IntHandle from the given BaseHandle instance.
//-----------------------------------------------------------------------------
bool IntHandleFromBaseHandle( CBaseHandle hBaseHandle, unsigned int& output )
{
	if (!hBaseHandle.IsValid())
		return false;

	unsigned int iEntityHandle = hBaseHandle.ToInt();
	if (!iEntityHandle)
		return false;

	output = iEntityHandle;
	return true;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given index.
//-----------------------------------------------------------------------------
bool IntHandleFromIndex( unsigned int iEntityIndex, unsigned int& output )
{
	CBaseHandle hBaseHandle;
	if (!BaseHandleFromIndex(iEntityIndex, hBaseHandle))
		return false;

	return IntHandleFromBaseHandle(hBaseHandle, output);
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Edict instance.
//-----------------------------------------------------------------------------
bool IntHandleFromEdict( edict_t *pEdict, unsigned int& output )
{
	CBaseHandle hBaseHandle;
	if (!BaseHandleFromEdict(pEdict, hBaseHandle))
		return false;

	return IntHandleFromBaseHandle(hBaseHandle, output);
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given BaseEntity instance.
//-----------------------------------------------------------------------------
bool IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, unsigned int& output )
{
	CBaseHandle hBaseHandle;
	if (!BaseHandleFromBaseEntity(pBaseEntity, hBaseHandle))
		return false;

	return IntHandleFromBaseHandle(hBaseHandle, output);
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Pointer instance.
//-----------------------------------------------------------------------------
bool IntHandleFromPointer( CPointer *pEntityPointer, unsigned int& output )
{
	if (!IsValidNetworkedEntityPointer(pEntityPointer))
		return false;

	return IntHandleFromBaseEntity((CBaseEntity*) pEntityPointer->m_ulAddr, output);
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given UserID.
//-----------------------------------------------------------------------------
bool IntHandleFromUserid( unsigned int iUserID, unsigned int& output )
{
	edict_t* pEdict;
	if (!EdictFromUserid(iUserID, pEdict))
		return false;

	return IntHandleFromEdict(pEdict, output);
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, unsigned int& output )
{
	edict_t* pEdict;
	if (!EdictFromPlayerInfo(pPlayerInfo, pEdict))
		return false;

	return IntHandleFromEdict(pEdict, output);
}
