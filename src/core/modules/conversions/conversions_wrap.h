/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
#ifndef _CONVERSIONS_WRAP_H
#define _CONVERSIONS_WRAP_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "edict.h"
#include "basehandle.h"
#include "public/game/server/iplayerinfo.h"
#include "utility/sp_util.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int index_from_edict( edict_t* edict )
{
	return IndexOfEdict(edict);
}

unsigned int index_from_basehandle( CBaseHandle* basehandle )
{
	return basehandle->GetEntryIndex();
}

unsigned int index_from_inthandle( int inthandle )
{
	return IndexOfIntHandle(inthandle);
}

unsigned int index_from_pointer( object pointer )
{
	return IndexOfPointer(pointer);
}

unsigned int index_from_userid( unsigned int userid )
{
	return IndexOfUserid(userid);
}

unsigned int index_from_playerinfo( IPlayerInfo* playerinfo )
{
	return IndexOfEdict(EdictOfPlayer(playerinfo));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
edict_t* edict_from_index( unsigned int index )
{
	return PEntityOfEntIndex(index);
}

edict_t* edict_from_basehandle( CBaseHandle* basehandle )
{
	return PEntityOfEntIndex(basehandle->GetEntryIndex());
}

edict_t* edict_from_inthandle( int inthandle )
{
	return PEntityOfEntIndex(IndexOfIntHandle(inthandle));
}

edict_t* edict_from_pointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer));
}

edict_t* edict_from_userid( unsigned int userid )
{
	return EdictOfPlayer(PlayerOfUserid(userid));
}

edict_t* edict_from_playerinfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const CBaseHandle basehandle_from_index( unsigned int index )
{
	return PEntityOfEntIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

const CBaseHandle basehandle_from_edict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

const CBaseHandle basehandle_from_inthandle( int inthandle )
{
	return CBaseHandle(inthandle);
}

const CBaseHandle basehandle_from_pointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

const CBaseHandle basehandle_from_userid( unsigned int userid )
{
	return EdictOfUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

const CBaseHandle basehandle_from_playerinfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int inthandle_from_index( unsigned int index )
{
	return PEntityOfEntIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

int inthandle_from_edict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

int inthandle_from_basehandle( CBaseHandle* basehandle )
{
	return basehandle->ToInt();
}

int inthandle_from_pointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

int inthandle_from_userid( unsigned int userid )
{
	return EdictOfUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

int inthandle_from_playerinfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned long pointer_from_index( unsigned int index )
{
	return (unsigned long)(PEntityOfEntIndex(index)->GetNetworkable()->GetBaseEntity());
}

unsigned long pointer_from_edict( edict_t* edict )
{
	return (unsigned long)(edict->GetNetworkable()->GetBaseEntity());
}

unsigned long pointer_from_basehandle( CBaseHandle* basehandle )
{
	return (unsigned long)(PEntityOfEntIndex(basehandle->GetEntryIndex())->GetNetworkable()->GetBaseEntity());
}

unsigned long pointer_from_inthandle( int inthandle )
{
	return (unsigned long)(PEntityOfEntIndex(IndexOfIntHandle(inthandle))->GetNetworkable()->GetBaseEntity());
}

unsigned long pointer_from_userid( unsigned int userid )
{
	return (unsigned long)(EdictOfPlayer(PlayerOfUserid(userid))->GetNetworkable()->GetBaseEntity());
}

unsigned long pointer_from_playerinfo( IPlayerInfo* playerinfo )
{
	return (unsigned long)(EdictOfPlayer(playerinfo)->GetNetworkable()->GetBaseEntity());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int userid_from_index( unsigned int index )
{
	return PlayerOfIndex(index)->GetUserID();
}

unsigned int userid_from_edict( edict_t* edict )
{
	return PlayerOfIndex(IndexOfEdict(edict))->GetUserID();
}

unsigned int userid_from_basehandle( CBaseHandle* basehandle )
{
	return PlayerOfIndex(basehandle->GetEntryIndex())->GetUserID();
}

unsigned int userid_from_inthandle( int inthandle )
{
	return PlayerOfIndex(IndexOfIntHandle(inthandle))->GetUserID();
}

unsigned int userid_from_pointer( object pointer )
{
	return PlayerOfIndex(IndexOfPointer(pointer))->GetUserID();
}

unsigned int userid_from_playerinfo( IPlayerInfo* playerinfo )
{
	return playerinfo->GetUserID();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
IPlayerInfo* playerinfo_from_index( unsigned int index )
{
	return PlayerOfIndex(index);
}

IPlayerInfo* playerinfo_from_edict( edict_t* edict )
{
	return PlayerOfIndex(IndexOfEdict(edict));
}

IPlayerInfo* playerinfo_from_basehandle( CBaseHandle* basehandle )
{
	return PlayerOfIndex(basehandle->GetEntryIndex());
}

IPlayerInfo* playerinfo_from_inthandle( int inthandle )
{
	return PlayerOfIndex(IndexOfIntHandle(inthandle));
}

IPlayerInfo* playerinfo_from_pointer( object pointer )
{
	return PlayerOfIndex(IndexOfPointer(pointer));
}

IPlayerInfo* playerinfo_from_userid( unsigned int userid )
{
	return PlayerOfUserid(userid);
}

#endif // _CONVERSIONS_WRAP_H
