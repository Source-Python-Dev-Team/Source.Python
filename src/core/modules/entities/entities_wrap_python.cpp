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
#include "entities_generator_wrap.h"
#include "entities_wrap.h"
#include "modules/export_main.h"
#include "utility/sp_util.h"

//-----------------------------------------------------------------------------
// Namespaces to use.
//-----------------------------------------------------------------------------
using namespace boost::python;

//-----------------------------------------------------------------------------
// Exposer functions.
//-----------------------------------------------------------------------------
void export_base_entity_handle();
void export_handle_entity();
void export_server_unknown();
void export_server_entity();
void export_server_networkable();
void export_edict();
void export_entity_generator();

//-----------------------------------------------------------------------------
// Entity module definition.
//-----------------------------------------------------------------------------
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
	BOOST_CLASS_CONSTRUCTOR(CBaseEntityHandle, int)
		
		CLASS_CONSTRUCTOR(CBaseHandle)

		CLASS_METHOD(CBaseEntityHandle,
			is_valid
		)

		CLASS_METHOD(CBaseEntityHandle,
			get_entry_index
		)

		CLASS_METHOD(CBaseEntityHandle,
			get_serial_number
		)

		CLASS_METHOD(CBaseEntityHandle,
			to_int
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exports CHandleEntity
//-----------------------------------------------------------------------------
void export_handle_entity()
{
	BOOST_ABSTRACT_CLASS(CHandleEntity)
		
		CLASS_METHOD(CHandleEntity,
			get_ref_ehandle,
			manage_new_object_policy()
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exports CServerUnknown.
//-----------------------------------------------------------------------------
void export_server_unknown()
{
	BOOST_ABSTRACT_CLASS(CServerUnknown)

		/*CLASS_METHOD(CServerUnknown,
			get_collideable,
			"Returns the ICollideable object for this entity."
		)*/

		CLASS_METHOD(CServerUnknown,
			get_networkable,
			"Returns the CServerNetworkable object for this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CServerUnknown,
			get_base_entity,
			"Returns the CBasEntity object for this entity."
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(index_of_pointer,
		"Returns the index of the given BaseEntity pointer"
	);
}

//-----------------------------------------------------------------------------
// Exports CServerEntity.
//-----------------------------------------------------------------------------
void export_server_entity()
{
	BOOST_ABSTRACT_CLASS(CServerEntity)
		
		CLASS_METHOD(CServerEntity,
			get_model_index,
			"Returns the model index for this entity."
		)

		CLASS_METHOD(CServerEntity,
			set_model_index,
			"Sets the model of this entity.",
			args("model_index")
		)

		CLASS_METHOD(CServerEntity,
			get_model_name,
			"Returns the name of the model this entity is using."
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exports CServerNetworkable.
//-----------------------------------------------------------------------------
void export_server_networkable()
{
	BOOST_ABSTRACT_CLASS(CServerNetworkable)

		CLASS_METHOD(CServerNetworkable,
			get_entity_handle,
			"Returns the CHandleEntity instance of this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CServerNetworkable,
			get_edict,
			"Returns the CEdict instance of this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CServerNetworkable,
			get_class_name,
			"Returns the class name of this entity."
		)

	BOOST_END_CLASS()
}

//-----------------------------------------------------------------------------
// Exports CEdict.
//-----------------------------------------------------------------------------
void export_edict()
{
	BOOST_CLASS_CONSTRUCTOR(CEdict, int)

		CLASS_CONSTRUCTOR(const char*, optional<bool>)

		CLASS_METHOD(CEdict,
			area_num
		)

		CLASS_METHOD(CEdict,
			get_class_name,
			"Returns a string containing the class name of this entity."
		)

		CLASS_METHOD(CEdict,
			is_free,
			"Returns True if the edict instance is occupied by a valid entity."
		)

		CLASS_METHOD(CEdict,
			set_free,
			"Sets the entity as free (not-valid)."
		)

		CLASS_METHOD(CEdict,
			clear_free,
			"Clears the entity free flag."
		)	

		CLASS_METHOD(CEdict,
			is_valid,
			"Returns true if this CEdict object has a valid edict."
		)

		CLASS_METHOD(CEdict,
			get_index,
			"Returns the index of this entity."
		)

		CLASS_METHOD(CEdict,
			get_networkable,
			"Returns the CServerNetworkable instance for this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CEdict,
			get_unknown,
			"Returns the CServerUnknown instance for this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CEdict,
			get_server_entity,
			"Returns the CServerEntity instance for this entity.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CEdict,
			get_prop_int,
			"Returns an integer value based on the given sendprop name.",
			args("prop_name")
		)

		CLASS_METHOD(CEdict,
			get_prop_float,
			"Returns a float value based on the given sendprop name.",
			args("prop_name")
		)

		CLASS_METHOD(CEdict,
			get_prop_string,
			"Returns a string value based on the given sendprop name.",
			args("prop_name")
		)

		CLASS_METHOD(CEdict,
			get_prop_vector,
			"Returns a vector value based on the given sendprop name.",
			args("prop_name"),
			manage_new_object_policy()
		)

		CLASS_METHOD(CEdict,
			set_prop_int,
			"Sets the given sendprop to the given integer value.",
			args("prop_name", "iValue")
		)

		CLASS_METHOD(CEdict,
			set_prop_float,
			"Sets the given sendprop to the given float value.",
			args("prop_name", "flValue")
		)

		CLASS_METHOD(CEdict,
			set_prop_string,
			"Sets the given sendprop to the given string value.",
			args("prop_name", "szValue")
		)

		CLASS_METHOD(CEdict,
			set_prop_vector,
			"Sets the given sendprop to the given vector value.",
			args("prop_name", "vecValue")
		)

	BOOST_END_CLASS()
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
