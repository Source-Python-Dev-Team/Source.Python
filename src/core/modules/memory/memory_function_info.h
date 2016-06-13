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

/*
* This file has been copied from the SourceHook library.
* https://github.com/alliedmodders/metamod-source/blob/master/core/sourcehook/sh_memfuncinfo.h
* All credits go to the Metamod:Source Development Team.
* The file has been modified and extended to fit into Source.Python.
*/

/* ======== SourceHook ========
* Copyright (C) 2004-2010 Metamod:Source Development Team
* No warranties of any kind
*
* License: zlib/libpng
*
* Author(s): Pavol "PM OnoTo" Marko
* ============================
*/

#ifndef _MEMORY_FUNCTION_INFO_H
#define _MEMORY_FUNCTION_INFO_H

#include <stdint.h>
    
// ========================================================================
// >> SourceHook code
// ========================================================================
#define SH_PTRSIZE sizeof(void *)

// Don Clugston:
//		implicit_cast< >
// I believe this was originally going to be in the C++ standard but
// was left out by accident. It's even milder than static_cast.
// I use it instead of static_cast<> to emphasize that I'm not doing
// anything nasty.
// Usage is identical to static_cast<>
template <class OutputClass, class InputClass>
inline OutputClass implicit_cast(InputClass input){
    return input;
}

struct MemFuncInfo
{
    bool isVirtual;		// Is the function virtual?
    int thisptroffs;	// The this pointer the function expects to be called with
                        // If -1, you need to call the GetFuncInfo_GetThisPtr function
    int vtblindex;		// The function's index in the vtable (0-based, 1=second entry, 2=third entry, ...)
    int vtbloffs;		// The vtable pointer
};

// Ideas by Don Clugston.
// Check out his excellent paper: http://www.codeproject.com/cpp/FastDelegate.asp

template<int N> struct MFI_Impl
{
    template<class MFP> static inline void GetFuncInfo(MFP *mfp, MemFuncInfo &out)
    {
        static char weird_memfunc_pointer_exclamation_mark_arrow_error[N-1000];
    }
};

# if __linux__

template<> struct MFI_Impl<2*SH_PTRSIZE>   // All of these have size==8/16
{
    struct GCC_MemFunPtr
    {
        union
        {
            void *funcadr;				// always even
            intptr_t vtable_index_plus1;		//  = vindex+1, always odd
        };
        intptr_t delta;
    };
    template<class MFP> static inline void GetFuncInfo(MFP mfp, MemFuncInfo &out)
    {
        GCC_MemFunPtr *mfp_detail = (GCC_MemFunPtr*)&mfp;
        out.thisptroffs = mfp_detail->delta;
        if (mfp_detail->vtable_index_plus1 & 1)
        {
            out.vtblindex = (mfp_detail->vtable_index_plus1 - 1) / SH_PTRSIZE;
            out.vtbloffs = 0;
            out.isVirtual = true;
        }
        else
            out.isVirtual = false;
    }
};

# elif _WIN32

