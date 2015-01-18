/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
// Includes
//-----------------------------------------------------------------------------
#include "edict.h"

#include "entities_helpers_wrap.h"
#include "entities_factories_wrap.h"
#include "entities_generator_wrap.h"
#include "entities_wrap.h"
#include "utility/sp_util.h"
#include "modules/export_main.h"
using namespace boost::python;

#include "modules/memory/memory_tools.h"
#include ENGINE_INCLUDE_PATH(entities_helpers_wrap.h)
#include ENGINE_INCLUDE_PATH(entities_wrap_python.h)


//-----------------------------------------------------------------------------
// Entity module definition.
//-----------------------------------------------------------------------------
void export_helper_functions();
void export_base_entity_handle();
void export_handle_entity();
void export_server_unknown();
void export_server_entity();
void export_server_networkable();
void export_edict();
void export_entity_generator();
void export_entity_factory();
void export_entity_factory_dictionary_interface();
void export_entity_factory_dictionary();

DECLARE_SP_MODULE(_entities)
{
	export_helper_functions();
	export_base_entity_handle();
	export_handle_entity();
	export_server_unknown();
	export_server_entity();
	export_server_networkable();
	export_edict();
	export_entity_generator();
	export_entity_factory();
	export_entity_factory_dictionary_interface();
	export_entity_factory_dictionary();
}


//-----------------------------------------------------------------------------
// Exports helper functions.
//-----------------------------------------------------------------------------
void export_helper_functions()
{
	def("create_entity", &create_entity, args("class_name"));
	def("remove_entity", &remove_entity, args("entity_index"));
	def("spawn_entity", &spawn_entity, args("entity_index"));
}


