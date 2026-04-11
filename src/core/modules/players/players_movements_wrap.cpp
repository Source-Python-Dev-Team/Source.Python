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
#include "export_main.h"
#include "modules/entities/entities_entity.h"
#include "modules/players/players_movements.h"

#include ENGINE_INCLUDE_PATH(players_movements_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_move_data(scope);
void export_game_movement(scope);


//-----------------------------------------------------------------------------
// Declare the _players._movements module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _movements)
{
	export_move_data(_movements);
	export_game_movement(_movements);
}


//-----------------------------------------------------------------------------
// Exports CMoveData.
//-----------------------------------------------------------------------------
void export_move_data(scope _movements)
{
	class_<CMoveData> MoveData("MoveData");

	// Constructors...
	MoveData.def(init<CMoveData &>());

	// Properties...
	MoveData.def_readwrite("player_basehandle", &CMoveData::m_nPlayerHandle);
	MoveData.def_readwrite("impulse_command", &CMoveData::m_nImpulseCommand);
	MoveData.add_property(
		"view_angles",
		make_getter(
			&CMoveData::m_vecViewAngles,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecViewAngles)
	);
	MoveData.add_property(
		"absolute_view_angles",
		make_getter(
			&CMoveData::m_vecAbsViewAngles,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecAbsViewAngles)
	);
	MoveData.def_readwrite("buttons", &CMoveData::m_nButtons);
	MoveData.def_readwrite("old_buttons", &CMoveData::m_nOldButtons);
	MoveData.def_readwrite("forward_move", &CMoveData::m_flForwardMove);
	MoveData.def_readwrite("side_move", &CMoveData::m_flSideMove);
	MoveData.def_readwrite("up_move", &CMoveData::m_flUpMove);
	MoveData.def_readwrite("max_speed", &CMoveData::m_flMaxSpeed);
	MoveData.def_readwrite("client_max_speed", &CMoveData::m_flClientMaxSpeed);
	MoveData.add_property(
		"velocity",
		make_getter(
			&CMoveData::m_vecVelocity,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecVelocity)
	);
	MoveData.add_property(
		"angles",
		make_getter(
			&CMoveData::m_vecAngles,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecAngles)
	);
	MoveData.add_property(
		"old_angles",
		make_getter(
			&CMoveData::m_vecOldAngles,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecOldAngles)
	);
	MoveData.def_readwrite("output_step_height", &CMoveData::m_outStepHeight);
	MoveData.add_property(
		"output_wish_velocity",
		make_getter(
			&CMoveData::m_outWishVel,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_outWishVel)
	);
	MoveData.add_property(
		"output_jump_velocity",
		make_getter(
			&CMoveData::m_outJumpVel,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_outJumpVel)
	);
	MoveData.add_property(
		"constraint_center",
		make_getter(
			&CMoveData::m_vecConstraintCenter,
			reference_existing_object_policy()
		),
		make_setter(&CMoveData::m_vecConstraintCenter)
	);
	MoveData.def_readwrite("constraint_radius", &CMoveData::m_flConstraintRadius);
	MoveData.def_readwrite("constraint_width", &CMoveData::m_flConstraintWidth);
	MoveData.def_readwrite("constraint_speed_factor", &CMoveData::m_flConstraintSpeedFactor);
	MoveData.add_property(
		"absolute_origin",
		make_function(
			&CMoveData::GetAbsOrigin,
			reference_existing_object_policy()
		),
		&CMoveData::SetAbsOrigin
	);

	// Memory tools...
	MoveData ADD_MEM_TOOLS(CMoveData);
}


