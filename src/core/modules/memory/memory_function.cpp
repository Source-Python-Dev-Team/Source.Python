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
#include "memory_wrap.h"

// DynamicHooks
#include "conventions/x86MsCdecl.h"
#include "conventions/x86MsThiscall.h"
#include "conventions/x86MsStdcall.h"
#include "conventions/x86MsFastcall.h"
#include "conventions/x86GccCdecl.h"
#include "conventions/x86GccThiscall.h"

// Source.Python
#include "utilities/call_python.h"


// ============================================================================
// >> EXTERNALS
// ============================================================================
extern std::map<CHook *, std::map<HookType_t, std::list<object> > > g_mapCallbacks;


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
		case CONV_FASTCALL: return DC_CALL_C_X86_WIN32_FAST_MS;
#endif
	}

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unsupported calling convention.")
	return -1;
}


// ============================================================================
// >> MakeDynamicHooksConvention
// ============================================================================
ICallingConvention* MakeDynamicHooksConvention(Convention_t eConv, std::vector<DataType_t> vecArgTypes, DataType_t returnType, int iAlignment)
{
#ifdef _WIN32
	switch (eConv)
	{
	case CONV_CDECL: return new x86MsCdecl(vecArgTypes, returnType, iAlignment);
	case CONV_THISCALL: return new x86MsThiscall(vecArgTypes, returnType, iAlignment);
	case CONV_STDCALL: return new x86MsStdcall(vecArgTypes, returnType, iAlignment);
	case CONV_FASTCALL: return new x86MsFastcall(vecArgTypes, returnType, iAlignment);
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
		m_oCallingConvention = object();
	}
	catch( ... )
	{
		PyErr_Clear();

		// A custom calling convention will be used...
		m_eCallingConvention = CONV_CUSTOM;
		m_oCallingConvention = oCallingConvention(m_tArgs, m_eReturnType);
		m_pCallingConvention = extract<ICallingConvention*>(m_oCallingConvention);

		// Reserve a Python reference for DynamicHooks.
		Py_INCREF(m_oCallingConvention.ptr());
	}

	// Step 4: Get the DynCall calling convention
	m_iCallingConvention = GetDynCallConvention(m_eCallingConvention);
}

CFunction::CFunction(unsigned long ulAddr, Convention_t eCallingConvention,
	int iCallingConvention, tuple tArgs, DataType_t eReturnType, object oConverter)
	:CPointer(ulAddr)
{
	m_eCallingConvention = eCallingConvention;
	m_iCallingConvention = iCallingConvention;
	m_pCallingConvention = NULL;
	m_oCallingConvention = object();

	m_tArgs = tArgs;
	m_eReturnType = eReturnType;
	m_oConverter = oConverter;
}

