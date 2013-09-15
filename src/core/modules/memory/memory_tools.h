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

#include "utility/wrap_macros.h"
#include "DynamicHooks.h"
using namespace DynamicHooks;

#include "boost/python.hpp"
using namespace boost::python;


//-----------------------------------------------------------------------------
//
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

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
class CFunction;

class CPointer
{
public:
	CPointer(unsigned long ulAddr = 0);

	template<class T>
	T get(int iOffset = 0)
	{
		if (!is_valid())
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

		return *(T *) (m_ulAddr + iOffset);
	}

	template<class T>
	void set(T value, int iOffset = 0)
	{
		if (!is_valid())
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

		unsigned long newAddr = m_ulAddr + iOffset;
		*(T *) newAddr = value;
	}

	const char *        get_string(int iOffset = 0, bool bIsPtr = true);
	void                set_string(char* szText, int iSize = 0, int iOffset = 0, bool bIsPtr = true);
	CPointer*           get_ptr(int iOffset = 0);
	void                set_ptr(CPointer* ptr, int iOffset = 0);

	unsigned long       get_size() { return UTIL_GetMemSize((void *) m_ulAddr); }
	unsigned long       get_address() { return m_ulAddr; }

	CPointer*           add(int iValue);
	CPointer*           sub(int iValue);
	bool                is_valid() { return m_ulAddr ? true: false; }

	CPointer*           get_virtual_func(int iIndex, bool bPlatformCheck = true);

	void                realloc(int iSize) { m_ulAddr = (unsigned long) UTIL_Realloc((void *) m_ulAddr, iSize); }
	void                dealloc() { UTIL_Dealloc((void *) m_ulAddr); m_ulAddr = 0; }

	CFunction*          make_function(Convention_t eConv, char* szParams);

	void                reset_vm();
	void                set_mode(int iMode);

	void                set_arg_bool(bool value);
	void                set_arg_char(char value);
	void                set_arg_uchar(unsigned char value);
	void                set_arg_short(short value);
	void                set_arg_ushort(unsigned short value);
	void                set_arg_int(int value);
	void                set_arg_uint(unsigned int value);
	void                set_arg_long(long value);
	void                set_arg_ulong(unsigned long value);
	void                set_arg_long_long(long long value);
	void                set_arg_ulong_long(unsigned long long value);
	void                set_arg_float(float value);
	void                set_arg_double(double value);
	void                set_arg_pointer(object value);
	void                set_arg_string(char* value);

	void                call_void();
	bool                call_bool();
	char                call_char();
	unsigned char       call_uchar();
	short               call_short();
	unsigned short      call_ushort();
	int                 call_int();
	unsigned int        call_uint();
	long                call_long();
	unsigned long       call_ulong();
	long long           call_long_long();
	unsigned long long  call_ulong_long();
	float               call_float();
	double              call_double();
	CPointer*           call_pointer();
	const char*         call_string();

protected:
	unsigned long m_ulAddr;
};

class CFunction: public CPointer
{
public:
	CFunction(unsigned long ulAddr, Convention_t eConv, char* szParams);
    
	object __call__(object args);
	object call_trampoline(object args);
	
	void add_hook(DynamicHooks::HookType_t eType, PyObject* pCallable);
	void remove_hook(DynamicHooks::HookType_t eType, PyObject* pCallable);
    
	void add_pre_hook(PyObject* pCallable);
	void add_post_hook(PyObject* pCallable);
    
	void remove_pre_hook(PyObject* pCallable);
	void remove_post_hook(PyObject* pCallable);
    
private:
	char*         m_szParams;
	Convention_t  m_eConv;
};

int get_error();
CPointer* alloc(int iSize);

#endif // _MEMORY_TOOLS_H