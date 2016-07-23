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
#include "modules/memory/memory_tools.h"
#include "export_main.h"
#include "utilities/conversions.h"
#include "entities_entity.h"
#include "modules/physics/physics.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_base_entity(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._entity module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _entity)
{
	export_base_entity(_entity);
}


//-----------------------------------------------------------------------------
// Exports CBaseEntity.
//-----------------------------------------------------------------------------
void export_base_entity(scope _entity)
{
	class_<CBaseEntityWrapper, boost::shared_ptr<CBaseEntityWrapper>, bases<IServerEntity>, boost::noncopyable> BaseEntity("BaseEntity", no_init);

	// Initializers...
	BaseEntity.def("__init__",
		make_constructor(
			&CBaseEntityWrapper::__init__,
			default_call_policies(),
			args("entity_index")
		)
	);

	BaseEntity.def("create",
		&CBaseEntityWrapper::create,
		return_by_value_policy()
	).staticmethod("create");

	BaseEntity.def("find",
		&CBaseEntityWrapper::find,
		return_by_value_policy()
	).staticmethod("find");

	BaseEntity.def("find_or_create",
		&CBaseEntityWrapper::find_or_create,
		return_by_value_policy()
	).staticmethod("find_or_create");

	// Others
	BaseEntity.def("is_player",
		&CBaseEntityWrapper::IsPlayer,
		"Return True if the entity is a player."
	);

	BaseEntity.def("remove",
		&CBaseEntityWrapper::remove,
		"Remove the entity."
	);

	BaseEntity.def("spawn",
		&CBaseEntityWrapper::spawn,
		"Spawn the entity."
	);

	// Properties...
	BaseEntity.add_property("server_class",
		make_function(&CBaseEntityWrapper::GetServerClass, reference_existing_object_policy()),
		"The ServerClass instance of this entity (read-only)."
	);

	BaseEntity.add_property("datamap",
		make_function(&CBaseEntityWrapper::GetDataDescMap, reference_existing_object_policy()),
		"The DataMap instance of this entity (read-only)."
	);

	BaseEntity.add_property("factory",
		make_function(GET_METHOD(IEntityFactory*, CBaseEntityWrapper, get_factory), reference_existing_object_policy()),
		"Return the entity's factory."
	);

	BaseEntity.add_property("edict", make_function(&CBaseEntityWrapper::GetEdict, reference_existing_object_policy()));
	BaseEntity.add_property("index", &CBaseEntityWrapper::GetIndex);
	BaseEntity.add_property("pointer", make_function(&CBaseEntityWrapper::GetPointer));
	BaseEntity.add_property("inthandle", &CBaseEntityWrapper::GetIntHandle);
	BaseEntity.add_property("physics_object", make_function(&CBaseEntityWrapper::GetPhysicsObject, manage_new_object_policy()));

	// KeyValue getter methods
	BaseEntity.def("get_key_value_string",
		&CBaseEntityWrapper::GetKeyValueString,
		"Returns the value of the given field name.",
		args("field_name")
	);

	BaseEntity.def("get_key_value_int",
		&CBaseEntityWrapper::GetKeyValueInt,
		"Returns the value of the given field name.",
		args("field_name")
	);

	BaseEntity.def("get_key_value_float",
		&CBaseEntityWrapper::GetKeyValueFloat,
		"Returns the value of the given field name.",
		args("field_name")
	);

	BaseEntity.def("get_key_value_vector",
		&CBaseEntityWrapper::GetKeyValueVector,
		"Returns the value of the given field name.",
		args("field_name")
	);

	BaseEntity.def("get_key_value_bool",
		&CBaseEntityWrapper::GetKeyValueBool,
		"Returns the value of the given field name.",
		args("field_name")
	);

	BaseEntity.def("get_key_value_color",
		&CBaseEntityWrapper::GetKeyValueColor,
		"Returns the value of the given field name.",
		args("field_name")
	);

	// KeyValue setter methods
	BaseEntity.def("set_key_value_int",
		&CBaseEntityWrapper::SetKeyValue<int>,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	BaseEntity.def("set_key_value_float",
		&CBaseEntityWrapper::SetKeyValue<float>,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	BaseEntity.def("set_key_value_string",
		&CBaseEntityWrapper::SetKeyValue<const char *>,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	BaseEntity.def("set_key_value_vector",
		&CBaseEntityWrapper::SetKeyValue<Vector>,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	BaseEntity.def("set_key_value_bool",
		&CBaseEntityWrapper::SetKeyValue<bool>,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	BaseEntity.def("set_key_value_color",
		&CBaseEntityWrapper::SetKeyValueColor,
		"Sets a field to the given value.",
		args("field_name", "value")
	);

	// Datamap getter methods
	BaseEntity.def("get_datamap_property_bool",
		&CBaseEntityWrapper::GetDatamapProperty<bool>
	);

	BaseEntity.def("get_datamap_property_char",
		&CBaseEntityWrapper::GetDatamapProperty<char>
	);

	BaseEntity.def("get_datamap_property_uchar",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned char>
	);

	BaseEntity.def("get_datamap_property_short",
		&CBaseEntityWrapper::GetDatamapProperty<short>
	);

	BaseEntity.def("get_datamap_property_ushort",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned short>
	);

	BaseEntity.def("get_datamap_property_int",
		&CBaseEntityWrapper::GetDatamapProperty<int>
	);

	BaseEntity.def("get_datamap_property_uint",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned int>
	);

	BaseEntity.def("get_datamap_property_long",
		&CBaseEntityWrapper::GetDatamapProperty<long>
	);

	BaseEntity.def("get_datamap_property_ulong",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned long>
	);

	BaseEntity.def("get_datamap_property_long_long",
		&CBaseEntityWrapper::GetDatamapProperty<long long>
	);

	BaseEntity.def("get_datamap_property_ulong_long",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned long long>
	);

	BaseEntity.def("get_datamap_property_float",
		&CBaseEntityWrapper::GetDatamapProperty<float>
	);

	BaseEntity.def("get_datamap_property_double",
		&CBaseEntityWrapper::GetDatamapProperty<double>
	);

	BaseEntity.def("get_datamap_property_string_pointer",
		&CBaseEntityWrapper::GetDatamapProperty<const char*>
	);

	BaseEntity.def("get_datamap_property_string_array",
		&CBaseEntityWrapper::GetDatamapPropertyStringArray
	);

	BaseEntity.def("get_datamap_property_pointer",
		&CBaseEntityWrapper::GetDatamapProperty<void*>,
		return_by_value_policy()
	);

	BaseEntity.def("get_datamap_property_vector",
		&CBaseEntityWrapper::GetDatamapProperty<Vector>
	);

	BaseEntity.def("get_datamap_property_color",
		&CBaseEntityWrapper::GetDatamapProperty<Color>
	);

	BaseEntity.def("get_datamap_property_interval",
		&CBaseEntityWrapper::GetDatamapProperty<interval_t>
	);

	BaseEntity.def("get_datamap_property_quaternion",
		&CBaseEntityWrapper::GetDatamapProperty<Quaternion>
	);

	// Datamap setter methods
	BaseEntity.def("set_datamap_property_bool",
		&CBaseEntityWrapper::SetDatamapProperty<bool>
	);

	BaseEntity.def("set_datamap_property_char",
		&CBaseEntityWrapper::SetDatamapProperty<char>
	);

	BaseEntity.def("set_datamap_property_uchar",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned char>
	);

	BaseEntity.def("set_datamap_property_short",
		&CBaseEntityWrapper::SetDatamapProperty<short>
	);

	BaseEntity.def("set_datamap_property_ushort",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned short>
	);

	BaseEntity.def("set_datamap_property_int",
		&CBaseEntityWrapper::SetDatamapProperty<int>
	);

	BaseEntity.def("set_datamap_property_uint",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned int>
	);

	BaseEntity.def("set_datamap_property_long",
		&CBaseEntityWrapper::SetDatamapProperty<long>
	);

	BaseEntity.def("set_datamap_property_ulong",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned long>
	);

	BaseEntity.def("set_datamap_property_long_long",
		&CBaseEntityWrapper::SetDatamapProperty<long long>
	);

	BaseEntity.def("set_datamap_property_ulong_long",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned long long>
	);

	BaseEntity.def("set_datamap_property_float",
		&CBaseEntityWrapper::SetDatamapProperty<float>
	);

	BaseEntity.def("set_datamap_property_double",
		&CBaseEntityWrapper::SetDatamapProperty<double>
	);

	BaseEntity.def("set_datamap_property_string_pointer",
		&CBaseEntityWrapper::SetDatamapProperty<const char*>
	);

	BaseEntity.def("set_datamap_property_string_array",
		&CBaseEntityWrapper::SetDatamapPropertyStringArray
	);

	BaseEntity.def("set_datamap_property_pointer",
		&CBaseEntityWrapper::SetDatamapProperty<void*>
	);

	BaseEntity.def("set_datamap_property_vector",
		&CBaseEntityWrapper::SetDatamapProperty<Vector>
	);

	BaseEntity.def("set_datamap_property_color",
		&CBaseEntityWrapper::SetDatamapProperty<Color>
	);

	BaseEntity.def("set_datamap_property_interval",
		&CBaseEntityWrapper::SetDatamapProperty<interval_t>
	);

	BaseEntity.def("set_datamap_property_quaternion",
		&CBaseEntityWrapper::SetDatamapProperty<Quaternion>
	);

	// Network property getters
	BaseEntity.def("get_network_property_bool",
		&CBaseEntityWrapper::GetNetworkProperty<bool>
	);

	BaseEntity.def("get_network_property_char",
		&CBaseEntityWrapper::GetNetworkProperty<char>
	);

	BaseEntity.def("get_network_property_uchar",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned char>
	);

	BaseEntity.def("get_network_property_short",
		&CBaseEntityWrapper::GetNetworkProperty<short>
	);

	BaseEntity.def("get_network_property_ushort",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned short>
	);

	BaseEntity.def("get_network_property_int",
		&CBaseEntityWrapper::GetNetworkProperty<int>
	);

	BaseEntity.def("get_network_property_uint",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned int>
	);

	BaseEntity.def("get_network_property_long",
		&CBaseEntityWrapper::GetNetworkProperty<long>
	);

	BaseEntity.def("get_network_property_ulong",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned long>
	);

	BaseEntity.def("get_network_property_long_long",
		&CBaseEntityWrapper::GetNetworkProperty<long long>
	);

	BaseEntity.def("get_network_property_ulong_long",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned long long>
	);

	BaseEntity.def("get_network_property_float",
		&CBaseEntityWrapper::GetNetworkProperty<float>
	);

	BaseEntity.def("get_network_property_double",
		&CBaseEntityWrapper::GetNetworkProperty<double>
	);

	BaseEntity.def("get_network_property_string_pointer",
		&CBaseEntityWrapper::GetNetworkProperty<const char*>
	);

	BaseEntity.def("get_network_property_string_array",
		&CBaseEntityWrapper::GetNetworkPropertyStringArray
	);

	BaseEntity.def("get_network_property_pointer",
		&CBaseEntityWrapper::GetNetworkProperty<void*>,
		return_by_value_policy()
	);

	BaseEntity.def("get_network_property_vector",
		&CBaseEntityWrapper::GetNetworkProperty<Vector>
	);

	BaseEntity.def("get_network_property_color",
		&CBaseEntityWrapper::GetNetworkProperty<Color>
	);

	BaseEntity.def("get_network_property_interval",
		&CBaseEntityWrapper::GetNetworkProperty<interval_t>
	);

	BaseEntity.def("get_network_property_quaternion",
		&CBaseEntityWrapper::GetNetworkProperty<Quaternion>
	);

	// Network property setters
	BaseEntity.def("set_network_property_bool",
		&CBaseEntityWrapper::SetNetworkProperty<bool>
	);

	BaseEntity.def("set_network_property_char",
		&CBaseEntityWrapper::SetNetworkProperty<char>
	);

	BaseEntity.def("set_network_property_uchar",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned char>
	);

	BaseEntity.def("set_network_property_short",
		&CBaseEntityWrapper::SetNetworkProperty<short>
	);

	BaseEntity.def("set_network_property_ushort",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned short>
	);

	BaseEntity.def("set_network_property_int",
		&CBaseEntityWrapper::SetNetworkProperty<int>
	);

	BaseEntity.def("set_network_property_uint",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned int>
	);

	BaseEntity.def("set_network_property_long",
		&CBaseEntityWrapper::SetNetworkProperty<long>
	);

	BaseEntity.def("set_network_property_ulong",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned long>
	);

	BaseEntity.def("set_network_property_long_long",
		&CBaseEntityWrapper::SetNetworkProperty<long long>
	);

	BaseEntity.def("set_network_property_ulong_long",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned long long>
	);

	BaseEntity.def("set_network_property_float",
		&CBaseEntityWrapper::SetNetworkProperty<float>
	);

	BaseEntity.def("set_network_property_double",
		&CBaseEntityWrapper::SetNetworkProperty<double>
	);

	BaseEntity.def("set_network_property_string_pointer",
		&CBaseEntityWrapper::SetNetworkProperty<const char*>
	);

	BaseEntity.def("set_network_property_string_array",
		&CBaseEntityWrapper::SetNetworkPropertyStringArray
	);

	BaseEntity.def("set_network_property_pointer",
		&CBaseEntityWrapper::SetNetworkProperty<void*>,
		return_by_value_policy()
	);

	BaseEntity.def("set_network_property_vector",
		&CBaseEntityWrapper::SetNetworkProperty<Vector>
	);

	BaseEntity.def("set_network_property_color",
		&CBaseEntityWrapper::SetNetworkProperty<Color>
	);

	BaseEntity.def("set_network_property_interval",
		&CBaseEntityWrapper::SetNetworkProperty<interval_t>
	);

	BaseEntity.def("set_network_property_quaternion",
		&CBaseEntityWrapper::SetNetworkProperty<Quaternion>
	);

	// Add memory tools...
	BaseEntity ADD_MEM_TOOLS(CBaseEntityWrapper);

	// This must be after ADD_MEM_TOOLS, because it overrides the _size attribute
	BaseEntity.add_property("_size",
		&CBaseEntityWrapper::get_size,
		"Return the entity's size."
	);
}
