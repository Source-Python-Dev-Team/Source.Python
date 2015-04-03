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

#ifndef _ENTITIES_HELPERS_WRAP_H
#define _ENTITIES_HELPERS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "toolframework/itoolentity.h"
#include "modules/memory/memory_tools.h"
#include "utilities/conversions.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools *servertools;


//-----------------------------------------------------------------------------
// Creates an entity of the given name and returns its index...
//-----------------------------------------------------------------------------
inline unsigned int create_entity(const char *szClassName)
{
	CBaseEntity *pBaseEntity = (CBaseEntity *)servertools->CreateEntityByName(szClassName);
	if (!pBaseEntity)
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to create an entity of type \"%s\".", szClassName);
	}
	CPointer pEntity = CPointer((unsigned long)pBaseEntity);
	return IndexFromPointer(&pEntity);
}


//-----------------------------------------------------------------------------
// Spawns the given entity index...
//-----------------------------------------------------------------------------
inline void spawn_entity(unsigned int uiEntityIndex)
{
	CPointer *pEntity = PointerFromIndex(uiEntityIndex);
	if (!pEntity)
	{
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Unable to find an entity " \
			"matching the given index \"%u\".", uiEntityIndex);
	}
	CBaseEntity *pBaseEntity = (CBaseEntity *)pEntity->m_ulAddr;
	servertools->DispatchSpawn(pBaseEntity);
}


//-----------------------------------------------------------------------------
// Helper template/macro to save some redundant typing...
//-----------------------------------------------------------------------------
template<typename to_type, typename from_type, to_type (*conversion_function)(from_type)>
struct FromToConversion
{
	static to_type convert(from_type from_value, bool raise_exception)
	{
		to_type return_value = conversion_function(from_value);
		if (!return_value && raise_exception) {
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Conversion failed...");
		}
		return return_value;
	}
};


template<typename from_type, CBaseHandle (*conversion_function)(from_type)>
struct FromToConversion<CBaseHandle, from_type, conversion_function>
{
	static CBaseHandle convert(from_type from_value, bool raise_exception)
	{
		CBaseHandle return_value = conversion_function(from_value);
		if (!return_value.IsValid() && raise_exception) {
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Conversion failed...");
		}
		return return_value.IsValid() ? return_value : NULL;
	}
};


#define EXPORT_CONVERSION_FUNCTION(to_type, to_name, from_type, from_name, ...) \
	def(extract<const char *>(str(XSTRINGIFY(to_name##_from_##from_name)).lower().ptr()), \
		&FromToConversion< to_type, from_type, &to_name##From##from_name >::convert, \
		XSTRINGIFY(Returns the to_name (of type #to_type) from the given from_name (of type #from_type).), \
		(XSTRINGIFY(from_name), arg("raise_exception")=true), \
		##__VA_ARGS__ \
	)


#endif // _ENTITIES_HELPERS_WRAP_H
