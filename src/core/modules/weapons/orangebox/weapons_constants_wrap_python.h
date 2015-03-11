/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _WEAPONS_CONSTANTS_ORANGEBOX_WRAP_PYTHON_H
#define _WEAPONS_CONSTANTS_ORANGEBOX_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utility/wrap_macros.h"

#include ENGINE_BRANCH_INCLUDE_PATH(weapons_constants_wrap_python.h)


//-----------------------------------------------------------------------------
// Expose WeaponSound_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_weapon_sound(T WeaponSound)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose WeaponID.
//-----------------------------------------------------------------------------
template<class T>
void export_weapon_id(T _constants)
{
	enum_<WeaponID> WeaponID_("WeaponID");

	// CS:S specific values...
	NOT_IMPLEMENTED_VALUE(WeaponID, "NONE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "P228");
	NOT_IMPLEMENTED_VALUE(WeaponID, "GLOCK");
	NOT_IMPLEMENTED_VALUE(WeaponID, "SCOUT");
	NOT_IMPLEMENTED_VALUE(WeaponID, "HEGRENADE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "XM1014");
	NOT_IMPLEMENTED_VALUE(WeaponID, "C4");
	NOT_IMPLEMENTED_VALUE(WeaponID, "MAC10");
	NOT_IMPLEMENTED_VALUE(WeaponID, "AUG");
	NOT_IMPLEMENTED_VALUE(WeaponID, "SMOKEGRENADE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "ELITE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "FIVESEVEN");
	NOT_IMPLEMENTED_VALUE(WeaponID, "UMP45");
	NOT_IMPLEMENTED_VALUE(WeaponID, "SG550");
	NOT_IMPLEMENTED_VALUE(WeaponID, "GALIL");
	NOT_IMPLEMENTED_VALUE(WeaponID, "FAMAS");
	NOT_IMPLEMENTED_VALUE(WeaponID, "USP");
	NOT_IMPLEMENTED_VALUE(WeaponID, "AWP");
	NOT_IMPLEMENTED_VALUE(WeaponID, "MP5NAVY");
	NOT_IMPLEMENTED_VALUE(WeaponID, "M249");
	NOT_IMPLEMENTED_VALUE(WeaponID, "M3");
	NOT_IMPLEMENTED_VALUE(WeaponID, "M4A1");
	NOT_IMPLEMENTED_VALUE(WeaponID, "TMP");
	NOT_IMPLEMENTED_VALUE(WeaponID, "G3SG1");
	NOT_IMPLEMENTED_VALUE(WeaponID, "FLASHBANG");
	NOT_IMPLEMENTED_VALUE(WeaponID, "DEAGLE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "SG552");
	NOT_IMPLEMENTED_VALUE(WeaponID, "AK47");
	NOT_IMPLEMENTED_VALUE(WeaponID, "KNIFE");
	NOT_IMPLEMENTED_VALUE(WeaponID, "P90");
	NOT_IMPLEMENTED_VALUE(WeaponID, "SHIELD");
	NOT_IMPLEMENTED_VALUE(WeaponID, "KEVLAR");
	NOT_IMPLEMENTED_VALUE(WeaponID, "ASSAULTSUIT");
	NOT_IMPLEMENTED_VALUE(WeaponID, "NIGHTVISION");

	// Export game specific values...
	export_game_specific_weapon_id(WeaponID_);
}


//-----------------------------------------------------------------------------
// Expose WeaponSlot.
//-----------------------------------------------------------------------------
template<class T>
void export_weapon_slot(T _constants)
{
	enum_<WeaponSlot> WeaponSlot_("WeaponSlot");

	// CS:S specific values...
	NOT_IMPLEMENTED_VALUE(WeaponSlot, "PRIMARY");
	NOT_IMPLEMENTED_VALUE(WeaponSlot, "SECONDARY");
	NOT_IMPLEMENTED_VALUE(WeaponSlot, "KNIFE");
	NOT_IMPLEMENTED_VALUE(WeaponSlot, "GRENADE");
	NOT_IMPLEMENTED_VALUE(WeaponSlot, "C4");

	// Export game specific values...
	export_game_specific_weapon_slot(WeaponSlot_);
}


//-----------------------------------------------------------------------------
// Expose MuzzleFlashStyle.
//-----------------------------------------------------------------------------
template<class T>
void export_muzzle_flash_styles(T _constants)
{
	enum_<MuzzleFlashStyle> MuzzleFlashStyle_("MuzzleFlashStyle");

	// CS:S specific values...
	NOT_IMPLEMENTED_VALUE(MuzzleFlashStyle, "NONE");
	NOT_IMPLEMENTED_VALUE(MuzzleFlashStyle, "NORMAL");
	NOT_IMPLEMENTED_VALUE(MuzzleFlashStyle, "SPECIAL");

	// Export game specific values...
	export_game_specific_muzzle_flash_styles(MuzzleFlashStyle_);
}


//-----------------------------------------------------------------------------
// Expose WeaponType.
//-----------------------------------------------------------------------------
template<class T>
void export_weapon_types(T _constants)
{
	enum_<WeaponType> WeaponType_("WeaponType");

	// CS:S specific values...
	NOT_IMPLEMENTED_VALUE(WeaponType, "KNIFE");
	NOT_IMPLEMENTED_VALUE(WeaponType, "PISTOL");
	NOT_IMPLEMENTED_VALUE(WeaponType, "SMG");
	NOT_IMPLEMENTED_VALUE(WeaponType, "RIFLE");
	NOT_IMPLEMENTED_VALUE(WeaponType, "SHOTGUN");
	NOT_IMPLEMENTED_VALUE(WeaponType, "SNIPER");
	NOT_IMPLEMENTED_VALUE(WeaponType, "MACHINEGUN");
	NOT_IMPLEMENTED_VALUE(WeaponType, "EXPLOSIVE");
	NOT_IMPLEMENTED_VALUE(WeaponType, "GRENADE");

	// Export game specific values...
	export_game_specific_weapon_types(WeaponType_);
}


#endif // _WEAPONS_CONSTANTS_ORANGEBOX_WRAP_PYTHON_H
