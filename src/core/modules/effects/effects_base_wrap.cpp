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
// These includes are really important. Be careful if you try to change the
// order or remove an include!
#include "utilities/wrap_macros.h"
#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "utilities/baseentity.h"
#include "mathlib/vector.h"
#include <stddef.h>
#include "wchartypes.h"
#include "string_t.h"
#include "shake.h"
#include "IEffects.h"
#include "KeyValues.h"
#include "game/shared/itempents.h"
#include "game/shared/effect_dispatch_data.h"
#include "game/server/basetempentity.h"
#include "effects_base.h"

#include ENGINE_INCLUDE_PATH(effects_base_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_base_temp_entity(scope);


//-----------------------------------------------------------------------------
// Declare the _effects._base module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_effects, _base)
{
	export_base_temp_entity(_base);
}


//-----------------------------------------------------------------------------
// Exports CBaseTempEntity.
//-----------------------------------------------------------------------------
void export_base_temp_entity(scope _base)
{
	class_<CBaseTempEntity, boost::noncopyable> BaseTempEntity("BaseTempEntity", no_init);

	// Constructor...
	BaseTempEntity.def("_copy_base", make_constructor(&BaseTempEntityExt::_copy_base, default_call_policies()));
	BaseTempEntity.def("_wrap_ptr", make_constructor(&BaseTempEntityExt::_wrap_ptr, default_call_policies()));

	// Properties...
	BaseTempEntity.add_property("name", &CBaseTempEntity::GetName);
	BaseTempEntity.add_property("next", make_function(&CBaseTempEntity::GetNext, reference_existing_object_policy()));
	BaseTempEntity.add_property("server_class", make_function(&CBaseTempEntity::GetServerClass, reference_existing_object_policy()));

	// Methods...
	BaseTempEntity.def("create", &CBaseTempEntity::Create, ("recipient_filter", arg("delay")=0.0));
	BaseTempEntity.def("precache", &CBaseTempEntity::Precache);
	BaseTempEntity.def("test", &CBaseTempEntity::Test);

	// Engine specific stuff...
	export_engine_specific_temp_entity(_base, BaseTempEntity);

	// Add memory tools...
	BaseTempEntity ADD_MEM_TOOLS(CBaseTempEntity);

	// Add class info...
	BEGIN_CLASS_INFO(CBaseTempEntity)
		FUNCTION_INFO(Create)
		FUNCTION_INFO(Precache)
		FUNCTION_INFO(Test)
	END_CLASS_INFO()
}
