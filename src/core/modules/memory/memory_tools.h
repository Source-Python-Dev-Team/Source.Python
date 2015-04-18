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
#include "conventions/x86MsCdecl.h"
#include "conventions/x86MsThiscall.h"
#include "conventions/x86MsStdcall.h"
#include "conventions/x86GccCdecl.h"
#include "conventions/x86GccThiscall.h"


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
#define ADD_MEM_TOOLS(classname, pyname) \
	ADD_PTR(classname) \
	ADD_OBJ(classname) \
	ADD_SIZE(classname) \
	STORE_CLASS(classname, pyname)

#define ADD_MEM_TOOLS_WRAPPER(classname, realname, pyname) \
	ADD_PTR(classname) \
	ADD_OBJ(classname) \
	ADD_SIZE(classname) \
	STORE_CLASS(realname, pyname)


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

//-----------------------------------------------------------------------------
// Convention enum
//-----------------------------------------------------------------------------
enum Convention_t
{
	CONV_CDECL,
	CONV_THISCALL,
	CONV_STDCALL
};

inline int GetDynCallConvention(Convention_t eConv)
{
	switch (eConv)
	{
		case CONV_CDECL: return DC_CALL_C_DEFAULT;
		case CONV_THISCALL:
			#ifdef _WIN32
				return DC_CALL_C_X86_WIN32_THIS_MS;
			#else
				return DC_CALL_C_X86_WIN32_THIS_GNU;
			#endif
#ifdef _WIN32
		case CONV_STDCALL: return DC_CALL_C_X86_WIN32_STD;
#endif
	}

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unsupported calling convention.")
	return 0;
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

inline ICallingConvention* MakeDynamicHooksConvention(Convention_t eConv, std::vector<DataType_t> vecArgTypes, DataType_t returnType, int iAlignment=4)
{
#ifdef _WIN32
	switch (eConv)
	{
	case CONV_CDECL: return new x86MsCdecl(vecArgTypes, returnType, iAlignment);
	case CONV_THISCALL: return new x86MsThiscall(vecArgTypes, returnType, iAlignment);
	case CONV_STDCALL: return new x86MsStdcall(vecArgTypes, returnType, iAlignment);
	}
#else
	switch (eConv)
	{
	case CONV_CDECL: return new x86GccCdecl(vecArgTypes, returnType, iAlignment);
	case CONV_THISCALL: return new x86GccThiscall(vecArgTypes, returnType, iAlignment);
	}
#endif

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unsupported calling convention.")
	return 0;
}

char DataTypeToDyncallType(DataType_t eType)
{
	switch(eType)
	{
	case DATA_TYPE_VOID: return DC_SIGCHAR_VOID;
	case DATA_TYPE_BOOL: return DC_SIGCHAR_BOOL;
	case DATA_TYPE_CHAR: return DC_SIGCHAR_CHAR;
	case DATA_TYPE_UCHAR: return DC_SIGCHAR_UCHAR;
	case DATA_TYPE_SHORT: return DC_SIGCHAR_SHORT;
	case DATA_TYPE_USHORT: return DC_SIGCHAR_USHORT;
	case DATA_TYPE_INT: return DC_SIGCHAR_INT;
	case DATA_TYPE_UINT: return DC_SIGCHAR_UINT;
	case DATA_TYPE_LONG: return DC_SIGCHAR_LONG;
	case DATA_TYPE_ULONG: return DC_SIGCHAR_ULONG;
	case DATA_TYPE_LONG_LONG: return DC_SIGCHAR_LONGLONG;
	case DATA_TYPE_ULONG_LONG: return DC_SIGCHAR_ULONGLONG;
	case DATA_TYPE_FLOAT: return DC_SIGCHAR_FLOAT;
	case DATA_TYPE_DOUBLE: return DC_SIGCHAR_DOUBLE;
	case DATA_TYPE_POINTER: return DC_SIGCHAR_POINTER;
	case DATA_TYPE_STRING: return DC_SIGCHAR_STRING;
	}

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unsupported data type.")
	return 0;
}

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
class CFunction;

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

	CFunction*          MakeFunction(Convention_t eConv, boost::python::object args, object return_type);
	CFunction*          MakeVirtualFunction(int iIndex, Convention_t eConv, boost::python::object args, object return_type);

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
	CFunction(unsigned long ulAddr, Convention_t eConv, boost::python::object args, object return_type);
    
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
    
public:
	boost::python::tuple	m_Args;
	object					m_oReturnType;
	Convention_t			m_eConv;
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
