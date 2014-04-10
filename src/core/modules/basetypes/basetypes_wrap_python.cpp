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
#include "tier0/basetypes.h"
#include "Color.h"
#include "dt_common.h"
#include "dt_send.h"
#include "server_class.h"
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"
#include "basetypes_wrap.h"

#include ENGINE_INCLUDE_PATH(basetypes_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_interval();
void export_color();
void export_send_table();
void export_send_prop();
void export_send_prop_types();
void export_send_prop_flags();
void export_server_class();
void export_take_damage_info();


//-----------------------------------------------------------------------------
// Declare the basetypes_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(basetypes_c)
{
	export_interval();
	export_color();
	export_send_table();
	export_send_prop();
	export_send_prop_types();
	export_send_prop_flags();
	export_server_class();
	export_take_damage_info();
}


//-----------------------------------------------------------------------------
// Expose interval_t.
//-----------------------------------------------------------------------------
void export_interval()
{
	class_<interval_t, interval_t *> Interval("Interval");
	
	// Properties...
	Interval.def_readwrite("start", &interval_t::start);
	Interval.def_readwrite("range", &interval_t::range);
	
	// Add memory tools...
	Interval ADD_MEM_TOOLS(interval_t, "Interval");
}


//-----------------------------------------------------------------------------
// Expose Color.
//-----------------------------------------------------------------------------
void export_color()
{
	class_<Color, Color *> Color_("Color");
	
	// Initializers...
	Color_.def(init<int, int, int, optional<int>>());
	
	
	// Properties...
	Color_.add_property("r", &Color::r, &IndexSetter<Color, unsigned char, 0>);
	Color_.add_property("g", &Color::g, &IndexSetter<Color, unsigned char, 1>);
	Color_.add_property("b", &Color::b, &IndexSetter<Color, unsigned char, 2>);
	Color_.add_property("a", &Color::a, &IndexSetter<Color, unsigned char, 3>);

	// Special methods...
	Color_.def("__getitem__", &GetItemIndexer<Color, unsigned char, 0, 3>);
	Color_.def("__setitem__", &SetItemIndexer<Color, unsigned char, 0, 3>);

	// Operators...
	Color_.def(self == self);
	Color_.def(self != self);

	// Add memory tools...
	Color_ ADD_MEM_TOOLS(Color, "Color");
}


//-----------------------------------------------------------------------------
// Expose SendTable.
//-----------------------------------------------------------------------------
void export_send_table()
{
	class_<SendTable, SendTable *> SendTable_("SendTable", no_init);
	
	// Properties...
	SendTable_.def_readonly("props", &SendTable::m_pProps);
	SendTable_.def_readonly("length", &SendTable::m_nProps);
	SendTable_.add_property("name", &SendTable::GetName);
	
	// Methods...
	SendTable_.def("get_name", &SendTable::GetName);
	SendTable_.def("get_length", &SendTable::GetNumProps);
	SendTable_.def("is_initialized", &SendTable::IsInitialized);
	SendTable_.def("set_initialized", &SendTable::SetInitialized);
	
	SendTable_.def("get_prop", &SendTable::GetProp,
		reference_existing_object_policy()
	);
	
	SendTable_.def("set_write_flag", &SendTable::SetWriteFlag);
	SendTable_.def("get_write_flag", &SendTable::GetWriteFlag);
	SendTable_.def("has_props_encoded_against_tick_count", &SendTable::HasPropsEncodedAgainstTickCount);
	SendTable_.def("set_has_props_encoded_against_tick_count", &SendTable::SetHasPropsEncodedAgainstTickcount);
	
	// Special methods...
	SendTable_.def("__getitem__",
		&SendTableSharedExt::__getitem__,
		reference_existing_object_policy()
	);
	
	SendTable_.def("__len__", &SendTable::GetNumProps);
	
	// Add memory tools...
	SendTable_ ADD_MEM_TOOLS(SendTable, "SendTable");
}


