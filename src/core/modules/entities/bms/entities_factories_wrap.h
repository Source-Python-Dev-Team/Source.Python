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

#ifndef _ENTITIES_FACTORIES_WRAP_PYTHON_BMS_H
#define _ENTITIES_FACTORIES_WRAP_PYTHON_BMS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "toolframework/itoolentity.h"
#include "utilities/conversions.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools *servertools;


//-----------------------------------------------------------------------------
// Exports CEntityFactoryDictionary.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_entity_factory_dictionary(T _factories, U EntityFactoryDictionary)
{
	// Singleton...
	_factories.attr("factory_dictionary") = object(ptr((CEntityFactoryDictionary *)servertools->GetEntityFactoryDictionary()));
}


#endif // _ENTITIES_FACTORIES_WRAP_PYTHON_BMS_H
