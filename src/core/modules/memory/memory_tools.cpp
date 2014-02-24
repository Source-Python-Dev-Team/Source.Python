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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string>

#include "dyncall.h"
#include "dyncall_signature.h"

#include "memory_hooks.h"
#include "memory_tools.h"
#include "utility/wrap_macros.h"
#include "utility/sp_util.h"


DCCallVM* g_pCallVM = dcNewCallVM(4096);
extern std::map<CHook *, std::map<DynamicHooks::HookType_t, std::list<PyObject *> > > g_mapCallbacks;

CHookManager* g_pHookMngr = new CHookManager;

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
CPointer::CPointer(unsigned long ulAddr /* = 0 */)
{
	m_ulAddr = ulAddr;
}

void CPointer::SetStringPtr(char* szText, int iOffset /* = 0 */)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	strcpy(*(char **) (m_ulAddr + iOffset), szText);
}

const char * CPointer::GetStringArray(int iOffset /* = 0 */)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	return (const char *) m_ulAddr + iOffset;
}

void CPointer::SetStringArray(char* szText, int iOffset /* = 0 */)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	strcpy((char *) (m_ulAddr + iOffset), szText);
}

CPointer* CPointer::GetPtr(int iOffset /* = 0 */)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	return new CPointer(*(unsigned long *) (m_ulAddr + iOffset));
}

void CPointer::SetPtr(CPointer* ptr, int iOffset /* = 0 */)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	*(unsigned long *) m_ulAddr = ptr->m_ulAddr;
}

int CPointer::Compare(object oOther, unsigned long ulNum)
{
	unsigned long ulOther = ExtractPyPtr(oOther);
	if (!m_ulAddr || !ulOther)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "At least one pointer is NULL.")

	return memcmp((void *) m_ulAddr, (void *) ulOther, ulNum);
}

bool CPointer::IsOverlapping(object oOther, unsigned long ulNumBytes)
{
	unsigned long ulOther = ExtractPyPtr(oOther);
	if (m_ulAddr <= ulOther)
		return m_ulAddr + ulNumBytes > ulOther;
       
	return ulOther + ulNumBytes > m_ulAddr;
}

CPointer* CPointer::SearchBytes(object oBytes, unsigned long ulNumBytes)
{
	if (!m_ulAddr)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	unsigned long iByteLen = len(oBytes);
	if (ulNumBytes < iByteLen)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Search range is too small.")

	unsigned char* base  = (unsigned char *) m_ulAddr;
	unsigned char* end   = (unsigned char *) (m_ulAddr + ulNumBytes - (iByteLen - 1));
	unsigned char* bytes = NULL;
	PyArg_Parse(oBytes.ptr(), "y", &bytes);

	while (base < end)
	{
		unsigned long i = 0;
		for(; i < iByteLen; i++)
		{
			if (base[i] == '\x2A')
				continue;

			if (bytes[i] != base[i])
				break;
		}

		if (i == iByteLen)
			return new CPointer((unsigned long) base);

		base++;
	}
	return NULL;
}

void CPointer::Copy(object oDest, unsigned long ulNumBytes)
{
	unsigned long ulDest = ExtractPyPtr(oDest);
	if (!m_ulAddr || ulDest)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "At least one pointer is NULL.")

	if (IsOverlapping(oDest, ulNumBytes))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointers are overlapping!")

	memcpy((void *) ulDest, (void *) m_ulAddr, ulNumBytes);
}

void CPointer::Move(object oDest, unsigned long ulNumBytes)
{
	unsigned long ulDest = ExtractPyPtr(oDest);
	if (!m_ulAddr || ulDest == 0)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "At least one pointer is NULL.")

	memmove((void *) ulDest, (void *) m_ulAddr, ulNumBytes);
}

CPointer* CPointer::GetVirtualFunc(int iIndex)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	void** vtable = *(void ***) m_ulAddr;
	if (!vtable)
		return new CPointer();

	return new CPointer((unsigned long) vtable[iIndex]);
}

CFunction* CPointer::MakeFunction(Convention_t eConv, char* szParams)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	return new CFunction(m_ulAddr, eConv, szParams);
}

CFunction* CPointer::MakeVirtualFunction(int iIndex, Convention_t eConv, char* szParams)
{
	return GetVirtualFunc(iIndex)->MakeFunction(eConv, szParams);
}

//-----------------------------------------------------------------------------
// CFunction class
//-----------------------------------------------------------------------------
CFunction::CFunction(unsigned long ulAddr, Convention_t eConv, char* szParams)
{
	m_ulAddr = ulAddr;
	m_eConv = eConv;
	m_szParams = strdup(szParams);
}

