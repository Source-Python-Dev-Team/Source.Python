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
// Memory
#include "memory_function_info.h"
#include "memory_pointer.h"

// Utilities
#include "utilities/wrap_macros.h"


// ============================================================================
// >> MACROS
// ============================================================================
// The name of the attribute that is added to an exposed function
#define FUNC_INFO_NAME "_func_info"


// ============================================================================
// >> ExtractPointer
// ============================================================================
inline CPointer* ExtractPointer(object oPtr)
{
	if(PyObject_HasAttrString(oPtr.ptr(), "_ptr"))
		oPtr = oPtr.attr("_ptr")();

	CPointer* pPtr = extract<CPointer *>(oPtr);
	return pPtr;
}


// ============================================================================
// >> ObjectToDataTypeVector
// ============================================================================
inline std::vector<DataType_t> ObjectToDataTypeVector(object oArgTypes)
{
	std::vector<DataType_t> vecArgTypes;
	for(int i=0; i < len(oArgTypes); i++)
	{
		vecArgTypes.push_back(extract<DataType_t>(oArgTypes[i]));
	}
	return vecArgTypes;
}


// ============================================================================
// >> AddGetFunctionInfo
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


// ============================================================================
// >> PyGetFunctionInfo
// ============================================================================
inline object PyGetFunctionInfo(object obj)
{
	if (!PyObject_HasAttrString(obj.ptr(), FUNC_INFO_NAME))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function info does not exist for this function.");

	return obj.attr(FUNC_INFO_NAME);
}





// ============================================================================
// >> TODO
// ============================================================================
// Externals
extern dict g_oExposedClasses;


//---------------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------------
// Use this macro to add this class to get_pointer_object()
#define ADD_PTR(classname) \
	.def("_ptr", \
		&__ptr__<classname>, \
		manage_new_object_policy() \
	)

// Use this macro to add this class to make_object()
#define ADD_OBJ(classname) \
	.def("_obj", \
		&__obj__<classname>, \
		reference_existing_object_policy() \
	).staticmethod("_obj")

// Use this macro to add this class to get_size()
// Note: This must be at the end of the class definition!
#define ADD_SIZE(classname) \
	.attr("_size") = sizeof(classname);

// Use this macro to add the class to the ExposedClasses dict
#define STORE_CLASS(classname, pyname) \
	extern dict g_oExposedClasses; \
	g_oExposedClasses[XSTRINGIFY(classname)] = scope().attr(pyname);

// Use this macro to add the class to the three functions
// Note: This must be at the end of the class definition!
#define ADD_MEM_TOOLS(classname) \
	ADD_PTR(classname) \
	ADD_OBJ(classname) \
	ADD_SIZE(classname) \
	STORE_CLASS(classname, converter::registry::query(typeid(classname))->m_class_object->tp_name)

#define ADD_MEM_TOOLS_WRAPPER(classname, realname) \
	ADD_PTR(classname) \
	ADD_OBJ(classname) \
	ADD_SIZE(classname) \
	STORE_CLASS(realname, converter::registry::query(typeid(classname))->m_class_object->tp_name)

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
template<class T>
CPointer* __ptr__(T* pThis)
{
	return new CPointer((unsigned long) pThis);
}

template<class T>
T* __obj__(CPointer* pPtr)
{
	return (T *) pPtr->m_ulAddr;
}

inline object GetObjectPointer(object obj)
{
	if (!PyObject_HasAttrString(obj.ptr(), "_ptr"))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve a pointer of this object.");

	return obj.attr("_ptr")();
}

inline object MakeObject(object cls, CPointer* pPtr)
{
	if (!PyObject_HasAttrString(cls.ptr(), "_obj"))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to make an object using this class.");

	return cls.attr("_obj")(ptr(pPtr));
}

inline object GetSize(object cls)
{
	if (!PyObject_HasAttrString(cls.ptr(), "_size"))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve the size of this class.");

	return cls.attr("_size");
}

#endif // _MEMORY_UTILITIES_H
