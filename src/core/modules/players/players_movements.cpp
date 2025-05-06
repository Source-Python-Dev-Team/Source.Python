/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2025 Source Python Development Team.  All rights reserved.
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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "modules/players/players_movements.h"


//-----------------------------------------------------------------------------
// CGameMovementWrapper class.
//-----------------------------------------------------------------------------
template<typename HookFunc, typename HookHandler>
CGameMovementListenerManager::CGameMovementListenerManager(HookFunc tFunc, HookHandler tHandler, HookType_t eType):
	m_pFunc(NULL), m_pHook(NULL)
{
	CFunctionInfo *pInfo = GetFunctionInfo(tFunc);
	if (pInfo) {
		m_pFunc = CPointer((unsigned long)GetGameMovement()).MakeVirtualFunction(*pInfo);
		delete pInfo;
	}

	m_pHandler = (HookHandlerFn *)tHandler;
	m_eType = eType;
}

CGameMovementListenerManager::~CGameMovementListenerManager()
{
	if (m_pFunc) {
		delete m_pFunc;
	}
}

void CGameMovementListenerManager::Initialize()
{
	if (!m_pFunc || !m_pFunc->IsHookable()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Function is invalid or not hookable."
		)
	}

	static CHookManager *s_pHookManager = GetHookManager();
	m_pHook = s_pHookManager->FindHook((void *)m_pFunc->m_ulAddr);
	if (!m_pHook) {
		m_pHook = s_pHookManager->HookFunction((void *)m_pFunc->m_ulAddr, m_pFunc->m_pCallingConvention);
	}

	if (!m_pHook) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Failed to hook the function."
		)
	}

	m_pHook->AddCallback(m_eType, m_pHandler);
};

void CGameMovementListenerManager::Finalize()
{
	if (!m_pHook) {
		m_pHook->RemoveCallback(m_eType, m_pHandler);
	}
}

inline CGameMovementWrapper *GetGameMovement()
{
	static CGameMovementWrapper *s_pGameMovement = (CGameMovementWrapper *)g_pGameMovement;
	return s_pGameMovement;
}


//-----------------------------------------------------------------------------
// Listeners.
//-----------------------------------------------------------------------------
DEFINE_MOVEMENT_LISTENER(OnPlayerJump, CheckJumpButton, POST)
{
	if (!pHook->GetReturnValue<bool>()) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerJump);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerLand, CheckFalling, PRE)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer || pPlayer->GetFallVelocity() <= 0.0) {
		return false;
	}

	CBaseEntity *pEntity;
	if (!BaseEntityFromIntHandle(pPlayer->GetGroundEntity(), pEntity)) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerLand);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerDuck, Duck, POST)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (pPlayer->GetIsDucked() || !pPlayer->GetIsDucking() || pPlayer->GetDuckTime() != GAMEMOVEMENT_DUCK_TIME) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerDuck);
}

DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerDucked, FinishDuck, PRE);

DEFINE_MOVEMENT_LISTENER(OnPlayerUnduck, Duck, POST)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer->GetIsDucked() || !pPlayer->GetIsDucking() || pPlayer->GetDuckTime() != GAMEMOVEMENT_DUCK_TIME) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerUnduck);
}

DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerUnducked, FinishUnDuck, PRE);

