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

#ifndef _MEMORY_TOOLS_H
#define _MEMORY_TOOLS_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_pointer.h"
#include "memory_utilities.h"


// ============================================================================
// >> GetObjectPointer
// ============================================================================
inline object GetObjectPointer(object obj)
{
	object _ptr;
	try
	{
		_ptr = obj.attr(GET_PTR_NAME);
	}
	catch (...)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve a pointer of this object.");
	}

	return _ptr();
}


// ============================================================================
// >> MakeObject
// ============================================================================
inline object MakeObject(object cls, CPointer *pPtr)
{
	object _obj;
	try
	{
		_obj = cls.attr(GET_OBJ_NAME);
	}
	catch (...)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to make an object using this class.");
	}

	if (!pPtr->IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.");

	return _obj(pPtr);
}

inline object MakeObject(object cls, object oPtr)
{
	oPtr = oPtr.attr(GET_PTR_NAME)();
	CPointer *pPtr = extract<CPointer *>(oPtr);

	return MakeObject(cls, pPtr);
}


// ============================================================================
// >> GetSize
// ============================================================================
inline object GetSize(object cls)
{
	object _size;
	try
	{
		_size = cls.attr(GET_SIZE_NAME);
	}
	catch (...)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve the size of this class.");
	}

	return _size;
}


#endif // _MEMORY_TOOLS_H
