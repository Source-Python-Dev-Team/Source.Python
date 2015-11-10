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
int IntHandleFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	int iEntityHandle = (int) INVALID_EHANDLE_INDEX;

	if (hBaseHandle.IsValid())
		iEntityHandle = hBaseHandle.ToInt();

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given index.
//-----------------------------------------------------------------------------
int IntHandleFromIndex( int iEntityIndex, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromIndex(iEntityIndex));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given index (%i).", iEntityIndex);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Edict instance.
//-----------------------------------------------------------------------------
int IntHandleFromEdict( edict_t *pEdict, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromEdict(pEdict));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Edict instance (%x).", pEdict);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given BaseEntity instance.
//-----------------------------------------------------------------------------
int IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromBaseHandle(BaseHandleFromBaseEntity(pBaseEntity));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given BaseEntity instance (%x).", pBaseEntity);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given Pointer instance.
//-----------------------------------------------------------------------------
int IntHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromBaseEntity(BaseEntityFromPointer(pEntityPointer));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given Pointer instance (%x).", pEntityPointer->m_ulAddr);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given UserID.
//-----------------------------------------------------------------------------
int IntHandleFromUserid( int iUserID, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromUserid(iUserID));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given UserID (%i).", iUserID);

	return iEntityHandle;
}


//-----------------------------------------------------------------------------
// Returns an IntHandle from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
int IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	int iEntityHandle = IntHandleFromEdict(EdictFromPlayerInfo(pPlayerInfo));

	if (iEntityHandle == (int) INVALID_EHANDLE_INDEX && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get an IntHandle from the given PlayerInfo instance (%x).", pPlayerInfo);

	return iEntityHandle;
}
