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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "players_generator.h"
#include "export_main.h"
#include "modules/memory/memory_tools.h"

#include "game/shared/shareddefs.h"
#include "game/shared/usercmd.h"
#include "public/game/server/iplayerinfo.h"
#include "iclient.h"
#include "inetchannelinfo.h"
#include "inetchannel.h"
#include "players_wrap.h"
#include "players_entity.h"

#include ENGINE_INCLUDE_PATH(players_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_playerinfo(scope);
void export_player_generator(scope);
void export_client(scope);
void export_user_cmd(scope);
void export_player_wrapper(scope);


//-----------------------------------------------------------------------------
// Declare the _players module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_players)
{
	export_playerinfo(_players);
	export_player_generator(_players);
	export_client(_players);
	export_user_cmd(_players);
	export_player_wrapper(_players);
}


//-----------------------------------------------------------------------------
// Exports IPlayerInfo.
//-----------------------------------------------------------------------------
void export_playerinfo(scope _players)
{
	class_<IPlayerInfo, boost::noncopyable>("PlayerInfo", no_init)
		.add_property("name",
			&IPlayerInfoExt::GetName,
			"Returns the player's name"
		)

		.add_property("userid",
			&IPlayerInfo::GetUserID,
			"Returns the player's userid."
		)

		.add_property("steamid",
			&IPlayerInfo::GetNetworkIDString,
			"Returns the player's SteamID."
		)

		.add_property("team",
			&IPlayerInfo::GetTeamIndex,
			&IPlayerInfo::ChangeTeam,
			"Returns the player's team number."
		)

		.add_property("kills",
			&IPlayerInfo::GetFragCount,
			"Returns the player's frag count."
		)
		
		.add_property("deaths",
			&IPlayerInfo::GetDeathCount,
			"Returns the player's death count."
		)

		.def("is_connected",
			&IPlayerInfo::IsConnected,
			"Returns whether the player is connected."
		)

		.add_property("armor",
			&IPlayerInfo::GetArmorValue,
			"Returns the player's armor value."
		)

		.def("is_hltv",
			&IPlayerInfo::IsHLTV,
			"Returns whether the player is HLTV."
		)

		.def("is_player",
			&IPlayerInfo::IsPlayer,
			"Returns whether the player is a client."
		)

		.def("is_fake_client",
			&IPlayerInfo::IsFakeClient,
			"Returns whether the player is a fake client."
		)

		.def("is_dead",
			&IPlayerInfo::IsDead,
			"Returns whether the player is dead."
		)

		.def("is_in_a_vehicle",
			&IPlayerInfo::IsInAVehicle,
			"Returns whether the player is in a vehicle."
		)

		.def("is_observer",
			&IPlayerInfo::IsObserver,
			"Returns whether the player is an observer."
		)

		.add_property("origin",
			&IPlayerInfo::GetAbsOrigin,
			"Returns the player's origin Vector instance."
		)

		.add_property("angles",
			&IPlayerInfo::GetAbsAngles,
			"Returns the player's angle QAngle instance."
		)

		.add_property("mins",
			&IPlayerInfo::GetPlayerMins,
			"Returns the player's min Vector instance."
		)

		.add_property("maxs",
			&IPlayerInfo::GetPlayerMaxs,
			"Returns the player's max Vector instance."
		)

		.add_property("weapon_name",
			&IPlayerInfo::GetWeaponName,
			"Returns the name of the weapon the player is carrying."
		)

		.add_property("model_name",
			&IPlayerInfo::GetModelName,
			"Returns the name of the player's model."
		)

		.add_property("health",
			&IPlayerInfo::GetHealth,
			"Returns the player's health."
		)

		.add_property("max_health",
			&IPlayerInfo::GetMaxHealth,
			"Returns the player's maximum health."
		)
		
		.add_property("last_user_command",
			&IPlayerInfo::GetLastUserCommand
		)

		ADD_MEM_TOOLS(IPlayerInfo)
	;
}


//-----------------------------------------------------------------------------
// Exports CPlayerGenerator.
//-----------------------------------------------------------------------------
void export_player_generator(scope _players)
{
	class_<CPlayerGenerator>("PlayerGenerator")
		.def("__iter__",
			&CPlayerGenerator::iter,
			"Returns the iterable object."
		)

		.def("__next__",
			&CPlayerGenerator::next,
			"Returns the next valid instance.",
			reference_existing_object_policy()
		)
	;
}


