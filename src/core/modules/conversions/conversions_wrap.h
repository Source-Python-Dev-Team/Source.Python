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
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline unsigned int IndexFromEdict( edict_t* edict )
{
	return IndexOfEdict(edict);
}

inline unsigned int IndexFromBaseHandle( CBaseHandle* basehandle )
{
	return basehandle->GetEntryIndex();
}

inline unsigned int IndexFromIntHandle( int inthandle )
{
	return IndexOfIntHandle(inthandle);
}

inline unsigned int IndexFromPointer( object pointer )
{
	return IndexOfPointer(pointer);
}

inline unsigned int IndexFromUserid( unsigned int userid )
{
	return IndexOfUserid(userid);
}

inline unsigned int IndexFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return IndexOfEdict(EdictOfPlayer(playerinfo));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline edict_t* EdictFromIndex( unsigned int index )
{
	return PEntityOfEntIndex(index);
}

inline edict_t* EdictFromBaseHandle( CBaseHandle* basehandle )
{
	return PEntityOfEntIndex(basehandle->GetEntryIndex());
}

inline edict_t* EdictFromIntHandle( int inthandle )
{
	return PEntityOfEntIndex(IndexOfIntHandle(inthandle));
}

inline edict_t* EdictFromPointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer));
}

inline edict_t* EdictFromUserid( unsigned int userid )
{
	return EdictOfPlayer(PlayerOfUserid(userid));
}

inline edict_t* EdictFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline const CBaseHandle BaseHandleFromIndex( unsigned int index )
{
	return PEntityOfEntIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromEdict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromIntHandle( int inthandle )
{
	return CBaseHandle(inthandle);
}

inline const CBaseHandle BaseHandleFromPointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromUserid( unsigned int userid )
{
	return EdictOfUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

inline const CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline int IntHandleFromIndex( unsigned int index )
{
	return PEntityOfEntIndex(index)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromEdict( edict_t* edict )
{
	return edict->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromBaseHandle( CBaseHandle* basehandle )
{
	return basehandle->ToInt();
}

inline int IntHandleFromPointer( object pointer )
{
	return PEntityOfEntIndex(IndexOfPointer(pointer))->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromUserid( unsigned int userid )
{
	return EdictOfUserid(userid)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

inline int IntHandleFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return EdictOfPlayer(playerinfo)->GetNetworkable()->GetEntityHandle()->GetRefEHandle().ToInt();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline CPointer* PointerFromIndex( unsigned int index )
{
	return new CPointer((unsigned int)(PEntityOfEntIndex(index)->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromEdict( edict_t* edict )
{
	return new CPointer((unsigned int)(edict->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromBaseHandle( CBaseHandle* basehandle )
{
	return new CPointer((unsigned int)(PEntityOfEntIndex(basehandle->GetEntryIndex())->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromIntHandle( int inthandle )
{
	return new CPointer((unsigned int)(PEntityOfEntIndex(IndexOfIntHandle(inthandle))->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromUserid( unsigned int userid )
{
	return new CPointer((unsigned int)(EdictOfPlayer(PlayerOfUserid(userid))->GetNetworkable()->GetBaseEntity()));
}

inline CPointer* PointerFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return new CPointer((unsigned int)(EdictOfPlayer(playerinfo)->GetNetworkable()->GetBaseEntity()));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline unsigned int UseridFromIndex( unsigned int index )
{
	return PlayerOfIndex(index)->GetUserID();
}

inline unsigned int UseridFromEdict( edict_t* edict )
{
	return PlayerOfIndex(IndexOfEdict(edict))->GetUserID();
}

inline unsigned int UseridFromBaseHandle( CBaseHandle* basehandle )
{
	return PlayerOfIndex(basehandle->GetEntryIndex())->GetUserID();
}

inline unsigned int UseridFromIntHandle( int inthandle )
{
	return PlayerOfIndex(IndexOfIntHandle(inthandle))->GetUserID();
}

inline unsigned int UseridFromPointer( object pointer )
{
	return PlayerOfIndex(IndexOfPointer(pointer))->GetUserID();
}

inline unsigned int UseridFromPlayerInfo( IPlayerInfo* playerinfo )
{
	return playerinfo->GetUserID();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline IPlayerInfo* PlayerInfoFromIndex( unsigned int index )
{
	return PlayerOfIndex(index);
}

inline IPlayerInfo* PlayerInfoFromEdict( edict_t* edict )
{
	return PlayerOfIndex(IndexOfEdict(edict));
}

inline IPlayerInfo* PlayerInfoFromBaseHandle( CBaseHandle* basehandle )
{
	return PlayerOfIndex(basehandle->GetEntryIndex());
}

inline IPlayerInfo* PlayerInfoFromIntHandle( int inthandle )
{
	return PlayerOfIndex(IndexOfIntHandle(inthandle));
}

inline IPlayerInfo* PlayerInfoFromPointer( object pointer )
{
	return PlayerOfIndex(IndexOfPointer(pointer));
}

inline IPlayerInfo* PlayerInfoFromUserid( unsigned int userid )
{
	return PlayerOfUserid(userid);
}

#endif // _CONVERSIONS_WRAP_H