CFunction::~CFunction()
{
	if (!m_pCallingConvention)
		return;

	// If the convention isn't flagged as hooked, then we need to take care of it.
	if (!m_pCallingConvention->m_bHooked)
	{
		// If we don't have a Python instance, then we can safely delete it.
		if (m_oCallingConvention.is_none())
			delete m_pCallingConvention;
		// Otherwise, just release our reference and let Python take care of it.
		else if (Py_REFCNT(m_oCallingConvention.ptr()) > 1)
			Py_DECREF(m_oCallingConvention.ptr());
	}
	// If we are using a built-in convention that is currently hooked, let's flag it as no longer hooked
	// so that we know we are not bound to a CFunction anymore and can be deleted.
	else if (m_eCallingConvention != CONV_CUSTOM && !dynamic_cast<ICallingConventionWrapper *>(m_pCallingConvention))
		m_pCallingConvention->m_bHooked = false;

	m_pCallingConvention = NULL;
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

CFunction* CFunction::GetTrampoline()
{
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function was not hooked.")

	return new CFunction((unsigned long) pHook->m_pTrampoline, m_eCallingConvention,
		m_iCallingConvention, m_tArgs, m_eReturnType, m_oConverter);
}

template<class ReturnType, class Function>
ReturnType CallHelper(Function func, DCCallVM* vm, unsigned long addr)
{
	ReturnType result;
	TRY_SEGV()
		result = (ReturnType) func(vm, addr);
	EXCEPT_SEGV()
	return result;
}

void CallHelperVoid(DCCallVM* vm, unsigned long addr)
{
	TRY_SEGV()
		dcCallVoid(vm, addr);
	EXCEPT_SEGV()
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
			case DATA_TYPE_BOOL:		dcArgBool(g_pCallVM, extract<bool>(arg)); break;
			case DATA_TYPE_CHAR:		dcArgChar(g_pCallVM, extract<char>(arg)); break;
			case DATA_TYPE_UCHAR:		dcArgChar(g_pCallVM, extract<unsigned char>(arg)); break;
			case DATA_TYPE_SHORT:		dcArgShort(g_pCallVM, extract<short>(arg)); break;
			case DATA_TYPE_USHORT:		dcArgShort(g_pCallVM, extract<unsigned short>(arg)); break;
			case DATA_TYPE_INT:			dcArgInt(g_pCallVM, extract<int>(arg)); break;
			case DATA_TYPE_UINT:		dcArgInt(g_pCallVM, extract<unsigned int>(arg)); break;
			case DATA_TYPE_LONG:		dcArgLong(g_pCallVM, extract<long>(arg)); break;
			case DATA_TYPE_ULONG:		dcArgLong(g_pCallVM, extract<unsigned long>(arg)); break;
			case DATA_TYPE_LONG_LONG:	dcArgLongLong(g_pCallVM, extract<long long>(arg)); break;
			case DATA_TYPE_ULONG_LONG:	dcArgLongLong(g_pCallVM, extract<unsigned long long>(arg)); break;
			case DATA_TYPE_FLOAT:		dcArgFloat(g_pCallVM, extract<float>(arg)); break;
			case DATA_TYPE_DOUBLE:		dcArgDouble(g_pCallVM, extract<double>(arg)); break;
			case DATA_TYPE_POINTER:
			{
				unsigned long ulAddr = 0;
				if (arg.ptr() != Py_None)
					ulAddr = ExtractAddress(arg);

				dcArgPointer(g_pCallVM, ulAddr);
				break;
			}
			case DATA_TYPE_STRING:		dcArgPointer(g_pCallVM, (unsigned long) (void *) extract<char *>(arg)); break;
			default:					BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unknown argument type.")
		}
	}

	// Call the function
	switch(m_eReturnType)
	{
		case DATA_TYPE_VOID:		CallHelperVoid(g_pCallVM, m_ulAddr); break;
		case DATA_TYPE_BOOL:		return object(CallHelper<bool>(dcCallBool, g_pCallVM, m_ulAddr));
		case DATA_TYPE_CHAR:		return object(CallHelper<char>(dcCallChar, g_pCallVM, m_ulAddr));
		case DATA_TYPE_UCHAR:		return object(CallHelper<unsigned char>(dcCallChar, g_pCallVM, m_ulAddr));
		case DATA_TYPE_SHORT:		return object(CallHelper<short>(dcCallShort, g_pCallVM, m_ulAddr));
		case DATA_TYPE_USHORT:		return object(CallHelper<unsigned short>(dcCallShort, g_pCallVM, m_ulAddr));
		case DATA_TYPE_INT:			return object(CallHelper<int>(dcCallInt, g_pCallVM, m_ulAddr));
		case DATA_TYPE_UINT:		return object(CallHelper<unsigned int>(dcCallInt, g_pCallVM, m_ulAddr));
		case DATA_TYPE_LONG:		return object(CallHelper<long>(dcCallLong, g_pCallVM, m_ulAddr));
		case DATA_TYPE_ULONG:		return object(CallHelper<unsigned long>(dcCallLong, g_pCallVM, m_ulAddr));
		case DATA_TYPE_LONG_LONG:	return object(CallHelper<long long>(dcCallLongLong, g_pCallVM, m_ulAddr));
		case DATA_TYPE_ULONG_LONG:	return object(CallHelper<unsigned long long>(dcCallLongLong, g_pCallVM, m_ulAddr));
		case DATA_TYPE_FLOAT:		return object(CallHelper<float>(dcCallFloat, g_pCallVM, m_ulAddr));
		case DATA_TYPE_DOUBLE:		return object(CallHelper<double>(dcCallDouble, g_pCallVM, m_ulAddr));
		case DATA_TYPE_POINTER:
		{
			CPointer pPtr = CPointer(CallHelper<unsigned long>(dcCallPointer, g_pCallVM, m_ulAddr));
			if (!m_oConverter.is_none())
				return m_oConverter(pPtr);

			return object(pPtr);
		}
		case DATA_TYPE_STRING:		return object(CallHelper<const char *>(dcCallPointer, g_pCallVM, m_ulAddr));
		default:					BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown return type.")
	}
	return object();
}