//-----------------------------------------------------------------------------
// Exports IClient.
//-----------------------------------------------------------------------------
void export_client(scope _players)
{
	class_<IClient, IClient*, bases<INetChannelHandler>, boost::noncopyable> Client("Client", no_init);

	Client.add_property(
		"name",
		&IClient::GetClientName,
		"Return the client's name."
	);

	Client.add_property(
		"userid",
		&IClient::GetUserID,
		"Return the client's userid."
	);

	Client.add_property(
		"net_channel",
		make_function(
			&IClient::GetNetChannel,
			reference_existing_object_policy()
		)
	);

	Client.add_property(
		"steamid",
		&IClient::GetNetworkIDString
	);

	Client.def("disconnect",
		&IClientExt::Disconnect
	);

	// TODO: Export more

	Client ADD_MEM_TOOLS(IClient);
}


//-----------------------------------------------------------------------------
// Exports CUserCmd.
//-----------------------------------------------------------------------------
void export_user_cmd(scope _players)
{
	class_<CUserCmd, CUserCmd*> UserCmd("UserCmd");

	UserCmd.def(
		"reset",
		&CUserCmd::Reset
	);

	UserCmd.add_property(
		"checksum",
		&CUserCmd::GetChecksum
	);

	UserCmd.def_readwrite("command_number",
		&CUserCmd::command_number,
		"For matching server and client commands for debugging."
	);

	UserCmd.def_readwrite("tick_count",
		&CUserCmd::tick_count,
		"The tick the client created this command."
	);

	UserCmd.def_readwrite("view_angles",
		&CUserCmd::viewangles,
		"Player instantaneous view angles."
	);

	UserCmd.NOT_IMPLEMENTED_ATTR("aim_direction");

	UserCmd.def_readwrite("forward_move",
		&CUserCmd::forwardmove,
		"Forward velocity."
	);

	UserCmd.def_readwrite("side_move",
		&CUserCmd::sidemove,
		"Sideways velocity."
	);

	UserCmd.def_readwrite("up_move",
		&CUserCmd::upmove,
		"Upward velocity."
	);

	UserCmd.def_readwrite("buttons",
		&CUserCmd::buttons,
		"Button states."
	);

	UserCmd.def_readwrite("impulse",
		&CUserCmd::impulse,
		"Impulse command."
	);

	UserCmd.def_readwrite("weaponselect",
		&CUserCmd::weaponselect,
		"Current weapon ID."
	);

	UserCmd.def_readwrite("weaponsubtype",
		&CUserCmd::weaponsubtype,
		"Current weapon ID."
	);

	UserCmd.def_readwrite("random_seed",
		&CUserCmd::random_seed,
		"For shared random functions."
	);

	UserCmd.def_readwrite("mousedx",
		&CUserCmd::mousedx,
		"Mouse accum in x from create move."
	);

	UserCmd.def_readwrite("mousedy",
		&CUserCmd::mousedy,
		"Mouse accum in y from create move."
	);

	UserCmd.def_readwrite("has_been_predicted",
		&CUserCmd::hasbeenpredicted,
		"Client only, tracks whether we've predicted this command at least once."
	);
	
	UserCmd.NOT_IMPLEMENTED_ATTR("head_angles");
	UserCmd.NOT_IMPLEMENTED_ATTR("head_offset");

	export_engine_specific_user_cmd(_players, UserCmd);

	UserCmd ADD_MEM_TOOLS(CUserCmd);
}

