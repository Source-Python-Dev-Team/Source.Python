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

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_callback.h"
#include "utility/wrap_macros.h"
#include "utility/call_python.h"

#include "AsmJit.h"
using namespace AsmJit;

#include "utilities.h"
#include "DynamicHooks.h"
using namespace DynamicHooks;


// ============================================================================
// >> MACROS
// ============================================================================
#define GET_CALLBACK_CALLER(type) \
	(void *) GET_FUNCTION(type, CallbackCaller<type>, CCallback*, unsigned long, unsigned long)


// ============================================================================
// >> CLASSES
// ============================================================================
CCallback::CCallback(object oCallback, Convention_t eConv, tuple args, ReturnType_t return_type)
    : CFunction(NULL, eConv, args, return_type)
{
    m_oCallback = oCallback;

    // Find the proper callback caller function
    void* pCallCallbackFunc = NULL;
    switch(return_type)
    {
        case SIGCHAR_VOID:      pCallCallbackFunc = GET_CALLBACK_CALLER(void); break;
        case SIGCHAR_BOOL:      pCallCallbackFunc = GET_CALLBACK_CALLER(bool); break;
        case SIGCHAR_CHAR:      pCallCallbackFunc = GET_CALLBACK_CALLER(char); break;
        case SIGCHAR_UCHAR:     pCallCallbackFunc = GET_CALLBACK_CALLER(unsigned char); break;
        case SIGCHAR_SHORT:     pCallCallbackFunc = GET_CALLBACK_CALLER(short); break;
        case SIGCHAR_USHORT:    pCallCallbackFunc = GET_CALLBACK_CALLER(unsigned short); break;
        case SIGCHAR_INT:       pCallCallbackFunc = GET_CALLBACK_CALLER(int); break;
        case SIGCHAR_UINT:      pCallCallbackFunc = GET_CALLBACK_CALLER(unsigned int); break;
        case SIGCHAR_LONG:      pCallCallbackFunc = GET_CALLBACK_CALLER(long); break;
        case SIGCHAR_ULONG:     pCallCallbackFunc = GET_CALLBACK_CALLER(unsigned long); break;
        case SIGCHAR_LONGLONG:  pCallCallbackFunc = GET_CALLBACK_CALLER(long long); break;
        case SIGCHAR_ULONGLONG: pCallCallbackFunc = GET_CALLBACK_CALLER(unsigned long long); break;
        case SIGCHAR_FLOAT:     pCallCallbackFunc = GET_CALLBACK_CALLER(float); break;
        case SIGCHAR_DOUBLE:    pCallCallbackFunc = GET_CALLBACK_CALLER(double); break;
        case SIGCHAR_POINTER:   pCallCallbackFunc = GET_CALLBACK_CALLER(void *); break;
        case SIGCHAR_STRING:    pCallCallbackFunc = GET_CALLBACK_CALLER(char *); break;
        default: BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unknown return type.");
    }
    
    // Generate the function
    Assembler a;

    // Epilog
    a.push(ebp);
    a.mov(ebp, esp);

    // Call callback caller
    a.push(ecx);
    a.push(ebp);
    a.push(imm((sysint_t) this));
    a.call(pCallCallbackFunc);
    a.add(esp, imm(12));

    // Prolog
    a.mov(esp, ebp);
    a.pop(ebp);

    // Return
    a.ret(imm(GetPopSize()));

    m_ulAddr = (unsigned long) a.make();
}

int CCallback::GetPopSize()
{
	/*
	Who cleans up the stack?

	Linux: Always the caller

	Windows:
		CDECL: Caller
		STDCALL + THISCALL: Callee 
	*/

#ifdef _WIN32
	if (m_eConv == CONV_CDECL)
		return 0;

	int i = 0;

	// Skip the this pointer. It's passed in an own register (ECX)
    if (m_eConv == CONV_THISCALL)
		i++;

	int size = 0;
	for(; i < len(m_Args); i++)
		size += GetTypeSize(extract<Argument_t>(m_Args[i]));

	return size;
#endif
    return 0;
}

int CCallback::GetArgumentOffset(int iIndex)
{
	int offset = 4;
	for(int i=0; i <= iIndex; i++)
	{
		offset += GetTypeSize(extract<Argument_t>(m_Args[i]));
	}

	// Subtract the this pointer on Windows
#ifdef _WIN32
    if (m_eConv == CONV_THISCALL)
        offset -= sizeof(void *);
#endif

	return offset;
}

void CCallback::Dealloc()
{
    MemoryManager::getGlobal()->free((void *) m_ulAddr);
    m_ulAddr = 0;
}

void CCallback::Realloc(int iSize)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "This function is not implemented for callbacks.")
}


// ============================================================================
// >> FUNCTIONS
// ============================================================================
template<class T>
T GetArgument(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX, int iIndex)
{
#ifdef _WIN32
    if (pCallback->m_eConv == CONV_THISCALL && iIndex == 0)
        return *(T *) &ulECX;
#endif

    return *(T *) (ulEBP + pCallback->GetArgumentOffset(iIndex));
}

object CallCallback(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX)
{
	// TODO: Make this crash proof
    BEGIN_BOOST_PY()
        list arg_list;
        for(int i=0; i < len(pCallback->m_Args); i++)
        {
            object val;
			switch(extract<Argument_t>(pCallback->m_Args[i]))
            {
                case SIGCHAR_BOOL:      val = object(GetArgument<bool>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_CHAR:      val = object(GetArgument<char>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_UCHAR:     val = object(GetArgument<unsigned char>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_SHORT:     val = object(GetArgument<short>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_USHORT:    val = object(GetArgument<unsigned short>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_INT:       val = object(GetArgument<int>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_UINT:      val = object(GetArgument<unsigned int>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_LONG:      val = object(GetArgument<long>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_ULONG:     val = object(GetArgument<unsigned long>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_LONGLONG:  val = object(GetArgument<long long>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_ULONGLONG: val = object(GetArgument<unsigned long long>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_FLOAT:     val = object(GetArgument<float>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_DOUBLE:    val = object(GetArgument<double>(pCallback, ulEBP, ulECX, i)); break;
                case SIGCHAR_POINTER:   val = object(CPointer(GetArgument<unsigned long>(pCallback, ulEBP, ulECX, i))); break;
                case SIGCHAR_STRING:    val = object(GetArgument<const char*>(pCallback, ulEBP, ulECX, i)); break;
                default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown argument type."); break;
            }
            arg_list.append(val);
        }
        arg_list.append(CPointer((unsigned long) ulEBP));
        return eval("lambda func, args: func(*args)")(pCallback->m_oCallback, arg_list);

    END_BOOST_PY_NORET()

	PythonLog(0, "An exception occured while calling the Python callback. The server will now crash!");

    // Throw an exception. We will crash now :(
    throw;
}