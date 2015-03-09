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

#ifndef _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_H
#define _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_H

//-----------------------------------------------------------------------------
// CSWeaponID wrapper enumerator.
//-----------------------------------------------------------------------------
enum WeaponID
{
	WEAPON_NONE = 0,
	WEAPON_P228,
	WEAPON_GLOCK,
	WEAPON_SCOUT,
	WEAPON_HEGRENADE,
	WEAPON_XM1014,
	WEAPON_C4,
	WEAPON_MAC10,
	WEAPON_AUG,
	WEAPON_SMOKEGRENADE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_UMP45,
	WEAPON_SG550,
	WEAPON_GALIL,
	WEAPON_FAMAS,
	WEAPON_USP,
	WEAPON_AWP,
	WEAPON_MP5NAVY,
	WEAPON_M249,
	WEAPON_M3,
	WEAPON_M4A1,
	WEAPON_TMP,
	WEAPON_G3SG1,
	WEAPON_FLASHBANG,
	WEAPON_DEAGLE,
	WEAPON_SG552,
	WEAPON_AK47,
	WEAPON_KNIFE,
	WEAPON_P90,
	WEAPON_SHIELD,
	WEAPON_KEVLAR,
	WEAPON_ASSAULTSUIT,
	WEAPON_NIGHTVISION
};


//-----------------------------------------------------------------------------
// CSWeaponSlot wrapper enumerator.
//-----------------------------------------------------------------------------
enum WeaponSlot
{
	SLOT_PRIMARY = 0,
	SLOT_SECONDARY,
	SLOT_KNIFE,
	SLOT_GRENADE,
	SLOT_C4,
};


//-----------------------------------------------------------------------------
// Muzzle flash styles wrapper enumerator.
//-----------------------------------------------------------------------------
enum MuzzleFlashStyle
{
	MUZZLE_NONE = -1,
	MUZZLE_NORMAL = 0,
	MUZZLE_SPECIAL = 1
};


//-----------------------------------------------------------------------------
// CSWeaponType wrapper enumerator.
//-----------------------------------------------------------------------------
enum WeaponType
{
	WEAPON_TYPE_KNIFE = 0,
	WEAPON_TYPE_PISTOL,
	WEAPON_TYPE_SMG,
	WEAPON_TYPE_RIFLE,
	WEAPON_TYPE_SHOTGUN,
	WEAPON_TYPE_SNIPER,
	WEAPON_TYPE_MACHINEGUN,
	WEAPON_TYPE_EXPLOSIVE,
	WEAPON_TYPE_GRENADE
};


#endif // _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_H
