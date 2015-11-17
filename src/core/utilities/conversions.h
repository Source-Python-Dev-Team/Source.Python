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


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;


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
// EdictFrom* declarations
//-----------------------------------------------------------------------------
edict_t* EdictFromIndex( int iEntityIndex, bool bRaiseException = false );
edict_t* EdictFromUserid( int iUserID, bool bRaiseException = false );
edict_t* EdictFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );
edict_t* EdictFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
edict_t* EdictFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
edict_t* EdictFromIntHandle( int iEntityHandle, bool bRaiseException = false );
edict_t* EdictFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// IntHandleFrom* declarations
//-----------------------------------------------------------------------------
int IntHandleFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
int IntHandleFromIndex( int iEntityIndex, bool bRaiseException = false );
int IntHandleFromEdict( edict_t *pEdict, bool bRaiseException = false );
int IntHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
int IntHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );
int IntHandleFromUserid( int iUserID, bool bRaiseException = false );
int IntHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// BaseEntityFrom* declarations
//-----------------------------------------------------------------------------
CBaseEntity* BaseEntityFromEdict( edict_t *pEdict, bool bRaiseException = false );
CBaseEntity* BaseEntityFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );
CBaseEntity* BaseEntityFromIndex( int iEntityIndex, bool bRaiseException = false );
CBaseEntity* BaseEntityFromIntHandle( int iEntityHandle, bool bRaiseException = false );
CBaseEntity* BaseEntityFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
CBaseEntity* BaseEntityFromUserid( int iUserID, bool bRaiseException = false );
CBaseEntity* BaseEntityFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// UseridFrom* declarations
//-----------------------------------------------------------------------------
int UseridFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );
int UseridFromIndex( int iEntityIndex, bool bRaiseException = false );
int UseridFromEdict( edict_t *pEdict, bool bRaiseException = false );
int UseridFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
int UseridFromIntHandle( int iEntityHandle, bool bRaiseException = false );
int UseridFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
int UseridFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// PlayerInfoFrom* declarations
//-----------------------------------------------------------------------------
IPlayerInfo* PlayerInfoFromIndex( int iEntityIndex, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromEdict( edict_t *pEdict, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromIntHandle( int iEntityHandle, bool bRaiseException = false );
IPlayerInfo* PlayerInfoFromUserid( int iUserID, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// IndexFrom* declarations
//-----------------------------------------------------------------------------
int IndexFromEdict( edict_t *pEdict, bool bRaiseException = false );
int IndexFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
int IndexFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );
int IndexFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
int IndexFromIntHandle( int iEntityHandle, bool bRaiseException = false );
int IndexFromUserid( int iUserID, bool bRaiseException = false );
int IndexFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// BaseHandleFrom* declarations
//-----------------------------------------------------------------------------
CBaseHandle BaseHandleFromIndex( int iEntityIndex, bool bRaiseException = false );
CBaseHandle BaseHandleFromIntHandle( int iEntityHandle, bool bRaiseException = false );
CBaseHandle BaseHandleFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false);
CBaseHandle BaseHandleFromPointer( CPointer *pEntityPointer, bool bRaiseException = false );
CBaseHandle BaseHandleFromUserid( int iUserID, bool bRaiseException = false );
CBaseHandle BaseHandleFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );
CBaseHandle BaseHandleFromEdict( edict_t *pEdict, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// PointerFrom* declarations
//-----------------------------------------------------------------------------
CPointer* PointerFromIndex( int iEntityIndex, bool bRaiseException = false );
CPointer* PointerFromBaseEntity( CBaseEntity *pBaseEntity, bool bRaiseException = false );
CPointer* PointerFromBaseHandle( CBaseHandle hBaseHandle, bool bRaiseException = false );
CPointer* PointerFromIntHandle( int iEntityHandle, bool bRaiseException = false );
CPointer* PointerFromUserid( int iUserID, bool bRaiseException = false );
CPointer* PointerFromPlayerInfo( IPlayerInfo *pPlayerInfo, bool bRaiseException = false );
CPointer* PointerFromEdict( edict_t *pEdict, bool bRaiseException = false );


//-----------------------------------------------------------------------------
// Helper macro to avoid some redundant typing...
//-----------------------------------------------------------------------------
#define EXPORT_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name, ...) \
	def(extract<const char *>(str(XSTRINGIFY(to_name##_from_##from_name)).lower().ptr()), \
		&to_name##From##from_name, \
		XSTRINGIFY(Return the to_name (of type `#to_type`) from the given from_name (of type `#from_type`).), \
		(XSTRINGIFY(from_name), arg("raise_exception")=true), \
		##__VA_ARGS__ \
	)


#endif // _CONVERSIONS_H
