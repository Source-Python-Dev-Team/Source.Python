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

#include "detour_class.h"
#include "detourman_class.h"
#include "memory_hooks.h"
#include "dd_utils.h"

#include "memory_tools.h"
#include "utility/wrap_macros.h"
#include "utility/sp_util.h"


DCCallVM* g_pCallVM = dcNewCallVM(4096);

//-----------------------------------------------------------------------------
// CPointer class
//-----------------------------------------------------------------------------
CPointer::CPointer(unsigned long ulAddr /* = 0 */)
{
	m_ulAddr = ulAddr;
}

CPointer* CPointer::add(int iValue)
{
	return new CPointer(m_ulAddr + iValue);
}

CPointer* CPointer::sub(int iValue)
{
	return add(-iValue);
}

const char * CPointer::get_string(int iOffset /* = 0 */, bool bIsPtr /* = true */)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	if (bIsPtr)
		return get<char *>(iOffset);

	return (char *) (m_ulAddr + iOffset);
}

void CPointer::set_string(char* szText, int iSize /* = 0 */, int iOffset /* = 0 */, bool bIsPtr /* = true */)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	if (!iSize)
	{
		iSize = UTIL_GetMemSize((void *) (m_ulAddr + iOffset));
		if(!iSize)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve size of address.")
	}

	if ((int ) strlen(szText) > iSize)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "String exceeds size of memory block.")

	if (bIsPtr)
		set<char *>(szText, iOffset);
	else
		strcpy((char *) (m_ulAddr + iOffset), szText);
}

CPointer* CPointer::get_ptr(int iOffset /* = 0 */)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	return new CPointer(*(unsigned long *) (m_ulAddr + iOffset));
}

void CPointer::set_ptr(CPointer* ptr, int iOffset /* = 0 */)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	*(unsigned long *) m_ulAddr = ptr->get_address();
}

CPointer* CPointer::get_virtual_func(int iIndex, bool bPlatformCheck /* = true */)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

#ifdef __linux__
	if (bPlatformCheck)
		iIndex++;
#endif

	void** vtable = *(void ***) m_ulAddr;
	if (!vtable)
		return new CPointer();

	return new CPointer((unsigned long) vtable[iIndex]);
}

CFunction* CPointer::make_function(Convention eConv, char* szParams)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pointer is NULL.")

	return new CFunction(m_ulAddr, eConv, szParams);
}

// DynCall
void CPointer::reset_vm()
{
	dcReset(g_pCallVM);
}

void CPointer::set_mode(int iMode)
{
	dcMode(g_pCallVM, iMode);
}

void CPointer::set_arg_bool(bool value)
{
	dcArgBool(g_pCallVM, value);
}

void CPointer::set_arg_char(char value)
{
	dcArgChar(g_pCallVM, value);
}

void CPointer::set_arg_uchar(unsigned char value)
{
	dcArgChar(g_pCallVM, value);
}

void CPointer::set_arg_short(short value)
{
	dcArgShort(g_pCallVM, value);
}

void CPointer::set_arg_ushort(unsigned short value)
{
	dcArgShort(g_pCallVM, value);
}

void CPointer::set_arg_int(int value)
{
	dcArgInt(g_pCallVM, value);
}

void CPointer::set_arg_uint(unsigned int value)
{
	dcArgInt(g_pCallVM, value);
}

void CPointer::set_arg_long(long value)
{
	dcArgLong(g_pCallVM, value);
}

void CPointer::set_arg_ulong(unsigned long value)
{
	dcArgLong(g_pCallVM, value);
}

void CPointer::set_arg_long_long(long long value)
{
	dcArgLongLong(g_pCallVM, value);
}

void CPointer::set_arg_ulong_long(unsigned long long value)
{
	dcArgLongLong(g_pCallVM, value);
}

void CPointer::set_arg_float(float value)
{
	dcArgFloat(g_pCallVM, value);
}

void CPointer::set_arg_double(double value)
{
	dcArgDouble(g_pCallVM, value);
}

void CPointer::set_arg_pointer(object value)
{
	unsigned long ptr = ExtractPyPtr(value);
	dcArgPointer(g_pCallVM, ptr);
}

void CPointer::set_arg_string(char* value)
{
	dcArgPointer(g_pCallVM, (unsigned long) value);
}

