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
// DynCall
#include "dyncall.h"
#include "dyncall_signature.h"

// Memory
#include "memory_function.h"
#include "memory_utilities.h"
#include "memory_hooks.h"

// DynamicHooks
#include "conventions/x86MsCdecl.h"
#include "conventions/x86MsThiscall.h"
#include "conventions/x86MsStdcall.h"
#include "conventions/x86GccCdecl.h"
#include "conventions/x86GccThiscall.h"

// Utilities
//#include "utilities/wrap_macros.h"


// ============================================================================
// >> EXTERNALS
// ============================================================================
extern std::map<CHook *, std::map<HookType_t, std::list<PyObject *> > > g_mapCallbacks;


// ============================================================================
// >> GLOBAL VARIABLES
// ============================================================================
DCCallVM* g_pCallVM = dcNewCallVM(4096);


// ============================================================================
// >> GetDynCallConvention
// ============================================================================
int GetDynCallConvention(Convention_t eConv)
{
	switch (eConv)
	{
		case CONV_CUSTOM: return -1;
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
	return -1;
}


// ============================================================================
// >> MakeDynamicHooksConvention
// ============================================================================
ICallingConvention* MakeDynamicHooksConvention(Convention_t eConv, std::vector<DataType_t> vecArgTypes, DataType_t returnType, int iAlignment=4)
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
	return NULL;
}


// ============================================================================
// >> CFunction
// ============================================================================
CFunction::CFunction(unsigned long ulAddr, object oCallingConvention, object oArgs, object oReturnType)
	:CPointer(ulAddr)
{
	// Step 1: Validate and convert the argument types
	m_tArgs = tuple(oArgs);

	// Step 2: Determine the return type/converter
	try
	{
		// If this line succeds...
		m_eReturnType = extract<DataType_t>(oReturnType);

		// ...no converter will be used
		m_oConverter = object();
	}
	catch( ... )
	{
		PyErr_Clear();

		// If this happens the return type is a converter for a pointer
		m_eReturnType = DATA_TYPE_POINTER;
		m_oConverter = oReturnType;
	}

	// Step 3: Determine the calling convention
	try
	{
		// If this line succeeds the user wants to create a function with the built-in calling conventions
		m_eCallingConvention = extract<Convention_t>(oCallingConvention);
		m_pCallingConvention = MakeDynamicHooksConvention(m_eCallingConvention, ObjectToDataTypeVector(m_tArgs), m_eReturnType);
	}
	catch( ... )
	{
		PyErr_Clear();
	
		// A custom calling convention will be used...
		m_eCallingConvention = CONV_CUSTOM;
		object _oCallingConvention = oCallingConvention(m_tArgs, m_eReturnType);

		// FIXME:
		// This is required to fix a crash, but it will also cause a memory leak,
		// because no calling convention object that is created via this method will ever be deleted.
		Py_INCREF(_oCallingConvention.ptr());
		m_pCallingConvention = extract<ICallingConvention*>(_oCallingConvention);
	}

	// Step 4: Get the DynCall calling convention
	m_iCallingConvention = GetDynCallConvention(m_eCallingConvention);
}

CFunction::CFunction(unsigned long ulAddr, Convention_t eCallingConvention,
	int iCallingConvention, ICallingConvention* pCallingConvention, tuple tArgs,
	DataType_t eReturnType, object oConverter)
	:CPointer(ulAddr)
{
	m_eCallingConvention = eCallingConvention;
	m_iCallingConvention = iCallingConvention;
	m_pCallingConvention = pCallingConvention;
	m_tArgs = tArgs;
	m_eReturnType = eReturnType;
	m_oConverter = oConverter;
}

bool CFunction::IsCallable()
{
	return (m_eCallingConvention != CONV_CUSTOM) && (m_iCallingConvention != -1);
}

bool CFunction::IsHookable()
{
	return m_pCallingConvention != NULL;
}

bool CFunction::IsHooked()
{
	return GetHookManager()->FindHook((void *) m_ulAddr) != NULL;
}

object CFunction::Call(tuple args, dict kw)
{
	if (!IsCallable())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not callable.")

	Validate();
	if (len(args) != len(m_tArgs))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Number of passed arguments is not equal to the required number.")

	// Reset VM and set the calling convention
	dcReset(g_pCallVM);
	dcMode(g_pCallVM, m_iCallingConvention);

