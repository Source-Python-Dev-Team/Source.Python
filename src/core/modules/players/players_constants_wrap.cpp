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
#include <climits>
#include "export_main.h"
#include "utilities/conversions.h"
#include "utilities/wrap_macros.h"
#include "const.h"
#include "game/shared/in_buttons.h"
#include "string_t.h"
#include "memalloc.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/shareddefs.h"

#include ENGINE_INCLUDE_PATH(players_constants_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_players_states(scope);
void export_life_state(scope);
void export_player_buttons(scope);
void export_hide_hud_flags(scope);
void export_hit_groups(scope);
void export_player_animation(scope);
void export_observer_modes(scope);
void export_players_miscellaneous_constants(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _constants)
{
	export_players_states(_constants);
	export_life_state(_constants);
	export_player_buttons(_constants);
	export_hide_hud_flags(_constants);
	export_hit_groups(_constants);
	export_player_animation(_constants);
	export_observer_modes(_constants);
	export_players_miscellaneous_constants(_constants);
}


//-----------------------------------------------------------------------------
// Exports PlayerStates.
//-----------------------------------------------------------------------------
void export_players_states(scope _constants)
{
	// Player state flags constants...
	_constants.attr("FL_ONGROUND") = FL_ONGROUND;
	_constants.attr("FL_DUCKING") = FL_DUCKING;
	_constants.attr("FL_WATERJUMP") = FL_WATERJUMP;
	_constants.attr("FL_ONTRAIN") = FL_ONTRAIN;
	_constants.attr("FL_INRAIN") = FL_INRAIN;
	_constants.attr("FL_FROZEN") = FL_FROZEN;
	_constants.attr("FL_ATCONTROLS") = FL_ATCONTROLS;
	_constants.attr("FL_CLIENT") = FL_CLIENT;
	_constants.attr("FL_FAKECLIENT") = FL_FAKECLIENT;
	_constants.attr("FL_INWATER") = FL_INWATER;
}


//-----------------------------------------------------------------------------
// Exports LifeState.
//-----------------------------------------------------------------------------
void export_life_state(scope _constants)
{
	_constants.attr("LIFE_ALIVE") = LIFE_ALIVE;
	_constants.attr("LIFE_DYING") = LIFE_DYING;
	_constants.attr("LIFE_DEAD") = LIFE_DEAD;
	_constants.attr("LIFE_RESPAWNABLE") = LIFE_RESPAWNABLE;
	_constants.attr("LIFE_DISCARDBODY") = LIFE_DISCARDBODY;
}


//-----------------------------------------------------------------------------
// Exports PlayerButtons.
//-----------------------------------------------------------------------------
void export_player_buttons(scope _constants)
{
	_constants.attr("IN_ATTACK") = IN_ATTACK;
	_constants.attr("IN_JUMP") = IN_JUMP;
	_constants.attr("IN_DUCK") = IN_DUCK;
	_constants.attr("IN_FORWARD") = IN_FORWARD;
	_constants.attr("IN_BACK") = IN_BACK;
	_constants.attr("IN_USE") = IN_USE;
	_constants.attr("IN_CANCEL") = IN_CANCEL;
	_constants.attr("IN_LEFT") = IN_LEFT;
	_constants.attr("IN_RIGHT") = IN_RIGHT;
	_constants.attr("IN_MOVELEFT") = IN_MOVELEFT;
	_constants.attr("IN_MOVERIGHT") = IN_MOVERIGHT;
	_constants.attr("IN_ATTACK2") = IN_ATTACK2;
	_constants.attr("IN_RUN") = IN_RUN;
	_constants.attr("IN_RELOAD") = IN_RELOAD;
	_constants.attr("IN_ALT1") = IN_ALT1;
	_constants.attr("IN_ALT2") = IN_ALT2;
	_constants.attr("IN_SCORE") = IN_SCORE;
	_constants.attr("IN_SPEED") = IN_SPEED;
	_constants.attr("IN_WALK") = IN_WALK;
	_constants.attr("IN_ZOOM") = IN_ZOOM;
	_constants.attr("IN_WEAPON1") = IN_WEAPON1;
	_constants.attr("IN_WEAPON2") = IN_WEAPON2;
	_constants.attr("IN_BULLRUSH") = IN_BULLRUSH;
	_constants.attr("IN_GRENADE1") = IN_GRENADE1;
	_constants.attr("IN_GRENADE2") = IN_GRENADE2;

	// CS:GO specific constants...
	_constants.attr("IN_LOOKSPIN") = -LONG_MAX;

	// Engine specific stuff...
	export_engine_specific_player_buttons(_constants);
}