object CFunction::Call(tuple args, dict kw)
{	
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	dcReset(g_pCallVM);
	dcMode(g_pCallVM, GetDynCallConvention(m_eConv));
	char* ptr = (char *) m_szParams;
	int pos = 0;
	char ch;
	while ((ch = *ptr) != '\0' && ch != ')')
	{
		if (ch == DC_SIGCHAR_VOID)
		{
			ptr++;
			break;
		}

		object arg = args[pos];
		switch(ch)
		{
			case DC_SIGCHAR_BOOL:      dcArgBool(g_pCallVM, extract<bool>(arg)); break;
			case DC_SIGCHAR_CHAR:      dcArgChar(g_pCallVM, extract<char>(arg)); break;
			case DC_SIGCHAR_UCHAR:     dcArgChar(g_pCallVM, extract<unsigned char>(arg)); break;
			case DC_SIGCHAR_SHORT:     dcArgShort(g_pCallVM, extract<short>(arg)); break;
			case DC_SIGCHAR_USHORT:    dcArgShort(g_pCallVM, extract<unsigned short>(arg)); break;
			case DC_SIGCHAR_INT:       dcArgInt(g_pCallVM, extract<int>(arg)); break;
			case DC_SIGCHAR_UINT:      dcArgInt(g_pCallVM, extract<unsigned int>(arg)); break;
			case DC_SIGCHAR_LONG:      dcArgLong(g_pCallVM, extract<long>(arg)); break;
			case DC_SIGCHAR_ULONG:     dcArgLong(g_pCallVM, extract<unsigned long>(arg)); break;
			case DC_SIGCHAR_LONGLONG:  dcArgLongLong(g_pCallVM, extract<long long>(arg)); break;
			case DC_SIGCHAR_ULONGLONG: dcArgLongLong(g_pCallVM, extract<unsigned long long>(arg)); break;
			case DC_SIGCHAR_FLOAT:     dcArgFloat(g_pCallVM, extract<float>(arg)); break;
			case DC_SIGCHAR_DOUBLE:    dcArgDouble(g_pCallVM, extract<double>(arg)); break;
			case DC_SIGCHAR_POINTER:   dcArgPointer(g_pCallVM, ExtractPyPtr(arg)); break;
			case DC_SIGCHAR_STRING:    dcArgPointer(g_pCallVM, (unsigned long) (void *) extract<char *>(arg)); break;
			default: BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unknown parameter type.")
		}
		pos++; ptr++;
	}

	if (pos != len(args))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "String parameter count does not equal with length of tuple.")

	if (ch == '\0')
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "String parameter has no return type.")

	switch(*++ptr)
	{
		case DC_SIGCHAR_VOID: dcCallVoid(g_pCallVM, m_ulAddr); break;
		case DC_SIGCHAR_BOOL:      return object(dcCallBool(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_CHAR:      return object(dcCallChar(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_UCHAR:     return object((unsigned char) dcCallChar(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_SHORT:     return object(dcCallShort(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_USHORT:    return object((unsigned short) dcCallShort(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_INT:       return object(dcCallInt(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_UINT:      return object((unsigned int) dcCallInt(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_LONG:      return object(dcCallLong(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_ULONG:     return object((unsigned long) dcCallLong(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_LONGLONG:  return object(dcCallLongLong(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_ULONGLONG: return object((unsigned long long) dcCallLongLong(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_FLOAT:     return object(dcCallFloat(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_DOUBLE:    return object(dcCallDouble(g_pCallVM, m_ulAddr));
		case DC_SIGCHAR_POINTER:   return object(CPointer(dcCallPointer(g_pCallVM, m_ulAddr)));
		case DC_SIGCHAR_STRING:    return object((const char *) dcCallPointer(g_pCallVM, m_ulAddr));
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown return type.")
	}
	return object();
}

object CFunction::CallTrampoline(tuple args, dict kw)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	CHook* pHook = g_pHookMngr->FindHook((void *) m_ulAddr);
	if (!pHook)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function was not hooked.")

	return CFunction((unsigned long) pHook->m_pTrampoline, m_eConv, m_szParams).Call(args, kw);
}

PyObject* CFunction::AddHook(DynamicHooks::HookType_t eType, PyObject* pCallable)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	CHook* pHook = g_pHookMngr->HookFunction((void *) m_ulAddr, m_eConv, m_szParams);
	pHook->AddCallback(eType, (void *) &SP_HookHandler);
	g_mapCallbacks[pHook][eType].push_back(pCallable);
	return pCallable;
}

void CFunction::RemoveHook(DynamicHooks::HookType_t eType, PyObject* pCallable)
{
	if (!IsValid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")
		
	CHook* pHook = g_pHookMngr->FindHook((void *) m_ulAddr);
	if (!pHook)
		return;

	g_mapCallbacks[pHook][eType].remove(pCallable);
}