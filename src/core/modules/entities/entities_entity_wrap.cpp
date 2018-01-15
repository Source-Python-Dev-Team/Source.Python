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

	BaseEntity.add_property(
		"origin",
		&CBaseEntityWrapper::GetOrigin,
		&CBaseEntityWrapper::SetOrigin,
		"Get/set the entity's origin.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"maxs",
		&CBaseEntityWrapper::GetMaxs,
		&CBaseEntityWrapper::SetMaxs,
		"Get/set the entity's maximum dimension.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"mins",
		&CBaseEntityWrapper::GetMins,
		&CBaseEntityWrapper::SetMins,
		"Get/set the entity's minimum dimension.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"solid_type",
		&CBaseEntityWrapper::GetSolidType,
		&CBaseEntityWrapper::SetSolidType,
		"Get/set the entity's solid type.\n\n"
		":rtype: SolidType"
	);

	BaseEntity.add_property(
		"solid_flags",
		&CBaseEntityWrapper::GetSolidFlags,
		&CBaseEntityWrapper::SetSolidFlags,
		"Get/set the entity's solid flags.\n\n"
		":rtype: SolidFlags"
	);

	BaseEntity.add_property(
		"collision_group",
		&CBaseEntityWrapper::GetCollisionGroup,
		&CBaseEntityWrapper::SetCollisionGroup,
		"Get/set the entity's collision group.\n\n"
		":rtype: CollisionGroup"
	);

	BaseEntity.add_property(
		"render_color",
		&CBaseEntityWrapper::GetRenderColor,
		&CBaseEntityWrapper::SetRenderColor,
		"Get/set the entity's render color.\n\n"
		":rtype: Color"
	);

	BaseEntity.add_property(
		"elasticity",
		&CBaseEntityWrapper::GetElasticity,
		&CBaseEntityWrapper::SetElasticity,
		"Get/set the entity's elasticity.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"ground_entity",
		&CBaseEntityWrapper::GetGroundEntity,
		&CBaseEntityWrapper::SetGroundEntity,
		"Get/set the entity's ground entity.\n\n"
		":return: ``-1`` if the entity has no ground entity (does not stand on an entity). The returned value is an int handle.\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"team_index",
		&CBaseEntityWrapper::GetTeamIndex,
		&CBaseEntityWrapper::SetTeamIndex,
		"Get/set the entity's team index.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"render_fx",
		&CBaseEntityWrapper::GetRenderFx,
		&CBaseEntityWrapper::SetRenderFx,
		"Get/set the entity's render effects.\n\n"
		":rtype: RenderEffects"
	);

	BaseEntity.add_property(
		"render_mode",
		&CBaseEntityWrapper::GetRenderMode,
		&CBaseEntityWrapper::SetRenderMode,
		"Get/set the entity's render mode.\n\n"
		":rtype: RenderMode"
	);

	BaseEntity.add_property(
		"move_type",
		&CBaseEntityWrapper::GetMoveType,
		&CBaseEntityWrapper::SetMoveType,
		"Get/set the entity's move type.\n\n"
		":rtype: MoveType"
	);

	BaseEntity.add_property(
		"parent_inthandle", // TODO: Rename this for consistency to parent_handle
		&CBaseEntityWrapper::GetParentHandle,
		&CBaseEntityWrapper::SetParentHandle,
		"Get/set the entity's parent handle.\n\n"
		":return: ``-1`` if the entity has no parent entity. The returned value is an int handle.\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"angles",
		&CBaseEntityWrapper::GetAngles,
		&CBaseEntityWrapper::SetAngles,
		"Get/set the entity's angles.\n\n"
		":rtype: QAngle"
	);

	BaseEntity.add_property(
		"target_name",
		&CBaseEntityWrapper::GetTargetName,
		&CBaseEntityWrapper::SetTargetName,
		"Get/set the entity's target name.\n\n"
		":rtype: str"
	);

	BaseEntity.add_property(
		"owner_handle",
		&CBaseEntityWrapper::GetOwnerHandle,
		&CBaseEntityWrapper::SetOwnerHandle,
		"Get/set the entity's owner handle.\n\n"
		":return: ``-1`` if the entity has no owner entity. The returned value is an int handle.\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"avelocity",
		&CBaseEntityWrapper::GetAvelocity,
		&CBaseEntityWrapper::SetAvelocity,
		"Get/set the entity's avelocity.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"base_velocity",
		&CBaseEntityWrapper::GetBaseVelocity,
		&CBaseEntityWrapper::SetBaseVelocity,
		"Get/set the entity's base velocity.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"damage_filter",
		&CBaseEntityWrapper::GetDamageFilter,
		&CBaseEntityWrapper::SetDamageFilter,
		"Get/set the entity's damage filter.\n\n"
		":rtype: str"
	);

	BaseEntity.add_property(
		"effects",
		&CBaseEntityWrapper::GetEffects,
		&CBaseEntityWrapper::SetEffects,
		"Get/set the entity's effects.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"friction",
		&CBaseEntityWrapper::GetFriction,
		&CBaseEntityWrapper::SetFriction,
		"Get/set the entity's friction.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"global_name",
		&CBaseEntityWrapper::GetGlobalName,
		&CBaseEntityWrapper::SetGlobalName,
		"Get/set the entity's global name.\n\n"
		":rtype: str"
	);

	BaseEntity.add_property(
		"gravity",
		&CBaseEntityWrapper::GetGravity,
		&CBaseEntityWrapper::SetGravity,
		"Get/set the entity's gravity. This is a multiplicator of ``sv_gravity``.\n"
		"E. g. set the value to ``0.0`` or ``1.0`` for normal gravity. ``0.5`` halves the gravity.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"hammerid",
		&CBaseEntityWrapper::GetHammerID,
		&CBaseEntityWrapper::SetHammerID,
		"Get/set the entity's Hammer ID.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"health",
		&CBaseEntityWrapper::GetHealth,
		&CBaseEntityWrapper::SetHealth,
		"Get/set the entity's health.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"local_time",
		&CBaseEntityWrapper::GetLocalTime,
		&CBaseEntityWrapper::SetLocalTime,
		"Get/set the entity's local time.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"max_health",
		&CBaseEntityWrapper::GetMaxHealth,
		&CBaseEntityWrapper::SetMaxHealth,
		"Get/set the entity's maximum health.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"shadow_cast_distance",
		&CBaseEntityWrapper::GetShadowCastDistance,
		&CBaseEntityWrapper::SetShadowCastDistance,
		"Get/set the entity's shadow cast distance.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"local_time",
		&CBaseEntityWrapper::GetLocalTime,
		&CBaseEntityWrapper::SetLocalTime,
		"Get/set the entity's local time.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"spawn_flags",
		&CBaseEntityWrapper::GetSpawnFlags,
		&CBaseEntityWrapper::SetSpawnFlags,
		"Get/set the entity's spawn flags.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"speed",
		&CBaseEntityWrapper::GetSpeed,
		&CBaseEntityWrapper::SetSpeed,
		"Get/set the entity's speed.\n\n"
		":rtype: float"
	);

	BaseEntity.add_property(
		"target",
		&CBaseEntityWrapper::GetTarget,
		&CBaseEntityWrapper::SetTarget,
		"Get/set the entity's target.\n\n"
		":rtype: str"
	);

	BaseEntity.add_property(
		"velocity",
		&CBaseEntityWrapper::GetVelocity,
		&CBaseEntityWrapper::SetVelocity,
		"Get/set the entity's velocity.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"view_offset",
		&CBaseEntityWrapper::GetViewOffset,
		&CBaseEntityWrapper::SetViewOffset,
		"Get/set the entity's view offset.\n\n"
		":rtype: Vector"
	);

	BaseEntity.add_property(
		"water_level",
		&CBaseEntityWrapper::GetWaterLevel,
		&CBaseEntityWrapper::SetWaterLevel,
		"Get/set the entity's water level.\n\n"
		"The water level is a value between 0 - 3 and indicates how far the entity is covered with water:\n\n"
		"1. Entity is completely dry.\n"
		"2. Entity has contact with water.\n"
		"3. Entity is half inside of water.\n"
		"4. Entity is completely under water.\n\n"
		":rtype: int"
	);

	BaseEntity.add_property(
		"color",
		&CBaseEntityWrapper::GetColor,
		&CBaseEntityWrapper::SetColor,
		"Get/set the entity's color.\n\n"
		":rtype: Color"
	);

	BaseEntity.add_property(
		"rotation",
		&CBaseEntityWrapper::GetRotation,
		&CBaseEntityWrapper::SetRotation,
		"Get/set the entity's rotation.\n\n"
		":rtype: QAngle"
	);

	BaseEntity.def(
		"get_eye_location",
		&CBaseEntityWrapper::GetEyeLocation,
		"Return the entitiy's eye location.\n\n"
		":rtype: Vector"
	);
	
	BaseEntity.def(
		"stop_sound",
		&CBaseEntityWrapper::StopSound,
		(arg("sample"), arg("channel")=(int)CHAN_AUTO),
		"Stop the given sound from being emitted by this entity.\n\n"
		":param str sample: Sound file relative to the ``sounds`` directory.\n"
        ":param Channel channel: The channel of the sound."
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

	BaseEntity.def("get_key_value_qangle",
		&CBaseEntityWrapper::GetKeyValueQAngle,
		"Returns the value of the given field name.\n\n"
		":rtype: QAngle",
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

	BaseEntity.def("set_key_value_qangle",
		&CBaseEntityWrapper::SetKeyValueQAngle,
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