//-----------------------------------------------------------------------------
// Exports HideHudFlags.
//-----------------------------------------------------------------------------
void export_hide_hud_flags(scope _constants)
{
	_constants.attr("HIDEHUD_WEAPONSELECTION") = HIDEHUD_WEAPONSELECTION;
	_constants.attr("HIDEHUD_FLASHLIGHT") = HIDEHUD_FLASHLIGHT;
	_constants.attr("HIDEHUD_ALL") = HIDEHUD_ALL;
	_constants.attr("HIDEHUD_HEALTH") = HIDEHUD_HEALTH;
	_constants.attr("HIDEHUD_PLAYERDEAD") = HIDEHUD_PLAYERDEAD;
	_constants.attr("HIDEHUD_NEEDSUIT") = HIDEHUD_NEEDSUIT;
	_constants.attr("HIDEHUD_MISCSTATUS") = HIDEHUD_MISCSTATUS;
	_constants.attr("HIDEHUD_CHAT") = HIDEHUD_CHAT;
	_constants.attr("HIDEHUD_CROSSHAIR") = HIDEHUD_CROSSHAIR;
	_constants.attr("HIDEHUD_VEHICLE_CROSSHAIR") = HIDEHUD_VEHICLE_CROSSHAIR;
	_constants.attr("HIDEHUD_INVEHICLE") = HIDEHUD_INVEHICLE;
	_constants.attr("HIDEHUD_BONUS_PROGRESS") = HIDEHUD_BONUS_PROGRESS;

	// CS:GO specific constants...
	_constants.attr("HIDEHUD_RADAR") = -LONG_MAX;

	// Engine specific stuff...
	export_engine_specific_hide_hud_flags(_constants);
}


//-----------------------------------------------------------------------------
// Exports HitGroup.
//-----------------------------------------------------------------------------
void export_hit_groups(scope _constants)
{
	_constants.attr("HITGROUP_GENERIC") = HITGROUP_GENERIC;
	_constants.attr("HITGROUP_HEAD") = HITGROUP_HEAD;
	_constants.attr("HITGROUP_CHEST") = HITGROUP_CHEST;
	_constants.attr("HITGROUP_STOMACH") = HITGROUP_STOMACH;
	_constants.attr("HITGROUP_LEFTARM") = HITGROUP_LEFTARM;
	_constants.attr("HITGROUP_RIGHTARM") = HITGROUP_RIGHTARM;
	_constants.attr("HITGROUP_LEFTLEG") = HITGROUP_LEFTLEG;
	_constants.attr("HITGROUP_RIGHTLEG") = HITGROUP_RIGHTLEG;
	_constants.attr("HITGROUP_GEAR") = HITGROUP_GEAR;
}


//-----------------------------------------------------------------------------
// Expose PLAYER_ANIM.
//-----------------------------------------------------------------------------
void export_player_animation(scope _constants)
{
	enum_<PLAYER_ANIM> PlayerAnimation("PlayerAnimation");

	// Values...
	PlayerAnimation.value("IDLE", PLAYER_IDLE);
	PlayerAnimation.value("WALK", PLAYER_WALK);
	PlayerAnimation.value("JUMP", PLAYER_JUMP);
	PlayerAnimation.value("SUPERJUMP", PLAYER_SUPERJUMP);
	PlayerAnimation.value("DIE", PLAYER_DIE);
	PlayerAnimation.value("ATTACK1", PLAYER_ATTACK1);
	PlayerAnimation.value("IN_VEHICLE", PLAYER_IN_VEHICLE);
	PlayerAnimation.value("RELOAD", PLAYER_RELOAD);
	PlayerAnimation.value("START_AIMING", PLAYER_START_AIMING);
	PlayerAnimation.value("LEAVE_AIMING", PLAYER_LEAVE_AIMING);
}


//-----------------------------------------------------------------------------
// Exports ObserverMode.
//-----------------------------------------------------------------------------
void export_observer_modes(scope _constants)
{
	_constants.attr("OBS_MODE_NONE") = (int)OBS_MODE_NONE;
	_constants.attr("OBS_MODE_DEATHCAM") = (int)OBS_MODE_DEATHCAM;
	_constants.attr("OBS_MODE_FREEZECAM") = (int)OBS_MODE_FREEZECAM;
	_constants.attr("OBS_MODE_FIXED") = (int)OBS_MODE_FIXED;
	_constants.attr("OBS_MODE_IN_EYE") = (int)OBS_MODE_IN_EYE;
	_constants.attr("OBS_MODE_CHASE") = (int)OBS_MODE_CHASE;
	_constants.attr("OBS_MODE_ROAMING") = (int)OBS_MODE_ROAMING;
}


//-----------------------------------------------------------------------------
// Exports miscellaneous constants.
//-----------------------------------------------------------------------------
void export_players_miscellaneous_constants(scope _constants)
{
	_constants.attr("INVALID_PLAYER_USERID") = INVALID_PLAYER_USERID;

}
