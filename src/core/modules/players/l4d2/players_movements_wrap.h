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

#ifndef _PLAYERS_MOVEMENTS_L4D2_WRAP_H
#define _PLAYERS_MOVEMENTS_L4D2_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/players/players_movements.h"


//-----------------------------------------------------------------------------
// Exports CGameMovementWrapper.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_game_movement(T _movements, U GameMovement)
{
	// Properties...
	GameMovement.def_readwrite("proximity_mins", &CGameMovementWrapper::m_vecProximityMins);
	GameMovement.def_readwrite("proximity_maxs", &CGameMovementWrapper::m_vecProximityMaxs);
	GameMovement.def_readwrite("frame_time", &CGameMovementWrapper::m_fFrameTime);
	GameMovement.def_readwrite("speed_cropped", &CGameMovementWrapper::m_bSpeedCropped);

	// Methods...
	GameMovement.def(
		"get_player_mins",
		GET_CONST_METHOD(const Vector &, CGameMovementWrapper, GetPlayerMins),
		reference_existing_object_policy()
	);
	GameMovement.def(
		"get_player_mins",
		GET_CONST_METHOD(const Vector &, CGameMovementWrapper, GetPlayerMins, bool),
		reference_existing_object_policy()
	);
	GameMovement.def(
		"get_player_maxs",
		GET_CONST_METHOD(const Vector &, CGameMovementWrapper, GetPlayerMaxs),
		reference_existing_object_policy()
	);
	GameMovement.def(
		"get_player_maxs",
		GET_CONST_METHOD(const Vector &, CGameMovementWrapper, GetPlayerMaxs, bool),
		reference_existing_object_policy()
	);
	GameMovement.def(
		"get_player_view_offset",
		&CGameMovementWrapper::GetPlayerViewOffset,
		reference_existing_object_policy()
	);
	GameMovement.NOT_IMPLEMENTED("try_touch_ground");
	GameMovement.NOT_IMPLEMENTED("get_air_speed_cap");
	GameMovement.NOT_IMPLEMENTED("on_jump");
	GameMovement.NOT_IMPLEMENTED("on_land");
	GameMovement.NOT_IMPLEMENTED("check_stuck");

	// Class info...
	BEGIN_CLASS_INFO_WRAPPER(CGameMovementWrapper, CGameMovement)
		BEGIN_FUNCTION_INFO_LIST("GetPlayerMins")
			FUNCTION_INFO_CONST_OVERLOAD(const Vector &, GetPlayerMins)
			FUNCTION_INFO_CONST_OVERLOAD(const Vector &, GetPlayerMins, bool)
		END_FUNCTION_INFO_LIST()
		BEGIN_FUNCTION_INFO_LIST("GetPlayerMaxs")
			FUNCTION_INFO_CONST_OVERLOAD(const Vector &, GetPlayerMaxs)
			FUNCTION_INFO_CONST_OVERLOAD(const Vector &, GetPlayerMaxs, bool)
		END_FUNCTION_INFO_LIST()
	END_CLASS_INFO()
}


#endif // _PLAYERS_MOVEMENTS_L4D2_WRAP_H
