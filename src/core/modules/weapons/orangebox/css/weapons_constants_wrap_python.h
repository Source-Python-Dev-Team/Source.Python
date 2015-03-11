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

#ifndef _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_PYTHON_H
#define _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "weapons_constants_wrap.h"


//-----------------------------------------------------------------------------
// Expose WeaponInfo.
//-----------------------------------------------------------------------------
template<class T>
void export_game_specific_weapon_info(T WeaponInfo_)
{
	// Properties...
	WeaponInfo_.add_property("addon", &WeaponInfoExt::get_addon, &WeaponInfoExt::set_addon);

	WeaponInfo_.def_readwrite("max_player_speed", &WeaponInfo::m_iMaxPlayerSpeed);
	WeaponInfo_.def_readwrite("type", &WeaponInfo::m_iWeaponType);
	WeaponInfo_.def_readwrite("full_auto", &WeaponInfo::m_bFullAuto);
	WeaponInfo_.def_readwrite("team", &WeaponInfo::m_iTeamNum);
	WeaponInfo_.def_readwrite("bot_audible_range", &WeaponInfo::m_iBotAudibleRange);
	WeaponInfo_.def_readwrite("armor_ratio", &WeaponInfo::m_fWeaponArmorRatio);
	WeaponInfo_.def_readwrite("crosshair_minimum_distance", &WeaponInfo::m_iCrosshairMinDistance);
	WeaponInfo_.def_readwrite("crosshair_delta_distance", &WeaponInfo::m_iCrosshairDeltaDistance);
	WeaponInfo_.def_readwrite("can_equip_with_shield", &WeaponInfo::m_bCanEquipWithShield);
	WeaponInfo_.def_readwrite("can_equip_with_shield", &WeaponInfo::m_bCanEquipWithShield);

	WeaponInfo_.add_property("wrong_team_message", &WeaponInfoExt::get_wrong_team_message, &WeaponInfoExt::set_wrong_team_message);
	WeaponInfo_.add_property("player_animation_extension", &WeaponInfoExt::get_player_animation_extension, &WeaponInfoExt::set_player_animation_extension);
	WeaponInfo_.add_property("shield_view_model", &WeaponInfoExt::get_shield_view_model, &WeaponInfoExt::set_shield_view_model);
	WeaponInfo_.add_property("addon_model", &WeaponInfoExt::get_addon_model, &WeaponInfoExt::set_addon_model);
	WeaponInfo_.add_property("dropped_model", &WeaponInfoExt::get_dropped_model, &WeaponInfoExt::set_dropped_model);
	WeaponInfo_.add_property("silencer_model", &WeaponInfoExt::get_silencer_model, &WeaponInfoExt::set_silencer_model);

	WeaponInfo_.def_readwrite("muzzle_flash_style", &WeaponInfo::m_iMuzzleFlashStyle);
	WeaponInfo_.def_readwrite("muzzle_flash_scale", &WeaponInfo::m_fMuzzleFlashScale);
	WeaponInfo_.def_readwrite("penetration", &WeaponInfo::m_iPenetration);
	WeaponInfo_.def_readwrite("damage", &WeaponInfo::m_iDamage);
	WeaponInfo_.def_readwrite("range", &WeaponInfo::m_fRange);
	WeaponInfo_.def_readwrite("range_modifier", &WeaponInfo::m_fRangeModifier);
	WeaponInfo_.def_readwrite("fire_bullets", &WeaponInfo::m_iTEFireBullets);
	WeaponInfo_.def_readwrite("cycle_time", &WeaponInfo::m_fCycleTime);
	WeaponInfo_.def_readwrite("accuracy_quadratic", &WeaponInfo::m_iAccuracyQuadratic);
	WeaponInfo_.def_readwrite("accuracy_divisor", &WeaponInfo::m_fAccuracyDivisor);
	WeaponInfo_.def_readwrite("accuracy_offset", &WeaponInfo::m_fAccuracyOffset);
	WeaponInfo_.def_readwrite("maximum_inaccuracy", &WeaponInfo::m_fMaxInaccuracy);
	WeaponInfo_.def_readwrite("spread", &WeaponInfo::m_fSpread);
	WeaponInfo_.def_readwrite("alternate_spread", &WeaponInfo::m_fAlternateSpread);
	WeaponInfo_.def_readwrite("inaccuracy_crouch", &WeaponInfo::m_fInaccuracyCrouch);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_crouch", &WeaponInfo::m_fAlternateInaccuracyCrouch);
	WeaponInfo_.def_readwrite("inaccuracy_stand", &WeaponInfo::m_fInaccuracyStand);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_stand", &WeaponInfo::m_fAlternateInaccuracyStand);
	WeaponInfo_.def_readwrite("inaccuracy_jump", &WeaponInfo::m_fInaccuracyJump);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_jump", &WeaponInfo::m_fAlternateInaccuracyJump);
	WeaponInfo_.def_readwrite("inaccuracy_land", &WeaponInfo::m_fInaccuracyLand);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_land", &WeaponInfo::m_fAlternateInaccuracyLand);
	WeaponInfo_.def_readwrite("inaccuracy_ladder", &WeaponInfo::m_fInaccuracyLadder);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_ladder", &WeaponInfo::m_fAlternateInaccuracyLadder);
	WeaponInfo_.def_readwrite("inaccuracy_fire", &WeaponInfo::m_fInaccuracyFire);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_fire", &WeaponInfo::m_fAlternateInaccuracyFire);
	WeaponInfo_.def_readwrite("inaccuracy_move", &WeaponInfo::m_fInaccuracyMove);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_move", &WeaponInfo::m_fAlternateInaccuracyMove);
	WeaponInfo_.def_readwrite("recovery_time_stand", &WeaponInfo::m_fRecoveryTimeStand);
	WeaponInfo_.def_readwrite("recovery_time_crouch", &WeaponInfo::m_fRecoveryTimeCrouch);
	WeaponInfo_.def_readwrite("inaccuracy_reload", &WeaponInfo::m_fInaccuracyReload);
	WeaponInfo_.def_readwrite("alternate_inaccuracy_reload", &WeaponInfo::m_fAlternateInaccuracyReload);
	WeaponInfo_.def_readwrite("time_to_idle", &WeaponInfo::m_fTimeToIdle);
	WeaponInfo_.def_readwrite("price", &WeaponInfo::m_iWeaponPrice);
	WeaponInfo_.def_readwrite("default_price", &WeaponInfo::m_iDefaultWeaponPrice);
	WeaponInfo_.def_readwrite("previous_price", &WeaponInfo::m_iPreviousWeaponPrice);
}


