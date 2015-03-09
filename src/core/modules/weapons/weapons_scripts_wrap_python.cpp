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
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "weapons_scripts_wrap.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(weapons_scripts_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_base_weapon_info(scope);


//-----------------------------------------------------------------------------
// Declare the _weapons._scripts module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_weapons, _scripts)
{
	export_base_weapon_info(_scripts);
	export_weapon_info(_scripts);
}


//-----------------------------------------------------------------------------
// Expose FileWeaponInfo_t.
//-----------------------------------------------------------------------------
void export_base_weapon_info(scope _scripts)
{
	class_<FileWeaponInfo_t, FileWeaponInfo_t *, boost::noncopyable> BaseWeaponInfo("BaseWeaponInfo", no_init);

	// Properties...
	BaseWeaponInfo.add_property("class_name", &BaseWeaponInfoSharedExt::get_class_name, &BaseWeaponInfoSharedExt::set_class_name);
	BaseWeaponInfo.add_property("print_name", &BaseWeaponInfoSharedExt::get_print_name, &BaseWeaponInfoSharedExt::set_print_name);
	BaseWeaponInfo.add_property("view_model", &BaseWeaponInfoSharedExt::get_view_model, &BaseWeaponInfoSharedExt::set_view_model);
	BaseWeaponInfo.add_property("world_model", &BaseWeaponInfoSharedExt::get_world_model, &BaseWeaponInfoSharedExt::set_world_model);
	BaseWeaponInfo.add_property("animation_prefix", &BaseWeaponInfoSharedExt::get_animation_prefix, &BaseWeaponInfoSharedExt::set_animation_prefix);

	BaseWeaponInfo.def_readwrite("parsed_state", &FileWeaponInfo_t::bParsedScript);
	BaseWeaponInfo.def_readwrite("hud_elements_loaded_state", &FileWeaponInfo_t::bLoadedHudElements);

	BaseWeaponInfo.add_property("slot", &BaseWeaponInfoSharedExt::get_slot, &BaseWeaponInfoSharedExt::set_slot);

	BaseWeaponInfo.def_readwrite("position", &FileWeaponInfo_t::iPosition);
	BaseWeaponInfo.def_readwrite("max_primary_clip", &FileWeaponInfo_t::iMaxClip1);
	BaseWeaponInfo.def_readwrite("max_primary_clip", &FileWeaponInfo_t::iMaxClip2);
	BaseWeaponInfo.def_readwrite("default_primary_clip", &FileWeaponInfo_t::iDefaultClip1);
	BaseWeaponInfo.def_readwrite("default_secondary_clip", &FileWeaponInfo_t::iDefaultClip2);
	BaseWeaponInfo.def_readwrite("weight", &FileWeaponInfo_t::iWeight);
	BaseWeaponInfo.def_readwrite("rumble_effect", &FileWeaponInfo_t::iRumbleEffect);
	BaseWeaponInfo.def_readwrite("auto_switch_to", &FileWeaponInfo_t::bAutoSwitchTo);
	BaseWeaponInfo.def_readwrite("auto_switch_from", &FileWeaponInfo_t::bAutoSwitchFrom);
	BaseWeaponInfo.def_readwrite("flags", &FileWeaponInfo_t::iFlags);

	BaseWeaponInfo.add_property("primary_ammo", &BaseWeaponInfoSharedExt::get_primary_ammo, &BaseWeaponInfoSharedExt::set_primary_ammo);
	BaseWeaponInfo.add_property("secondary_ammo", &BaseWeaponInfoSharedExt::get_secondary_ammo, &BaseWeaponInfoSharedExt::set_secondary_ammo);

	BaseWeaponInfo.def_readwrite("primary_ammo_type", &FileWeaponInfo_t::iAmmoType);
	BaseWeaponInfo.def_readwrite("secondary_ammo_type", &FileWeaponInfo_t::iAmmo2Type);
	BaseWeaponInfo.def_readwrite("is_melee", &FileWeaponInfo_t::m_bMeleeWeapon);
	BaseWeaponInfo.def_readwrite("built_right_handed", &FileWeaponInfo_t::m_bBuiltRightHanded);
	BaseWeaponInfo.def_readwrite("allow_flipping", &FileWeaponInfo_t::m_bAllowFlipping);
	BaseWeaponInfo.def_readwrite("sprite_count", &FileWeaponInfo_t::iSpriteCount);

	// Methods...
	BaseWeaponInfo.def("get_shoot_sound", &BaseWeaponInfoSharedExt::get_shoot_sound);
	BaseWeaponInfo.def("set_shoot_sound", &BaseWeaponInfoSharedExt::set_shoot_sound);

	// CS:GO specific properties...
	BaseWeaponInfo.NOT_IMPLEMENTED_ATTR("ai_addon");

	// Engine specific stuff...
	export_engine_specific_base_weapon_info(BaseWeaponInfo);

	// Add memory tools...
	BaseWeaponInfo ADD_MEM_TOOLS(FileWeaponInfo_t, "BaseWeaponInfo");
}