	// Loop through all passed arguments and add them to the VM
	for(int i=0; i < len(args); i++)
	{
		object arg = args[i];
		switch(extract<DataType_t>(m_tArgs[i]))
		{
			case DATA_TYPE_BOOL:      dcArgBool(g_pCallVM, extract<bool>(arg)); break;
			case DATA_TYPE_CHAR:      dcArgChar(g_pCallVM, extract<char>(arg)); break;
			case DATA_TYPE_UCHAR:     dcArgChar(g_pCallVM, extract<unsigned char>(arg)); break;
			case DATA_TYPE_SHORT:     dcArgShort(g_pCallVM, extract<short>(arg)); break;
			case DATA_TYPE_USHORT:    dcArgShort(g_pCallVM, extract<unsigned short>(arg)); break;
			case DATA_TYPE_INT:       dcArgInt(g_pCallVM, extract<int>(arg)); break;
			case DATA_TYPE_UINT:      dcArgInt(g_pCallVM, extract<unsigned int>(arg)); break;
			case DATA_TYPE_LONG:      dcArgLong(g_pCallVM, extract<long>(arg)); break;
			case DATA_TYPE_ULONG:     dcArgLong(g_pCallVM, extract<unsigned long>(arg)); break;
			case DATA_TYPE_LONG_LONG:  dcArgLongLong(g_pCallVM, extract<long long>(arg)); break;
			case DATA_TYPE_ULONG_LONG: dcArgLongLong(g_pCallVM, extract<unsigned long long>(arg)); break;
			case DATA_TYPE_FLOAT:     dcArgFloat(g_pCallVM, extract<float>(arg)); break;
			case DATA_TYPE_DOUBLE:    dcArgDouble(g_pCallVM, extract<double>(arg)); break;
			case DATA_TYPE_POINTER:
			{
				unsigned long ulAddr = 0;
				if (arg.ptr() != Py_None)
					ulAddr = ExtractPointer(arg)->m_ulAddr;
				dcArgPointer(g_pCallVM, ulAddr);
			} break;
			case DATA_TYPE_STRING:    dcArgPointer(g_pCallVM, (unsigned long) (void *) extract<char *>(arg)); break;
			default: BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unknown argument type.")
		}
	}

	// Call the function
	switch(m_eReturnType)
	{
		case DATA_TYPE_VOID:      dcCallVoid(g_pCallVM, m_ulAddr); break;
		case DATA_TYPE_BOOL:      return object(dcCallBool(g_pCallVM, m_ulAddr));
		case DATA_TYPE_CHAR:      return object(dcCallChar(g_pCallVM, m_ulAddr));
		case DATA_TYPE_UCHAR:     return object((unsigned char) dcCallChar(g_pCallVM, m_ulAddr));
		case DATA_TYPE_SHORT:     return object(dcCallShort(g_pCallVM, m_ulAddr));
		case DATA_TYPE_USHORT:    return object((unsigned short) dcCallShort(g_pCallVM, m_ulAddr));
		case DATA_TYPE_INT:       return object(dcCallInt(g_pCallVM, m_ulAddr));
		case DATA_TYPE_UINT:      return object((unsigned int) dcCallInt(g_pCallVM, m_ulAddr));
		case DATA_TYPE_LONG:      return object(dcCallLong(g_pCallVM, m_ulAddr));
		case DATA_TYPE_ULONG:     return object((unsigned long) dcCallLong(g_pCallVM, m_ulAddr));
		case DATA_TYPE_LONG_LONG:  return object(dcCallLongLong(g_pCallVM, m_ulAddr));
		case DATA_TYPE_ULONG_LONG: return object((unsigned long long) dcCallLongLong(g_pCallVM, m_ulAddr));
		case DATA_TYPE_FLOAT:     return object(dcCallFloat(g_pCallVM, m_ulAddr));
		case DATA_TYPE_DOUBLE:    return object(dcCallDouble(g_pCallVM, m_ulAddr));
		case DATA_TYPE_POINTER:
		{
			CPointer pPtr = CPointer(dcCallPointer(g_pCallVM, m_ulAddr));
			if (!m_oConverter.is_none())
				return m_oConverter(pPtr);
			return object(pPtr);
		}
		case DATA_TYPE_STRING:    return object((const char *) dcCallPointer(g_pCallVM, m_ulAddr));
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown return type.")
	}
	return object();
}

object CFunction::CallTrampoline(tuple args, dict kw)
{
	if (!IsCallable())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not callable.")

	Validate();
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function was not hooked.")

	return CFunction((unsigned long) pHook->m_pTrampoline, m_eCallingConvention,
		m_iCallingConvention, m_pCallingConvention, m_tArgs, m_eReturnType, m_oConverter).Call(args, kw);
}

object CFunction::SkipHooks(tuple args, dict kw)
{
	if (IsHooked())
		return CallTrampoline(args, kw);

	return Call(args, kw);
}

handle<> CFunction::AddHook(HookType_t eType, PyObject* pCallable)
{
	if (!IsHookable())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not hookable.")
		
	Validate();
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook) {
		pHook = GetHookManager()->HookFunction((void *) m_ulAddr, m_pCallingConvention);
	}
	
	// Add the hook handler. If it's already added, it won't be added twice
	pHook->AddCallback(eType, (HookHandlerFn *) (void *) &SP_HookHandler);
	g_mapCallbacks[pHook][eType].push_back(pCallable);
	
	// Return the callback, so we can use this method as a decorator
	return handle<>(borrowed(pCallable));
}

void CFunction::RemoveHook(HookType_t eType, PyObject* pCallable)
{
	Validate();
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		return;

	g_mapCallbacks[pHook][eType].remove(pCallable);
}

void CFunction::DeleteHook()
{
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		return;

	g_mapCallbacks.erase(pHook);
	// Set the calling convention to NULL, because DynamicHooks will delete it otherwise.
	pHook->m_pCallingConvention = NULL;
	GetHookManager()->UnhookFunction((void *) m_ulAddr);
}