inline int MFI_GetVtblOffset(void *mfp)
{
    unsigned char *addr = (unsigned char*)mfp;
    if (*addr == 0xE9)		// Jmp
    {
        // May or may not be!
        // Check where it'd jump
        addr += 5 /*size of the instruction*/ + *(unsigned long*)(addr + 1);
    }

    // Check whether it's a virtual function call
    // They look like this:
    // 004125A0 8B 01            mov         eax,dword ptr [ecx]
    // 004125A2 FF 60 04         jmp         dword ptr [eax+4]
    //		==OR==
    // 00411B80 8B 01            mov         eax,dword ptr [ecx]
    // 00411B82 FF A0 18 03 00 00 jmp         dword ptr [eax+318h]

    // However, for vararg functions, they look like this:
    // 0048F0B0 8B 44 24 04      mov         eax,dword ptr [esp+4]
    // 0048F0B4 8B 00            mov         eax,dword ptr [eax]
    // 0048F0B6 FF 60 08         jmp         dword ptr [eax+8]
    //		==OR==
    // 0048F0B0 8B 44 24 04      mov         eax,dword ptr [esp+4]
    // 0048F0B4 8B 00            mov         eax,dword ptr [eax]
    // 00411B82 FF A0 18 03 00 00 jmp         dword ptr [eax+318h]

    // With varargs, the this pointer is passed as if it was the first argument

    bool ok = false;
    if (addr[0] == 0x8B && addr[1] == 0x44 && addr[2] == 0x24 && addr[3] == 0x04 &&
        addr[4] == 0x8B && addr[5] == 0x00)
    {
        addr += 6;
        ok = true;
    }
    else if (addr[0] == 0x8B && addr[1] == 0x01)
    {
        addr += 2;
        ok = true;
    }
    if (!ok)
        return -1;

    if (*addr++ == 0xFF)
    {
        if (*addr == 0x60)
        {
            return *++addr / 4;
        }
        else if (*addr == 0xA0)
        {
            return *((unsigned int*)++addr) / 4;
        }
        else if (*addr == 0x20)
            return 0;
        else
            return -1;
    }
    return -1;
}

template<> struct MFI_Impl<1*SH_PTRSIZE>   // simple ones
{
    template<class MFP> static inline void GetFuncInfo(MFP mfp, MemFuncInfo &out)
    {
        out.vtblindex = MFI_GetVtblOffset(*(void**)&mfp);
        out.isVirtual = out.vtblindex >= 0 ? true : false;
        out.thisptroffs = 0;
        out.vtbloffs = 0;
    }
};

template<> struct MFI_Impl<2*SH_PTRSIZE>   // more complicated ones!
{
    struct MSVC_MemFunPtr2
    {
        void *funcadr;
        int delta;
    };
    template<class MFP> static inline void GetFuncInfo(MFP mfp, MemFuncInfo &out)
    {
        out.vtblindex = MFI_GetVtblOffset(*(void**)&mfp);
        out.isVirtual = out.vtblindex >= 0 ? true : false;
        out.thisptroffs = reinterpret_cast<MSVC_MemFunPtr2*>(&mfp)->delta;
        out.vtbloffs = 0;
    }
};

// By Don Clugston, adapted
template<> struct MFI_Impl<3*SH_PTRSIZE>   // WOW IT"S GETTING BIGGER OMGOMOGMG
{
    class __single_inheritance GenericClass;
    class GenericClass {};

    struct MicrosoftVirtualMFP {
        void (GenericClass::*codeptr)(); // points to the actual member function
        int delta;		// #bytes to be added to the 'this' pointer
        int vtable_index; // or 0 if no virtual inheritance
    };

    struct GenericVirtualClass : virtual public GenericClass
    {
        typedef GenericVirtualClass * (GenericVirtualClass::*ProbePtrType)();
        GenericVirtualClass * GetThis() { return this; }
    };

    template<class MFP> static inline void GetFuncInfo(MFP mfp, MemFuncInfo &out)
    {
        out.vtblindex = MFI_GetVtblOffset(*(void**)&mfp);
        out.isVirtual = out.vtblindex >= 0 ? true : false;
        // This pointer
        /*
        union {
            MFP func;
            GenericClass* (T::*ProbeFunc)();
            MicrosoftVirtualMFP s;
        } u;
        u.func = mfp;
        union {
            GenericVirtualClass::ProbePtrType virtfunc;
            MicrosoftVirtualMFP s;
        } u2;

        // Check that the horrible_cast<>s will work
        typedef int ERROR_CantUsehorrible_cast[sizeof(mfp)==sizeof(u.s)
            && sizeof(mfp)==sizeof(u.ProbeFunc)
            && sizeof(u2.virtfunc)==sizeof(u2.s) ? 1 : -1];
        // Unfortunately, taking the address of a MF prevents it from being inlined, so
        // this next line can't be completely optimised away by the compiler.
        u2.virtfunc = &GenericVirtualClass::GetThis;
        u.s.codeptr = u2.s.codeptr;
        out.thisptroffs = (reinterpret_cast<T*>(NULL)->*u.ProbeFunc)();
        */
        out.thisptroffs = -1;
        out.vtbloffs = 0;
    }
};