//-----------------------------------------------------------------------------
// Expose SendProp.
//-----------------------------------------------------------------------------
void export_send_prop()
{
	class_<SendProp, SendProp *> SendProp_("SendProp", no_init);
	
	// Properties...
	SendProp_.def_readonly("type", &SendProp::m_Type);
	SendProp_.def_readonly("bits", &SendProp::m_nBits);
	SendProp_.def_readonly("low_value", &SendProp::m_fLowValue);
	SendProp_.def_readonly("high_value", &SendProp::m_fHighValue);
	SendProp_.def_readonly("array_prop", &SendProp::m_pArrayProp);
	SendProp_.def_readonly("length", &SendProp::m_nElements);
	SendProp_.def_readonly("element_stride", &SendProp::m_ElementStride);
	SendProp_.def_readonly("exclude_data_table_name", &SendProp::m_pExcludeDTName);
	SendProp_.def_readonly("parent_array_prop_name", &SendProp::m_pParentArrayPropName);
	SendProp_.add_property("name", &SendProp::GetName);
	SendProp_.def_readonly("high_low_mul", &SendProp::m_fHighLowMul);
	SendProp_.add_property("flags", &SendProp::GetFlags);
	
	SendProp_.add_property("data_table",
		make_function(
			&SendProp::GetDataTable,
			reference_existing_object_policy()
		)
	);
	
	SendProp_.add_property("offset", &SendProp::GetOffset);
	
	// CS:GO specific properties...
	SendProp_.NOT_IMPLEMENTED_ATTR("priority");
	
	// Methods...
	SendProp_.def("get_offset", &SendProp::GetOffset);
	
	SendProp_.def("get_data_table",
		&SendProp::GetDataTable,
		reference_existing_object_policy()
	);

	SendProp_.def("get_exclude_data_table_name", &SendProp::GetExcludeDTName);
	SendProp_.def("get_parent_array_prop_name", &SendProp::GetParentArrayPropName);
	SendProp_.def("get_name", &SendProp::GetName);
	SendProp_.def("is_signed", &SendProp::IsSigned);
	SendProp_.def("is_exclude_prop", &SendProp::IsExcludeProp);
	SendProp_.def("is_inside_array", &SendProp::IsInsideArray);
	
	SendProp_.def("get_array_prop",
		&SendProp::GetArrayProp,
		reference_existing_object_policy()
	);

	SendProp_.def("get_length", &SendProp::GetNumElements);
	SendProp_.def("get_element_stride", &SendProp::GetElementStride);
	SendProp_.def("get_type", &SendProp::GetType);
	SendProp_.def("get_flags", &SendProp::GetFlags);
	
	// CS:GO specific methods...
	SendProp_.NOT_IMPLEMENTED("get_priority");
	
	// Engine specific stuff...
	export_engine_specific_send_prop(SendProp_);
	
	// Add memory tools...
	SendProp_ ADD_MEM_TOOLS(SendProp, "SendProp");
}


//-----------------------------------------------------------------------------
// Expose SendPropType.
//-----------------------------------------------------------------------------
void export_send_prop_types()
{
	enum_<SendPropType> SendPropTypes("SendPropTypes");
	
	// Values...
	SendPropTypes.value("INT", DPT_Int);
	SendPropTypes.value("FLOAT", DPT_Float);
	SendPropTypes.value("VECTOR", DPT_Vector);
	SendPropTypes.value("VECTORXY", DPT_VectorXY);
	SendPropTypes.value("STRING", DPT_String);
	SendPropTypes.value("ARRAY", DPT_Array);
	SendPropTypes.value("DATATABLE", DPT_DataTable);
	
	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(SendPropType, "INT64");
	
	// Engine specific stuff...
	export_engine_specific_send_prop_types(SendPropTypes);
}


