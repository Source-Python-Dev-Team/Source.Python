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
#include "entities_helpers_wrap.h"
#include "export_main.h"
#include "utilities/conversions.h"
#include "utilities/wrap_macros.h"

#include ENGINE_INCLUDE_PATH(entities_helpers_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_entity_conversion_functions();


//-----------------------------------------------------------------------------
// Declare the _entities._helpers module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _helpers)
{
	// Invalid results...
	_helpers.attr("INVALID_ENTITY_INDEX") = INVALID_ENTITY_INDEX;
	_helpers.attr("INVALID_ENTITY_INTHANDLE") = INVALID_ENTITY_INTHANDLE;

	// Conversion functions...
	export_entity_conversion_functions();

	// Helper functions...
	def("create_entity", &create_entity, args("class_name"));
	def("remove_entity", &remove_entity, args("entity_index"));
	def("spawn_entity", &spawn_entity, args("entity_index"));
}


//-----------------------------------------------------------------------------
// Exports conversion functions.
//-----------------------------------------------------------------------------
void export_entity_conversion_functions()
{
	// To index conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, CPointer *, Pointer);

	// To edict conversions...
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, int, IntHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, CPointer *, Pointer, reference_existing_object_policy());

	// To base handle conversions...
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, CPointer *, Pointer);

	// To int handle conversions...
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, CPointer *, Pointer);

	// To pointer conversions...
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, edict_t *, Edict, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, int, IntHandle, reference_existing_object_policy());
}