DEFINE_MOVEMENT_LISTENER(OnPlayerWaterJump, CheckWaterJump, POST)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!(pPlayer->GetFlags() & FL_WATERJUMP)) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerWaterJump);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerGroundEntityChanged, SetGroundEntity, PRE)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer) {
		return false;
	}

	CBaseEntity *pGround = NULL;
	BaseEntityFromIntHandle(pPlayer->GetGroundEntity(), pGround);

	CBaseEntity *pNewGround = NULL;
	trace_t *pTrace = pHook->GetArgument<trace_t *>(1 /* pm */);
	if (pTrace) {
		pNewGround = pTrace->m_pEnt;
	}

	if (pGround == pNewGround) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerGroundEntityChanged,
		GetEntityObject(pGround), GetEntityObject(pNewGround)
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerWaterLevelChanged, FullWalkMove, POST)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer) {
		return false;
	}
 
	unsigned char nWaterLevel = pPlayer->GetWaterLevel();
	int nOldWaterLevel = GetGameMovement()->m_nOldWaterLevel;
	if (nWaterLevel == nOldWaterLevel) {
		return false;
	}

	static object PlayerWaterLevel = import("players.constants").attr("PlayerWaterLevel");
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerWaterLevelChanged,
		PlayerWaterLevel(nOldWaterLevel), PlayerWaterLevel(nWaterLevel)
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerStartClimbingLadder, OnLadder, POST)
{
	if (!pHook->GetReturnValue<bool>()) {
		return false;
	}

	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer || pPlayer->GetMoveType() == MOVETYPE_LADDER) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerStartClimbingLadder);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerStopClimbingLadder, LadderMove, POST)
{
	if (pHook->GetReturnValue<bool>()) {
		return false;
	}

	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer || pPlayer->GetMoveType() != MOVETYPE_LADDER) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerStopClimbingLadder);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerCheckJumpButton, CheckJumpButton, PRE)
{
	FOREACH_CALLBACK_WITH_MNGR(
		GetOnPlayerCheckJumpButtonListenerManager(),
		object return_value,
		if (!return_value.is_none()) {
			pHook->SetReturnValue<bool>(extract<bool>(return_value));
			return true;
		},
		GetEntityObject(GetGameMovement()->player)
	);

	return false;
}

DEFINE_MOVEMENT_LISTENER(OnPlayerRoughLandingEffects, PlayerRoughLandingEffects, PRE)
{
	float fVolume = pHook->GetArgument<float>(1 /* fvol */);
	if (fVolume <= 0.0) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerRoughLandingEffects, fVolume);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerAccelerate, Accelerate, PRE)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerAccelerate, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerPostAccelerate, Accelerate, POST)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerPostAccelerate, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerAirAccelerate, AirAccelerate, PRE)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerAirAccelerate, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerPostAirAccelerate, AirAccelerate, POST)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerPostAirAccelerate, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);
}

DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerMove, PlayerMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishMove, ProcessMovement, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerWaterMove, WaterMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishWaterMove, WaterMove, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerAirMove, AirMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishAirMove, AirMove, POST, MOVEDATA_OBJECT);

DEFINE_MOVEMENT_LISTENER(OnPlayerFallMove, AirMove, PRE)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer || pPlayer->GetFallVelocity() <= 0.0) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerFallMove, MOVEDATA_OBJECT);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerFinishFallMove, CheckFalling, PRE)
{
	PlayerMixin *pPlayer = (PlayerMixin *)GetGameMovement()->player;
	if (!pPlayer || pPlayer->GetFallVelocity() <= 0.0) {
		return false;
	}

	NOTIFY_MOVEMENT_LISTENER(OnPlayerFinishFallMove, MOVEDATA_OBJECT);
}

DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerWalkMove, WalkMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishWalkMove, WalkMove, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFullWalkMove, FullWalkMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishFullWalkMove, FullWalkMove, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerTossMove, FullTossMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishTossMove, FullTossMove, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerLadderMove, LadderMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishLadderMove, LadderMove, POST, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFullLadderMove, FullLadderMove, PRE, MOVEDATA_OBJECT);
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerFinishFullLadderMove, FullLadderMove, POST, MOVEDATA_OBJECT);

DEFINE_MOVEMENT_LISTENER(OnPlayerStepMove, StepMove, PRE)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerStepMove, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* vecDestination */)),
		object(ptr(pHook->GetArgument<trace_t *>(2 /* trace */)))
	);
}

DEFINE_MOVEMENT_LISTENER(OnPlayerFinishStepMove, StepMove, POST)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerFinishStepMove, MOVEDATA_OBJECT,
		object(pHook->GetArgument<Vector>(1 /* vecDestination */)),
		object(ptr(pHook->GetArgument<trace_t *>(2 /* trace */)))
	);
}
