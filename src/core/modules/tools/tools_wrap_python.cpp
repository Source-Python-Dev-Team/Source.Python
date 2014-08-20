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
#include "string_t.h"
#include "game/shared/itempents.h"
#include "tools_wrap.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(tools_wrap_python.h)


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools* servertools;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_server_tools(scope tools);
void export_entity_factory(scope tools);
void export_entity_factory_dictionary_interface(scope tools);
void export_entity_factory_dictionary(scope tools);


//-----------------------------------------------------------------------------
// Declare the tools module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(tools)
{
	scope tools = scope();

	export_server_tools(tools);
	export_entity_factory(tools);
	export_entity_factory_dictionary_interface(tools);
	export_entity_factory_dictionary(tools);
}


//-----------------------------------------------------------------------------
// Expose IServerTools.
//-----------------------------------------------------------------------------
void export_server_tools(scope tools)
{
	class_<IServerTools, boost::noncopyable> ServerTools("_ServerTools", no_init);

	// Methods...
	ServerTools.def("create_entity", &ServerToolsSharedExt::create_entity);
	ServerTools.def("spawn_entity", &ServerToolsSharedExt::spawn_entity);

	// OrangeBox methods...
	ServerTools.NOT_IMPLEMENTED("remove_entity_immediate");
	ServerTools.NOT_IMPLEMENTED("get_entity_factory_dictionary");
	ServerTools.NOT_IMPLEMENTED("get_temp_entities");

	// Engine specific stuff...
	export_engine_specific_server_tools(ServerTools);

	// Add memory tools...
	ServerTools ADD_MEM_TOOLS(IServerTools, "_ServerTools");

	// Singleton...
	tools.attr("server_tools") = object(ptr(servertools));
}


//-----------------------------------------------------------------------------
// Expose IEntityFactory.
//-----------------------------------------------------------------------------
void export_entity_factory(scope tools)
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
void export_entity_factory_dictionary_interface(scope tools)
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
void export_entity_factory_dictionary(scope tools)
{
	class_<CEntityFactoryDictionary, CEntityFactoryDictionary *, bases<IEntityFactoryDictionary>,
		boost::noncopyable> EntityFactoryDictionary("EntityFactoryDictionary", no_init);

	// Special methods...
	EntityFactoryDictionary.def("__getitem__",&EntityFactoryDictionarySharedExt::__getitem__);

	// Add memory tools...
	EntityFactoryDictionary ADD_MEM_TOOLS(CEntityFactoryDictionary, "EntityFactoryDictionary");
}
