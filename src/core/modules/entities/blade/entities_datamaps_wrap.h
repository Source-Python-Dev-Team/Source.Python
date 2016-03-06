/**
* =============================================================================
* Source Python
* Copyright (C) 2012 - 2016 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_DATAMAPS_WRAP_BLADE_H
#define _ENTITIES_DATAMAPS_WRAP_BLADE_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// #include "game/shared/predictioncopy.h"
#include "entities_datamaps.h"


//-----------------------------------------------------------------------------
// Exports datamap_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_datamap(T _datamaps, U DataMap)
{
	DataMap.def_readonly("packed_size", &datamap_t::m_nPackedSize);

	// TODO: Expose optimized_datamap_t...
	// DataMap.def_readonly("optimized_datamap", &datamap_t::m_pOptimizedDataMap);
}


//-----------------------------------------------------------------------------
// Exports typedescription_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_type_description(T _datamaps, U TypeDescription)
{
	TypeDescription.add_property("flat_offset", &TypeDescriptionExt::get_flat_offset);
	TypeDescription.add_property("packed_offset", &TypeDescriptionExt::get_packed_offset);
	TypeDescription.def_readonly("flat_group", &typedescription_t::flatGroup);
}

//-----------------------------------------------------------------------------
// Exports fieldtype_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_field_types(T _datamaps, U FieldTypes)
{
	FieldTypes.value("INTEGER64", FIELD_INTEGER64);
	FieldTypes.value("VECTOR4D", FIELD_VECTOR4D);
}


#endif // _ENTITIES_DATAMAPS_WRAP_BLADE_H
