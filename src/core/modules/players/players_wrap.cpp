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
	class_<PlayerMixin, bases<CBaseEntityWrapper>, boost::noncopyable> _PlayerMixin("PlayerMixin", no_init);

	_PlayerMixin.def("__init__",
		make_constructor(
			&PlayerMixin::__init__,
			default_call_policies(),
			args("entity_index")
		)
	);

	_PlayerMixin.def(
		"is_networked",
		&PlayerMixin::IsNetworked,
		"Return True if the entity is networked.\n\n"
		":rtype: bool"
	);

	_PlayerMixin.def(
		"is_player",
		&PlayerMixin::IsPlayer,
		"Return True if the entity is a player.\n\n"
		":rtype: bool"
	);

	_PlayerMixin.def(
		"is_weapon",
		&PlayerMixin::IsWeapon,
		"Return True if the entity is a weapon.\n\n"
		":rtype: bool"
	);

	_PlayerMixin.add_property(
		"speed",
		&PlayerMixin::GetSpeed,
		&PlayerMixin::SetSpeed,
		"Get/set the player's speed.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"is_ducked",
		&PlayerMixin::GetIsDucked,
		&PlayerMixin::SetIsDucked,
		"Return whether the player is ducked.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"is_ducking",
		&PlayerMixin::GetIsDucking,
		&PlayerMixin::SetIsDucking,
		"Return whether the player is duckeding.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"flags",
		&PlayerMixin::GetFlags,
		&PlayerMixin::SetFlags,
		"Get/set the player's flags.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"last_weapon",
		&PlayerMixin::GetLastWeapon,
		&PlayerMixin::SetLastWeapon,
		"Get/set the player's last weapon.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"observer_target",
		&PlayerMixin::GetObserverTarget,
		&PlayerMixin::SetObserverTarget,
		"Get/set the player's observer target.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"deaths",
		&PlayerMixin::GetDeaths,
		&PlayerMixin::SetDeaths,
		"Get/set the player's death count.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"kills",
		&PlayerMixin::GetKills,
		&PlayerMixin::SetKills,
		"Get/set the player's kill count.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"observer_mode",
		&PlayerMixin::GetObserverMode,
		&PlayerMixin::SetObserverMode,
		"Get/set the player's observer mode.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"life_state",
		&PlayerMixin::GetLifeState,
		&PlayerMixin::SetLifeState,
		"Get/set the player's life state.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"place",
		&PlayerMixin::GetPlace,
		&PlayerMixin::SetPlace,
		"Get/set the player's current place.\n\n"
		":rtype: str");

	_PlayerMixin.add_property(
		"dead",
		&PlayerMixin::GetDead,
		&PlayerMixin::SetDead,
		"Return whether the player is dead.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"fall_velocity",
		&PlayerMixin::GetFallVelocity,
		&PlayerMixin::SetFallVelocity,
		"Get/set the player's fall velocity.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"buttons",
		&PlayerMixin::GetButtons,
		&PlayerMixin::SetButtons,
		"Get/set the player's currently pressed buttons.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"hidden_huds",
		&PlayerMixin::GetHiddenHUDs,
		&PlayerMixin::SetHiddenHUDs,
		"Get/set the player's hidden HUDs.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"draw_view_model",
		&PlayerMixin::GetDrawViewModel,
		&PlayerMixin::SetDrawViewModel,
		"Get/set the player's draw view model.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"fov",
		&PlayerMixin::GetFOV,
		&PlayerMixin::SetFOV,
		"Get/set the player's field of view (FOV).\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"fov_start",
		&PlayerMixin::GetFOVStart,
		&PlayerMixin::SetFOVStart,
		"Get/set the player's field of view (FOV) start.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"fov_time",
		&PlayerMixin::GetFOVTime,
		&PlayerMixin::SetFOVTime,
		"Get/set the player's field of view (FOV) time.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"default_fov",
		&PlayerMixin::GetDefaultFOV,
		&PlayerMixin::SetDefaultFOV,
		"Get/set the player's default field of view (FOV).\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"default_fov",
		&PlayerMixin::GetDefaultFOV,
		&PlayerMixin::SetDefaultFOV,
		"Get/set the player's default field of view (FOV).\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"fov_rate",
		&PlayerMixin::GetFOVRate,
		&PlayerMixin::SetFOVRate,
		"Get/set the player's field of view (FOV) rate.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"gun_offset",
		&PlayerMixin::GetGunOffset,
		&PlayerMixin::SetGunOffset,
		"Get/set the player's gun offset.\n\n"
		":rtype: Vector");

	_PlayerMixin.add_property(
		"last_hitgroup",
		&PlayerMixin::GetLastHitgroup,
		&PlayerMixin::SetLastHitgroup,
		"Get/set the player's last hitgroup.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"active_weapon_handle",
		&PlayerMixin::GetActiveWeaponHandle,
		&PlayerMixin::SetActiveWeaponHandle,
		"Get/set the player's active weapon_handle.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"relationship",
		&PlayerMixin::GetRelationship,
		&PlayerMixin::SetRelationship,
		"Get/set the player's relationship.\n\n"
		":rtype: str");

	_PlayerMixin.add_property(
		"phys_damage_scale",
		&PlayerMixin::GetPhysDamageScale,
		&PlayerMixin::SetPhysDamageScale,
		"Get/set the player's physical damage scale.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"eye_angle",
		&PlayerMixin::GetEyeAngle,
		&PlayerMixin::SetEyeAngle,
		"Get/set the player's eye angle.\n\n"
		":rtype: QAngle");

	_PlayerMixin.add_property(
		"view_vector",
		&PlayerMixin::GetViewVector,
		&PlayerMixin::SetViewVector,
		"Get/set the player's view vector.\n\n"
		":rtype: Vector");

	_PlayerMixin.add_property(
		"view_angle",
		&PlayerMixin::GetViewAngle,
		&PlayerMixin::SetViewAngle,
		"Get/set the player's view angle.\n\n"
		":rtype: QAngle");

	_PlayerMixin.add_property(
		"view_offset",
		&PlayerMixin::GetViewOffset,
		&PlayerMixin::SetViewOffset,
		"Get/set the player's view offset.\n\n"
		":rtype: Vector");

	// Game specific
	_PlayerMixin.add_property(
		"stamina",
		&PlayerMixin::GetStamina,
		&PlayerMixin::SetStamina,
		"Get/set the player's stamina.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"shots_fired",
		&PlayerMixin::GetShotsFired,
		&PlayerMixin::SetShotsFired,
		"Get/set the the number of shots fired by the player.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"armor",
		&PlayerMixin::GetArmor,
		&PlayerMixin::SetArmor,
		"Get/set the player's armor.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"has_defuser",
		&PlayerMixin::GetHasDefuser,
		&PlayerMixin::SetHasDefuser,
		"Get/set whether the player has a defuser.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"has_helmet",
		&PlayerMixin::GetHasHelmet,
		&PlayerMixin::SetHasHelmet,
		"Get/set whether the player has a helmet.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"in_bomb_zone",
		&PlayerMixin::GetIsInBombZone,
		&PlayerMixin::SetIsInBombZone,
		"Get/set whether the player is in a bomb zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"in_buy_zone",
		&PlayerMixin::GetIsInBuyZone,
		&PlayerMixin::SetIsInBuyZone,
		"Get/set whether the player is in a buy zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"in_rescue_zone",
		&PlayerMixin::GetIsInHostageRescueZone,
		&PlayerMixin::SetIsInHostageRescueZone,
		"Get/set whether the player is in a hostage rescue zone.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"is_defusing",
		&PlayerMixin::GetIsDefusing,
		&PlayerMixin::SetIsDefusing,
		"Get/set whether the player is currently defusing the bomb.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"nightvision_on",
		&PlayerMixin::GetNightvisionOn,
		&PlayerMixin::SetNightvisionOn,
		"Get/set whether the player is currently using nightvision.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: bool");

	_PlayerMixin.add_property(
		"flash_duration",
		&PlayerMixin::GetFlashDuration,
		&PlayerMixin::SetFlashDuration,
		"Get/set the player's flash duration.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"flash_alpha",
		&PlayerMixin::GetFlashAlpha,
		&PlayerMixin::SetFlashAlpha,
		"Get/set the player's flash alpha.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"cash",
		&PlayerMixin::GetCash,
		&PlayerMixin::SetCash,
		"Get/set the player's cash.\n\n"
		".. note:: Only available in CS:GO and CS:S.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"player_class",
		&PlayerMixin::GetPlayerClass,
		&PlayerMixin::SetPlayerClass,
		"Get/set the player's player class.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"player_state",
		&PlayerMixin::GetPlayerState,
		&PlayerMixin::SetPlayerState,
		"Get/set the player's player state.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"ragdoll",
		&PlayerMixin::GetRagdoll,
		&PlayerMixin::SetRagdoll,
		"Get/set the player's ragdoll.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"active_devices",
		&PlayerMixin::GetActiveDevices,
		&PlayerMixin::SetActiveDevices,
		"Get/set the player's active devices.\n\n"
		".. note:: Only available in HL2.\n\n"
		":rtype: int");

	_PlayerMixin.add_property(
		"suit_power_load",
		&PlayerMixin::GetSuitPowerLoad,
		&PlayerMixin::SetSuitPowerLoad,
		"Get/set the player's suit power load.\n\n"
		".. note:: Only available in HL2.\n\n"
		":rtype: float");

	_PlayerMixin.add_property(
		"desired_player_class",
		&PlayerMixin::GetDesiredPlayerClass,
		&PlayerMixin::SetDesiredPlayerClass,
		"Get/set the player's desired player class.\n\n"
		".. note:: Only available in TF2.\n\n"
		":rtype: int");

	CLASSMETHOD(_PlayerMixin, GET_OBJ_NAME, &PlayerMixin::_obj)
	ADD_PTR(PlayerMixin)
	ADD_SIZE(PlayerMixin)
	STORE_CLASS(PlayerMixin, "PlayerMixin")
}
