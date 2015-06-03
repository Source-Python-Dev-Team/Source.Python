/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _MEMORY_UTILITIES_H
#define _MEMORY_UTILITIES_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_function_info.h"


// ============================================================================
// >> MACROS
// ============================================================================
// The name of the attribute that is added to an exposed function
#define FUNC_INFO_NAME "_func_info"


// ============================================================================
// >> FUNCTIONS
// ============================================================================
template<class BoostExposedClass, class Function>
void AddGetFunctionInfo(BoostExposedClass obj, const char* szName, Function func)
{
	dict d(handle<>(borrowed(downcast<PyTypeObject>(obj.ptr())->tp_dict)));
    object method = (object)(d[szName]);

	CFunctionInfo info;
	GetFunctionInfo(func, info);

	PyDict_SetItemString(
		method.ptr()->ob_type->tp_dict,
		FUNC_INFO_NAME,
		object(info).ptr()
	);
}

inline object PyGetFunctionInfo(object obj)
{
	if (!PyObject_HasAttrString(obj.ptr(), FUNC_INFO_NAME))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function info does not exist for this function.");

	return obj.attr(FUNC_INFO_NAME);
}

#endif // _MEMORY_UTILITIES_H
