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
// Returns a Pointer instance from the given Edict instance.
//-----------------------------------------------------------------------------
CPointer *PointerFromEdict( edict_t *pEdict, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromEdict(pEdict);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given Edict instance (%x).", pEdict);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given index.
//-----------------------------------------------------------------------------
CPointer *PointerFromIndex( int iEntityIndex, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromIndex(iEntityIndex);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given index (%i).", iEntityIndex);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseEntity instance.
//-----------------------------------------------------------------------------
CPointer *PointerFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException )
{
	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseEntity instance (%x).", pBaseEntity);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given BaseHandle instance.
//-----------------------------------------------------------------------------
CPointer *PointerFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromBaseHandle(hBaseHandle);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given BaseHandle instance (%i).", hBaseHandle.ToInt());

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given IntHandle.
//-----------------------------------------------------------------------------
CPointer *PointerFromIntHandle( int iEntityHandle, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromIntHandle(iEntityHandle);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given IntHandle (%i).", iEntityHandle);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given UserID.
//-----------------------------------------------------------------------------
CPointer *PointerFromUserid( int iUserID, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromUserid(iUserID);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given UserID (%i).", iUserID);

	return new CPointer((unsigned long)pBaseEntity);
}


//-----------------------------------------------------------------------------
// Returns a Pointer instance from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
CPointer *PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException )
{
	CBaseEntity *pBaseEntity = BaseEntityFromPlayerInfo(pPlayerInfo);

	if (!pBaseEntity && bRaiseException)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to get a Pointer instance from the given PlayerInfo instance (%x).", pPlayerInfo);

	return new CPointer((unsigned long)pBaseEntity);
}
