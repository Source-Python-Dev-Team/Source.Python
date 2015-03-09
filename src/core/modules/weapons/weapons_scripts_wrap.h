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

#ifndef _WEAPONS_SCRIPTS_WRAP_H
#define _WEAPONS_SCRIPTS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utility/wrap_macros.h"
#include "tier1/KeyValues.h"
#include "game/shared/weapon_parse.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
enum WeaponSlot;


//-----------------------------------------------------------------------------
// FileWeaponInfo_t extension class.
//-----------------------------------------------------------------------------
class BaseWeaponInfoSharedExt
{
public:
	static WeaponSlot get_slot(FileWeaponInfo_t *pWeaponInfo)
	{
		return (WeaponSlot)pWeaponInfo->iSlot;
	}

	static void set_slot(FileWeaponInfo_t *pWeaponInfo, WeaponSlot iWeaponSlot)
	{
		pWeaponInfo->iSlot = (int)iWeaponSlot;
	}

	static const char *get_class_name(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szClassName;
	}

	static void set_class_name(FileWeaponInfo_t *pWeaponInfo, const char *szClassName)
	{
		strncpy(pWeaponInfo->szClassName, szClassName, MAX_WEAPON_STRING);
	}

	static const char *get_print_name(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szPrintName;
	}

	static void set_print_name(FileWeaponInfo_t *pWeaponInfo, const char *szViewModel)
	{
		strncpy(pWeaponInfo->szViewModel, szViewModel, MAX_WEAPON_STRING);
	}

	static const char *get_view_model(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szViewModel;
	}

	static void set_view_model(FileWeaponInfo_t *pWeaponInfo, const char *szViewModel)
	{
		strncpy(pWeaponInfo->szViewModel, szViewModel, MAX_WEAPON_STRING);
	}

	static const char *get_world_model(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szWorldModel;
	}

	static void set_world_model(FileWeaponInfo_t *pWeaponInfo, const char *szWorldModel)
	{
		strncpy(pWeaponInfo->szWorldModel, szWorldModel, MAX_WEAPON_STRING);
	}

	static const char *get_animation_prefix(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szAnimationPrefix;
	}

	static void set_animation_prefix(FileWeaponInfo_t *pWeaponInfo, const char *szAnimationPrefix)
	{
		strncpy(pWeaponInfo->szAnimationPrefix, szAnimationPrefix, MAX_WEAPON_PREFIX);
	}

	static const char *get_primary_ammo(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szAmmo1;
	}

	static void set_primary_ammo(FileWeaponInfo_t *pWeaponInfo, const char *szAmmo1)
	{
		strncpy(pWeaponInfo->szAmmo1, szAmmo1, MAX_WEAPON_AMMO_NAME);
	}

	static const char *get_secondary_ammo(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szAmmo2;
	}

	static void set_secondary_ammo(FileWeaponInfo_t *pWeaponInfo, const char *szAmmo2)
	{
		strncpy(pWeaponInfo->szAmmo2, szAmmo2, MAX_WEAPON_AMMO_NAME);
	}

	static const char *get_shoot_sound(FileWeaponInfo_t *pWeaponInfo, WeaponSound_t iSoundType)
	{
		return (const char *)pWeaponInfo->aShootSounds[iSoundType];
	}

	static void set_shoot_sound(FileWeaponInfo_t *pWeaponInfo, WeaponSound_t iSoundType, const char *szWeaponSound)
	{
		strncpy(pWeaponInfo->aShootSounds[iSoundType], szWeaponSound, MAX_WEAPON_STRING);
	}
};


#endif // _WEAPONS_SCRIPTS_WRAP_H