//-----------------------------------------------------------------------------
// Exports CBaseEntityHandle.
//-----------------------------------------------------------------------------
void export_base_entity_handle()
{
	class_<CBaseHandle>("BaseEntityHandle")
		.def(init<CBaseHandle&>())
		.def(init<unsigned long>())
		.def(init<int, int>())

		.def("init",
			&CBaseHandle::Init,
			args("entry", "serial_number")
		)

		.def("term",
			&CBaseHandle::Term
		)

		.def("is_valid",
			&CBaseHandle::IsValid,
			"Returns whether the handle has been initted with any values."
		)

		.def("get_entry_index",
			&CBaseHandle::GetEntryIndex
		)

		.def("get_serial_number",
			&CBaseHandle::GetSerialNumber
		)

		.def("to_int",
			&CBaseHandle::ToInt
		)

		.def(self != self)
		.def(self == self)
		.def(self < self)

		.def("set",
			&CBaseHandle::Set,
			args("entity"),
			"Assigns a value to the handle.",
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(CBaseHandle, "BaseEntityHandle")
	;
}

//-----------------------------------------------------------------------------
// Exports CHandleEntity
//-----------------------------------------------------------------------------
void export_handle_entity()
{
	class_<IHandleEntity, boost::noncopyable>("HandleEntity", no_init)
		.def("set_ref_ehandle",
			&IHandleEntity::SetRefEHandle,
			args("handle")
		)

		.def("get_ref_ehandle",
			&IHandleEntity::GetRefEHandle,
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(IHandleEntity, "HandleEntity")
	;
}

//-----------------------------------------------------------------------------
// Exports CServerUnknown.
//-----------------------------------------------------------------------------
void export_server_unknown()
{
	class_< IServerUnknown, bases<IHandleEntity>, boost::noncopyable >("ServerUnknown", no_init)
		.def("get_collideable",
			&IServerUnknown::GetCollideable,
			"Returns the Collideable object for this entity.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&IServerUnknown::GetNetworkable,
			"Returns the ServerNetworkable object for this entity.",
			reference_existing_object_policy()
		)

		.def("get_base_entity",
			&IServerUnknownExt::GetBaseEntity,
			"Returns the CBasEntity pointer for this entity."
		)

		ADD_MEM_TOOLS(IServerUnknown, "ServerUnknown")
	;
}

//-----------------------------------------------------------------------------
// Exports CServerEntity.
//-----------------------------------------------------------------------------
void export_server_entity()
{
	class_< IServerEntity, bases<IServerUnknown>, boost::noncopyable >("ServerEntity", no_init)
		.def("get_model_index",
			&IServerEntity::GetModelIndex,
			"Returns the model index for this entity."
		)

		.def("get_model_name",
			&IServerEntity::GetModelName,
			"Returns the name of the model this entity is using."
		)

		.def("set_model_index",
			&IServerEntity::SetModelIndex,
			"Sets the model of this entity.",
			args("index")
		)

		ADD_MEM_TOOLS(IServerEntity, "ServerEntity")
	;
}


//-----------------------------------------------------------------------------
// Exports CServerNetworkable.
//-----------------------------------------------------------------------------
void export_server_networkable()
{
	class_< IServerNetworkable, IServerNetworkable *, boost::noncopyable >("ServerNetworkable", no_init)
		.def("get_entity_handle",
			&IServerNetworkable::GetEntityHandle,
			"Returns the HandleEntity instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_server_class",
			&IServerNetworkable::GetServerClass,
			"Returns the ServerClass instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_edict",
			&IServerNetworkable::GetEdict,
			"Returns the edict_t instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_class_name",
			&IServerNetworkable::GetClassName,
			"Returns the class name of this entity."
		)

		.def("release",
			&IServerNetworkable::Release
		)

		.def("area_num",
			&IServerNetworkable::AreaNum
		)

		/*
		.def("get_base_networkable",
			&IServerNetworkable::GetBaseNetworkable,
			reference_existing_object_policy()
		)
		*/

		.def("get_base_entity",
			&ServerNetworkableSharedExt::get_base_entity,
			reference_existing_object_policy()
		)

		.def("get_pvs_info",
			&IServerNetworkable::GetPVSInfo,
			"Returns the current visible data.",
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(IServerNetworkable, "ServerNetworkable")
	;
}

//-----------------------------------------------------------------------------
// Exports edict_t.
//-----------------------------------------------------------------------------
void export_edict()
{
	class_< CBaseEdict >("_BaseEdict")
		.def("get_server_entity",
			GET_METHOD(IServerEntity*, CBaseEdict, GetIServerEntity),
			"Returns its ServerEntity instance.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&CBaseEdict::GetNetworkable,
			"Returns its ServerNetworkable instance.",
			reference_existing_object_policy()
		)

		.def("get_unknown",
			&CBaseEdict::GetUnknown,
			"Returns its ServerUnknown instance.",
			reference_existing_object_policy()
		)

		.def("set_edict",
			&CBaseEdict::SetEdict,
			"Set when initting an entity. If it's only a networkable, this is false.",
			args("unknown", "full_edict")
		)

		.def("area_num",
			&CBaseEdict::AreaNum
		)

		.def("get_class_name",
			&CBaseEdict::GetClassName,
			"Returns the edict's class name."
		)

		.def("is_free",
			&CBaseEdict::IsFree
		)

		.def("set_free",
			&CBaseEdict::SetFree
		)

		.def("clear_free",
			&CBaseEdict::ClearFree
		)

		.def("has_state_changed",
			&CBaseEdict::HasStateChanged
		)

		.def("clear_state_changed",
			&CBaseEdict::ClearStateChanged
		)

		.def("state_changed",
			GET_METHOD(void, CBaseEdict, StateChanged)
		)

		/*
		.def("state_changed",
			static_cast< void(CBaseEdict::*)(unsigned short) >(&CBaseEdict::StateChanged),
			args("offset")
		)
		*/

		.def("clear_transmit_state",
			&CBaseEdict::ClearTransmitState
		)

		.def("set_change_info",
			&CBaseEdict::SetChangeInfo,
			args("info")
		)

		.def("set_change_info_serial_number",
			&CBaseEdict::SetChangeInfoSerialNumber,
			args("serial_number")
		)

		.def("get_change_info",
			&CBaseEdict::GetChangeInfo
		)

		.def("get_change_info_serial_number",
			&CBaseEdict::GetChangeInfoSerialNumber
		)

		.def("get_change_accessor",
			GET_METHOD(IChangeInfoAccessor*, CBaseEdict, GetChangeAccessor),
			reference_existing_object_policy()
		)

		// Class attributes
		.def_readwrite("state_flags",
			&CBaseEdict::m_fStateFlags
		)

		.def_readwrite("serial_number",
			&CBaseEdict::m_NetworkSerialNumber
		)

		.def_readwrite("networkable",
			&CBaseEdict::m_pNetworkable
		)

		ADD_MEM_TOOLS(CBaseEdict, "_BaseEdict")
	;

	class_< edict_t, bases<CBaseEdict> >("Edict")
		.def("get_collidable",
			&edict_t::GetCollideable,
			"Returns its Collideable instance.",
			reference_existing_object_policy()
		)

		.def("get_key_value_string",
			&CEdictExt::GetKeyValueString,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("get_key_value_int",
			&CEdictExt::GetKeyValueInt,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("get_key_value_float",
			&CEdictExt::GetKeyValueFloat,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("get_key_value_vector",
			&CEdictExt::GetKeyValueVector,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("get_key_value_bool",
			&CEdictExt::GetKeyValueBool,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("get_key_value_color",
			&CEdictExt::GetKeyValueColor,
			"Returns the value of the given field name.",
			args("field_name")
		)

		.def("set_key_value_int",
			&CEdictExt::SetKeyValue<int>,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		.def("set_key_value_float",
			&CEdictExt::SetKeyValue<float>,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		.def("set_key_value_string",
			&CEdictExt::SetKeyValue<const char *>,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		.def("set_key_value_vector",
			&CEdictExt::SetKeyValue<Vector>,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		.def("set_key_value_bool",
			&CEdictExt::SetKeyValue<bool>,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		.def("set_key_value_color",
			&CEdictExt::SetKeyValueColor,
			"Sets a field to the given value.",
			args("field_name", "value")
		)

		// Class attributes
		/*
		// TODO: Missing on CS:GO
		.def_readwrite("free_time",
			&edict_t::freetime,
			"The server timestampe at which the edict was freed."
		)
		*/

		ADD_MEM_TOOLS(edict_t, "Edict")
	;
}

//-----------------------------------------------------------------------------
// Exports CEntityGenerator.
//-----------------------------------------------------------------------------
void export_entity_generator()
{
	class_<CEntityGenerator>("EntityGenerator")
		.def(init<const char*>())
		.def(init<const char*, bool>())

		.def("__iter__",
			&CEntityGenerator::iter,
			"Returns the iterable object."
		)

		.def("__next__",
			&CEntityGenerator::next,
			"Returns the next valid instance.",
			reference_existing_object_policy()
		)
	;
}

//-----------------------------------------------------------------------------
// Expose IEntityFactory.
//-----------------------------------------------------------------------------
void export_entity_factory()
{
	class_<IEntityFactory, boost::noncopyable> EntityFactory("EntityFactory", no_init);
	
	// Methods...
	EntityFactory.def("create",
		&IEntityFactory::Create,
		reference_existing_object_policy()
	);
	
	EntityFactory.def("destroy", &IEntityFactory::Destroy);
	
	// Properties...
	EntityFactory.add_property("size", &IEntityFactory::GetEntitySize);
	
	// Add memory tools...
	EntityFactory ADD_MEM_TOOLS(IEntityFactory, "EntityFactory");
}


//-----------------------------------------------------------------------------
// Expose IEntityFactoryDictionary.
//-----------------------------------------------------------------------------
void export_entity_factory_dictionary_interface()
{
	class_<IEntityFactoryDictionary, IEntityFactoryDictionary *,
		boost::noncopyable> _EntityFactoryDictionary("_EntityFactoryDictionary", no_init);

	// Methods...
	_EntityFactoryDictionary.def("install_factory", &IEntityFactoryDictionary::InstallFactory);

	_EntityFactoryDictionary.def("create",
		&IEntityFactoryDictionary::Create,
		reference_existing_object_policy()
	);

	_EntityFactoryDictionary.def("destroy", &IEntityFactoryDictionary::Destroy);

	_EntityFactoryDictionary.def("find_factory",
		&IEntityFactoryDictionary::FindFactory,
		reference_existing_object_policy()
	);

	_EntityFactoryDictionary.def("get_cannonical_name", &IEntityFactoryDictionary::GetCannonicalName);

	// Add memory tools...
	_EntityFactoryDictionary ADD_MEM_TOOLS(IEntityFactoryDictionary, "_EntityFactoryDictionary");
}


//-----------------------------------------------------------------------------
// Expose CEntityFactoryDictionary.
//-----------------------------------------------------------------------------
void export_entity_factory_dictionary()
{
	class_<CEntityFactoryDictionary, CEntityFactoryDictionary *, bases<IEntityFactoryDictionary>,
		boost::noncopyable> EntityFactoryDictionary("EntityFactoryDictionary", no_init);

	// Special methods...
	EntityFactoryDictionary.def("__getitem__",&EntityFactoryDictionarySharedExt::__getitem__);

	// Engine specific stuff...
	export_engine_specific_entity_factory_dictionary(EntityFactoryDictionary);

	// Add memory tools...
	EntityFactoryDictionary ADD_MEM_TOOLS(CEntityFactoryDictionary, "EntityFactoryDictionary");
}