//-----------------------------------------------------------------------------
// Expose WeaponID.
//-----------------------------------------------------------------------------
template<class T>
void export_game_specific_weapon_id(T WeaponID_)
{
	WeaponID_.value("NONE", WEAPON_NONE);
	WeaponID_.value("P228", WEAPON_P228);
	WeaponID_.value("GLOCK", WEAPON_GLOCK);
	WeaponID_.value("SCOUT", WEAPON_SCOUT);
	WeaponID_.value("HEGRENADE", WEAPON_HEGRENADE);
	WeaponID_.value("XM1014", WEAPON_XM1014);
	WeaponID_.value("C4", WEAPON_C4);
	WeaponID_.value("MAC10", WEAPON_MAC10);
	WeaponID_.value("AUG", WEAPON_AUG);
	WeaponID_.value("SMOKEGRENADE", WEAPON_SMOKEGRENADE);
	WeaponID_.value("ELITE", WEAPON_ELITE);
	WeaponID_.value("FIVESEVEN", WEAPON_FIVESEVEN);
	WeaponID_.value("UMP45", WEAPON_UMP45);
	WeaponID_.value("SG550", WEAPON_SG550);
	WeaponID_.value("GALIL", WEAPON_GALIL);
	WeaponID_.value("FAMAS", WEAPON_FAMAS);
	WeaponID_.value("USP", WEAPON_USP);
	WeaponID_.value("AWP", WEAPON_AWP);
	WeaponID_.value("MP5NAVY", WEAPON_MP5NAVY);
	WeaponID_.value("M249", WEAPON_M249);
	WeaponID_.value("M3", WEAPON_M3);
	WeaponID_.value("M4A1", WEAPON_M4A1);
	WeaponID_.value("TMP", WEAPON_TMP);
	WeaponID_.value("G3SG1", WEAPON_G3SG1);
	WeaponID_.value("FLASHBANG", WEAPON_FLASHBANG);
	WeaponID_.value("DEAGLE", WEAPON_DEAGLE);
	WeaponID_.value("SG552", WEAPON_SG552);
	WeaponID_.value("AK47", WEAPON_AK47);
	WeaponID_.value("KNIFE", WEAPON_KNIFE);
	WeaponID_.value("P90", WEAPON_P90);
	WeaponID_.value("SHIELD", WEAPON_SHIELD);
	WeaponID_.value("KEVLAR", WEAPON_KEVLAR);
	WeaponID_.value("ASSAULTSUIT", WEAPON_ASSAULTSUIT);
	WeaponID_.value("NIGHTVISION", WEAPON_NIGHTVISION);
}


//-----------------------------------------------------------------------------
// Expose WeaponID.
//-----------------------------------------------------------------------------
template<class T>
void export_game_specific_weapon_slot(T WeaponSlot_)
{
	WeaponSlot_.value("PRIMARY", SLOT_PRIMARY);
	WeaponSlot_.value("SECONDARY", SLOT_SECONDARY);
	WeaponSlot_.value("KNIFE", SLOT_KNIFE);
	WeaponSlot_.value("GRENADE", SLOT_GRENADE);
	WeaponSlot_.value("C4", SLOT_C4);
}


//-----------------------------------------------------------------------------
// Expose MuzzleFlashStyle.
//-----------------------------------------------------------------------------
template<class T>
void export_game_specific_muzzle_flash_styles(T MuzzleFlashStyle_)
{
	MuzzleFlashStyle_.value("NONE", MUZZLE_NONE);
	MuzzleFlashStyle_.value("NORMAL", MUZZLE_NORMAL);
	MuzzleFlashStyle_.value("SPECIAL", MUZZLE_SPECIAL);
}


//-----------------------------------------------------------------------------
// Expose MuzzleFlashStyle.
//-----------------------------------------------------------------------------
template<class T>
void export_game_specific_weapon_types(T WeaponType_)
{
	WeaponType_.value("KNIFE", WEAPON_TYPE_KNIFE);
	WeaponType_.value("PISTOL", WEAPON_TYPE_PISTOL);
	WeaponType_.value("SMG", WEAPON_TYPE_SMG);
	WeaponType_.value("RIFLE",WEAPON_TYPE_RIFLE);
	WeaponType_.value("SHOTGUN", WEAPON_TYPE_SHOTGUN);
	WeaponType_.value("SNIPER", WEAPON_TYPE_SNIPER);
	WeaponType_.value("MACHINEGUN", WEAPON_TYPE_MACHINEGUN);
	WeaponType_.value("GRENADE", WEAPON_TYPE_GRENADE);
}


#endif // _WEAPONS_CONSTANTS_ORANGEBOX_CSS_WRAP_PYTHON_H
