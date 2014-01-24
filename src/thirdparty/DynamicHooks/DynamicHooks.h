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

#ifndef _DYNAMIC_HOOKS_H
#define _DYNAMIC_HOOKS_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include <list>
#include <map>

namespace DynamicHooks {

// ============================================================================
// >> TYPE DEFINITIONS
// ============================================================================
#define SIGCHAR_VOID         'v'
#define SIGCHAR_BOOL         'b'
#define SIGCHAR_CHAR         'c'
#define SIGCHAR_UCHAR        'C'
#define SIGCHAR_SHORT        's'
#define SIGCHAR_USHORT       'S'
#define SIGCHAR_INT          'i'
#define SIGCHAR_UINT         'I'
#define SIGCHAR_LONG         'j'
#define SIGCHAR_ULONG        'J'
#define SIGCHAR_LONGLONG     'l'
#define SIGCHAR_ULONGLONG    'L'
#define SIGCHAR_FLOAT        'f'
#define SIGCHAR_DOUBLE       'd'
#define SIGCHAR_POINTER      'p'
#define SIGCHAR_STRING       'Z'


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
// >> Convention_t
// ============================================================================
enum Convention_t
{
	CONV_CDECL,
	CONV_THISCALL,
	CONV_STDCALL
};


// ============================================================================
// >> Param_t
// ============================================================================
struct Param_t
{
	char     m_cParam;
	int      m_iOffset;
	int      m_iSize;
	Param_t* m_pNext;
};


// ============================================================================
// >> CHook
// ============================================================================
class CHook
{
public:
	CHook(void* pFunc, Convention_t eConvention, char* szParams);
	~CHook();

	/*
		Returns the return value of the trampoline. Returns only valid data in
		a post-hook callback.
	*/
	template<class T>
	T GetReturnValue()
	{
		return *(T *) m_pRetReg;
	}
	
	/*
		Sets a new return value.
	*/
	template<class T>
	void SetReturnValue(T value)
	{
		*(T *) m_pRetReg = value;
	}
	
	/*
		Returns the argument at the given index.
		0 = 1st argument or this pointer
		1 = 2nd argument
		etc...
	*/
	template<class T>
	T GetArgument(int iIndex)
	{
#ifdef _WIN32
		if (m_eConvention == CONV_THISCALL && iIndex == 0)
			return *(T *) &m_pECX;
#endif

		unsigned long reg = ((unsigned long) m_pESP) + GetArgument(iIndex)->m_iOffset + 4;
		return *(T *) reg;
	}

	/*
		Overrides an argument at the given index.
		0 = 1st argument or this pointer
		1 = 2nd argument
		etc...
	*/
	template<class T>
	void SetArgument(int iIndex, T value)
	{
#ifdef _WIN32
		if (m_eConvention == CONV_THISCALL && iIndex == 0)
		{
			m_pECX = *(void **) &value;
			return;
		}
#endif

		unsigned long reg = ((unsigned long) m_pESP) + GetArgument(iIndex)->m_iOffset + 4;
		*(T *) reg = value;
	}

	/*
		Adds a new callback to the callback list.
	*/
	void AddCallback(HookType_t eHookType, void* pCallback);

	/*
		Removes an existing callback from the callback list.
	*/
	void RemoveCallback(HookType_t eHookType, void* pCallback);

	/*
		Returns true if the callback is in the callback list.
	*/
	bool IsCallbackRegistered(HookType_t eHookType, void* pCallback);

	/*
		Returns the size you have to pop off from stack as a callee.
	*/
	int  GetPopSize();

	/*
		Returns the number of arguments + this pointer (if it's a thiscall).
	*/
	int  GetArgumentCount();

	/*
		Returns a Param_t pointer for the given argument index.
	*/
	Param_t* GetArgument(int iIndex);

public:
	// This is the return register buffer (eax and st0)
	void* m_pRetReg;

	// Stack pointer register buffer
	void* m_pESP;

	// Counter register. This will only be set on Windows
	void* m_pECX;

	// Contains the original return address
	void* m_pRetAddr;

	// New return address
	void* m_pNewRetAddr;


	// Parameter struct
	Param_t* m_pParams;

	// Return type struct
	Param_t* m_pRetParam;


	// Address of the trampoline
	void* m_pTrampoline;

	// Address of the bridge
	void* m_pBridge;

	// Address of the hooked function
	void* m_pFunc;


	// Calling convention of the original function
	Convention_t  m_eConvention;

	// Stringified parameter list
	char* m_szParams;


	// A map for all callbacks
	std::map< HookType_t, std::list<void *> > m_Callbacks;
};


// ============================================================================
// >> CHookManager
// ============================================================================
class CHookManager
{
public:
	/*
		Hooks the given function and returns a new CHook instance. If the
		function was already hooked, the existing CHook instance will be
		returned.
	*/
    CHook* HookFunction(void* pFunc, Convention_t eConvention, char* szParams);
	
	/*
		Removes all callbacks and restores the original function.
	*/
    void UnhookFunction(void* pFunc);

	/*
		Returns either NULL or the found CHook instance.
	*/
	CHook* FindHook(void* pFunc);

	/*
		Removes all callbacks and restores all functions.
	*/
	void UnhookAllFunctions();

public:
	std::list<CHook *> m_Hooks;
};


// ============================================================================
// >> GetHookManager
// ============================================================================
/*
	Returns a pointer to a static CHookManager object.
*/
CHookManager* GetHookManager();

} // namespace DynamicHooks

#endif // _DYNAMIC_HOOKS_H