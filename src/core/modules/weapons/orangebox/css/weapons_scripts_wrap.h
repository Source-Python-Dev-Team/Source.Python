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

#ifndef _WEAPONS_SCRIPTS_ORANGEBOX_CSS_WRAP_H
#define _WEAPONS_SCRIPTS_ORANGEBOX_CSS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "tier1/KeyValues.h"
#include "game/shared/weapon_parse.h"
#include "datamap.h"
#include "game/shared/shareddefs.h"


//-----------------------------------------------------------------------------
// Defines.
//-----------------------------------------------------------------------------
#define MAX_PLAYER_ANIM_EXTENSION 16
#define MAX_SHIELD_VIEW_MODEL_LENGTH 64


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
enum MuzzleFlashStyle;
enum WeaponType;


//-----------------------------------------------------------------------------
// CCSWeaponInfo wrapper class.
//-----------------------------------------------------------------------------
class WeaponInfo: public FileWeaponInfo_t
{
public:
	char m_szAIAddOn[MAX_WEAPON_STRING];
	float m_fMaxPlayerSpeed;
	int m_iWeaponType;
	bool m_bFullAuto;
	int m_iTeamNum;
	float m_fBotAudibleRange;
	float m_fWeaponArmorRatio;
	int m_iCrosshairMinDistance;
	int m_iCrosshairDeltaDistance;
	bool m_bCanEquipWithShield;
	char m_szWrongTeamMsg[MAX_TEAM_NAME_LENGTH];
	char m_szPlayerAnimationExtension[MAX_PLAYER_ANIM_EXTENSION];
	char m_szShieldViewModel[MAX_SHIELD_VIEW_MODEL_LENGTH];
	char m_szAddonModel[MAX_WEAPON_STRING];
	char m_szDroppedModel[MAX_WEAPON_STRING];
	char m_szSilencerModel[MAX_WEAPON_STRING];
	int m_iMuzzleFlashStyle;
	float m_fMuzzleFlashScale;
	int m_iPenetration;
	int m_iDamage;
	float m_fRange;
	float m_fRangeModifier;
	int m_iTEFireBullets;
	float m_fCycleTime;
	int m_iAccuracyQuadratic;
	float m_fAccuracyDivisor;
	float m_fAccuracyOffset;
	float m_fMaxInaccuracy;
	float m_fSpread;
	float m_fAlternateSpread;
	float m_fInaccuracyCrouch;
	float m_fAlternateInaccuracyCrouch;
	float m_fInaccuracyStand;
	float m_fAlternateInaccuracyStand;
	float m_fInaccuracyJump;
	float m_fAlternateInaccuracyJump;
	float m_fInaccuracyLand;
	float m_fAlternateInaccuracyLand;
	float m_fInaccuracyLadder;
	float m_fAlternateInaccuracyLadder;
	float m_fInaccuracyFire;
	float m_fAlternateInaccuracyFire;
	float m_fInaccuracyMove;
	float m_fAlternateInaccuracyMove;
	float m_fRecoveryTimeStand;
	float m_fRecoveryTimeCrouch;
	float m_fInaccuracyReload;
	float m_fAlternateInaccuracyReload;
	float m_fTimeToIdle;
	float m_fIdleInterval;
	int m_iWeaponPrice;
	int m_iDefaultWeaponPrice;
	int m_iPreviousWeaponPrice;
};


//-----------------------------------------------------------------------------
// WeaponInfo extension class.
//-----------------------------------------------------------------------------
class WeaponInfoExt
{
public:
	static WeaponType get_weapon_type(WeaponInfo *pWeaponInfo)
	{
		return (WeaponType)pWeaponInfo->m_iWeaponType;
	}

	static void set_weapon_type(WeaponInfo *pWeaponInfo, WeaponType iWeaponType)
	{
		pWeaponInfo->m_iWeaponType = (int)iWeaponType;
	}

	static const char *get_addon(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szAIAddOn;
	}

	static void set_addon(WeaponInfo *pWeaponInfo, const char *szAIAddOn)
	{
		strncpy(pWeaponInfo->m_szAIAddOn, szAIAddOn, MAX_WEAPON_STRING);
	}

	static const char *get_wrong_team_message(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szWrongTeamMsg;
	}

	static void set_wrong_team_message(WeaponInfo *pWeaponInfo, const char *szWrongTeamMsg)
	{
		strncpy(pWeaponInfo->m_szWrongTeamMsg, szWrongTeamMsg, MAX_TEAM_NAME_LENGTH);
	}

	static const char *get_player_animation_extension(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szPlayerAnimationExtension;
	}

	static void set_player_animation_extension(WeaponInfo *pWeaponInfo, const char *szPlayerAnimationExtension)
	{
		strncpy(pWeaponInfo->m_szPlayerAnimationExtension, szPlayerAnimationExtension, MAX_PLAYER_ANIM_EXTENSION);
	}

	static const char *get_shield_view_model(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szShieldViewModel;
	}

	static void set_shield_view_model(WeaponInfo *pWeaponInfo, const char *szShieldViewModel)
	{
		strncpy(pWeaponInfo->m_szShieldViewModel, szShieldViewModel, MAX_SHIELD_VIEW_MODEL_LENGTH);
	}

	static const char *get_addon_model(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szAddonModel;
	}

	static void set_addon_model(WeaponInfo *pWeaponInfo, const char *szAddonModel)
	{
		strncpy(pWeaponInfo->m_szAddonModel, szAddonModel, MAX_WEAPON_STRING);
	}

	static const char *get_dropped_model(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szDroppedModel;
	}

	static void set_dropped_model(WeaponInfo *pWeaponInfo, const char *szDroppedModel)
	{
		strncpy(pWeaponInfo->m_szDroppedModel, szDroppedModel, MAX_WEAPON_STRING);
	}

	static const char *get_silencer_model(WeaponInfo *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->m_szSilencerModel;
	}

	static void set_silencer_model(WeaponInfo *pWeaponInfo, const char *szSilencerModel)
	{
		strncpy(pWeaponInfo->m_szSilencerModel, szSilencerModel, MAX_WEAPON_STRING);
	}

	static MuzzleFlashStyle get_muzzle_flash_style(WeaponInfo *pWeaponInfo)
	{
		return (MuzzleFlashStyle)pWeaponInfo->m_iMuzzleFlashStyle;
	}

	static void set_muzzle_flash_style(WeaponInfo *pWeaponInfo, MuzzleFlashStyle iMuzzleFlashStyle)
	{
		pWeaponInfo->m_iMuzzleFlashStyle = (int)iMuzzleFlashStyle;
	}
};


#endif // _WEAPONS_SCRIPTS_ORANGEBOX_CSS_WRAP_H
