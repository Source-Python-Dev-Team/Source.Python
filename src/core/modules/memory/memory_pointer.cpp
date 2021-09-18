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
#include "utilities/call_python.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <sys/mman.h>
	#include <unistd.h>
	int PAGE_SIZE = sysconf(_SC_PAGESIZE);
	#define ALIGN_PAGE(ar) ((void*) ((long) ar & ~(PAGE_SIZE-1)))
#endif


// ============================================================================
// >> HELPER FUNCTIONS
// ============================================================================
inline int GetProtection(Protection_t prot)
{
	switch (prot)
	{
	#ifdef _WIN32
		case PROTECTION_NONE:				return PAGE_NOACCESS;
		case PROTECTION_READ:				return PAGE_READONLY;
		case PROTECTION_READ_WRITE:			return PAGE_READWRITE;
		case PROTECTION_EXECUTE:			return PAGE_EXECUTE;
		case PROTECTION_EXECUTE_READ:		return PAGE_EXECUTE_READ;
		case PROTECTION_EXECUTE_READ_WRITE:	return PAGE_EXECUTE_READWRITE;
	#elif __linux__
		case PROTECTION_NONE:				return PROT_NONE;
		case PROTECTION_READ:				return PROT_READ;
		case PROTECTION_READ_WRITE:			return PROT_READ | PROT_WRITE;
		case PROTECTION_EXECUTE:			return PROT_EXEC;
		case PROTECTION_EXECUTE_READ:		return PROT_EXEC | PROT_READ;
		case PROTECTION_EXECUTE_READ_WRITE:	return PROT_EXEC | PROT_READ | PROT_WRITE;
	#else
		#error Unsupported platform.
	#endif
		default: BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unsupported protection type.")
	}
	return -1; // To fix a warning...
}

inline int align(int value, int alignment)
{
	int unaligned = value % alignment;
	if (unaligned == 0)
		return value;

	return value + alignment - unaligned;
}


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
	TRY_SEGV()
		return (const char *) m_ulAddr + iOffset;
	EXCEPT_SEGV()
	return NULL;
}

void CPointer::SetStringArray(char* szText, int iOffset /* = 0 */)
{
	Validate();
	TRY_SEGV()
		strcpy((char *) (m_ulAddr + iOffset), szText);
	EXCEPT_SEGV()
}

CPointer* CPointer::GetPtr(int iOffset /* = 0 */)
{
	Validate();
	return new CPointer(GetPtrHelper(m_ulAddr + iOffset));
}

void SetPtrHelper(unsigned long addr, unsigned long ptr)
{
	TRY_SEGV()
		*(unsigned long *) addr = ptr;
	EXCEPT_SEGV()
}

void CPointer::SetPtr(object oPtr, int iOffset /* = 0 */)
{
	Validate();
	SetPtrHelper(m_ulAddr + iOffset, ExtractAddress(oPtr));
}

int CompareHelper(void* first, void* second, unsigned long length)
{
	TRY_SEGV()
		return memcmp(first, second, length);
	EXCEPT_SEGV()
	return 0;
}

int CPointer::Compare(object oOther, unsigned long ulNum)
{
	Validate();
	return CompareHelper((void *) m_ulAddr, (void *) ExtractAddress(oOther, true), ulNum);
}

bool CPointer::IsOverlapping(object oOther, unsigned long ulNumBytes)
{
	unsigned long ulOther = ExtractAddress(oOther);
	if (m_ulAddr <= ulOther)
		return m_ulAddr + ulNumBytes > ulOther;

	return ulOther + ulNumBytes > m_ulAddr;
}

void* SearchBytesHelper(unsigned char* base, unsigned char* end, unsigned char* bytes, unsigned long length)
{	
	TRY_SEGV()
		while (base < end)
		{
			unsigned long i = 0;
			for(; i < length; i++)
			{
				if (bytes[i] == '\x2A')
					continue;

				if (bytes[i] != base[i])
					break;
			}

			if (i == length)
				return base;

			base++;
		}
	EXCEPT_SEGV()
	return NULL;
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

	return new CPointer((unsigned long) SearchBytesHelper(base, end, bytes, iByteLen));
}

void CopyHelper(void* dest, void* source, unsigned long length)
{
	TRY_SEGV()
		memcpy(dest, source, length);
	EXCEPT_SEGV()
}

void CPointer::Copy(object oDest, unsigned long ulNumBytes)
{
	Validate();
	if (ulNumBytes == 0) {
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "'num_bytes' must be greater than 0.")
	}

	if (IsOverlapping(oDest, ulNumBytes))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointers are overlapping!")

	CopyHelper((void *) ExtractAddress(oDest, true), (void *) m_ulAddr, ulNumBytes);
}

void MoveHelper(void* dest, void* source, unsigned long length)
{
	TRY_SEGV()
		memmove(dest, source, length);
	EXCEPT_SEGV()
}

void CPointer::Move(object oDest, unsigned long ulNumBytes)
{
	Validate();
	if (ulNumBytes == 0) {
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "'num_bytes' must be greater than 0.")
	}

	MoveHelper((void *) ExtractAddress(oDest, true), (void *) m_ulAddr, ulNumBytes);
}

unsigned long GetVirtualFuncHelper(unsigned long addr, int index)
{
	TRY_SEGV()
		void** vtable = *(void ***) addr;
		if (!vtable)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to get the virtual function table.")

		return (unsigned long) vtable[index];
	EXCEPT_SEGV()
	return 0;
}

CPointer* CPointer::GetVirtualFunc(int iIndex)
{
	Validate();
	return new CPointer(GetVirtualFuncHelper(m_ulAddr, iIndex));
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
	CPointer *pPtr = GetVirtualFunc(iIndex);
	CFunction *pFunc = pPtr->MakeFunction(oCallingConvention, args, return_type);
	delete pPtr;
	return pFunc;
}

CFunction* CPointer::MakeVirtualFunction(CFunctionInfo& info)
{
	Validate();
	if (!info.m_bIsVirtual)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not a virtual function.")

	return new CFunction(
		GetVirtualFuncHelper(m_ulAddr + info.m_iVtableOffset, info.m_iVtableIndex), 
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
	TRY_SEGV()
		return GetType((void*) m_ulAddr);
	EXCEPT_SEGV()
	return NULL;
}

void CPointer::Validate()
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")
}

void CPointer::SetProtection(Protection_t prot, int size)
{
#ifdef _WIN32
	DWORD old_protect;
	if (!VirtualProtect((void *) m_ulAddr, size, GetProtection(prot), &old_protect))
#elif __linux__
	unsigned long addr = (unsigned long) ALIGN_PAGE(m_ulAddr);
	if (mprotect((void*) addr, align(size + m_ulAddr - addr, PAGE_SIZE), GetProtection(prot)) != 0)
#else
	#error Unsupported platform.
#endif
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to set memory protection to %i (size=%i).", prot, size)
}

void CPointer::Protect(int size)
{
	SetProtection(PROTECTION_READ, size);
}

void CPointer::UnProtect(int size)
{
	SetProtection(PROTECTION_EXECUTE_READ_WRITE, size);
}
