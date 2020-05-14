/**
* =============================================================================
* DynamicHooks
* Copyright (C) 2015 Robin Gohmert. All rights reserved.
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

#ifndef _HOOK_H
#define _HOOK_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include <list>
#include <map>

#include "registers.h"
#include "convention.h"


// ============================================================================
// >> HookType_t
// ============================================================================
enum HookType_t
{
	// Callback will be executed before the original function.
	HOOKTYPE_PRE,

	// Callback will be executed after the original function.
	HOOKTYPE_POST
};


// ============================================================================
// >> TYPEDEFS
// ============================================================================
class CHook;
typedef bool (*HookHandlerFn)(HookType_t, CHook*);

#ifdef __linux__
#define __cdecl
#endif


// ============================================================================
// >> CLASSES
// ============================================================================
namespace AsmJit {
	struct Assembler;
}
using namespace AsmJit;

class CHook
{
private:
	friend class CHookManager;

	/*
	Creates a new function hook.

	@param <pFunc>:
	The address of the function to hook

	@param <pConvention>:
	The calling convention of <pFunc>.
	*/
	CHook(void* pFunc, ICallingConvention* pConvention);
	~CHook();

public:
	/*
	Adds a hook handler to the hook.

	@param type The hook type.
	@param pFunc The hook handler that should be added.
	*/
	void AddCallback(HookType_t type, HookHandlerFn* pFunc);
	
	/*
	Removes a hook handler to the hook.

	@param type The hook type.
	@param pFunc The hook handler that should be removed.
	*/
	void RemoveCallback(HookType_t type, HookHandlerFn* pFunc);
	
	/*
	Checks if a hook handler is already added.

	@param type The hook type.
	@param pFunc The hook handler that should be checked.
	*/
	bool IsCallbackRegistered(HookType_t type, HookHandlerFn* pFunc);

	template<class T>
	T GetArgument(int iIndex)
	{
		return *(T *) m_pCallingConvention->GetArgumentPtr(iIndex, m_pRegisters);
	}

	template<class T>
	void SetArgument(int iIndex, T value)
	{
		void* pPtr = m_pCallingConvention->GetArgumentPtr(iIndex, m_pRegisters);
		*(T *) pPtr = value;
		m_pCallingConvention->ArgumentPtrChanged(iIndex, m_pRegisters, pPtr);
	}

	template<class T>
	T GetReturnValue()
	{
		return *(T *)  m_pCallingConvention->GetReturnPtr(m_pRegisters);
	}

	template<class T>
	void SetReturnValue(T value)
	{
		void* pPtr = m_pCallingConvention->GetReturnPtr(m_pRegisters);
		*(T *)  pPtr = value;
		m_pCallingConvention->ReturnPtrChanged(m_pRegisters, pPtr);
	}

private:
	void* CreateBridge();

	void Write_ModifyReturnAddress(Assembler& a);
	void Write_CallHandler(Assembler& a, HookType_t type);
	void Write_SaveRegisters(Assembler& a);
	void Write_RestoreRegisters(Assembler& a);

	void* CreatePostCallback();

	bool __cdecl HookHandler(HookType_t type);

	void* __cdecl GetReturnAddress(void* pESP);
	void __cdecl SetReturnAddress(void* pRetAddr, void* pESP);

public:
	std::map<HookType_t, std::list<HookHandlerFn*> > m_hookHandler;

	// Address of the original function
	void* m_pFunc;

	ICallingConvention* m_pCallingConvention;

	// Address of the bridge
	void* m_pBridge;

	// Address of the trampoline
	void* m_pTrampoline;

	// Register storage
	CRegisters* m_pRegisters;

	// New return address
	void* m_pNewRetAddr;

	std::map<void*, std::vector<void*> > m_RetAddr;
};

#endif // _HOOK_H