/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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

#ifndef _CONSTANTS_CONST_WRAP_H
#define _CONSTANTS_CONST_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "const.h"


//-----------------------------------------------------------------------------
// PlayerFlags wrapper.
//-----------------------------------------------------------------------------
enum PlayerFlags
{
	ONGROUND = FL_ONGROUND,
	DUCKING = FL_DUCKING,
	WATERJUMP = FL_WATERJUMP,
	ONTRAIN = FL_ONTRAIN,
	INRAIN = FL_INRAIN,
	FROZEN = FL_FROZEN,
	ATCONTROLS = FL_ATCONTROLS,
	CLIENT = FL_CLIENT,
	FAKECLIENT = FL_FAKECLIENT,
	INWATER = FL_INWATER
};


//-----------------------------------------------------------------------------
// EntityFlags wrapper.
//-----------------------------------------------------------------------------
enum EntityFlags
{
	FLY = FL_FLY,
	SWIM = FL_SWIM,
	CONVEYOR = FL_SWIM,
	NPC = FL_NPC,
	GODMODE = FL_GODMODE,
	NOTARGET = FL_NOTARGET,
	AIMTARGET = FL_AIMTARGET,
	PARTIALGROUND = FL_PARTIALGROUND,
	STATICPROP = FL_STATICPROP,
	GRAPHED = FL_GRAPHED,
	GRENADE = FL_GRENADE,
	STEPMOVEMENT = FL_STEPMOVEMENT,
	DONTTOUCH = FL_DONTTOUCH,
	BASEVELOCITY = FL_BASEVELOCITY,
	WORLDBRUSH = FL_WORLDBRUSH,
	OBJECT = FL_OBJECT,
	KILLME = FL_KILLME,
	ONFIRE = FL_ONFIRE,
	DISSOLVING = FL_DISSOLVING,
	TRANSRAGDOLL = FL_TRANSRAGDOLL,
	UNBLOCKABLE_BY_PLAYER = FL_UNBLOCKABLE_BY_PLAYER
};


//-----------------------------------------------------------------------------
// LifeStates wrapper.
//-----------------------------------------------------------------------------
enum LifeStates
{
	ALIVE = LIFE_ALIVE,
	DYING = LIFE_DYING,
	DEAD = LIFE_DEAD,
	RESPAWNABLE = LIFE_RESPAWNABLE,
	DISCARD_BODY = LIFE_DISCARDBODY
};


//-----------------------------------------------------------------------------
// EntityEffects wrapper.
//-----------------------------------------------------------------------------
enum EntityEffects
{
	BONEMERGE = EF_BONEMERGE,
	BRIGHTLIGHT = EF_BRIGHTLIGHT,
	DIMLIGHT = EF_DIMLIGHT,
	NOINTERP = EF_NOINTERP,
	NOSHADOW = EF_NOSHADOW,
	NODRAW = EF_NODRAW,
	NORECEIVESHADOW = EF_NORECEIVESHADOW,
	BONEMERGE_FASTCULL = EF_BONEMERGE_FASTCULL,
	ITEM_BLINK = EF_ITEM_BLINK,
	PARENT_ANIMATES = EF_PARENT_ANIMATES
};


#endif // _CONSTANTS_CONST_WRAP_H
