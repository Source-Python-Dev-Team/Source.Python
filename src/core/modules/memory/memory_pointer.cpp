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
// >> INCLUDES
// ============================================================================
// Memory
#include "memory_pointer.h"
#include "memory_utilities.h"

// Utilities
//#include "utilities/wrap_macros.h"
#include "utilities/call_python.h"


// ============================================================================
// >> CPointer
// ============================================================================
CPointer::CPointer(unsigned long ulAddr /* = 0 */, bool bAutoDealloc /* false */)
{
	m_ulAddr = ulAddr;
	m_bAutoDealloc = bAutoDealloc;
}

const char * CPointer::GetStringArray(int iOffset /* = 0 */)
{
	Validate();
	return (const char *) m_ulAddr + iOffset;
}

void CPointer::SetStringArray(char* szText, int iOffset /* = 0 */)
{
	Validate();
	strcpy((char *) (m_ulAddr + iOffset), szText);
}

CPointer* CPointer::GetPtr(int iOffset /* = 0 */)
{
	Validate();
	return new CPointer(*(unsigned long *) (m_ulAddr + iOffset));
}

void CPointer::SetPtr(object oPtr, int iOffset /* = 0 */)
{
	Validate();
	CPointer* pPtr = ExtractPointer(oPtr);
	*(unsigned long *) (m_ulAddr + iOffset) = pPtr->m_ulAddr;
}

int CPointer::Compare(object oOther, unsigned long ulNum)
{
	Validate();
	CPointer* pOther = ExtractPointer(oOther);
	pOther->Validate();
	return memcmp((void *) m_ulAddr, (void *) pOther->m_ulAddr, ulNum);
}

bool CPointer::IsOverlapping(object oOther, unsigned long ulNumBytes)
{
	CPointer* pOther = ExtractPointer(oOther);
	if (m_ulAddr <= pOther->m_ulAddr)
		return m_ulAddr + ulNumBytes > pOther->m_ulAddr;
       
	return pOther->m_ulAddr + ulNumBytes > m_ulAddr;
}

CPointer* CPointer::SearchBytes(object oBytes, unsigned long ulNumBytes)
{
	Validate();
	unsigned long iByteLen = len(oBytes);
	if (ulNumBytes < iByteLen)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Search range is too small.")

	unsigned char* base  = (unsigned char *) m_ulAddr;
	unsigned char* end   = (unsigned char *) (m_ulAddr + ulNumBytes - (iByteLen - 1));

	unsigned char* bytes = (unsigned char *) PyBytes_AsString(oBytes.ptr());
	if (!bytes)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to read the given signature.");

	while (base < end)
	{
		unsigned long i = 0;
		for(; i < iByteLen; i++)
		{
			if (bytes[i] == '\x2A')
				continue;

			if (bytes[i] != base[i])
				break;
		}

		if (i == iByteLen)
			return new CPointer((unsigned long) base);

		base++;
	}
	return new CPointer();
}

void CPointer::Copy(object oDest, unsigned long ulNumBytes)
{
	Validate();
	CPointer* pDest = ExtractPointer(oDest);
	pDest->Validate();

	if (IsOverlapping(oDest, ulNumBytes))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointers are overlapping!")

	memcpy((void *) pDest->m_ulAddr, (void *) m_ulAddr, ulNumBytes);
}

void CPointer::Move(object oDest, unsigned long ulNumBytes)
{
	Validate();
	CPointer* pDest = ExtractPointer(oDest);
	pDest->Validate();
	memmove((void *) pDest->m_ulAddr, (void *) m_ulAddr, ulNumBytes);
}

CPointer* CPointer::GetVirtualFunc(int iIndex)
{
	Validate();
	void** vtable = *(void ***) m_ulAddr;
	if (!vtable)
		return new CPointer();

	return new CPointer((unsigned long) vtable[iIndex]);
}

CPointer* CPointer::Realloc(int iSize)
{ 
	return new CPointer((unsigned long) UTIL_Realloc((void *) m_ulAddr, iSize)); 
}

CFunction* CPointer::MakeFunction(CFunctionInfo& info)
{
	Validate();
	return new CFunction(
		m_ulAddr, 
		object(info.m_eCallingConvention),
		info.GetArgumentTypes(), 
		object(info.m_eReturnType)
	);
}

CFunction* CPointer::MakeFunction(object oCallingConvention, object oArgs, object oReturnType)
{
	Validate();
	return new CFunction(m_ulAddr, oCallingConvention, oArgs, oReturnType);
}

CFunction* CPointer::MakeVirtualFunction(int iIndex, object oCallingConvention, object args, object return_type)
{
	return GetVirtualFunc(iIndex)->MakeFunction(oCallingConvention, args, return_type);
}

CFunction* CPointer::MakeVirtualFunction(CFunctionInfo& info)
{
	Validate();
	if (!info.m_bIsVirtual)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not a virtual function.")

	void** vtable = *(void ***) (this->m_ulAddr + info.m_iVtableOffset);
	void* pFunc = vtable[info.m_iVtableIndex];

	return new CFunction(
		(unsigned long) pFunc, 
		object(info.m_eCallingConvention),
		info.GetArgumentTypes(), 
		object(info.m_eReturnType)
	);
}

void CPointer::CallCallback(PyObject* self, char* szCallback)
{
	if (PyObject_HasAttrString(self, szCallback))
	{
		PyObject* callback = PyObject_GetAttrString(self, szCallback);
		if (callback && PyCallable_Check(callback))
		{
			if (!PyObject_HasAttrString(callback, "__self__"))
			{
				xdecref(PyObject_CallFunction(callback, "O", self));
			}
			else
			{
				xdecref(PyObject_CallMethod(self, szCallback, NULL));
			}
		}
		xdecref(callback);
	}
}

void CPointer::PreDealloc(PyObject* self)
{
	CallCallback(self, "on_dealloc");
	CPointer* pointer = extract<CPointer *>(self);
	pointer->Dealloc();
}

CPointer* CPointer::PreRealloc(PyObject* self, int iSize)
{
	CallCallback(self, "on_realloc");
	CPointer* pointer = extract<CPointer *>(self);
	return pointer->Realloc(iSize);
}
 
void CPointer::__del__(PyObject* self)
{
	CPointer* ptr = extract<CPointer *>(self);
	if (ptr->m_bAutoDealloc)
	{
		PythonLog(4, "Automatically deallocating pointer at %u.", ptr->m_ulAddr);
		PreDealloc(self);
	}
}
 
IBaseType* CPointer::GetTypeInfo()
{
	Validate();
	return GetType((void*) m_ulAddr);
}

void CPointer::Validate()
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")
}