// Don: Nasty hack for Microsoft and Intel (IA32 and Itanium)
// unknown_inheritance classes go here
// This is probably the ugliest bit of code I've ever written. Look at the casts!
// There is a compiler bug in MSVC6 which prevents it from using this code.
template<> struct MFI_Impl<4*SH_PTRSIZE>   // THE BIGGEST ONE!!!1GABEN
{
    template<class MFP> static inline void GetFuncInfo(MFP mfp, MemFuncInfo &out)
    {
        out.vtblindex = MFI_GetVtblOffset(*(void**)&mfp);
        out.isVirtual = out.vtblindex >= 0 ? true : false;

        // The member function pointer is 16 bytes long. We can't use a normal cast, but
        // we can use a union to do the conversion.
        union {
            MFP func;
            // In VC++ and ICL, an unknown_inheritance member pointer
            // is internally defined as:
            struct {
                void *m_funcaddress; // points to the actual member function
                int delta;		// #bytes to be added to the 'this' pointer
                int vtordisp;		// #bytes to add to 'this' to find the vtable
                int vtable_index; // or 0 if no virtual inheritance
            } s;
        } u;
        // Check that the horrible_cast will work
        typedef int ERROR_CantUsehorrible_cast[sizeof(u.func)==sizeof(u.s)? 1 : -1];
        u.func = mfp;
        int virtual_delta = 0;
        if (u.s.vtable_index) { // Virtual inheritance is used
            /*
            // First, get to the vtable.
            // It is 'vtordisp' bytes from the start of the class.
            int * vtable = *reinterpret_cast<int **>(
                reinterpret_cast<char *>(thisptr) + u.s.vtordisp );

            // 'vtable_index' tells us where in the table we should be looking.
            virtual_delta = u.s.vtordisp + *reinterpret_cast<const int *>(
                reinterpret_cast<const char *>(vtable) + u.s.vtable_index);
        // The int at 'virtual_delta' gives us the amount to add to 'this'.
        // Finally we can add the three components together. Phew!
        out.thisptr = reinterpret_cast<void *>(
            reinterpret_cast<char *>(thisptr) + u.s.delta + virtual_delta);
            */
            out.vtbloffs = u.s.vtordisp;
            out.thisptroffs = -1;
        }
        else
        {
            out.vtbloffs = out.vtblindex < 0 ? 0 : u.s.delta;
            out.thisptroffs = u.s.delta;
        }
    };
};
# else
#  error Unsupported compiler
# endif


// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_signature.h"
#include "memory_calling_convention.h"

#include "boost/python/signature.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/mpl/for_each.hpp"


// ========================================================================
// >> TypeToDataType_t
// ========================================================================
template<class _T>
DataType_t TypeToDataType_t()
{
    // Remove "const" and "volatile", so it's easier to compare
    typedef typename boost::remove_cv<_T>::type T;

    if (boost::is_same<T, char*>::value || boost::is_same<T, const char*>::value)
        return DATA_TYPE_STRING;

    if (boost::is_class<T>::value || boost::is_reference<T>::value || boost::is_pointer<T>::value)
        return DATA_TYPE_POINTER;
            
    if (boost::is_same<T, void>::value)
        return DATA_TYPE_VOID;

    if (boost::is_same<T, bool>::value)
        return DATA_TYPE_BOOL;

    if (boost::is_same<T, char>::value)
        return DATA_TYPE_CHAR;

    if (boost::is_same<T, unsigned char>::value)
        return DATA_TYPE_UCHAR;

    if (boost::is_same<T, short>::value)
        return DATA_TYPE_SHORT;

    if (boost::is_same<T, unsigned short>::value)
        return DATA_TYPE_USHORT;

    if (boost::is_same<T, int>::value)
        return DATA_TYPE_INT;

    if (boost::is_same<T, unsigned int>::value)
        return DATA_TYPE_UINT;

    if (boost::is_same<T, long>::value)
        return DATA_TYPE_LONG;

    if (boost::is_same<T, unsigned long>::value)
        return DATA_TYPE_ULONG;

    if (boost::is_same<T, long long>::value)
        return DATA_TYPE_LONG_LONG;

    if (boost::is_same<T, unsigned long long>::value)
        return DATA_TYPE_ULONG_LONG;

    if (boost::is_same<T, float>::value)
        return DATA_TYPE_FLOAT;

    if (boost::is_same<T, double>::value)
        return DATA_TYPE_DOUBLE;

	if (boost::is_enum<T>::value)
		return DATA_TYPE_INT;

    puts("Unknown data type!");
	puts(typeid(_T).name());
	puts(typeid(T).name());
    return DATA_TYPE_POINTER;
}