//-----------------------------------------------------------------------------
// Expose SendPropFlags.
//-----------------------------------------------------------------------------
void export_send_prop_flags()
{
	enum_<SendPropSharedExt::SendPropFlags> SendPropFlags("SendPropFlags");
	
	// Values...
	SendPropFlags.value("UNSIGNED", SendPropSharedExt::UNSIGNED);
	SendPropFlags.value("COORD", SendPropSharedExt::COORD);
	SendPropFlags.value("NO_SCALE", SendPropSharedExt::NO_SCALE);
	SendPropFlags.value("ROUND_DOWN", SendPropSharedExt::ROUND_DOWN);
	SendPropFlags.value("ROUND_UP", SendPropSharedExt::ROUND_UP);
	SendPropFlags.value("NORMAL", SendPropSharedExt::NORMAL);
	SendPropFlags.value("EXCLUDE", SendPropSharedExt::EXCLUDE);
	SendPropFlags.value("XYZE", SendPropSharedExt::XYZE);
	SendPropFlags.value("INSIDE_ARRAY", SendPropSharedExt::INSIDE_ARRAY);
	SendPropFlags.value("PROXY_ALWAYS_YES", SendPropSharedExt::PROXY_ALWAYS_YES);
	SendPropFlags.value("IS_A_VECTOR_ELEM", SendPropSharedExt::IS_A_VECTOR_ELEM);
	SendPropFlags.value("COLLAPSIBLE", SendPropSharedExt::COLLAPSIBLE);
	SendPropFlags.value("COORD_MP", SendPropSharedExt::COORD_MP);
	SendPropFlags.value("COORD_MP_LOW_PRECISION", SendPropSharedExt::COORD_MP_LOW_PRECISION);
	SendPropFlags.value("COORD_MP_INTEGRAL", SendPropSharedExt::COORD_MP_INTEGRAL);
	SendPropFlags.value("CHANGE_OFTEN", SendPropSharedExt::CHANGE_OFTEN);
}


//-----------------------------------------------------------------------------
// Expose ServerClass.
//-----------------------------------------------------------------------------
void export_server_class()
{
	class_<ServerClass, ServerClass *> ServerClass_("ServerClass", no_init);
	
	// Properties...
	ServerClass_.def_readonly("table", &ServerClass::m_pTable);
	ServerClass_.def_readonly("next", &ServerClass::m_pNext);
	ServerClass_.def_readonly("id", &ServerClass::m_ClassID);
	
	// Engine specific stuff...
	export_engine_specific_server_class(ServerClass_);
	
	// Add memory tools...
	ServerClass_ ADD_MEM_TOOLS(ServerClass, "ServerClass");
}


//-----------------------------------------------------------------------------
// Expose CTakeDamageInfo.
//-----------------------------------------------------------------------------
void export_take_damage_info()
{
	class_<CTakeDamageInfo, CTakeDamageInfo *> TakeDamageInfo("TakeDamageInfo");
	
	// Properties...
	TakeDamageInfo.add_property("force", &CTakeDamageInfo::GetDamageForce, &CTakeDamageInfo::SetDamageForce);
	TakeDamageInfo.add_property("position", &CTakeDamageInfo::GetDamagePosition, &CTakeDamageInfo::SetDamagePosition);
	
	TakeDamageInfo.add_property("reported_position", &CTakeDamageInfo::GetReportedPosition,
		&CTakeDamageInfo::SetReportedPosition
	);
	
	TakeDamageInfo.add_property("damage", &CTakeDamageInfo::GetDamage, &CTakeDamageInfo::SetDamage);
	TakeDamageInfo.add_property("base_damage", &CTakeDamageInfo::GetBaseDamage, &TakeDamageInfoSharedExt::set_base_damage);
	TakeDamageInfo.add_property("type", &CTakeDamageInfo::GetDamageType, &CTakeDamageInfo::SetDamageType);
	TakeDamageInfo.add_property("stats", &CTakeDamageInfo::GetDamageStats, &CTakeDamageInfo::SetDamageStats);
	TakeDamageInfo.add_property("ammo", &CTakeDamageInfo::GetAmmoType, &CTakeDamageInfo::SetAmmoType);
	
	TakeDamageInfo.add_property("damaged_other_players", &TakeDamageInfoSharedExt::get_damaged_other_players,
		&TakeDamageInfoSharedExt::set_damaged_other_players
	);

	TakeDamageInfo.add_property("inflictor", &TakeDamageInfoSharedExt::get_inflictor, &TakeDamageInfoSharedExt::set_inflictor);
	TakeDamageInfo.add_property("attacker", &TakeDamageInfoSharedExt::get_attacker, &TakeDamageInfoSharedExt::set_attacker);
	TakeDamageInfo.add_property("weapon", &TakeDamageInfoSharedExt::get_weapon, &TakeDamageInfoSharedExt::set_weapon);
	
	// CS:GO properties...
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("radius");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("bullet");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("recoil");
	
	// Engine specific stuff...
	export_engine_specific_take_damage_info(TakeDamageInfo);
	
	// Add memory tools...
	TakeDamageInfo ADD_MEM_TOOLS(CTakeDamageInfo, "TakeDamageInfo");
}
