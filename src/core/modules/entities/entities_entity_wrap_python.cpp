/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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
#include "utilities/baseentity.h"
#include "modules/memory/memory_tools.h"
#include "export_main.h"
#include "utilities/conversions.h"
#include "entities_entity_wrap.h"


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
// Exports CBaseEntityHandle.
//-----------------------------------------------------------------------------
void export_base_entity(scope _entity)
{
	class_<CBaseEntity, boost::shared_ptr<CBaseEntity>, bases<IServerEntity>, boost::noncopyable> BaseEntity("BaseEntity", no_init);

	// Initializer...
	BaseEntity.def("__init__",
		make_constructor(&BaseEntityExt::__init__,
			default_call_policies(),
			args("entity_index")
		)
	);

	// Properties...
	BaseEntity.add_property("datamap",
		make_function(&CBaseEntity::GetDataDescMap, reference_existing_object_policy()),
		"Returns the DataMap object of the entity."
	);

	BaseEntity.add_property("server_class",
		make_function(&CBaseEntity::GetServerClass, reference_existing_object_policy()),
		"Returns the ServerClass object of the entity."
	);

	// Add memory tools...
	BaseEntity ADD_MEM_TOOLS(CBaseEntity);
}
