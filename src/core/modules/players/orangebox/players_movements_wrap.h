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

#ifndef _PLAYERS_MOVEMENTS_ORANGEBOX_WRAP_H
#define _PLAYERS_MOVEMENTS_ORANGEBOX_WRAP_H

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
	GameMovement.def_readwrite("speed_cropped", &CGameMovementWrapper::m_iSpeedCropped);

	// Methods...
	GameMovement.def("get_player_mins", GET_CONST_METHOD(Vector, CGameMovementWrapper, GetPlayerMins));
	GameMovement.def("get_player_mins", GET_CONST_METHOD(Vector, CGameMovementWrapper, GetPlayerMins, bool));
	GameMovement.def("get_player_maxs", GET_CONST_METHOD(Vector, CGameMovementWrapper, GetPlayerMaxs));
	GameMovement.def("get_player_maxs", GET_CONST_METHOD(Vector, CGameMovementWrapper, GetPlayerMaxs, bool));
	GameMovement.def("try_touch_ground", &CGameMovementWrapper::TryTouchGround);
	GameMovement.def("get_air_speed_cap", &CGameMovementWrapper::GetAirSpeedCap);
	GameMovement.def("on_jump", &CGameMovementWrapper::OnJump);
	GameMovement.def("on_land", &CGameMovementWrapper::OnLand);
	GameMovement.def("check_stuck", &CGameMovementWrapper::CheckStuck);

	// Class info...
	BEGIN_CLASS_INFO_WRAPPER(CGameMovementWrapper, CGameMovement)
		BEGIN_FUNCTION_INFO_LIST("GetPlayerMins")
			FUNCTION_INFO_CONST_OVERLOAD(Vector, GetPlayerMins)
			FUNCTION_INFO_CONST_OVERLOAD(Vector, GetPlayerMins, bool)
		END_FUNCTION_INFO_LIST()
		BEGIN_FUNCTION_INFO_LIST("GetPlayerMaxs")
			FUNCTION_INFO_CONST_OVERLOAD(Vector, GetPlayerMaxs)
			FUNCTION_INFO_CONST_OVERLOAD(Vector, GetPlayerMaxs, bool)
		END_FUNCTION_INFO_LIST()
		FUNCTION_INFO(TryTouchGround)
		FUNCTION_INFO(GetAirSpeedCap)
		FUNCTION_INFO(OnJump)
		FUNCTION_INFO(OnLand)
		FUNCTION_INFO(CheckStuck)
	END_CLASS_INFO()
}


#endif // _PLAYERS_MOVEMENTS_ORANGEBOX_WRAP_H
