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
#include "conversions_wrap.h"
#include "modules/export_main.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_functions();


//-----------------------------------------------------------------------------
// Conversions module definition.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_conversions)
{
	export_functions();
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
		&FromToConversion<##to_type, from_type, &to_name##From##from_name>::convert, \
		XSTRINGIFY(Returns the to_name (of type to_type##) from the given from_name (of type from_type##).), \
		(XSTRINGIFY(from_name), arg("raise_exception")=true), \
		__VA_ARGS__ \
	)


//-----------------------------------------------------------------------------
// Exports conversion functions.
//-----------------------------------------------------------------------------
void export_functions()
{
	// To index conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, CPointer *, Pointer);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, IPlayerInfo *, PlayerInfo);

	// To edict conversions...
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, int, IntHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, CPointer *, Pointer, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Userid, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, IPlayerInfo *, PlayerInfo, reference_existing_object_policy());

	// To base handle conversions...
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, CPointer *, Pointer);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, IPlayerInfo *, PlayerInfo);

	// To int handle conversions...
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, CPointer *, Pointer);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(int, IntHandle, IPlayerInfo *, PlayerInfo);

	// To pointer conversions...
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, edict_t *, Edict, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, int, IntHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, unsigned int, Userid, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(CPointer *, Pointer, IPlayerInfo *, PlayerInfo, reference_existing_object_policy());

	// To userid conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, CPointer *, Pointer);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, IPlayerInfo *, PlayerInfo);
	
	// To playerinfo conversions...
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, edict_t *, Edict, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, int, IntHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CPointer *, Pointer, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Userid, reference_existing_object_policy());
}
