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

#ifndef _DATAMAP_ORANGEBOX_H
#define _DATAMAP_ORANGEBOX_H

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
		.def_readonly("chains_validated", &datamap_t::chains_validated)
		.def_readonly("packed_offsets_computed", &datamap_t::packed_offsets_computed)
		.def_readonly("packed_size", &datamap_t::packed_size)
	;
	return cls;
}

//-----------------------------------------------------------------------------
// Expose typedescription_t.
//-----------------------------------------------------------------------------
class TypeDescExt
{
public:
	static int get_offset(typedescription_t pTypeDesc)
	{
		return pTypeDesc.fieldOffset[TD_OFFSET_NORMAL];
	}
};

template<class T>
T TypeDesc_Visitor(T cls)
{
	cls
		.add_property("offset", &TypeDescExt::get_offset)	
	;
	return cls;
}

//-----------------------------------------------------------------------------
// Expose fieldtype_t.
//-----------------------------------------------------------------------------
template<class T>
T FieldTypes_Visitor(T cls)
{
	return cls; // Nothing specific to orangebox...
}

#endif // _DATAMAP_ORANGEBOX_H
