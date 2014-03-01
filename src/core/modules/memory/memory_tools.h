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

#include "utility/wrap_macros.h"
#include "DynamicHooks.h"
using namespace DynamicHooks;

#include "boost/python.hpp"
using namespace boost::python;


// Externals
extern DCCallVM* g_pCallVM;


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
#ifdef _WIN32
	return g_pMemAlloc->IndirectAlloc(size);
#elif defined(__linux__)
	return malloc(size);
#else
	#error "Implement me!"
#endif
}

inline void* UTIL_Realloc(void* ptr, size_t size)
{
#ifdef _WIN32
	return g_pMemAlloc->Realloc(ptr, size);
#elif defined(__linux__)
	return realloc(ptr, size);
#else
	#error "Implement me!"
#endif
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
		case CONV_STDCALL: return DC_CALL_C_X86_WIN32_STD;
	}

	// TODO: Throw exception
	return 0;
}

enum Argument_t
{
	ARG_BOOL = DC_SIGCHAR_BOOL,
	ARG_CHAR = DC_SIGCHAR_CHAR,
	ARG_UCHAR = DC_SIGCHAR_UCHAR,
	ARG_SHORT = DC_SIGCHAR_SHORT,
	ARG_USHORT = DC_SIGCHAR_USHORT,
	ARG_INT = DC_SIGCHAR_INT,
	ARG_UINT = DC_SIGCHAR_UINT,
	ARG_LONG = DC_SIGCHAR_LONG,
	ARG_ULONG = DC_SIGCHAR_ULONG,
	ARG_LONGLONG = DC_SIGCHAR_LONGLONG,
	ARG_ULONGLONG = DC_SIGCHAR_ULONGLONG,
	ARG_FLOAT = DC_SIGCHAR_FLOAT,
	ARG_DOUBLE = DC_SIGCHAR_DOUBLE,
	ARG_POINTER = DC_SIGCHAR_POINTER,
	ARG_STRING = DC_SIGCHAR_STRING
};

enum ReturnType_t
{
	RET_VOID = DC_SIGCHAR_VOID,
	RET_BOOL = DC_SIGCHAR_BOOL,
	RET_CHAR = DC_SIGCHAR_CHAR,
	RET_UCHAR = DC_SIGCHAR_UCHAR,
	RET_SHORT = DC_SIGCHAR_SHORT,
	RET_USHORT = DC_SIGCHAR_USHORT,
	RET_INT = DC_SIGCHAR_INT,
	RET_UINT = DC_SIGCHAR_UINT,
	RET_LONG = DC_SIGCHAR_LONG,
	RET_ULONG = DC_SIGCHAR_ULONG,
	RET_LONGLONG = DC_SIGCHAR_LONGLONG,
	RET_ULONGLONG = DC_SIGCHAR_ULONGLONG,
	RET_FLOAT = DC_SIGCHAR_FLOAT,
	RET_DOUBLE = DC_SIGCHAR_DOUBLE,
	RET_POINTER = DC_SIGCHAR_POINTER,
	RET_STRING = DC_SIGCHAR_STRING
};

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
class CFunction;

class CPointer
{
public:
	CPointer(unsigned long ulAddr = 0, bool bAutoDealloc = false);
	virtual ~CPointer();

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
	
	void                SetStringPtr(char* szText, int iOffset = 0);

	const char *        GetStringArray(int iOffset = 0);
	void                SetStringArray(char* szText, int iOffset = 0);

	CPointer*           GetPtr(int iOffset = 0);
	void                SetPtr(object ptr, int iOffset = 0);

	bool                IsOverlapping(object oOther, unsigned long ulNumBytes);
	CPointer*           SearchBytes(object oBytes, unsigned long ulNumBytes);

	int                 Compare(object oOther, unsigned long ulNum);
	void                Copy(object oDest, unsigned long ulNumBytes);
	void                Move(object oDest, unsigned long ulNumBytes);


	unsigned long       GetSize() { return UTIL_GetMemSize((void *) m_ulAddr); }

	bool                IsValid() { return m_ulAddr != 0; }

	CPointer*           GetVirtualFunc(int iIndex);

	virtual void        Realloc(int iSize) { m_ulAddr = (unsigned long) UTIL_Realloc((void *) m_ulAddr, iSize); }
	virtual void        Dealloc() { UTIL_Dealloc((void *) m_ulAddr); m_ulAddr = 0; }

	CFunction*          MakeFunction(Convention_t eConv, boost::python::tuple args, ReturnType_t return_type);
	CFunction*          MakeVirtualFunction(int iIndex, Convention_t eConv, boost::python::tuple args, ReturnType_t return_type);

public:
	unsigned long m_ulAddr;
	bool          m_bAutoDealloc;
};


//---------------------------------------------------------------------------------
// Converts a Python CPointer object or an integer to an unsigned long.
//---------------------------------------------------------------------------------
inline unsigned long ExtractPyPtr(object obj)
{
	// Try to get an unsigned long at first
	try
	{
		return extract<unsigned long>(obj);
	}
	catch(...)
	{
		PyErr_Clear();
	}

	// If that fails, try to extract a CPointer representation
	CPointer* pPtr = extract<CPointer *>(obj);
	return pPtr->m_ulAddr;
}


class CFunction: public CPointer
{
public:
	CFunction(unsigned long ulAddr, Convention_t eConv, boost::python::tuple args, ReturnType_t return_type);
    
	object Call(boost::python::tuple args, dict kw);
	object CallTrampoline(boost::python::tuple args, dict kw);
	
	handle<> AddHook(DynamicHooks::HookType_t eType, PyObject* pCallable);
	void RemoveHook(DynamicHooks::HookType_t eType, PyObject* pCallable);
    
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
	ReturnType_t			m_ReturnType;
	Convention_t			m_eConv;
};


class Wrap
{
public:
	template<class T>
	static T* WrapIt(object ptr)
	{
		unsigned long ulPtr = ExtractPyPtr(ptr);
		return (T *) ulPtr;
	}
};


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
inline int GetError()
{
	return dcGetError(g_pCallVM);
}

inline CPointer* Alloc(int iSize, bool bAutoDealloc = false)
{
	return new CPointer((unsigned long) UTIL_Alloc(iSize), bAutoDealloc);
}

template<class T>
CPointer* GetPointer(T* ptr)
{
	return new CPointer((unsigned long) ptr);
}

#endif // _MEMORY_TOOLS_H