void CPointer::call_void()
{
	dcCallVoid(g_pCallVM, m_ulAddr);
}

bool CPointer::call_bool()
{
	return dcCallBool(g_pCallVM, m_ulAddr);
}

char CPointer::call_char()
{
	return dcCallChar(g_pCallVM, m_ulAddr);
}

unsigned char CPointer::call_uchar()
{
	return dcCallChar(g_pCallVM, m_ulAddr);
}

short CPointer::call_short()
{
	return dcCallShort(g_pCallVM, m_ulAddr);
}

unsigned short CPointer::call_ushort()
{
	return dcCallShort(g_pCallVM, m_ulAddr);
}

int CPointer::call_int()
{
	return dcCallInt(g_pCallVM, m_ulAddr);
}

unsigned int CPointer::call_uint()
{
	return dcCallInt(g_pCallVM, m_ulAddr);
}

long CPointer::call_long()
{
	return dcCallLong(g_pCallVM, m_ulAddr);
}

unsigned long CPointer::call_ulong()
{
	return dcCallLong(g_pCallVM, m_ulAddr);
}

long long CPointer::call_long_long()
{
	return dcCallLongLong(g_pCallVM, m_ulAddr);
}

unsigned long long CPointer::call_ulong_long()
{
	return dcCallLongLong(g_pCallVM, m_ulAddr);
}

float CPointer::call_float()
{
	return dcCallFloat(g_pCallVM, m_ulAddr);
}

double CPointer::call_double()
{
	return dcCallDouble(g_pCallVM, m_ulAddr);
}

CPointer* CPointer::call_pointer()
{
	return new CPointer(dcCallPointer(g_pCallVM, m_ulAddr));
}

const char* CPointer::call_string()
{
	return (const char *) dcCallPointer(g_pCallVM, m_ulAddr);
}

//-----------------------------------------------------------------------------
// CFunction class
//-----------------------------------------------------------------------------
CFunction::CFunction(unsigned long ulAddr, Convention eConv, char* szParams)
{
	m_ulAddr = ulAddr;
	m_eConv = eConv;
	m_szParams = szParams;
}

object CFunction::__call__(object args)
{	
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	dcReset(g_pCallVM);
	dcMode(g_pCallVM, m_eConv);
	char* ptr = (char *) m_szParams.data();
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

object CFunction::call_trampoline(object args)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	CDetour* pDetour = g_DetourManager.Find_Detour((void *) m_ulAddr);
	if (!pDetour)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function was not hooked.")

	return CFunction((unsigned long) pDetour->GetTrampoline(), m_eConv, (char *) m_szParams.data()).__call__(args);
}

void CFunction::hook(eHookType eType, PyObject* pCallable)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	CDetour* pDetour = g_DetourManager.Add_Detour((void*) m_ulAddr, m_szParams.data(), (eCallConv) m_eConv);
	if (!pDetour)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to hook function.")

	ICallbackManager* mngr = pDetour->GetManager("Python", eType);
	if (!mngr)
	{
		mngr = new CCallbackManager;
		pDetour->AddManager(mngr, eType);
	}

	mngr->Add((void *) pCallable, eType);
}

void CFunction::unhook(eHookType eType, PyObject* pCallable)
{
	if (!is_valid())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function pointer is NULL.")

	CDetour* pDetour = g_DetourManager.Find_Detour((void *) m_ulAddr);
	if (!pDetour)
		return;

	ICallbackManager* mngr = pDetour->GetManager("Python", eType);
	if (mngr)
		mngr->Remove((void *) pCallable, eType);
}

void CFunction::add_pre_hook(PyObject* pCallable)
{
	hook(TYPE_PRE, pCallable);
}

void CFunction::add_post_hook(PyObject* pCallable)
{
	hook(TYPE_POST, pCallable);
}

void CFunction::remove_pre_hook(PyObject* pCallable)
{
	unhook(TYPE_PRE, pCallable);
}

void CFunction::remove_post_hook(PyObject* pCallable)
{
	unhook(TYPE_POST, pCallable);
}

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
int get_error()
{
	return dcGetError(g_pCallVM);
}

CPointer* alloc(int iSize)
{
	return new CPointer((unsigned long) UTIL_Alloc(iSize));
}