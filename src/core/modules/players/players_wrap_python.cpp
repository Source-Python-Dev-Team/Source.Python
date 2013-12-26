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

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "players_generator_wrap.h"
#include "players_wrap.h"
#include "modules/entities/entities_wrap.h"
#include "modules/export_main.h"

// ----------------------------------------------------------------------------
// Namespaces to use.
// ----------------------------------------------------------------------------
using namespace boost::python;

// ----------------------------------------------------------------------------
// Exposer functions.
// ----------------------------------------------------------------------------
void export_playerinfo();
void export_netinfo();
void export_player_generator();

// ----------------------------------------------------------------------------
// Entity module definition.
// ----------------------------------------------------------------------------
DECLARE_SP_MODULE(player_c)
{
	export_playerinfo();
	export_netinfo();
	export_player_generator();
}

// ----------------------------------------------------------------------------
// Exports CBaseEntityHandle.
// ----------------------------------------------------------------------------
void export_playerinfo()
{
	BOOST_CLASS_CONSTRUCTOR(CPlayerInfo, int)

		CLASS_CONSTRUCTOR(CEdict*)

		CLASS_METHOD(CPlayerInfo,
			get_name,
			"Returns the player's name"
		)

		CLASS_METHOD(CPlayerInfo,
			get_userid,
			"Returns the player's userid."
		)

		CLASS_METHOD(CPlayerInfo,
			get_networkid_string,
			"Returns the player's SteamID."
		)

		CLASS_METHOD(CPlayerInfo,
			get_team_index,
			"Returns the player's team number."
		)

		CLASS_METHOD(CPlayerInfo,
			change_team,
			"Moves the player to the given team.",
			args("iTeamNum")
		)

		CLASS_METHOD(CPlayerInfo,
			get_frag_count,
			"Returns the player's frag count."
		)

		CLASS_METHOD(CPlayerInfo,
			get_death_count,
			"Returns the player's death count."
		)

		CLASS_METHOD(CPlayerInfo,
			is_connected,
			"Returns whether the player is connected."
		)

		CLASS_METHOD(CPlayerInfo,
			get_armor_value,
			"Returns the player's armor value."
		)

		CLASS_METHOD(CPlayerInfo,
			is_hltv,
			"Returns whether the player is HLTV."
		)

		CLASS_METHOD(CPlayerInfo,
			is_player,
			"Returns whether the player is a client."
		)

		CLASS_METHOD(CPlayerInfo,
			is_fake_client,
			"Returns whether the player is a fake client."
		)

		CLASS_METHOD(CPlayerInfo,
			is_dead,
			"Returns whether the player is dead."
		)

		CLASS_METHOD(CPlayerInfo,
			is_in_a_vehicle,
			"Returns whether the player is in a vehicle."
		)

		CLASS_METHOD(CPlayerInfo,
			is_observer,
			"Returns whether the player is an observer."
		)

		CLASS_METHOD(CPlayerInfo,
			get_abs_origin,
			"Returns the player's origin Vector instance."
		)

		CLASS_METHOD(CPlayerInfo,
			get_abs_angles,
			"Returns the player's angle QAngle instance."
		)

		CLASS_METHOD(CPlayerInfo,
			get_player_mins,
			"Returns the player's min Vector instance."
		)

		CLASS_METHOD(CPlayerInfo,
			get_player_maxs,
			"Returns the player's max Vector instance."
		)

		CLASS_METHOD(CPlayerInfo,
			get_weapon_name,
			"Returns the name of the weapon the player is carrying."
		)

		CLASS_METHOD(CPlayerInfo,
			get_model_name,
			"Returns the name of the player's model."
		)

		CLASS_METHOD(CPlayerInfo,
			get_health,
			"Returns the player's health."
		)

		CLASS_METHOD(CPlayerInfo,
			get_max_health,
			"Returns the player's maximum health."
		)

		CLASS_METHOD(CPlayerInfo,
			get_edict,
			"Returns the player's CEdict instance.",
			reference_existing_object_policy()
		)

	BOOST_END_CLASS()
}

void export_netinfo()
{
	BOOST_ABSTRACT_CLASS(CNetChannelInfo)

		CLASS_METHOD(CNetChannelInfo,
			get_address,
			"Returns the net address of the player."
		)

		CLASS_METHOD(CNetChannelInfo,
			get_time_connected,
			"Returns the amount of time the player has been connected."
		)

	BOOST_END_CLASS()
}

void export_player_generator()
{
	BOOST_GENERATOR_CLASS(CPlayerGenerator)
	BOOST_END_CLASS()
}
