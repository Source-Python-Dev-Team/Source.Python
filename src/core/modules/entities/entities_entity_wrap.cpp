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
	
	BaseEntity.def(
		"__eq__", 
		&CBaseEntityWrapper::operator==,
		"Return True if both entities are the same."
	);

	BaseEntity.def("create",
		&CBaseEntityWrapper::create,
		return_by_value_policy(),
		"Create an entity by its class name.\n\n"
		":rtype: BaseEntity"
	).staticmethod("create");

	BaseEntity.def("find",
		&CBaseEntityWrapper::find,
		return_by_value_policy(),
		"Return the first entity that has a matching class name.\n\n"
		":rtype: BaseEntity"
	).staticmethod("find");

	BaseEntity.def("find_or_create",
		&CBaseEntityWrapper::find_or_create,
		return_by_value_policy(),
		"Try to find an entity that has a matching class name. If no entity has been found, it will be created.\n\n"
		":rtype: BaseEntity"
	).staticmethod("find_or_create");

	// Others
	BaseEntity.def("is_player",
		&CBaseEntityWrapper::IsPlayer,
		"Return True if the entity is a player.\n\n"
		":rtype: bool"
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
		"The server class of this entity (read-only).\n\n"
		":rtype: ServerClass"
	);

	BaseEntity.add_property("datamap",
		make_function(&CBaseEntityWrapper::GetDataDescMap, reference_existing_object_policy()),
		"The data map of this entity (read-only).\n\n"
		":rtype: DataMap"
	);

	BaseEntity.add_property("factory",
		make_function(GET_METHOD(IEntityFactory*, CBaseEntityWrapper, get_factory), reference_existing_object_policy()),
		"Return the entity's factory.\n\n"
		":rtype: EntityFactory"
	);

	BaseEntity.add_property(
		"edict", 
		make_function(&CBaseEntityWrapper::GetEdict, reference_existing_object_policy()),
		"Return the edict of the entity.\n\n"
		":rtype: Edict");

	BaseEntity.add_property(
		"index",
		&CBaseEntityWrapper::GetIndex,
		"Return the index of the entity.\n\n"
		":raise ValueError: Raised if the entity does not have an index.\n"
		":rtype: int");

	BaseEntity.add_property(
		"pointer",
		make_function(&CBaseEntityWrapper::GetPointer),
		"Return the pointer of the entity.\n\n"
		":rtype: Pointer");

	BaseEntity.add_property(
		"inthandle",
		&CBaseEntityWrapper::GetIntHandle,
		"Return the handle of the entity.\n\n"
		":rtype: int");

	BaseEntity.add_property(
		"physics_object",
		make_function(&CBaseEntityWrapper::GetPhysicsObject, manage_new_object_policy()),
		"Return the physics object of the entity.\n\n"
		":rtype: PhysicsObject");

	// KeyValue getter methods
	BaseEntity.def("get_key_value_string",
		&CBaseEntityWrapper::GetKeyValueString,
		"Returns the value of the given field name.\n\n"
		":rtype: str",
		args("field_name")
	);

	BaseEntity.def("get_key_value_int",
		&CBaseEntityWrapper::GetKeyValueInt,
		"Returns the value of the given field name.\n\n"
		":rtype: int",
		args("field_name")
	);

	BaseEntity.def("get_key_value_float",
		&CBaseEntityWrapper::GetKeyValueFloat,
		"Returns the value of the given field name.\n\n"
		":rtype: float",
		args("field_name")
	);

	BaseEntity.def("get_key_value_vector",
		&CBaseEntityWrapper::GetKeyValueVector,
		"Returns the value of the given field name.\n\n"
		":rtype: Vector",
		args("field_name")
	);

	BaseEntity.def("get_key_value_bool",
		&CBaseEntityWrapper::GetKeyValueBool,
		"Returns the value of the given field name.\n\n"
		":rtype: bool",
		args("field_name")
	);

	BaseEntity.def("get_key_value_color",
		&CBaseEntityWrapper::GetKeyValueColor,
		"Returns the value of the given field name.\n\n"
		":rtype: Color",
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
		&CBaseEntityWrapper::GetDatamapProperty<bool>,
		"Return the value of the given data map field name.\n\n"
		":rtype: bool"
	);

	BaseEntity.def("get_datamap_property_char",
		&CBaseEntityWrapper::GetDatamapProperty<char>,
		"Return the value of the given data map field name.\n\n"
		":rtype: str"
	);

	BaseEntity.def("get_datamap_property_uchar",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned char>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_short",
		&CBaseEntityWrapper::GetDatamapProperty<short>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_ushort",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned short>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_int",
		&CBaseEntityWrapper::GetDatamapProperty<int>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_uint",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned int>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_long",
		&CBaseEntityWrapper::GetDatamapProperty<long>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_ulong",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned long>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_long_long",
		&CBaseEntityWrapper::GetDatamapProperty<long long>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_ulong_long",
		&CBaseEntityWrapper::GetDatamapProperty<unsigned long long>,
		"Return the value of the given data map field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_datamap_property_float",
		&CBaseEntityWrapper::GetDatamapProperty<float>,
		"Return the value of the given data map field name.\n\n"
		":rtype: float"
	);

	BaseEntity.def("get_datamap_property_double",
		&CBaseEntityWrapper::GetDatamapProperty<double>,
		"Return the value of the given data map field name.\n\n"
		":rtype: float"
	);

	BaseEntity.def("get_datamap_property_string_pointer",
		&CBaseEntityWrapper::GetDatamapProperty<const char*>,
		"Return the value of the given data map field name.\n\n"
		":rtype: str"
	);

	BaseEntity.def("get_datamap_property_string_array",
		&CBaseEntityWrapper::GetDatamapPropertyStringArray,
		"Return the value of the given data map field name.\n\n"
		":rtype: bstrool"
	);

	BaseEntity.def("get_datamap_property_pointer",
		&CBaseEntityWrapper::GetDatamapProperty<void*>,
		return_by_value_policy(),
		"Return the value of the given data map field name.\n\n"
		":rtype: Pointer"
	);

	BaseEntity.def("get_datamap_property_vector",
		&CBaseEntityWrapper::GetDatamapProperty<Vector>,
		"Return the value of the given data map field name.\n\n"
		":rtype: Vector"
	);

	BaseEntity.def("get_datamap_property_color",
		&CBaseEntityWrapper::GetDatamapProperty<Color>,
		"Return the value of the given data map field name.\n\n"
		":rtype: Color"
	);

	BaseEntity.def("get_datamap_property_interval",
		&CBaseEntityWrapper::GetDatamapProperty<interval_t>,
		"Return the value of the given data map field name.\n\n"
		":rtype: Interval"
	);

	BaseEntity.def("get_datamap_property_quaternion",
		&CBaseEntityWrapper::GetDatamapProperty<Quaternion>,
		"Return the value of the given data map field name.\n\n"
		":rtype: Quaternion"
	);

	// Datamap setter methods
	BaseEntity.def("set_datamap_property_bool",
		&CBaseEntityWrapper::SetDatamapProperty<bool>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_char",
		&CBaseEntityWrapper::SetDatamapProperty<char>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_uchar",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned char>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_short",
		&CBaseEntityWrapper::SetDatamapProperty<short>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_ushort",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned short>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_int",
		&CBaseEntityWrapper::SetDatamapProperty<int>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_uint",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned int>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_long",
		&CBaseEntityWrapper::SetDatamapProperty<long>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_ulong",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned long>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_long_long",
		&CBaseEntityWrapper::SetDatamapProperty<long long>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_ulong_long",
		&CBaseEntityWrapper::SetDatamapProperty<unsigned long long>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_float",
		&CBaseEntityWrapper::SetDatamapProperty<float>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_double",
		&CBaseEntityWrapper::SetDatamapProperty<double>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_string_pointer",
		&CBaseEntityWrapper::SetDatamapProperty<const char*>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_string_array",
		&CBaseEntityWrapper::SetDatamapPropertyStringArray,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_pointer",
		&CBaseEntityWrapper::SetDatamapProperty<void*>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_vector",
		&CBaseEntityWrapper::SetDatamapProperty<Vector>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_color",
		&CBaseEntityWrapper::SetDatamapProperty<Color>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_interval",
		&CBaseEntityWrapper::SetDatamapProperty<interval_t>,
		"Set the value of the given data map field name."
	);

	BaseEntity.def("set_datamap_property_quaternion",
		&CBaseEntityWrapper::SetDatamapProperty<Quaternion>,
		"Set the value of the given data map field name."
	);

	// Network property getters
	BaseEntity.def("get_network_property_bool",
		&CBaseEntityWrapper::GetNetworkProperty<bool>,
		"Return the value of the given server class field name.\n\n"
		":rtype: bool"
	);

	BaseEntity.def("get_network_property_char",
		&CBaseEntityWrapper::GetNetworkProperty<char>,
		"Return the value of the given server class field name.\n\n"
		":rtype: str"
	);

	BaseEntity.def("get_network_property_uchar",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned char>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_short",
		&CBaseEntityWrapper::GetNetworkProperty<short>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_ushort",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned short>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_int",
		&CBaseEntityWrapper::GetNetworkProperty<int>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_uint",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned int>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_long",
		&CBaseEntityWrapper::GetNetworkProperty<long>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_ulong",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned long>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_long_long",
		&CBaseEntityWrapper::GetNetworkProperty<long long>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_ulong_long",
		&CBaseEntityWrapper::GetNetworkProperty<unsigned long long>,
		"Return the value of the given server class field name.\n\n"
		":rtype: int"
	);

	BaseEntity.def("get_network_property_float",
		&CBaseEntityWrapper::GetNetworkProperty<float>,
		"Return the value of the given server class field name.\n\n"
		":rtype: float"
	);

	BaseEntity.def("get_network_property_double",
		&CBaseEntityWrapper::GetNetworkProperty<double>,
		"Return the value of the given server class field name.\n\n"
		":rtype: float"
	);

	BaseEntity.def("get_network_property_string_pointer",
		&CBaseEntityWrapper::GetNetworkProperty<const char*>,
		"Return the value of the given server class field name.\n\n"
		":rtype: str"
	);

	BaseEntity.def("get_network_property_string_array",
		&CBaseEntityWrapper::GetNetworkPropertyStringArray,
		"Return the value of the given server class field name.\n\n"
		":rtype: str"
	);

	BaseEntity.def("get_network_property_pointer",
		&CBaseEntityWrapper::GetNetworkProperty<void*>,
		return_by_value_policy(),
		"Return the value of the given server class field name.\n\n"
		":rtype: Pointer"
	);

	BaseEntity.def("get_network_property_vector",
		&CBaseEntityWrapper::GetNetworkProperty<Vector>,
		"Return the value of the given server class field name.\n\n"
		":rtype: Vector"
	);

	BaseEntity.def("get_network_property_color",
		&CBaseEntityWrapper::GetNetworkProperty<Color>,
		"Return the value of the given server class field name.\n\n"
		":rtype: Color"
	);

	BaseEntity.def("get_network_property_interval",
		&CBaseEntityWrapper::GetNetworkProperty<interval_t>,
		"Return the value of the given server class field name.\n\n"
		":rtype: Interval"
	);

	BaseEntity.def("get_network_property_quaternion",
		&CBaseEntityWrapper::GetNetworkProperty<Quaternion>,
		"Return the value of the given server class field name.\n\n"
		":rtype: Quaternion"
	);

	// Network property setters
	BaseEntity.def("set_network_property_bool",
		&CBaseEntityWrapper::SetNetworkProperty<bool>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_char",
		&CBaseEntityWrapper::SetNetworkProperty<char>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_uchar",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned char>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_short",
		&CBaseEntityWrapper::SetNetworkProperty<short>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_ushort",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned short>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_int",
		&CBaseEntityWrapper::SetNetworkProperty<int>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_uint",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned int>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_long",
		&CBaseEntityWrapper::SetNetworkProperty<long>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_ulong",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned long>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_long_long",
		&CBaseEntityWrapper::SetNetworkProperty<long long>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_ulong_long",
		&CBaseEntityWrapper::SetNetworkProperty<unsigned long long>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_float",
		&CBaseEntityWrapper::SetNetworkProperty<float>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_double",
		&CBaseEntityWrapper::SetNetworkProperty<double>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_string_pointer",
		&CBaseEntityWrapper::SetNetworkProperty<const char*>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_string_array",
		&CBaseEntityWrapper::SetNetworkPropertyStringArray,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_pointer",
		&CBaseEntityWrapper::SetNetworkProperty<void*>,
		return_by_value_policy(),
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_vector",
		&CBaseEntityWrapper::SetNetworkProperty<Vector>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_color",
		&CBaseEntityWrapper::SetNetworkProperty<Color>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_interval",
		&CBaseEntityWrapper::SetNetworkProperty<interval_t>,
		"Set the value of the given server class field name."
	);

	BaseEntity.def("set_network_property_quaternion",
		&CBaseEntityWrapper::SetNetworkProperty<Quaternion>,
		"Set the value of the given server class field name."
	);

	// Add memory tools...
	BaseEntity ADD_MEM_TOOLS(CBaseEntityWrapper);

	// This must be after ADD_MEM_TOOLS, because it overrides the _size attribute
	BaseEntity.add_property("_size",
		&CBaseEntityWrapper::get_size,
		"Return the entity's size.\n\n"
		":rtype: int"
	);
}