object CFunction::CallTrampoline(tuple args, dict kw)
{
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function was not hooked.")

	return CFunction((unsigned long) pHook->m_pTrampoline, m_eCallingConvention,
		m_iCallingConvention, m_tArgs, m_eReturnType, m_oConverter).Call(args, kw);
}

object CFunction::SkipHooks(tuple args, dict kw)
{
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (pHook)
		return CFunction((unsigned long) pHook->m_pTrampoline, m_eCallingConvention,
			m_iCallingConvention, m_tArgs, m_eReturnType, m_oConverter).Call(args, kw);

	return Call(args, kw);
}

CHook* HookFunctionHelper(void* addr, ICallingConvention* pConv)
{
	CHook* result;
	TRY_SEGV()
		result = GetHookManager()->HookFunction(addr, pConv);
	EXCEPT_SEGV()
	return result;
}

void CFunction::AddHook(HookType_t eType, PyObject* pCallable)
{
	if (!IsHookable())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Function is not hookable.")

	Validate();
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);

	// Prepare arguments for log message
	str type = str(eType);
	const char* szType = extract<const char*>(type);

	str convention = str(m_eCallingConvention);
	const char* szConvention = extract<const char*>(convention);

	str args = str(m_tArgs);
	const char* szArgs = extract<const char*>(args);

	str return_type = str(m_eReturnType);
	const char* szReturnType = extract<const char*>(return_type);

	object oCallback = object(handle<>(borrowed(pCallable)));
	str callback = str(oCallback);
	const char* szCallback = extract<const char*>(callback);

	PythonLog(
		4,
		"Hooking function: type=%s, addr=%u, conv=%s, args=%s, rtype=%s, callback=%s",
		szType,
		m_ulAddr,
		szConvention,
		szArgs,
		szReturnType,
		szCallback
	);

	if (!pHook) {
		pHook = HookFunctionHelper((void *) m_ulAddr, m_pCallingConvention);
	}

	// Add the hook handler. If it's already added, it won't be added twice
	pHook->AddCallback(eType, (HookHandlerFn *) (void *) &SP_HookHandler);
	g_mapCallbacks[pHook][eType].push_back(object(handle<>(borrowed(pCallable))));
}

bool CFunction::AddHook(HookType_t eType, HookHandlerFn* pFunc)
{
	if (!IsHookable())
		return false;

	CHook* pHook = GetHookManager()->FindHook((void*) m_ulAddr);

	if (!pHook) {
		pHook = GetHookManager()->HookFunction((void*) m_ulAddr, m_pCallingConvention);

		if (!pHook)
			return false;
	}

	pHook->AddCallback(eType, pFunc);
	return true;
}

void CFunction::RemoveHook(HookType_t eType, PyObject* pCallable)
{
	Validate();
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		return;

	g_mapCallbacks[pHook][eType].remove(object(handle<>(borrowed(pCallable))));
}

void CFunction::DeleteHook()
{
	CHook* pHook = GetHookManager()->FindHook((void *) m_ulAddr);
	if (!pHook)
		return;

	g_mapCallbacks.erase(pHook);

	ICallingConventionWrapper *pConv = dynamic_cast<ICallingConventionWrapper *>(pHook->m_pCallingConvention);
	if (pConv)
	{
		if (pConv->m_bHooked)
		{
			// Flag the convention as no longer hooked and being taken care of by DynamicHooks.
			pHook->m_pCallingConvention->m_bHooked = false;

			// Release the Python reference we reserved for DynamicHooks.
			PyObject *pOwner = detail::wrapper_base_::owner(pConv);
			if (pOwner && Py_REFCNT(pOwner))
				Py_DECREF(pOwner);
		}
	}
	// If we are a built-in convention bound to a CHook instance but not flagged as hooked anymore, then that
	// means we are no longer bound to a CFunction instance and can be safely deleted.
	else if (!pHook->m_pCallingConvention->m_bHooked)
		delete pHook->m_pCallingConvention;

	// Set the calling convention to NULL, because DynamicHooks will delete it otherwise.
	pHook->m_pCallingConvention = NULL;
	GetHookManager()->UnhookFunction((void *) m_ulAddr);
}