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

#ifndef _MEMORY_TOOLS_H
#define _MEMORY_TOOLS_H

#include <malloc.h>
#include "memalloc.h"
#include "dyncall.h"
#include "dyncall_signature.h"

#include "utilities/wrap_macros.h"

#include "boost/python.hpp"
using namespace boost::python;

// DynamicHooks
#include "manager.h"


// Externals
extern DCCallVM* g_pCallVM;
extern dict g_oExposedClasses;


//---------------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------------
// Use this macro to add this class to get_pointer()
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
// Memory functions
//-----------------------------------------------------------------------------
inline size_t UTIL_GetMemSize(void* ptr)
{
#ifdef _WIN32
	return g_pMemAlloc->GetSize(ptr);
#elif defined(__linux__)
	return malloc_usable_size(ptr);
#else
	#error "Implement me!"
#endif
}

inline void* UTIL_Alloc(size_t size)
{
	void* pPtr = NULL;
#ifdef _WIN32
	pPtr = g_pMemAlloc->IndirectAlloc(size);
#elif defined(__linux__)
	pPtr = malloc(size);
#else
	#error "Implement me!"
#endif
	memset(pPtr, 0, size);
	return pPtr;
}

inline void* UTIL_Realloc(void* ptr, size_t size)
{
	void* pPtr = NULL;
#ifdef _WIN32
	pPtr = g_pMemAlloc->Realloc(ptr, size);
#elif defined(__linux__)
	pPtr = realloc(ptr, size);
#else
	#error "Implement me!"
#endif
	return pPtr;
}

inline void UTIL_Dealloc(void* ptr)
{
#ifdef _WIN32
	g_pMemAlloc->Free(ptr);
#elif defined(__linux__)
	free(ptr);
#else
	#error "Implement me!"
#endif
}

inline std::vector<DataType_t> ObjectToDataTypeVector(object oArgTypes)
{
	std::vector<DataType_t> vecArgTypes;
	for(int i=0; i < len(oArgTypes); i++)
	{
		vecArgTypes.push_back(extract<DataType_t>(oArgTypes[i]));
	}
	return vecArgTypes;
}

//-----------------------------------------------------------------------------
// Convention enum
//-----------------------------------------------------------------------------
enum Convention_t
{
	CONV_CUSTOM,
	CONV_CDECL,
	CONV_THISCALL,
	CONV_STDCALL
};

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
class CFunction;
class CFunctionInfo;

class CPointer
{
public:
	CPointer(unsigned long ulAddr = 0, bool bAutoDealloc = false);
	
	operator unsigned long() const { return m_ulAddr; }

	// Implement some operators
	template<class T>
	const CPointer* operator+(T const& rhs)
	{ return new CPointer(m_ulAddr + rhs); }

	template<class T>
	const CPointer* operator-(T const& rhs)
	{ return new CPointer(m_ulAddr - rhs); }

	template<class T>
	const CPointer* operator+=(T const& rhs)
	{ m_ulAddr += rhs; return this; }

	template<class T>
	const CPointer* operator-=(T const& rhs)
	{ m_ulAddr -= rhs; return this; }

	bool operator!()
	{ return !m_ulAddr; }

	template<class T>
	bool operator==(T const& rhs)
	{ return m_ulAddr == rhs; }

	template<class T>
	bool operator!=(T const& rhs)
	{ return m_ulAddr != rhs; }


	template<class T>
	T Get(int iOffset = 0)
	{
		if (!IsValid())
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

		return *(T *) (m_ulAddr + iOffset);
	}

	template<class T>
	void Set(T value, int iOffset = 0)
	{
		if (!IsValid())
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

		unsigned long newAddr = m_ulAddr + iOffset;
		*(T *) newAddr = value;
	}

	const char *        GetStringArray(int iOffset = 0);
	void                SetStringArray(char* szText, int iOffset = 0);

	CPointer*           GetPtr(int iOffset = 0);
	void                SetPtr(object oPtr, int iOffset = 0);

	bool                IsOverlapping(object oOther, unsigned long ulNumBytes);
	CPointer*           SearchBytes(object oBytes, unsigned long ulNumBytes);

	int                 Compare(object oOther, unsigned long ulNum);
	void                Copy(object oDest, unsigned long ulNumBytes);
	void                Move(object oDest, unsigned long ulNumBytes);


	unsigned long       GetSize() { return UTIL_GetMemSize((void *) m_ulAddr); }

	bool                IsValid() { return m_ulAddr != 0; }

	CPointer*           GetVirtualFunc(int iIndex);

	virtual CPointer*   Realloc(int iSize);
	virtual void        Dealloc() { UTIL_Dealloc((void *) m_ulAddr); m_ulAddr = 0; }

	CFunction*			MakeFunction(CFunctionInfo& info);
	CFunction*			MakeFunction(object oCallingConvention, object args, object return_type);
	CFunction*			MakeVirtualFunction(int iIndex, object oCallingConvention, object args, object return_type);

	static void         CallCallback(PyObject* self, char* szCallback);
	static void         PreDealloc(PyObject* self);
	static CPointer*    PreRealloc(PyObject* self, int iSize);
	static void         __del__(PyObject* self);

public:
	unsigned long m_ulAddr;
	bool          m_bAutoDealloc;
};


//---------------------------------------------------------------------------------
// Converts a Python CPointer object or an integer to an unsigned long.
//---------------------------------------------------------------------------------
class CFunction: public CPointer
{
public:
	CFunction(unsigned long ulAddr, object oCallingConvention, object oArgs, object oReturnType);
	CFunction(unsigned long ulAddr, Convention_t eCallingConvention, int iCallingConvention,
		ICallingConvention* pCallingConvention, boost::python::tuple tArgs,
		DataType_t eReturnType, object oConverter);

	bool IsCallable();
	bool IsHookable();

	bool IsHooked();
    
	object Call(boost::python::tuple args, dict kw);
	object CallTrampoline(boost::python::tuple args, dict kw);
	
	handle<> AddHook(HookType_t eType, PyObject* pCallable);
	void RemoveHook(HookType_t eType, PyObject* pCallable);
    
	handle<> AddPreHook(PyObject* pCallable)
	{ return AddHook(HOOKTYPE_PRE, pCallable); }

	handle<> AddPostHook(PyObject* pCallable)
	{ return AddHook(HOOKTYPE_POST, pCallable); }
    
	void RemovePreHook(PyObject* pCallable)
	{ RemoveHook(HOOKTYPE_PRE, pCallable); }

	void RemovePostHook(PyObject* pCallable)
	{ RemoveHook(HOOKTYPE_POST, pCallable);	}

	void DeleteHook();
    
public:
	boost::python::tuple	m_tArgs;
	object					m_oConverter;
	DataType_t				m_eReturnType;

	// Shared built-in calling convention identifier
	Convention_t			m_eCallingConvention;

	// DynCall calling convention
	int						m_iCallingConvention;

	// DynamicHooks calling convention (built-in and custom)
	ICallingConvention*		m_pCallingConvention;
};


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
inline CPointer* ExtractPointer(object oPtr)
{
	if(PyObject_HasAttrString(oPtr.ptr(), "_ptr"))
		oPtr = oPtr.attr("_ptr")();

	CPointer* pPtr = extract<CPointer *>(oPtr);
	return pPtr;
}

inline CPointer* Alloc(int iSize, bool bAutoDealloc = true)
{
	return new CPointer((unsigned long) UTIL_Alloc(iSize), bAutoDealloc);
}

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

#endif // _MEMORY_TOOLS_H
