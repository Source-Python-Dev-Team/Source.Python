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

#ifndef _DATAMAP_CSGO_H
#define _DATAMAP_CSGO_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "datamap.h"

//-----------------------------------------------------------------------------
// Expose datamap_t.
//-----------------------------------------------------------------------------
template<class T>
T DataMap_Visitor(T cls)
{
	cls
		.def_readonly("packed_size", &datamap_t::m_nPackedSize)
		
		// TODO: Expose optimized_datamap_t...
		.def_readonly("optimized_datamap", &datamap_t::m_pOptimizedDataMap)
	;
	return cls;
}

//-----------------------------------------------------------------------------
// Expose typedescription_t.
//-----------------------------------------------------------------------------
class TypeDescExt
{
public:
	static int get_flat_offset(typedescription_t pTypeDesc)
	{
		// return pTypeDesc.flatOffset[TD_OFFSET_NORMAL];
		return NULL;
	}
};

template<class T>
T TypeDesc_Visitor(T cls)
{
	cls
		.def_readonly("offset", &typedescription_t::fieldOffset)
		.def_readonly("flat_offset", &TypeDescExt::get_flat_offset)
		.def_readonly("flat_group", &typedescription_t::flatGroup)
	;
	return cls;
}

//-----------------------------------------------------------------------------
// Expose fieldtype_t.
//-----------------------------------------------------------------------------
template<class T>
T FieldTypes_Visitor(T cls)
{
	cls
		.value("INTEGER64", FIELD_INTEGER64)
		.value("VECTOR4D", FIELD_VECTOR4D)
	;
	return cls;
}

#endif // _DATAMAP_CSGO_H
