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

// ============================================================================
// >> MACROS
// ============================================================================
// The name of the attribute that is added to an exposed function.
#ifndef FUNC_INFO_NAME
#define FUNC_INFO_NAME "_func_info"
#endif

// The name of the method that is added to a class to return its pointer.
#ifndef GET_PTR_NAME
#define GET_PTR_NAME "_ptr"
#endif

// The name of the static method that is added to a class to wrap a pointer of this class.
#ifndef GET_OBJ_NAME
#define GET_OBJ_NAME "_obj"
#endif

// The name of the class attribute that contains the size of the class
#ifndef GET_SIZE_NAME
#define GET_SIZE_NAME "_size"
#endif


#ifndef _MEMORY_UTILITIES_H
#define _MEMORY_UTILITIES_H

// ============================================================================
// >> INCLUDES
// ============================================================================
// Memory
#include "memory_function_info.h"
#include "memory_pointer.h"
#include "memory_tools.h"


// ============================================================================
// >> ExtractAddress
// ============================================================================
inline unsigned long ExtractAddress(object oPtr, bool bValidate = false)
{
	CPointer* pPtr;

	extract<CPointer *> extractor(oPtr);
	if (!extractor.check())
	{
		oPtr = oPtr.attr(GET_PTR_NAME)();
		pPtr = extract<CPointer *>(oPtr);
	}
	else
	{
		pPtr = extractor();
	}

	if (bValidate)
		pPtr->Validate();

	return pPtr->m_ulAddr;
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
// >> ADD_PTR
// ============================================================================
template<class T>
CPointer* __ptr__(T* pThis)
{
	return new CPointer((unsigned long) pThis);
}

// Use this macro to add this class to get_pointer_object()
#define ADD_PTR(classname) \
	.def(GET_PTR_NAME, \
		&__ptr__<classname>, \
		manage_new_object_policy() \
	)


// ============================================================================
// >> ADD_OBJ
// ============================================================================
template<class T>
T* __obj__(CPointer* pPtr)
{
	return (T *) pPtr->m_ulAddr;
}

// Use this macro to add this class to make_object()
#define ADD_OBJ(classname) \
	.def(GET_OBJ_NAME, \
		&__obj__<classname>, \
		reference_existing_object_policy() \
	).staticmethod(GET_OBJ_NAME)


// ============================================================================
// >> ADD_SIZE
// ============================================================================
// Use this macro to add this class to get_size()
// Note: This must be at the end of the class definition!
#define ADD_SIZE(classname) \
	.attr(GET_SIZE_NAME) = sizeof(classname);


// ============================================================================
// >> STORE_CLASS
// ============================================================================
// Use this macro to add the class to the ExposedClasses dict
#define STORE_CLASS(classname, pyname) \
	extern dict g_oExposedClasses; \
	g_oExposedClasses[XSTRINGIFY(classname)] = scope().attr(pyname);


// ============================================================================
// >> ADD_MEM_TOOLS_WRAPPER
// ============================================================================
#define ADD_MEM_TOOLS_WRAPPER(classname, realname) \
	ADD_PTR(classname) \
	ADD_OBJ(classname) \
	ADD_SIZE(classname) \
	STORE_CLASS(realname, converter::registry::query(typeid(classname))->m_class_object->tp_name)


// ============================================================================
// >> ADD_MEM_TOOLS
// ============================================================================
// Use this macro to add the class to the three functions
// Note: This must be at the end of the class definition!
#define ADD_MEM_TOOLS(classname) \
	ADD_MEM_TOOLS_WRAPPER(classname, classname)


// ============================================================================
// >> FUNCTION INFO
// ============================================================================
/*
Macros to create class info dictionaries. The following example shows the usage:

class A
{
public:
	virtual void f() = 0;
	virtual void f() const = 0;
	virtual void f(int x) = 0;
	virtual void g() = 0;
};

BEGIN_CLASS_INFO(A)
	// Start a new function info list, because "f" is overloaded
	BEGIN_FUNCTION_INFO_LIST("f")
		FUNCTION_INFO_OVERLOAD(void, f)
		FUNCTION_INFO_CONST_OVERLOAD(void, f)
		FUNCTION_INFO_OVERLOAD(void, f, int)
	END_FUNCTION_INFO_LIST()

	// Use this macro if there are no function overloads
	FUNCTION_INFO(g)
END_CLASS_INFO()
*/

// Start a new class info dictionary
#define BEGIN_CLASS_INFO(classname) \
	{ \
		typedef classname functionInfoClass; \
		extern dict g_oClassInfo; \
		dict classInfoDict; \
		if (g_oClassInfo.contains( #classname )) \
			classInfoDict = extract<dict>(g_oClassInfo[ #classname ]); \
		else \
			g_oClassInfo[ #classname ] = classInfoDict;

// Finish a class info dictionary
#define END_CLASS_INFO() \
	}

// Start a function info list
#define BEGIN_FUNCTION_INFO_LIST(name) \
	{ \
		list functionInfoList; \
		classInfoDict[ name ] = functionInfoList;

// Finish a function info list
#define END_FUNCTION_INFO_LIST() \
	}

// Requires a function address
#define ADD_FUNCTION_INFO(function) \
	functionInfoList.append(ptr(GetFunctionInfo( function )));

// Use this macro if there are no function overloads
#define FUNCTION_INFO(function) \
	BEGIN_FUNCTION_INFO_LIST( #function ) \
	ADD_FUNCTION_INFO( &functionInfoClass::function ) \
	END_FUNCTION_INFO_LIST()

// Use this macro to add a specific overloaded function
#define FUNCTION_INFO_OVERLOAD(return_type, method, ...) \
	ADD_FUNCTION_INFO( GET_METHOD( return_type, functionInfoClass, method, __VA_ARGS__ ) )

// Use this macro to add a specific overloaded function which was decorated with the "const" modifier
#define FUNCTION_INFO_CONST_OVERLOAD(return_type, method, ...) \
	ADD_FUNCTION_INFO( GET_CONST_METHOD( return_type, functionInfoClass, method, __VA_ARGS__ ) )

#endif // _MEMORY_UTILITIES_H
