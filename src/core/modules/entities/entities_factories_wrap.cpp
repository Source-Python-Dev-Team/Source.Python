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
// Includes
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "entities_factories.h"
#include "export_main.h"

#include ENGINE_INCLUDE_PATH(entities_factories_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_entity_factory(scope);
void export_entity_factory_dictionary_interface(scope);
void export_entity_factory_dictionary(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._factories module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _factories)
{
	export_entity_factory(_factories);
	export_entity_factory_dictionary_interface(_factories);
	export_entity_factory_dictionary(_factories);
}


//-----------------------------------------------------------------------------
// Expose IEntityFactory.
//-----------------------------------------------------------------------------
void export_entity_factory(scope _factories)
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
	EntityFactory ADD_MEM_TOOLS(IEntityFactory);
}


//-----------------------------------------------------------------------------
// Expose IEntityFactoryDictionary.
//-----------------------------------------------------------------------------
void export_entity_factory_dictionary_interface(scope _factories)
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
	_EntityFactoryDictionary ADD_MEM_TOOLS(IEntityFactoryDictionary);
}


//-----------------------------------------------------------------------------
// Expose CEntityFactoryDictionary.
//-----------------------------------------------------------------------------
void export_entity_factory_dictionary(scope _factories)
{
	class_<CEntityFactoryDictionary, CEntityFactoryDictionary *, bases<IEntityFactoryDictionary>,
		boost::noncopyable> EntityFactoryDictionary("EntityFactoryDictionary", no_init);

	// Special methods...
	EntityFactoryDictionary.def("__getitem__",&EntityFactoryDictionarySharedExt::__getitem__);

	// Engine specific stuff...
	export_engine_specific_entity_factory_dictionary(_factories, EntityFactoryDictionary);

	// Add memory tools...
	EntityFactoryDictionary ADD_MEM_TOOLS(CEntityFactoryDictionary);
}
