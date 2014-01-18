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

#include "entities_generator_wrap.h"
#include "entities_wrap.h"
#include "utility/sp_util.h"
#include "modules/export_main.h"
using namespace boost::python;


//-----------------------------------------------------------------------------
// Entity module definition.
//-----------------------------------------------------------------------------
void export_base_entity_handle();
void export_handle_entity();
void export_server_unknown();
void export_server_entity();
void export_server_networkable();
void export_edict();
void export_entity_generator();

DECLARE_SP_MODULE(entity_c)
{
	export_base_entity_handle();
	export_handle_entity();
	export_server_unknown();
	export_server_entity();
	export_server_networkable();
	export_edict();
	export_entity_generator();
}

//-----------------------------------------------------------------------------
// Exports CBaseEntityHandle.
//-----------------------------------------------------------------------------
void export_base_entity_handle()
{
	// TODO: Renamed it to CBaseHandle
	class_<CBaseHandle>("CBaseEntityHandle")
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
	;
}

//-----------------------------------------------------------------------------
// Exports CHandleEntity
//-----------------------------------------------------------------------------
void export_handle_entity()
{
	// TODO: Rename it to IHandleEntity
	class_<IHandleEntity, boost::noncopyable>("CHandleEntity", no_init)
		.def("set_ref_ehandle",
			&IHandleEntity::SetRefEHandle,
			args("handle")
		)

		.def("get_ref_ehandle",
			&IHandleEntity::GetRefEHandle,
			reference_existing_object_policy()
		)
	;
}

//-----------------------------------------------------------------------------
// Exports CServerUnknown.
//-----------------------------------------------------------------------------
void export_server_unknown()
{
	// TODO: Rename it to IServerUnknown
	class_< IServerUnknown, bases<IHandleEntity>, boost::noncopyable >("CServerUnknown", no_init)
		.def("get_collideable",
			&IServerUnknown::GetCollideable,
			"Returns the ICollideable object for this entity.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&IServerUnknown::GetNetworkable,
			"Returns the CServerNetworkable object for this entity.",
			reference_existing_object_policy()
		)

		/*
		.def("get_base_entity",
			&IServerUnknown::GetBaseEntity,
			"Returns the CBasEntity object for this entity.",
			reference_existing_object_policy()
		)
		*/
	;

	def("index_of_pointer",
		index_of_pointer,
		"Returns the index of the given BaseEntity pointer"
	);
}

//-----------------------------------------------------------------------------
// Exports CServerEntity.
//-----------------------------------------------------------------------------
void export_server_entity()
{
	// TODO: Rename it to IServerEntity
	class_< IServerEntity, bases<IServerUnknown>, boost::noncopyable >("CServerEntity", no_init)
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
			"Sets the model of this entity."
		)
	;
}

//-----------------------------------------------------------------------------
// Exports CServerNetworkable.
//-----------------------------------------------------------------------------
void export_server_networkable()
{
	// TODO: Rename it to IServerNetworkable
	class_< IServerNetworkable, boost::noncopyable >("CServerNetworkable", no_init)
		.def("get_entity_handle",
			&IServerNetworkable::GetEntityHandle,
			"Returns the CHandleEntity instance of this entity.",
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

		.def("get_base_entity",
			&IServerNetworkable::GetBaseEntity,
			reference_existing_object_policy()
		)
		*/

		.def("get_pvs_info",
			&IServerNetworkable::GetPVSInfo,
			"Returns the current visible data.",
			reference_existing_object_policy()
		)
	;
}

//-----------------------------------------------------------------------------
// Exports edict_t.
//-----------------------------------------------------------------------------
void export_edict()
{
	class_< CBaseEdict >("CBaseEdict")
		.def("get_server_entity",
			GET_METHOD(IServerEntity*, CBaseEdict, GetIServerEntity),
			"Returns its IServerEntity instance.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&CBaseEdict::GetNetworkable,
			"Returns its IServerNetworkable instance.",
			reference_existing_object_policy()
		)

		.def("get_unknown",
			&CBaseEdict::GetUnknown,
			"Returns its IServerUnknown instance.",
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

		/*
		// TODO: Missing on CS:GO
		.def_readwrite("index",
			&CBaseEdict::m_iIndex
		)
		*/

		.def_readwrite("networkable",
			&CBaseEdict::m_pNetworkable
		)
	;

	class_< edict_t, bases<CBaseEdict> >("CEdict")
		.def("get_collidable",
			&edict_t::GetCollideable,
			"Returns its ICollideable instance.",
			reference_existing_object_policy()
		)

		.def("get_prop_int",
			&CEdictExt::GetPropInt,
			"Returns the value of a network property as an int.",
			args("prop_name")
		)

		.def("get_prop_float",
			&CEdictExt::GetPropFloat,
			"Returns the value of a network property as a float.",
			args("prop_name")
		)

		.def("get_prop_string",
			&CEdictExt::GetPropString,
			"Returns the value of a network property as a string.",
			args("prop_name")
		)

		.def("get_prop_vector",
			&CEdictExt::GetPropVector,
			"Returns the value of a network property as a vector.",
			args("prop_name")
		)

		.def("set_prop_int",
			&CEdictExt::SetPropInt,
			"Set the a network property to the given value.",
			args("prop_name", "value")
		)

		.def("set_prop_float",
			&CEdictExt::SetPropFloat,
			"Set the a network property to the given value.",
			args("prop_name", "value")
		)

		.def("set_prop_string",
			&CEdictExt::SetPropString,
			"Set the a network property to the given value.",
			args("prop_name", "value")
		)

		.def("set_prop_vector",
			&CEdictExt::SetPropVector,
			"Set the a network property to the given value.",
			args("prop_name", "value")
		)

		// Class attributes
		/*
		// TODO: Missing on CS:GO
		.def_readwrite("free_time",
			&edict_t::freetime,
			"The server timestampe at which the edict was freed."
		)
		*/
	;
}

//-----------------------------------------------------------------------------
// Exports CEntityGenerator.
//-----------------------------------------------------------------------------
void export_entity_generator()
{
	BOOST_GENERATOR_CLASS(CEntityGenerator)
		CLASS_CONSTRUCTOR(const char*)
		CLASS_CONSTRUCTOR(const char*, bool)
	BOOST_END_CLASS()
}