// ============================================================================
// >> CFunctionInfo
// ============================================================================
class CFunctionInfo
{
public:
	boost::python::tuple GetArgumentTypes()
	{
		list argumentTypes;
		for(std::vector<DataType_t>::iterator it=this->m_vecArgumentTypes.begin(); it != this->m_vecArgumentTypes.end(); ++it)
		{
			argumentTypes.append(*it);
		}

		return boost::python::tuple(argumentTypes);
	}
    
public:
    // True if the function is virtual
    bool m_bIsVirtual;
    
    // The offset that needs to be added to the this pointer to call the function
    int m_iThisPtrOffset;
    
    // The function's index in the vtable
    int m_iVtableIndex;
    
    // The offset that needs to be added to the this pointer to get the vtable pointer
    int m_iVtableOffset;
    
    // The return type of the function
    DataType_t m_eReturnType;
    
    // All argument types of the function
    std::vector<DataType_t> m_vecArgumentTypes;
    
    // The functions calling convention
    Convention_t m_eCallingConvention;
};
    
// ========================================================================
// >> FunctionAnalyzer
// ========================================================================
class FunctionAnalyzer
{
public:
    template<class Signature>
    FunctionAnalyzer(Signature sig, CFunctionInfo& info)
        : result(info)
    {
        typedef typename boost::mpl::at<Signature, boost::mpl::int_<0> >::type return_type;
        typedef typename boost::mpl::pop_front<Signature>::type parameter_types;

        // Get the return type
        info.m_eReturnType = TypeToDataType_t<return_type>();

        // Analyze the parameters. We need to add 1 pointer to every type to compile without any errors.
        boost::mpl::for_each<typename boost::mpl::transform< parameter_types, boost::add_pointer<boost::mpl::_1> >::type >(*this);
    }
public:
    template<class T>
    void operator()(T)
    {
        // Strip off the pointer we have added
        typedef typename boost::remove_pointer<T>::type without_pointer;
        result.m_vecArgumentTypes.push_back(TypeToDataType_t< without_pointer >());
    }

public:
    CFunctionInfo& result;
};


// ============================================================================
// >> GetFunctionInfo
// ============================================================================
template<class Function>
inline CFunctionInfo* GetFunctionInfo(Function func)
{
	CFunctionInfo* pInfo = new CFunctionInfo();

    MemFuncInfo out;
    MFI_Impl<sizeof(func)>::GetFuncInfo((void *&) func, out);
    
    pInfo->m_bIsVirtual = out.isVirtual;
    pInfo->m_iThisPtrOffset = out.thisptroffs;
    pInfo->m_iVtableIndex = out.vtblindex;
    pInfo->m_iVtableOffset = out.vtbloffs;
    
    // Get return type and parameters
    FunctionAnalyzer(memory::get_signature(func), *pInfo);

    // Get calling convention
    pInfo->m_eCallingConvention = GetCallingConvention(func);
	return pInfo;
}

#endif // _MEMORY_FUNCTION_INFO_H