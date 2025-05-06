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
// Listeners.
//-----------------------------------------------------------------------------
DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerMove, PlayerMove, PRE)
DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerFinishMove, ProcessMovement, POST)
DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerWaterMove, WaterMove, PRE)
DEFINE_MOVEMENT_LISTENER(OnPlayerAirAccelerate, AirAccelerate, PRE)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerAirAccelerate,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);

	return false;
}

DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerAirMove, AirMove, PRE)
DEFINE_MOVEMENT_LISTENER(OnPlayerAccelerate, Accelerate, PRE)
{
	NOTIFY_MOVEMENT_LISTENER(
		OnPlayerAccelerate,
		object(pHook->GetArgument<Vector>(1 /* wishdir */)),
		pHook->GetArgument<float>(2 /* wishspeed */), pHook->GetArgument<float>(3 /* accel */)
	);

	return false;
}

DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerWalkMove, FullWalkMove, PRE)
DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerTossMove, FullTossMove, PRE)
DEFINE_GENERIC_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerLadderMove, FullLadderMove, PRE)
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerDuck, FinishDuck, PRE)
DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerUnduck, FinishUnDuck, PRE)

DEFINE_MOVEMENT_LISTENER(OnPlayerStepMove, StepMove, PRE)
{
	NOTIFY_MOVEMENT_LISTENER_WITH_MOVEDATA(
		OnPlayerStepMove,
		object(pHook->GetArgument<Vector>(1 /* vecDestination */)),
		object(ptr(reinterpret_cast<trace_t *>(pHook->GetArgument<unsigned long>(2 /* trace */))))
	);

	return false;
}

DEFINE_MOVEMENT_LISTENER(OnPlayerFallMove, AirMove, PRE)
{
	if (GetGameMovement()->GetPlayerMixin()->GetFallVelocity() > 0.0) {
		NOTIFY_MOVEMENT_LISTENER_WITH_MOVEDATA(OnPlayerFallMove);
	}

	return false;
}

DEFINE_MOVEMENT_LISTENER(OnPlayerGroundChanged, SetGroundEntity, PRE)
{
	trace_t *pTrace = pHook->GetArgument<trace_t *>(1 /* pm */);
	CBaseEntity *pNew = pTrace ? pTrace->m_pEnt : NULL;
	CBaseEntity *pCurrent = NULL;
	BaseEntityFromIntHandle(GetGameMovement()->GetPlayerMixin()->GetGroundEntity(), pCurrent);
	if (pCurrent != pNew) {
		NOTIFY_MOVEMENT_LISTENER(
			OnPlayerGroundChanged, GetEntityObject(pCurrent), GetEntityObject(pNew)
		);
	}

	return false;
}

DEFINE_GENERIC_MOVEMENT_LISTENER(OnPlayerCheckJumpButton, CheckJumpButton, PRE)


//-----------------------------------------------------------------------------
// CGameMovementBaseWrapper class.
//-----------------------------------------------------------------------------
PlayerMixin *CGameMovementWrapper::GetPlayerMixin()
{
	return reinterpret_cast<PlayerMixin *>(GetGameMovement()->player);
}

CGameMovementWrapper *GetGameMovement()
{
	static CGameMovementWrapper *s_pGameMovement =
		reinterpret_cast<CGameMovementWrapper *>(g_pGameMovement);
	return s_pGameMovement;
}


//-----------------------------------------------------------------------------
// CGameMovementListenerManager class.
//-----------------------------------------------------------------------------
void CGameMovementListenerManager::Initialize()
{
	if (!m_pInfo) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Failed to retrieve the function's info."
		)
	}

	CFunction *pFunc = CPointer((unsigned long)((void *)GetGameMovement())).MakeVirtualFunction(*m_pInfo);
	if (!pFunc || !pFunc->IsHookable()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Function is invalid or not hookable."
		)
	}

	static CHookManager *s_pHookManager = GetHookManager();
	void *pAddr = (void *)pFunc->m_ulAddr;
	m_pHook = s_pHookManager->FindHook(pAddr);
	if (!m_pHook) {
		m_pHook = s_pHookManager->HookFunction(pAddr, pFunc->m_pCallingConvention);
		if (!m_pHook) {
			delete pFunc;
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook the function."
			)
		}
	}

	delete pFunc;
	m_pHook->AddCallback(m_eType, m_pHandler);
}

void CGameMovementListenerManager::Finalize()
{
	if (!m_pHook) {
		m_pHook->RemoveCallback(m_eType, m_pHandler);
	}
}
