/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "datamap.h"
#include "game/shared/shareddefs.h"
#include "game/shared/weapon_parse.h"
#include "game/shared/weapon_proficiency.h"

#include ENGINE_INCLUDE_PATH(weapons_constants_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_item_flags(scope);
void export_weapon_sound(scope);
void export_weapon_states(scope);
void export_weapon_proficiency(scope);


//-----------------------------------------------------------------------------
// Declare the _weapons._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_weapons, _constants)
{
	export_item_flags(_constants);
	export_weapon_sound(_constants);
	export_weapon_states(_constants);
	export_weapon_proficiency(_constants);
}


//-----------------------------------------------------------------------------
// Exports ItemFlags.
//-----------------------------------------------------------------------------
void export_item_flags(scope _constants)
{
	_constants.attr("ITEM_FLAG_SELECTONEMPTY") = ITEM_FLAG_SELECTONEMPTY;
	_constants.attr("ITEM_FLAG_NOAUTORELOAD") = ITEM_FLAG_NOAUTORELOAD;
	_constants.attr("ITEM_FLAG_NOAUTOSWITCHEMPTY") = ITEM_FLAG_NOAUTOSWITCHEMPTY;
	_constants.attr("ITEM_FLAG_LIMITINWORLD") = ITEM_FLAG_LIMITINWORLD;
	_constants.attr("ITEM_FLAG_EXHAUSTIBLE") = ITEM_FLAG_EXHAUSTIBLE;
	_constants.attr("ITEM_FLAG_DOHITLOCATIONDMG") = ITEM_FLAG_DOHITLOCATIONDMG;
	_constants.attr("ITEM_FLAG_NOAMMOPICKUPS") = ITEM_FLAG_NOAMMOPICKUPS;
	_constants.attr("ITEM_FLAG_NOITEMPICKUP") = ITEM_FLAG_NOITEMPICKUP;
}


//-----------------------------------------------------------------------------
// Exports FileSound_t.
//-----------------------------------------------------------------------------
void export_weapon_sound(scope _constants)
{
	enum_<WeaponSound_t> WeaponSound("WeaponSound");

	// Values...
	WeaponSound.value("EMPTY", EMPTY);
	WeaponSound.value("SINGLE", SINGLE);
	WeaponSound.value("SINGLE_NPC", SINGLE_NPC);
	WeaponSound.value("DOUBLE", WPN_DOUBLE);
	WeaponSound.value("DOUBLE_NPC", DOUBLE_NPC);
	WeaponSound.value("BURST", BURST);
	WeaponSound.value("RELOAD", RELOAD);
	WeaponSound.value("RELOAD_NPC", RELOAD_NPC);
	WeaponSound.value("MELEE_MISS", MELEE_MISS);
	WeaponSound.value("MELEE_HIT", MELEE_HIT);
	WeaponSound.value("MELEE_HIT_WORLD", MELEE_HIT_WORLD);
	WeaponSound.value("SPECIAL1", SPECIAL1);
	WeaponSound.value("SPECIAL2", SPECIAL2);
	WeaponSound.value("SPECIAL3", SPECIAL3);
	WeaponSound.value("TAUNT", TAUNT);

	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(WeaponSound_t, "FAST_RELOAD");

	// Engine specific values...
	export_engine_specific_weapon_sound(_constants, WeaponSound);
}


//-----------------------------------------------------------------------------
// Exports WeaponState.
//-----------------------------------------------------------------------------
void export_weapon_states(scope _constants)
{
	_constants.attr("WEAPON_NOT_CARRIED") = WEAPON_NOT_CARRIED;
	_constants.attr("WEAPON_IS_CARRIED_BY_PLAYER") = WEAPON_IS_CARRIED_BY_PLAYER;
	_constants.attr("WEAPON_IS_ACTIVE") = WEAPON_IS_ACTIVE;
}


//-----------------------------------------------------------------------------
// Exports WeaponProficiency_t.
//-----------------------------------------------------------------------------
void export_weapon_proficiency(scope _constants)
{
	enum_<WeaponProficiency_t> WeaponProficiency("WeaponProficiency");

	// Values...
	WeaponProficiency.value("POOR", WEAPON_PROFICIENCY_POOR);
	WeaponProficiency.value("AVERAGE", WEAPON_PROFICIENCY_AVERAGE);
	WeaponProficiency.value("GOOD", WEAPON_PROFICIENCY_GOOD);
	WeaponProficiency.value("VERY_GOOD", WEAPON_PROFICIENCY_VERY_GOOD);
	WeaponProficiency.value("PERFECT", WEAPON_PROFICIENCY_PERFECT);
}