//-----------------------------------------------------------------------------
// Exports CGameMovementWrapper.
//-----------------------------------------------------------------------------
void export_game_movement(scope _movements)
{
	class_<CGameMovementWrapper, boost::noncopyable> GameMovement("GameMovement", no_init);

	// Converters...
	converter::registry::insert(
		+[](PyObject *pSelf) -> void * {
			return extract<CGameMovementWrapper *>(pSelf);
		},
		type_id<CGameMovement>()
	);

	// Properties...
	GameMovement.def_readwrite("player", &CGameMovementWrapper::player);
	GameMovement.add_property(
		"move_data",
		make_function(
			&CGameMovementWrapper::GetMoveData,
			reference_existing_object_policy()
		),
		make_setter(&CGameMovementWrapper::mv)
	);
	GameMovement.def_readwrite("old_water_level", &CGameMovementWrapper::m_nOldWaterLevel);
	GameMovement.def_readwrite("water_entry_time", &CGameMovementWrapper::m_flWaterEntryTime);
	GameMovement.def_readwrite("on_ladder", &CGameMovementWrapper::m_nOnLadder);
	GameMovement.def_readwrite("forward", &CGameMovementWrapper::m_vecForward);
	GameMovement.def_readwrite("right", &CGameMovementWrapper::m_vecRight);
	GameMovement.def_readwrite("up", &CGameMovementWrapper::m_vecUp);

	// Methods...
	GameMovement.def("process_movement", &CGameMovementWrapper::ProcessMovement);
	GameMovement.def("start_track_prediction_errors", &CGameMovementWrapper::StartTrackPredictionErrors);
	GameMovement.def("finish_track_prediction_errors", &CGameMovementWrapper::FinishTrackPredictionErrors);
	GameMovement.def("trace_player_bbox", &CGameMovementWrapper::TracePlayerBBox);
	GameMovement.def("player_solid_mask", &CGameMovementWrapper::PlayerSolidMask);
	GameMovement.def("player_move", &CGameMovementWrapper::PlayerMove);
	GameMovement.def("calc_roll", &CGameMovementWrapper::CalcRoll);
	GameMovement.def("decay_punch_angle", &CGameMovementWrapper::DecayPunchAngle);
	GameMovement.def("check_water_jump", &CGameMovementWrapper::CheckWaterJump);
	GameMovement.def("water_move", &CGameMovementWrapper::WaterMove);
	GameMovement.def("air_accelerate", &CGameMovementWrapper::AirAccelerate);
	GameMovement.def("air_move", &CGameMovementWrapper::AirMove);
	GameMovement.def("can_accelerate", &CGameMovementWrapper::CanAccelerate);
	GameMovement.def("accelerate", &CGameMovementWrapper::Accelerate);
	GameMovement.def("walk_move", &CGameMovementWrapper::WalkMove);
	GameMovement.def("full_walk_move", &CGameMovementWrapper::FullWalkMove);
	GameMovement.def("on_try_player_collision", &CGameMovementWrapper::OnTryPlayerMoveCollision);
	GameMovement.def("get_check_interval", &CGameMovementWrapper::GetCheckInterval);
	GameMovement.def("check_jump_button", &CGameMovementWrapper::CheckJumpButton);
	GameMovement.def("full_toss_move", &CGameMovementWrapper::FullTossMove);
	GameMovement.def("full_ladder_move", &CGameMovementWrapper::FullLadderMove);
	GameMovement.def("try_player_move", &CGameMovementWrapper::TryPlayerMove);
	GameMovement.def("ladder_move", &CGameMovementWrapper::LadderMove);
	GameMovement.def("on_ladder", &CGameMovementWrapper::OnLadder);
	GameMovement.def("ladder_distance", &CGameMovementWrapper::LadderDistance);
	GameMovement.def("ladder_mask", &CGameMovementWrapper::LadderMask);
	GameMovement.def("climb_speed", &CGameMovementWrapper::ClimbSpeed);
	GameMovement.def("ladder_lateral_multiplier", &CGameMovementWrapper::LadderLateralMultiplier);
	GameMovement.def("check_water", &CGameMovementWrapper::CheckWater);
	GameMovement.def("categorize_position", &CGameMovementWrapper::CategorizePosition);
	GameMovement.def("check_parameters", &CGameMovementWrapper::CheckParameters);
	GameMovement.def("reduce_timers", &CGameMovementWrapper::ReduceTimers);
	GameMovement.def("check_falling", &CGameMovementWrapper::CheckFalling);
	GameMovement.def("player_rough_landing_effects", &CGameMovementWrapper::PlayerRoughLandingEffects);
	GameMovement.def("duck", &CGameMovementWrapper::Duck);
	GameMovement.def("handle_ducking_speed_crop", &CGameMovementWrapper::HandleDuckingSpeedCrop);
	GameMovement.def("finish_unduck", &CGameMovementWrapper::FinishUnDuck);
	GameMovement.def("finish_duck", &CGameMovementWrapper::FinishDuck);
	GameMovement.def("can_unduck", &CGameMovementWrapper::CanUnduck);
	GameMovement.def("test_player_position", &CGameMovementWrapper::TestPlayerPosition);
	GameMovement.def("set_ground_entity", &CGameMovementWrapper::SetGroundEntity);
	GameMovement.def("step_move", &CGameMovementWrapper::StepMove);
	GameMovement.def("game_has_ladders", &CGameMovementWrapper::GameHasLadders);

	// Singleton...
	_movements.attr("game_movement") = object(ptr(GetGameMovement()));

	// Memory tools...
	GameMovement ADD_MEM_TOOLS_WRAPPER(CGameMovementWrapper, CGameMovement);

	// Class info...
	BEGIN_CLASS_INFO_WRAPPER(CGameMovementWrapper, CGameMovement)
		FUNCTION_INFO(ProcessMovement)
		FUNCTION_INFO(StartTrackPredictionErrors)
		FUNCTION_INFO(FinishTrackPredictionErrors)
		FUNCTION_INFO(DiffPrint)
		FUNCTION_INFO(GetPlayerViewOffset)
		FUNCTION_INFO(TracePlayerBBox)
		FUNCTION_INFO(PlayerSolidMask)
		FUNCTION_INFO(PlayerMove)
		FUNCTION_INFO(CalcRoll)
		FUNCTION_INFO(DecayPunchAngle)
		FUNCTION_INFO(CheckWaterJump)
		FUNCTION_INFO(WaterMove)
		FUNCTION_INFO(AirAccelerate)
		FUNCTION_INFO(AirMove)
		FUNCTION_INFO(CanAccelerate)
		FUNCTION_INFO(Accelerate)
		FUNCTION_INFO(WalkMove)
		FUNCTION_INFO(FullWalkMove)
		FUNCTION_INFO(OnTryPlayerMoveCollision)
		FUNCTION_INFO(GetCheckInterval)
		FUNCTION_INFO(CheckJumpButton)
		FUNCTION_INFO(FullTossMove)
		FUNCTION_INFO(FullLadderMove)
		FUNCTION_INFO(TryPlayerMove)
		FUNCTION_INFO(LadderMove)
		FUNCTION_INFO(OnLadder)
		FUNCTION_INFO(LadderDistance)
		FUNCTION_INFO(LadderMask)
		FUNCTION_INFO(ClimbSpeed)
		FUNCTION_INFO(LadderLateralMultiplier)
		FUNCTION_INFO(CheckWater)
		FUNCTION_INFO(CategorizePosition)
		FUNCTION_INFO(CheckParameters)
		FUNCTION_INFO(ReduceTimers)
		FUNCTION_INFO(CheckFalling)
		FUNCTION_INFO(PlayerRoughLandingEffects)
		FUNCTION_INFO(Duck)
		FUNCTION_INFO(HandleDuckingSpeedCrop)
		FUNCTION_INFO(FinishUnDuck)
		FUNCTION_INFO(FinishDuck)
		FUNCTION_INFO(CanUnduck)
		FUNCTION_INFO(TestPlayerPosition)
		FUNCTION_INFO(SetGroundEntity)
		FUNCTION_INFO(StepMove)
		FUNCTION_INFO(GameHasLadders)
	END_CLASS_INFO()

	// Engine specific stuff...
	export_engine_specific_game_movement(_movements, GameMovement);
}
