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

#ifndef _PLAYERS_ENTITY_H
#define _PLAYERS_ENTITY_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "boost/python.hpp"
using namespace boost::python;

#include "modules/entities/entities_entity.h"


//-----------------------------------------------------------------------------
// MACROS
//-----------------------------------------------------------------------------
// Eye angle property
#if defined(ENGINE_BRANCH_TF2)
	#define EYE_ANGLE_PROPERTY(index) "tfnonlocaldata.m_angEyeAngles[" #index "]"
#elif defined(ENGINE_BRANCH_BMS)
	#define EYE_ANGLE_PROPERTY(index) "blackmesalocaldata.m_angEyeAngles[" #index "]"
#elif defined(ENGINE_BRANCH_GMOD)
	#define EYE_ANGLE_PROPERTY(index) "hl2mplocaldata.m_angEyeAngles[" #index "]"
#else 
	#define EYE_ANGLE_PROPERTY(index) "m_angEyeAngles[" #index "]"
#endif

// Player class property
#if defined(ENGINE_BRANCH_TF2)
	#define PLAYER_CLASS_PROPERTY "m_PlayerClass.m_iClass"
#else
	#define PLAYER_CLASS_PROPERTY "m_iClass"
#endif


//-----------------------------------------------------------------------------
// CBaseEntity extension class for players.
//-----------------------------------------------------------------------------
class PlayerMixin: public CBaseEntityWrapper
{
public:
	static boost::shared_ptr<PlayerMixin> __init__(unsigned int uiEntityIndex);
	static boost::shared_ptr<PlayerMixin> wrap(CBaseEntity* pEntity);
	
	// CBasePlayer
	// TODO: Return for some of these the proper entity class instead of a handle/index
	// E. g. BaseEntity, Entity, Weapon, Player, etc.
	float GetSpeed();
	void SetSpeed(float value);

	bool GetIsDucked();
	void SetIsDucked(bool value);

	bool GetIsDucking();
	void SetIsDucking(bool value);

	int GetFlags();
	void SetFlags(int value);

	int GetLastWeapon();
	void SetLastWeapon(int value);

	int GetObserverTarget();
	void SetObserverTarget(int value);

	int GetDeaths();
	void SetDeaths(int value);

	int GetKills();
	void SetKills(int value);

	// TODO: Return ObserverMode enum
	int GetObserverMode();
	void SetObserverMode(int value);

	// TODO: Return LifeState enum
	int GetLifeState();
	void SetLifeState(int value);

	str GetPlace();
	void SetPlace(const char* value);

	bool GetDead();
	void SetDead(bool value);

	float GetFallVelocity();
	void SetFallVelocity(float value);

	int GetButtons();
	void SetButtons(int value);

	int GetHiddenHUDs();
	void SetHiddenHUDs(int value);

	int GetDrawViewModel();
	void SetDrawViewModel(int value);

	int GetFOV();
	void SetFOV(int value);

	int GetFOVStart();
	void SetFOVStart(int value);

	float GetFOVTime();
	void SetFOVTime(float value);

	int GetDefaultFOV();
	void SetDefaultFOV(int value);

	float GetFOVRate();
	void SetFOVRate(float value);

	// CBaseCombatCharacter
	Vector GetGunOffset();
	void SetGunOffset(Vector& value);

	int GetLastHitgroup();
	void SetLastHitgroup(int value);

	int GetActiveWeaponHandle();
	void SetActiveWeaponHandle(int value);

	str GetRelationship();
	void SetRelationship(const char* value);

	float GetPhysDamageScale();
	void SetPhysDamageScale(float value);

	QAngle GetEyeAngle();
	void SetEyeAngle(QAngle& value);

	Vector GetViewVector();
	void SetViewVector(Vector& value);

	QAngle GetViewAngle();
	void SetViewAngle(QAngle& value);

	// Game specific
	// CS:S, CS:GO
	float GetStamina();
	void SetStamina(float value);

	int GetShotsFired();
	void SetShotsFired(int value);

	int GetArmor();
	void SetArmor(int value);

	bool GetHasDefuser();
	void SetHasDefuser(bool value);

	bool GetHasHelmet();
	void SetHasHelmet(bool value);

	bool GetHasNightvision();
	void SetHasNightvision(bool value);

	bool GetIsInBombZone();
	void SetIsInBombZone(bool value);

	bool GetIsInBuyZone();
	void SetIsInBuyZone(bool value);

	bool GetIsInHostageRescueZone();
	void SetIsInHostageRescueZone(bool value);

	bool GetIsDefusing();
	void SetIsDefusing(bool value);

	bool GetNightvisionOn();
	void SetNightvisionOn(bool value);

	float GetFlashDuration();
	void SetFlashDuration(float value);

	float GetFlashAlpha();
	void SetFlashAlpha(float value);

	int GetCash();
	void SetCash(int value);

	int GetPlayerClass();
	void SetPlayerClass(int value);

	int GetPlayerState();
	void SetPlayerState(int value);

	int GetRagdoll();
	void SetRagdoll(int value);

	// HL2
	int GetActiveDevices();
	void SetActiveDevices(int value);

	// HL2
	float GetSuitPowerLoad();
	void SetSuitPowerLoad(float value);

	// TF2
	int GetDesiredPlayerClass();
	void SetDesiredPlayerClass(int value);
};


#endif // _PLAYERS_ENTITY_H
