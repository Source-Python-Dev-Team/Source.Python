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

	// Properties...
	BaseEntity.add_property("server_class",
		make_function(&CBaseEntityWrapper::GetServerClass, reference_existing_object_policy()),
		"The ServerClass instance of this entity (read-only)."
	);

	BaseEntity.add_property("datamap",
		make_function(&CBaseEntityWrapper::GetDataDescMap, reference_existing_object_policy()),
		"The DataMap instance of this entity (read-only)."
	);

	BaseEntity.add_property("edict", make_function(&CBaseEntityWrapper::GetEdict, reference_existing_object_policy()));
	BaseEntity.add_property("index", &CBaseEntityWrapper::GetIndex);
	BaseEntity.add_property("pointer", make_function(&CBaseEntityWrapper::GetPointer));
	BaseEntity.add_property("inthandle", &CBaseEntityWrapper::GetIntHandle);

	// Methods...
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

	BaseEntity.def("is_player",
		&CBaseEntityWrapper::IsPlayer,
		"Return True if the entity is a player."
	);

	// Add memory tools...
	BaseEntity ADD_MEM_TOOLS(CBaseEntityWrapper);
}
