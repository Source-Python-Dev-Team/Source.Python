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

#ifndef _MEMORY_POINTER_H
#define _MEMORY_POINTER_H

// ============================================================================
// >> INCLUDES
// ============================================================================
// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;

// Memory
#include "memory_alloc.h"
#include "memory_rtti.h"

// Utilities
#include "utilities/wrap_macros.h"

// Must be included at last...
#include "memory_exception.h"


// ============================================================================
// >> CPointer
// ============================================================================
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
		Validate();
		T result;
		TRY_SEGV()
			result = *(T *) (m_ulAddr + iOffset);
		EXCEPT_SEGV()
		return result;
	}

	template<class T>
	void Set(T value, int iOffset = 0)
	{
		Validate();
		TRY_SEGV()
			*(T *) (m_ulAddr + iOffset) = value;
		EXCEPT_SEGV()
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
	CFunction*			MakeVirtualFunction(CFunctionInfo& info);

	static void         CallCallback(PyObject* self, char* szCallback);
	static void         PreDealloc(PyObject* self);
	static CPointer*    PreRealloc(PyObject* self, int iSize);
	static void         __del__(PyObject* self);

	IBaseType*			GetTypeInfo();
	void				Validate();

public:
	unsigned long m_ulAddr;
	bool          m_bAutoDealloc;
};


// ============================================================================
// >> Alloc
// ============================================================================
inline CPointer* Alloc(int iSize, bool bAutoDealloc = true)
{
	return new CPointer((unsigned long) UTIL_Alloc(iSize), bAutoDealloc);
}

#endif // _MEMORY_POINTER_H