void export_player_wrapper(scope _players)
{
	class_<PlayerWrapper, bases<CBaseEntityWrapper>, boost::noncopyable> _PlayerWrapper("PlayerMixin", no_init);

	_PlayerWrapper.def("__init__",
		make_constructor(
			&PlayerWrapper::__init__,
			default_call_policies(),
			args("entity_index")
		)
	);

	_PlayerWrapper.add_property(
		"speed",
		&PlayerWrapper::GetSpeed,
		&PlayerWrapper::SetSpeed,
		"Get/set the player's speed.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"is_ducked",
		&PlayerWrapper::GetIsDucked,
		&PlayerWrapper::SetIsDucked,
		"Return whether the player is ducked.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"is_ducking",
		&PlayerWrapper::GetIsDucked,
		&PlayerWrapper::SetIsDucked,
		"Return whether the player is duckeding.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"flags",
		&PlayerWrapper::GetFlags,
		&PlayerWrapper::SetFlags,
		"Get/set the player's flags.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"last_weapon",
		&PlayerWrapper::GetLastWeapon,
		&PlayerWrapper::SetLastWeapon,
		"Get/set the player's last weapon.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"observer_target",
		&PlayerWrapper::GetObserverTarget,
		&PlayerWrapper::SetObserverTarget,
		"Get/set the player's observer target.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"deaths",
		&PlayerWrapper::GetDeaths,
		&PlayerWrapper::SetDeaths,
		"Get/set the player's death count.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"kills",
		&PlayerWrapper::GetKills,
		&PlayerWrapper::SetKills,
		"Get/set the player's kill count.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"observer_mode",
		&PlayerWrapper::GetObserverMode,
		&PlayerWrapper::SetObserverMode,
		"Get/set the player's observer mode.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"life_state",
		&PlayerWrapper::GetLifeState,
		&PlayerWrapper::SetLifeState,
		"Get/set the player's life state.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"place",
		&PlayerWrapper::GetPlace,
		&PlayerWrapper::SetPlace,
		"Get/set the player's current place.\n\n"
		":rtype: str");

	_PlayerWrapper.add_property(
		"dead",
		&PlayerWrapper::GetDead,
		&PlayerWrapper::SetDead,
		"Return whether the player is dead.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"fall_velocity",
		&PlayerWrapper::GetFallVelocity,
		&PlayerWrapper::SetFallVelocity,
		"Get/set the player's fall velocity.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"buttons",
		&PlayerWrapper::GetButtons,
		&PlayerWrapper::SetButtons,
		"Get/set the player's currently pressed buttons.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"hidden_huds",
		&PlayerWrapper::GetHiddenHUDs,
		&PlayerWrapper::SetHiddenHUDs,
		"Get/set the player's hidden HUDs.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"draw_view_model",
		&PlayerWrapper::GetDrawViewModel,
		&PlayerWrapper::SetDrawViewModel,
		"Get/set the player's draw view model.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"fov",
		&PlayerWrapper::GetFOV,
		&PlayerWrapper::SetFOV,
		"Get/set the player's field of view (FOV).\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"fov_start",
		&PlayerWrapper::GetFOVStart,
		&PlayerWrapper::SetFOVStart,
		"Get/set the player's field of view (FOV) start.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"fov_time",
		&PlayerWrapper::GetFOVTime,
		&PlayerWrapper::SetFOVTime,
		"Get/set the player's field of view (FOV) time.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"default_fov",
		&PlayerWrapper::GetDefaultFOV,
		&PlayerWrapper::SetDefaultFOV,
		"Get/set the player's default field of view (FOV).\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"default_fov",
		&PlayerWrapper::GetDefaultFOV,
		&PlayerWrapper::SetDefaultFOV,
		"Get/set the player's default field of view (FOV).\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"fov_rate",
		&PlayerWrapper::GetFOVRate,
		&PlayerWrapper::SetFOVRate,
		"Get/set the player's field of view (FOV) rate.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"gun_offset",
		&PlayerWrapper::GetGunOffset,
		&PlayerWrapper::SetGunOffset,
		"Get/set the player's gun offset.\n\n"
		":rtype: Vector");

	_PlayerWrapper.add_property(
		"last_hitgroup",
		&PlayerWrapper::GetLastHitgroup,
		&PlayerWrapper::SetLastHitgroup,
		"Get/set the player's last hitgroup.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"active_weapon_handle",
		&PlayerWrapper::GetLastHitgroup,
		&PlayerWrapper::SetLastHitgroup,
		"Get/set the player's active weapon_handle.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"relationship",
		&PlayerWrapper::GetRelationship,
		&PlayerWrapper::SetRelationship,
		"Get/set the player's relationship.\n\n"
		":rtype: str");

	_PlayerWrapper.add_property(
		"phys_damage_scale",
		&PlayerWrapper::GetPhysDamageScale,
		&PlayerWrapper::SetPhysDamageScale,
		"Get/set the player's physical damage scale.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"eye_angle",
		&PlayerWrapper::GetEyeAngle,
		&PlayerWrapper::SetEyeAngle,
		"Get/set the player's eye angle.\n\n"
		":rtype: QAngle");

	_PlayerWrapper.add_property(
		"view_vector",
		&PlayerWrapper::GetViewVector,
		&PlayerWrapper::SetViewVector,
		"Get/set the player's view vector.\n\n"
		":rtype: Vector");

	_PlayerWrapper.add_property(
		"view_angle",
		&PlayerWrapper::GetViewAngle,
		&PlayerWrapper::SetViewAngle,
		"Get/set the player's view angle.\n\n"
		":rtype: QAngle");

	_PlayerWrapper.add_property(
		"view_offset",
		&PlayerWrapper::GetViewOffset,
		&PlayerWrapper::SetViewOffset,
		"Get/set the player's view offset.\n\n"
		":rtype: Vector");

	// Game specific
	_PlayerWrapper.add_property(
		"stamina",
		&PlayerWrapper::GetStamina,
		&PlayerWrapper::SetStamina,
		"Get/set the player's stamina.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"shots_fired",
		&PlayerWrapper::GetShotsFired,
		&PlayerWrapper::SetShotsFired,
		"Get/set the the number of shots fired by the player.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"armor",
		&PlayerWrapper::GetArmor,
		&PlayerWrapper::SetArmor,
		"Get/set the player's armor.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"has_defuser",
		&PlayerWrapper::GetHasDefuser,
		&PlayerWrapper::SetHasDefuser,
		"Get/set whether the player has a defuser.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"has_helmet",
		&PlayerWrapper::GetHasHelmet,
		&PlayerWrapper::SetHasHelmet,
		"Get/set whether the player has a helmet.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"in_bomb_zone",
		&PlayerWrapper::GetIsInBombZone,
		&PlayerWrapper::SetIsInBombZone,
		"Get/set whether the player is in a bomb zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"in_buy_zone",
		&PlayerWrapper::GetIsInBuyZone,
		&PlayerWrapper::SetIsInBuyZone,
		"Get/set whether the player is in a buy zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"in_rescue_zone",
		&PlayerWrapper::GetIsInHostageRescueZone,
		&PlayerWrapper::SetIsInHostageRescueZone,
		"Get/set whether the player is in a hostage rescue zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"is_defusing",
		&PlayerWrapper::GetIsDefusing,
		&PlayerWrapper::SetIsDefusing,
		"Get/set whether the player is currently defusing the bomb.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"nightvision_on",
		&PlayerWrapper::GetNightvisionOn,
		&PlayerWrapper::SetNightvisionOn,
		"Get/set whether the player is currently using nightvision.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerWrapper.add_property(
		"flash_duration",
		&PlayerWrapper::GetFlashDuration,
		&PlayerWrapper::SetFlashDuration,
		"Get/set the player's flash duration.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"flash_alpha",
		&PlayerWrapper::GetFlashAlpha,
		&PlayerWrapper::SetFlashAlpha,
		"Get/set the player's flash alpha.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"cash",
		&PlayerWrapper::GetCash,
		&PlayerWrapper::SetCash,
		"Get/set the player's cash.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"player_class",
		&PlayerWrapper::GetPlayerClass,
		&PlayerWrapper::SetPlayerClass,
		"Get/set the player's player class.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"player_state",
		&PlayerWrapper::GetPlayerState,
		&PlayerWrapper::SetPlayerState,
		"Get/set the player's player state.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"ragdoll",
		&PlayerWrapper::GetRagdoll,
		&PlayerWrapper::SetRagdoll,
		"Get/set the player's ragdoll.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"active_devices",
		&PlayerWrapper::GetActiveDevices,
		&PlayerWrapper::SetActiveDevices,
		"Get/set the player's active devices.\n\n"
		".. note:: Only available in HL2.\n\n"
		":rtype: int");

	_PlayerWrapper.add_property(
		"suit_power_load",
		&PlayerWrapper::GetSuitPowerLoad,
		&PlayerWrapper::SetSuitPowerLoad,
		"Get/set the player's suit power load.\n\n"
		".. note:: Only available in HL2.\n\n"
		":rtype: float");

	_PlayerWrapper.add_property(
		"desired_player_class",
		&PlayerWrapper::GetDesiredPlayerClass,
		&PlayerWrapper::SetDesiredPlayerClass,
		"Get/set the player's desired player class.\n\n"
		".. note:: Only available in TF2.\n\n"
		":rtype: int");

	_PlayerWrapper ADD_MEM_TOOLS(PlayerWrapper);
}
