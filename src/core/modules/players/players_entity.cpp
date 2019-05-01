/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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

// ============================================================================
// >> INCLUDES
// ============================================================================
// Source.Python
#include "players_entity.h"


// ============================================================================
// >> PlayerMixin
// ============================================================================
boost::shared_ptr<PlayerMixin> PlayerMixin::__init__(unsigned int uiEntityIndex)
{
	CBaseEntityWrapper* pEntity = (CBaseEntityWrapper*) ExcBaseEntityFromIndex(uiEntityIndex);
	if (!pEntity->IsPlayer())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Index '%d' is not a valid player.", uiEntityIndex);

	return PlayerMixin::wrap(pEntity->GetBaseEntity());
}

boost::shared_ptr<PlayerMixin> PlayerMixin::wrap(CBaseEntity* pEntity)
{
	return boost::shared_ptr<PlayerMixin>(
		(PlayerMixin *) pEntity,
		&NeverDeleteDeleter<PlayerMixin *>
	);
}


// CBasePlayer
float PlayerMixin::GetSpeed()
{
	static int offset = FindNetworkPropertyOffset("localdata.m_flLaggedMovementValue");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetSpeed(float value)
{
	static int offset = FindNetworkPropertyOffset("localdata.m_flLaggedMovementValue");
	SetNetworkPropertyByOffset<float>(offset, value);
}


bool PlayerMixin::GetIsDucked()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucked");
	return GetDatamapPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsDucked(bool value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucked");
	SetDatamapPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetIsDucking()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucking");
	return GetDatamapPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsDucking(bool value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucking");
	SetDatamapPropertyByOffset<bool>(offset, value);
}


int PlayerMixin::GetFlags()
{
	static int offset = FindNetworkPropertyOffset("m_fFlags");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetFlags(int value)
{
	static int offset = FindNetworkPropertyOffset("m_fFlags");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetLastWeapon()
{
	static int offset = FindNetworkPropertyOffset("localdata.m_hLastWeapon");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetLastWeapon(int value)
{
	static int offset = FindNetworkPropertyOffset("localdata.m_hLastWeapon");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetObserverTarget()
{
	static int offset = FindNetworkPropertyOffset("m_hObserverTarget");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetObserverTarget(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hObserverTarget");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetDeaths()
{
	static int offset = FindDatamapPropertyOffset("m_iDeaths");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetDeaths(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iDeaths");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetKills()
{
	static int offset = FindDatamapPropertyOffset("m_iFrags");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetKills(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iFrags");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetObserverMode()
{
	static int offset = FindNetworkPropertyOffset("m_iObserverMode");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetObserverMode(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iObserverMode");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetLifeState()
{
	static int offset = FindNetworkPropertyOffset("m_lifeState");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetLifeState(int value)
{
	static int offset = FindNetworkPropertyOffset("m_lifeState");
	SetNetworkPropertyByOffset<int>(offset, value);
}


str PlayerMixin::GetPlace()
{
	static int offset = FindNetworkPropertyOffset("m_szLastPlaceName");
	return str(GetNetworkPropertyStringArrayByOffset(offset));
}

void PlayerMixin::SetPlace(const char* value)
{
	static int offset = FindNetworkPropertyOffset("m_szLastPlaceName");
	SetNetworkPropertyStringArrayByOffset(offset, value);
}


bool PlayerMixin::GetDead()
{
	static int offset = FindNetworkPropertyOffset("pl.deadflag");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetDead(bool value)
{
	static int offset = FindNetworkPropertyOffset("pl.deadflag");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


float PlayerMixin::GetFallVelocity()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFallVelocity");
	return GetDatamapPropertyByOffset<float>(offset);
}

void PlayerMixin::SetFallVelocity(float value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFallVelocity");
	SetDatamapPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetButtons()
{
	static int offset = FindDatamapPropertyOffset("m_nButtons");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetButtons(int value)
{
	static int offset = FindDatamapPropertyOffset("m_nButtons");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetHiddenHUDs()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_iHideHUD");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetHiddenHUDs(int value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_iHideHUD");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetDrawViewModel()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDrawViewmodel");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetDrawViewModel(int value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDrawViewmodel");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetFOV()
{
	static int offset = FindNetworkPropertyOffset("m_iFOV");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetFOV(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iFOV");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetFOVStart()
{
	static int offset = FindNetworkPropertyOffset("m_iFOVStart");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetFOVStart(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iFOVStart");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerMixin::GetFOVTime()
{
	static int offset = FindNetworkPropertyOffset("m_flFOVTime");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetFOVTime(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFOVTime");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetDefaultFOV()
{
	static int offset = FindNetworkPropertyOffset("m_iDefaultFOV");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetDefaultFOV(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iDefaultFOV");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerMixin::GetFOVRate()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFOVRate");
	return GetDatamapPropertyByOffset<float>(offset);
}

void PlayerMixin::SetFOVRate(float value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFOVRate");
	SetDatamapPropertyByOffset<float>(offset, value);
}


// CBaseCombatCharacter
Vector PlayerMixin::GetGunOffset()
{
	static int offset = FindDatamapPropertyOffset("m_HackedGunPos");
	return GetDatamapPropertyByOffset<Vector>(offset);
}

void PlayerMixin::SetGunOffset(Vector& value)
{
	static int offset = FindDatamapPropertyOffset("m_HackedGunPos");
	SetDatamapPropertyByOffset<Vector>(offset, value);
}


int PlayerMixin::GetLastHitgroup()
{
	static int offset = FindDatamapPropertyOffset("m_LastHitGroup");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerMixin::SetLastHitgroup(int value)
{
	static int offset = FindDatamapPropertyOffset("m_LastHitGroup");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetActiveWeaponHandle()
{
	static int offset = FindNetworkPropertyOffset("m_hActiveWeapon");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetActiveWeaponHandle(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hActiveWeapon");
	SetNetworkPropertyByOffset<int>(offset, value);
}


str PlayerMixin::GetRelationship()
{
	return GetKeyValueString("Relationship");
}

void PlayerMixin::SetRelationship(const char* value)
{
	SetKeyValue("Relationship", value);
}


float PlayerMixin::GetPhysDamageScale()
{
	return GetKeyValueFloat("physdamagescale");
}

void PlayerMixin::SetPhysDamageScale(float value)
{
	SetKeyValue("physdamagescale", value);
}


QAngle PlayerMixin::GetEyeAngle()
{
	static int offset_x = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(0));
	static int offset_y = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(1));
	return QAngle(
		GetNetworkPropertyByOffset<float>(offset_x),
		GetNetworkPropertyByOffset<float>(offset_y),
		0);
}

void PlayerMixin::SetEyeAngle(QAngle& value)
{
	static int offset_x = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(0));
	static int offset_y = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(1));
	SetNetworkPropertyByOffset<float>(offset_x, value.x);
	SetNetworkPropertyByOffset<float>(offset_y, value.y);
}


Vector PlayerMixin::GetViewVector()
{
	QAngle eye_angle = GetEyeAngle();

	float yaw = DEG2RAD(eye_angle.y);
	float pitch = DEG2RAD(eye_angle.x);

	float sy, cy, sp, cp;
	SinCos(yaw, &sy, &cy);
	SinCos(pitch, &sp, &cp);

	return Vector(cp * cy, cp * sy, -sp);
}

void PlayerMixin::SetViewVector(Vector& value)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Setting view_vector is not implemented.");
}


QAngle PlayerMixin::GetViewAngle()
{
	QAngle eye_angle = GetEyeAngle();
	return QAngle(
		eye_angle.x,
		eye_angle.y < 0 ? eye_angle.y + 360 : eye_angle.y,
		GetRotation().z);
}

void PlayerMixin::SetViewAngle(QAngle& value)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Setting view_angle is not implemented.");
}


float PlayerMixin::GetStamina()
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_flStamina");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetStamina(float value)
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_flStamina");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetShotsFired()
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_iShotsFired");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetShotsFired(int value)
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_iShotsFired");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetArmor()
{
	static int offset = FindNetworkPropertyOffset("m_ArmorValue");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetArmor(int value)
{
	static int offset = FindNetworkPropertyOffset("m_ArmorValue");
	SetNetworkPropertyByOffset<int>(offset, value);
}


bool PlayerMixin::GetHasDefuser()
{
	static int offset = FindNetworkPropertyOffset("m_bHasDefuser");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetHasDefuser(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasDefuser");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetHasHelmet()
{
	static int offset = FindNetworkPropertyOffset("m_bHasHelmet");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetHasHelmet(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasHelmet");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetHasNightvision()
{
	static int offset = FindNetworkPropertyOffset("m_bHasNightVision");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetHasNightvision(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasNightVision");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetIsInBombZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInBombZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsInBombZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInBombZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetIsInBuyZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInBuyZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsInBuyZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInBuyZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetIsInHostageRescueZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInHostageRescueZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsInHostageRescueZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInHostageRescueZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetIsDefusing()
{
	static int offset = FindNetworkPropertyOffset("m_bIsDefusing");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetIsDefusing(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bIsDefusing");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerMixin::GetNightvisionOn()
{
	static int offset = FindNetworkPropertyOffset("m_bNightVisionOn");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerMixin::SetNightvisionOn(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bNightVisionOn");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


float PlayerMixin::GetFlashDuration()
{
	static int offset = FindNetworkPropertyOffset("m_flFlashDuration");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetFlashDuration(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFlashDuration");
	SetNetworkPropertyByOffset<float>(offset, value);
}


float PlayerMixin::GetFlashAlpha()
{
	static int offset = FindNetworkPropertyOffset("m_flFlashMaxAlpha");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetFlashAlpha(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFlashMaxAlpha");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetCash()
{
	static int offset = FindNetworkPropertyOffset("m_iAccount");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetCash(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iAccount");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetPlayerClass()
{
	static int offset = FindNetworkPropertyOffset(PLAYER_CLASS_PROPERTY);
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetPlayerClass(int value)
{
	static int offset = FindNetworkPropertyOffset(PLAYER_CLASS_PROPERTY);
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetPlayerState()
{
	static int offset = FindNetworkPropertyOffset("m_iPlayerState");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetPlayerState(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iPlayerState");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetRagdoll()
{
	static int offset = FindNetworkPropertyOffset("m_hRagdoll");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetRagdoll(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hRagdoll");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerMixin::GetActiveDevices()
{
	static int offset = FindNetworkPropertyOffset("m_HL2Local.m_bitsActiveDevices");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetActiveDevices(int value)
{
	static int offset = FindNetworkPropertyOffset("m_HL2Local.m_bitsActiveDevices");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerMixin::GetSuitPowerLoad()
{
	static int offset = FindNetworkPropertyOffset("m_flSuitPowerLoad");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerMixin::SetSuitPowerLoad(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flSuitPowerLoad");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerMixin::GetDesiredPlayerClass()
{
	static int offset = FindNetworkPropertyOffset("m_Shared.m_iDesiredPlayerClass");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerMixin::SetDesiredPlayerClass(int value)
{
	static int offset = FindNetworkPropertyOffset("m_Shared.m_iDesiredPlayerClass");
	SetNetworkPropertyByOffset<int>(offset, value);
}
