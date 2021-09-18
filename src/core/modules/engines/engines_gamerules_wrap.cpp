/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
// Source.Python
#include "export_main.h"
#include "sp_main.h"
#include "engines_gamerules.h"

// SDK
// CS:GO/Blade doesn't compile without the next two lines
#if defined(ENGINE_CSGO) | defined(ENGINE_BLADE)
	#include "baseanimating.h"
	extern IUniformRandomStream* randomStr;
	#define random randomStr
#endif
#include "game/shared/gamerules.h"

//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_game_rules(scope);


//---------------------------------------------------------------------------------
// Declare the _sound module.
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _gamerules)
{
	def(
		"find_game_rules_property_offset",
		&find_game_rules_property_offset,
		"Find the offset of a game rules property.\n\n"
		":rtype: int");
	def(
		"find_game_rules_proxy_name",
		&find_game_rules_proxy_name,
		"Find the server class of the game rules proxy entity (e. g. ``CCSGameRulesProxy``).\n\n"
		":raise ValueError:\n"
		"	Raised if the game rules proxy name wasn't found.\n"
		":rtype: str");

	def(
		"find_game_rules",
		&find_game_rules,
		"Find the game rules instance.\n\n"
		":raise ValueError:\n"
		"    Raised if the game rules instance wasn't found.\n"
		":rtype: GameRules\n",
		reference_existing_object_policy());

	export_game_rules(_gamerules);
}


//-----------------------------------------------------------------------------
// Expose GameRules.
//-----------------------------------------------------------------------------
void export_game_rules(scope _gamerules)
{
	class_<CGameRulesWrapper, boost::noncopyable> GameRules("GameRules", no_init);

	// Generic property getters
	GameRules.def("get_property_bool",
		&CGameRulesWrapper::GetProperty<bool>,
		"Return the value of the given field name.\n\n"
		":rtype: bool"
	);

	GameRules.def("get_property_char",
		&CGameRulesWrapper::GetProperty<char>,
		"Return the value of the given field name.\n\n"
		":rtype: str"
	);

	GameRules.def("get_property_uchar",
		&CGameRulesWrapper::GetProperty<unsigned char>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_short",
		&CGameRulesWrapper::GetProperty<short>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_ushort",
		&CGameRulesWrapper::GetProperty<unsigned short>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_int",
		&CGameRulesWrapper::GetProperty<int>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_uint",
		&CGameRulesWrapper::GetProperty<unsigned int>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_long",
		&CGameRulesWrapper::GetProperty<long>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_ulong",
		&CGameRulesWrapper::GetProperty<unsigned long>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_long_long",
		&CGameRulesWrapper::GetProperty<long long>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_ulong_long",
		&CGameRulesWrapper::GetProperty<unsigned long long>,
		"Return the value of the given field name.\n\n"
		":rtype: int"
	);

	GameRules.def("get_property_float",
		&CGameRulesWrapper::GetProperty<float>,
		"Return the value of the given field name.\n\n"
		":rtype: float"
	);

	GameRules.def("get_property_double",
		&CGameRulesWrapper::GetProperty<double>,
		"Return the value of the given field name.\n\n"
		":rtype: float"
	);

	GameRules.def("get_property_string_pointer",
		&CGameRulesWrapper::GetProperty<const char*>,
		"Return the value of the given field name.\n\n"
		":rtype: str"
	);

	GameRules.def("get_property_string_array",
		&CGameRulesWrapper::GetPropertyStringArray,
		"Return the value of the given field name.\n\n"
		":rtype: str"
	);

	// Backward compatibility
	GameRules.attr("get_property_string") = GameRules.attr("get_property_string_array");

	GameRules.def("get_property_pointer",
		&CGameRulesWrapper::GetProperty<void*>,
		return_by_value_policy(),
		"Return the value of the given field name.\n\n"
		":rtype: Pointer"
	);

	GameRules.def("get_property_vector",
		&CGameRulesWrapper::GetProperty<Vector>,
		"Return the value of the given field name.\n\n"
		":rtype: Vector"
	);

	GameRules.def("get_property_color",
		&CGameRulesWrapper::GetProperty<Color>,
		"Return the value of the given field name.\n\n"
		":rtype: Color"
	);

	GameRules.def("get_property_interval",
		&CGameRulesWrapper::GetProperty<interval_t>,
		"Return the value of the given field name.\n\n"
		":rtype: Interval"
	);

	GameRules.def("get_property_quaternion",
		&CGameRulesWrapper::GetProperty<Quaternion>,
		"Return the value of the given field name.\n\n"
		":rtype: Quaternion"
	);

	GameRules.def("get_property_edict",
		&CGameRulesWrapper::GetProperty<edict_t *>,
		return_by_value_policy(),
		"Return the value of the given field name.\n\n"
		":rtype: Edict"
	);

	// Generic property setters
	GameRules.def("set_property_bool",
		&CGameRulesWrapper::SetProperty<bool>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_char",
		&CGameRulesWrapper::SetProperty<char>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_uchar",
		&CGameRulesWrapper::SetProperty<unsigned char>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_short",
		&CGameRulesWrapper::SetProperty<short>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_ushort",
		&CGameRulesWrapper::SetProperty<unsigned short>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_int",
		&CGameRulesWrapper::SetProperty<int>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_uint",
		&CGameRulesWrapper::SetProperty<unsigned int>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_long",
		&CGameRulesWrapper::SetProperty<long>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_ulong",
		&CGameRulesWrapper::SetProperty<unsigned long>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_long_long",
		&CGameRulesWrapper::SetProperty<long long>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_ulong_long",
		&CGameRulesWrapper::SetProperty<unsigned long long>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_float",
		&CGameRulesWrapper::SetProperty<float>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_double",
		&CGameRulesWrapper::SetProperty<double>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_string_pointer",
		&CGameRulesWrapper::SetProperty<const char*>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_string_array",
		&CGameRulesWrapper::SetPropertyStringArray,
		"Set the value of the given field name."
	);

	// Backward compatibility
	GameRules.attr("set_property_string") = GameRules.attr("set_property_string_array");

	GameRules.def("set_property_pointer",
		&CGameRulesWrapper::SetProperty<void*>,
		return_by_value_policy(),
		"Set the value of the given field name."
	);

	GameRules.def("set_property_vector",
		&CGameRulesWrapper::SetProperty<Vector>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_color",
		&CGameRulesWrapper::SetProperty<Color>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_interval",
		&CGameRulesWrapper::SetProperty<interval_t>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_quaternion",
		&CGameRulesWrapper::SetProperty<Quaternion>,
		"Set the value of the given field name."
	);

	GameRules.def("set_property_edict",
		&CGameRulesWrapper::SetProperty<edict_t *>,
		"Set the value of the given field name."
	);
	
	GameRules ADD_MEM_TOOLS_WRAPPER(CGameRulesWrapper, CGameRules);

	BEGIN_CLASS_INFO(CGameRules)
		FUNCTION_INFO(DeathNotice)
	END_CLASS_INFO()
}
