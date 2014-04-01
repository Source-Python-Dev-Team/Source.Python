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

#ifndef _TOOLS_WRAP_PYTHON_ORANGEBOX_H
#define _TOOLS_WRAP_PYTHON_ORANGEBOX_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "tools_wrap.h"


//-----------------------------------------------------------------------------
// Expose IServerTools.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_server_tools(T ServerTools)
{
	ServerTools.def("remove_entity", &ServerToolsExt::remove_entity);
	ServerTools.def("remove_entity_immediate", &ServerToolsExt::remove_entity_immediate);
	
	ServerTools.def(
		"get_temp_entities",
		&IServerTools::GetTempEntsSystem,
		reference_existing_object_policy()
	);
}


//-----------------------------------------------------------------------------
// Expose CEffectData.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_dispatch_effect_data(T DispatchEffectData)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose ITempEntsSystem.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_temp_entities_system(T TempEntities)
{
	TempEntities.def("dispatch_effect", &ITempEntsSystem::DispatchEffect);
}


#endif // _TOOLS_WRAP_PYTHON_ORANGEBOX_H
