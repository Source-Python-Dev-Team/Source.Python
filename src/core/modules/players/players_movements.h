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

#ifndef _PLAYERS_MOVEMENTS_H
#define _PLAYERS_MOVEMENTS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "sp_main.h"
#include "utilities/baseplayer.h"
#include "modules/entities/entities_entity.h"
#include "modules/listeners/listeners_manager.h"
#include "modules/memory/memory_hooks.h"
#include "modules/players/players_entity.h"

// SDK
#include "networkvar.h"
#include "basehandle.h"
#include "game/shared/shareddefs.h"
#include "game/shared/gamemovement.h"

#include ENGINE_INCLUDE_PATH(players_movements.h)


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IGameMovement *g_pGameMovement;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
class CGameMovementListenerManager;
class CGameMovementWrapper;


//-----------------------------------------------------------------------------
// Helper macros.
//-----------------------------------------------------------------------------
#define DECLARE_MOVEMENT_LISTENER(name) \
	CListenerManager *Get##name##ListenerManager(); \
	extern CListenerManager *Get##name##ListenerManager(); \
	bool name##HookHandler(HookType_t eHookType, CHook *pHook);

#define DEFINE_MOVEMENT_LISTENER(name, hook, type) \
	DECLARE_MOVEMENT_LISTENER(name); \
	CListenerManager *Get##name##ListenerManager() \
	{ \
		static CGameMovementListenerManager *s_pManager = new CGameMovementListenerManager( \
			&CGameMovementWrapper::##hook, \
			&name##HookHandler, \
			HOOKTYPE_##type \
		); \
		return s_pManager; \
	} \
	bool name##HookHandler(HookType_t eHookType, CHook *pHook)

#define DEFINE_GENERIC_MOVEMENT_LISTENER(name, hook, type, ...) \
	DEFINE_MOVEMENT_LISTENER(name, hook, type) \
	{ \
		NOTIFY_MOVEMENT_LISTENER(name, __VA_ARGS__); \
		return false; \
	}

#define NOTIFY_MOVEMENT_LISTENER(name, ...) \
	FOREACH_CALLBACK_WITH_MNGR( \
		Get##name##ListenerManager(), \
		object return_value, \
		if (!return_value.is_none() && !extract<bool>(return_value)) return true;, \
		GetEntityObject(GetGameMovement()->player), \
		__VA_ARGS__ \
	); \
	return false;

#define MOVEDATA_OBJECT object(ptr(GetGameMovement()->mv))


//-----------------------------------------------------------------------------
// Listeners.
//-----------------------------------------------------------------------------
DECLARE_MOVEMENT_LISTENER(OnPlayerJump);
DECLARE_MOVEMENT_LISTENER(OnPlayerLand);
DECLARE_MOVEMENT_LISTENER(OnPlayerDuck);
DECLARE_MOVEMENT_LISTENER(OnPlayerDucked);
DECLARE_MOVEMENT_LISTENER(OnPlayerUnduck);
DECLARE_MOVEMENT_LISTENER(OnPlayerUnducked);
DECLARE_MOVEMENT_LISTENER(OnPlayerWaterJump);
DECLARE_MOVEMENT_LISTENER(OnPlayerGroundEntityChanged);
DECLARE_MOVEMENT_LISTENER(OnPlayerWaterLevelChanged);
DECLARE_MOVEMENT_LISTENER(OnPlayerStartClimbingLadder);
DECLARE_MOVEMENT_LISTENER(OnPlayerStopClimbingLadder);
DECLARE_MOVEMENT_LISTENER(OnPlayerCheckJumpButton);
DECLARE_MOVEMENT_LISTENER(OnPlayerRoughLandingEffects);
DECLARE_MOVEMENT_LISTENER(OnPlayerAccelerate);
DECLARE_MOVEMENT_LISTENER(OnPlayerPostAccelerate);
DECLARE_MOVEMENT_LISTENER(OnPlayerAirAccelerate);
DECLARE_MOVEMENT_LISTENER(OnPlayerPostAirAccelerate);
DECLARE_MOVEMENT_LISTENER(OnPlayerMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerWaterMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishWaterMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerAirMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishAirMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFallMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishFallMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerWalkMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishWalkMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFullWalkMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishFullWalkMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerTossMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishTossMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerLadderMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishLadderMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFullLadderMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishFullLadderMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerStepMove);
DECLARE_MOVEMENT_LISTENER(OnPlayerFinishStepMove);


//-----------------------------------------------------------------------------
// CGameMovementBaseWrapper class.
//-----------------------------------------------------------------------------
class CGameMovementWrapper: public CGameMovementBaseWrapper
{
public:
	// Members...
	using CGameMovement::player;
	using CGameMovement::mv;
	using CGameMovement::m_nOldWaterLevel;
	using CGameMovement::m_flWaterEntryTime;
	using CGameMovement::m_nOnLadder;
	using CGameMovement::m_vecForward;
	using CGameMovement::m_vecRight;
	using CGameMovement::m_vecUp;

	// Methods...
	using CGameMovement::GetPlayerMins;
	using CGameMovement::GetPlayerMaxs;
	using CGameMovement::PlayerSolidMask;
	using CGameMovement::PlayerMove;
	using CGameMovement::CalcRoll;
	using CGameMovement::DecayPunchAngle;
	using CGameMovement::CheckWaterJump;
	using CGameMovement::WaterMove;
	using CGameMovement::AirAccelerate;
	using CGameMovement::AirMove;
	using CGameMovement::CanAccelerate;
	using CGameMovement::Accelerate;
	using CGameMovement::WalkMove;
	using CGameMovement::FullWalkMove;
	using CGameMovement::OnTryPlayerMoveCollision;
	using CGameMovement::GetCheckInterval;
	using CGameMovement::CheckJumpButton;
	using CGameMovement::FullTossMove;
	using CGameMovement::FullLadderMove;
	using CGameMovement::TryPlayerMove;
	using CGameMovement::LadderMove;
	using CGameMovement::OnLadder;
	using CGameMovement::LadderDistance;
	using CGameMovement::LadderMask;
	using CGameMovement::ClimbSpeed;
	using CGameMovement::LadderLateralMultiplier;
	using CGameMovement::CheckWater;
	using CGameMovement::CategorizePosition;
	using CGameMovement::CheckParameters;
	using CGameMovement::ReduceTimers;
	using CGameMovement::CheckFalling;
	using CGameMovement::PlayerRoughLandingEffects;
	using CGameMovement::Duck;
	using CGameMovement::HandleDuckingSpeedCrop;
	using CGameMovement::FinishUnDuck;
	using CGameMovement::FinishDuck;
	using CGameMovement::CanUnduck;
	using CGameMovement::TestPlayerPosition;
	using CGameMovement::SetGroundEntity;
	using CGameMovement::StepMove;
	using CGameMovement::GameHasLadders;
};

inline CGameMovementWrapper *GetGameMovement();


//-----------------------------------------------------------------------------
// CGameMovementListenerManager class.
//-----------------------------------------------------------------------------
class CGameMovementListenerManager: public CListenerManager
{
public:
	template<typename HookFunc, typename HookHandler>
	CGameMovementListenerManager(HookFunc tFunc, HookHandler tHandler, HookType_t eType);
	~CGameMovementListenerManager();

	virtual void Initialize();
	virtual void Finalize();

private:
	CFunction *m_pFunc;
	CHook *m_pHook;
	HookHandlerFn *m_pHandler;
	HookType_t m_eType;
};


#endif // _PLAYERS_MOVEMENTS_H
