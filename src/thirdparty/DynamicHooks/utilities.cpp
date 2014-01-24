/**
* =============================================================================
* DynamicHooks
* Copyright (C) 2013 Robin Gohmert. All rights reserved.
* =============================================================================
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from 
* the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, 
* including commercial applications, and to alter it and redistribute it 
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not 
* claim that you wrote the original software. If you use this software in a 
* product, an acknowledgment in the product documentation would be 
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*
* asm.h/cpp from devmaster.net (thanks cybermind) edited by pRED* to handle gcc
* -fPIC thunks correctly
*
* Idea and trampoline code taken from DynDetours (thanks your-name-here).
*/

// ============================================================================
// >> INCLUDES
// ============================================================================
#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef __linux__
	#include <sys/mman.h>
	#include <unistd.h>
	#define PAGE_SIZE 4096
	#define ALIGN(ar) ((long)ar & ~(PAGE_SIZE-1))
	#define PAGE_EXECUTE_READWRITE PROT_READ|PROT_WRITE|PROT_EXEC
#endif

#include "asm.h"
#include "utilities.h"
using namespace DynamicHooks;


// ============================================================================
// >> GetTypeSize
// ============================================================================
int GetTypeSize(char cType)
{
    switch(cType)
    {
        case SIGCHAR_VOID:      return 0;
        case SIGCHAR_BOOL:
        case SIGCHAR_CHAR:
        case SIGCHAR_UCHAR:
        case SIGCHAR_SHORT:
        case SIGCHAR_USHORT:
        case SIGCHAR_INT:       return sizeof(int);
        case SIGCHAR_UINT:      return sizeof(unsigned int);
        case SIGCHAR_LONG:      return sizeof(long);
        case SIGCHAR_ULONG:     return sizeof(unsigned long);
        case SIGCHAR_LONGLONG:  return sizeof(long long);
        case SIGCHAR_ULONGLONG: return sizeof(unsigned long long);
        case SIGCHAR_FLOAT:     return sizeof(float);
        case SIGCHAR_DOUBLE:    return sizeof(double);
        case SIGCHAR_POINTER:   return sizeof(void *);
        case SIGCHAR_STRING:    return sizeof(char *);
    }

    // TODO: Throw exception here
    return 0;
}


// ============================================================================
// >> ParseParams
// ============================================================================
void ParseParams(Convention_t eConvention, char* szParams, Param_t* pParams, Param_t* pRetParam)
{
	Param_t* param = pParams;
	char* ptr = szParams;
	char ch;
	int offset = 0;

#ifdef _WIN32
		if (eConvention == CONV_THISCALL)
			offset -= sizeof(void *);
#endif

	while ((ch = *ptr) != '\0' && ch != ')')
	{
		ptr++;
		if (ch == SIGCHAR_VOID)
			continue;

		int size = GetTypeSize(ch);
		param->m_cParam = ch;
		param->m_iOffset = offset;
		param->m_iSize = size;
		param = param->m_pNext = new Param_t;
		offset += size;
	}

	param->m_pNext = NULL;
	
	if(ch == '\0')
		ch = SIGCHAR_VOID;
	else
		ch = *++ptr;

	pRetParam->m_cParam  = ch;
	pRetParam->m_iOffset = 0;
	pRetParam->m_iSize   = GetTypeSize(ch);
	pRetParam->m_pNext   = NULL;
}


// ============================================================================
// >> ParseParams
// ============================================================================
void SetMemPatchable(void* pAddr, size_t size)
{
#if defined __linux__
	mprotect((void *) ALIGN(pAddr), sysconf(_SC_PAGESIZE), PAGE_EXECUTE_READWRITE);
#elif defined _WIN32
	DWORD old_prot;
	VirtualProtect(pAddr, size, PAGE_EXECUTE_READWRITE, &old_prot);
#endif
}


// ============================================================================
// >> WriteJMP
// ============================================================================
void WriteJMP(unsigned char* src, void* dest)
{
	SetMemPatchable(src, 20);
	inject_jmp((void *)src, dest);
}