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
// >> PlayerWrapper
// ============================================================================
boost::shared_ptr<PlayerWrapper> PlayerWrapper::__init__(unsigned int uiEntityIndex)
{
	CBaseEntityWrapper* pEntity = (CBaseEntityWrapper*) ExcBaseEntityFromIndex(uiEntityIndex);
	if (!pEntity->IsPlayer())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Index '%d' is not a valid player.", uiEntityIndex);

	return PlayerWrapper::wrap(pEntity->GetBaseEntity());
}

boost::shared_ptr<PlayerWrapper> PlayerWrapper::wrap(CBaseEntity* pEntity)
{
	return boost::shared_ptr<PlayerWrapper>(
		(PlayerWrapper *) pEntity,
		&NeverDeleteDeleter<PlayerWrapper *>
	);
}


// CBasePlayer
float PlayerWrapper::GetSpeed()
{
	static int offset = FindNetworkPropertyOffset("localdata.m_flLaggedMovementValue");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetSpeed(float value)
{
	static int offset = FindNetworkPropertyOffset("localdata.m_flLaggedMovementValue");
	SetNetworkPropertyByOffset<float>(offset, value);
}


bool PlayerWrapper::GetIsDucked()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucked");
	return GetDatamapPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsDucked(bool value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucked");
	SetDatamapPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetIsDucking()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucking");
	return GetDatamapPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsDucking(bool value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDucking");
	SetDatamapPropertyByOffset<bool>(offset, value);
}


int PlayerWrapper::GetFlags()
{
	static int offset = FindNetworkPropertyOffset("m_fFlags");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetFlags(int value)
{
	static int offset = FindNetworkPropertyOffset("m_fFlags");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetLastWeapon()
{
	static int offset = FindNetworkPropertyOffset("localdata.m_hLastWeapon");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetLastWeapon(int value)
{
	static int offset = FindNetworkPropertyOffset("localdata.m_hLastWeapon");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetObserverTarget()
{
	static int offset = FindNetworkPropertyOffset("m_hObserverTarget");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetObserverTarget(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hObserverTarget");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetDeaths()
{
	static int offset = FindDatamapPropertyOffset("m_iDeaths");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetDeaths(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iDeaths");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetKills()
{
	static int offset = FindDatamapPropertyOffset("m_iFrags");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetKills(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iFrags");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetObserverMode()
{
	static int offset = FindNetworkPropertyOffset("m_iObserverMode");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetObserverMode(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iObserverMode");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetLifeState()
{
	static int offset = FindNetworkPropertyOffset("m_lifeState");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetLifeState(int value)
{
	static int offset = FindNetworkPropertyOffset("m_lifeState");
	SetNetworkPropertyByOffset<int>(offset, value);
}


str PlayerWrapper::GetPlace()
{
	static int offset = FindNetworkPropertyOffset("m_szLastPlaceName");
	return str(GetNetworkPropertyStringArrayByOffset(offset));
}

void PlayerWrapper::SetPlace(const char* value)
{
	static int offset = FindNetworkPropertyOffset("m_szLastPlaceName");
	SetNetworkPropertyStringArrayByOffset(offset, value);
}


bool PlayerWrapper::GetDead()
{
	static int offset = FindNetworkPropertyOffset("pl.deadflag");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetDead(bool value)
{
	static int offset = FindNetworkPropertyOffset("pl.deadflag");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


float PlayerWrapper::GetFallVelocity()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFallVelocity");
	return GetDatamapPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetFallVelocity(float value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFallVelocity");
	SetDatamapPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetButtons()
{
	static int offset = FindDatamapPropertyOffset("m_nButtons");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetButtons(int value)
{
	static int offset = FindDatamapPropertyOffset("m_nButtons");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetHiddenHUDs()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_iHideHUD");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetHiddenHUDs(int value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_iHideHUD");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetDrawViewModel()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDrawViewmodel");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetDrawViewModel(int value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_bDrawViewmodel");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetFOV()
{
	static int offset = FindNetworkPropertyOffset("m_iFOV");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetFOV(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iFOV");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetFOVStart()
{
	static int offset = FindNetworkPropertyOffset("m_iFOVStart");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetFOVStart(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iFOVStart");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerWrapper::GetFOVTime()
{
	static int offset = FindNetworkPropertyOffset("m_flFOVTime");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetFOVTime(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFOVTime");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetDefaultFOV()
{
	static int offset = FindNetworkPropertyOffset("m_iDefaultFOV");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetDefaultFOV(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iDefaultFOV");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerWrapper::GetFOVRate()
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFOVRate");
	return GetDatamapPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetFOVRate(float value)
{
	static int offset = FindDatamapPropertyOffset("m_Local.m_flFOVRate");
	SetDatamapPropertyByOffset<float>(offset, value);
}


// CBaseCombatCharacter
Vector PlayerWrapper::GetGunOffset()
{
	static int offset = FindDatamapPropertyOffset("m_HackedGunPos");
	return GetDatamapPropertyByOffset<Vector>(offset);
}

void PlayerWrapper::SetGunOffset(Vector& value)
{
	static int offset = FindDatamapPropertyOffset("m_HackedGunPos");
	SetDatamapPropertyByOffset<Vector>(offset, value);
}


int PlayerWrapper::GetLastHitgroup()
{
	static int offset = FindDatamapPropertyOffset("m_LastHitGroup");
	return GetDatamapPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetLastHitgroup(int value)
{
	static int offset = FindDatamapPropertyOffset("m_LastHitGroup");
	SetDatamapPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetActiveWeaponHandle()
{
	static int offset = FindNetworkPropertyOffset("m_hActiveWeapon");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetActiveWeaponHandle(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hActiveWeapon");
	SetNetworkPropertyByOffset<int>(offset, value);
}


str PlayerWrapper::GetRelationship()
{
	return GetKeyValueString("Relationship");
}

void PlayerWrapper::SetRelationship(const char* value)
{
	SetKeyValue("Relationship", value);
}


float PlayerWrapper::GetPhysDamageScale()
{
	return GetKeyValueFloat("physdamagescale");
}

void PlayerWrapper::SetPhysDamageScale(float value)
{
	SetKeyValue("physdamagescale", value);
}


QAngle PlayerWrapper::GetEyeAngle()
{
	static int offset_x = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(0));
	static int offset_y = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(1));
	return QAngle(
		GetNetworkPropertyByOffset<float>(offset_x),
		GetNetworkPropertyByOffset<float>(offset_y),
		0);
}

void PlayerWrapper::SetEyeAngle(QAngle& value)
{
	static int offset_x = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(0));
	static int offset_y = FindNetworkPropertyOffset(EYE_ANGLE_PROPERTY(1));
	SetNetworkPropertyByOffset<float>(offset_x, value.x);
	SetNetworkPropertyByOffset<float>(offset_y, value.y);
}


Vector PlayerWrapper::GetViewVector()
{
	QAngle eye_angle = GetEyeAngle();

	float yaw = DEG2RAD(eye_angle.y);
	float pitch = DEG2RAD(eye_angle.x);

	float sy, cy, sp, cp;
	SinCos(yaw, &sy, &cy);
	SinCos(pitch, &sp, &cp);

	return Vector(cp * cy, cp * sy, -sp);
}

void PlayerWrapper::SetViewVector(Vector& value)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Setting view_vector is not implemented.");
}


QAngle PlayerWrapper::GetViewAngle()
{
	QAngle eye_angle = GetEyeAngle();
	return QAngle(
		eye_angle.x,
		eye_angle.y < 0 ? eye_angle.y + 360 : eye_angle.y,
		GetRotation().z);
}

void PlayerWrapper::SetViewAngle(QAngle& value)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Setting view_angle is not implemented.");
}


float PlayerWrapper::GetStamina()
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_flStamina");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetStamina(float value)
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_flStamina");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetShotsFired()
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_iShotsFired");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetShotsFired(int value)
{
	static int offset = FindNetworkPropertyOffset("cslocaldata.m_iShotsFired");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetArmor()
{
	static int offset = FindNetworkPropertyOffset("m_ArmorValue");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetArmor(int value)
{
	static int offset = FindNetworkPropertyOffset("m_ArmorValue");
	SetNetworkPropertyByOffset<int>(offset, value);
}


bool PlayerWrapper::GetHasDefuser()
{
	static int offset = FindNetworkPropertyOffset("m_bHasDefuser");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetHasDefuser(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasDefuser");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetHasHelmet()
{
	static int offset = FindNetworkPropertyOffset("m_bHasHelmet");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetHasHelmet(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasHelmet");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetHasNightvision()
{
	static int offset = FindNetworkPropertyOffset("m_bHasNightVision");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetHasNightvision(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bHasNightVision");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetIsInBombZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInBombZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsInBombZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInBombZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetIsInBuyZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInBuyZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsInBuyZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInBuyZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetIsInHostageRescueZone()
{
	static int offset = FindNetworkPropertyOffset("m_bInHostageRescueZone");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsInHostageRescueZone(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bInHostageRescueZone");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetIsDefusing()
{
	static int offset = FindNetworkPropertyOffset("m_bIsDefusing");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetIsDefusing(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bIsDefusing");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


bool PlayerWrapper::GetNightvisionOn()
{
	static int offset = FindNetworkPropertyOffset("m_bNightVisionOn");
	return GetNetworkPropertyByOffset<bool>(offset);
}

void PlayerWrapper::SetNightvisionOn(bool value)
{
	static int offset = FindNetworkPropertyOffset("m_bNightVisionOn");
	SetNetworkPropertyByOffset<bool>(offset, value);
}


float PlayerWrapper::GetFlashDuration()
{
	static int offset = FindNetworkPropertyOffset("m_flFlashDuration");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetFlashDuration(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFlashDuration");
	SetNetworkPropertyByOffset<float>(offset, value);
}


float PlayerWrapper::GetFlashAlpha()
{
	static int offset = FindNetworkPropertyOffset("m_flFlashMaxAlpha");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetFlashAlpha(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flFlashMaxAlpha");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetCash()
{
	static int offset = FindNetworkPropertyOffset("m_iAccount");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetCash(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iAccount");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetPlayerClass()
{
	static int offset = FindNetworkPropertyOffset(PLAYER_CLASS_PROPERTY);
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetPlayerClass(int value)
{
	static int offset = FindNetworkPropertyOffset(PLAYER_CLASS_PROPERTY);
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetPlayerState()
{
	static int offset = FindNetworkPropertyOffset("m_iPlayerState");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetPlayerState(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iPlayerState");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetRagdoll()
{
	static int offset = FindNetworkPropertyOffset("m_hRagdoll");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetRagdoll(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hRagdoll");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int PlayerWrapper::GetActiveDevices()
{
	static int offset = FindNetworkPropertyOffset("m_HL2Local.m_bitsActiveDevices");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetActiveDevices(int value)
{
	static int offset = FindNetworkPropertyOffset("m_HL2Local.m_bitsActiveDevices");
	SetNetworkPropertyByOffset<int>(offset, value);
}


float PlayerWrapper::GetSuitPowerLoad()
{
	static int offset = FindNetworkPropertyOffset("m_flSuitPowerLoad");
	return GetNetworkPropertyByOffset<float>(offset);
}

void PlayerWrapper::SetSuitPowerLoad(float value)
{
	static int offset = FindNetworkPropertyOffset("m_flSuitPowerLoad");
	SetNetworkPropertyByOffset<float>(offset, value);
}


int PlayerWrapper::GetDesiredPlayerClass()
{
	static int offset = FindNetworkPropertyOffset("m_Shared.m_iDesiredPlayerClass");
	return GetNetworkPropertyByOffset<int>(offset);
}

void PlayerWrapper::SetDesiredPlayerClass(int value)
{
	static int offset = FindNetworkPropertyOffset("m_Shared.m_iDesiredPlayerClass");
	SetNetworkPropertyByOffset<int>(offset, value);
}
