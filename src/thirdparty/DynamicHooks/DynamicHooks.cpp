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
#include "DynamicHooks.h"
using namespace DynamicHooks;

#include "asm.h"
#include "utilities.h"

#include "AsmJit.h"
using namespace AsmJit;


// ============================================================================
// >> DEFINITIONS
// ============================================================================
#define JMP_SIZE 6


// ============================================================================
// >> CHookManager
// ============================================================================
CHook* CHookManager::HookFunction(void* pFunc, Convention_t eConvention, char* szParams)
{
	if (!pFunc)
		return NULL;

	CHook* pHook = FindHook(pFunc);
	if (pHook)
		return pHook;
	
	pHook = new CHook(pFunc, eConvention, szParams);
	m_Hooks.push_back(pHook);
	return pHook;
}

void CHookManager::UnhookFunction(void* pFunc)
{
	CHook* pHook = FindHook(pFunc);
	if (pHook)
	{
		m_Hooks.remove(pHook);
		delete pHook;
	}
}

CHook* CHookManager::FindHook(void* pFunc)
{
	if (!pFunc)
		return NULL;

	for(std::list<CHook *>::iterator it=m_Hooks.begin(); it != m_Hooks.end(); it++)
	{
		CHook* pHook = *it;
		if (pHook->m_pFunc == pFunc)
			return pHook;
	}
	return NULL;
}

void CHookManager::UnhookAllFunctions()
{
	for(std::list<CHook *>::iterator it=m_Hooks.begin(); it != m_Hooks.end(); it++)
		delete *it;

	m_Hooks.clear();
}


// ============================================================================
// >> CHook
// ============================================================================
void* CreateBridge(CHook*);

CHook::CHook(void* pFunc, Convention_t eConvention, char* szParams)
{
	m_pFunc       = pFunc;
	m_eConvention = eConvention;
	m_szParams    = szParams;

	// Parse the parameters
	m_pParams    = new Param_t;
	m_pRetParam  = new Param_t;
	ParseParams(eConvention, szParams, m_pParams, m_pRetParam);

	// Allocate space for the return register buffer
	m_pRetReg = malloc(m_pRetParam->m_iSize);
	m_pESP = NULL;
	m_pECX = NULL;

	unsigned char* pTarget = (unsigned char *) pFunc;

	// Determine the number of bytes we need to copy
	int iBytesToCopy = copy_bytes(pTarget, NULL, JMP_SIZE);

	// Create an array for the bytes to copy + a jump to the rest of the
	// function.
	unsigned char* pCopiedBytes = new unsigned char[iBytesToCopy + JMP_SIZE];

	// Fill the array with NOP instructions
	memset(pCopiedBytes, 0x90, iBytesToCopy + JMP_SIZE);

	// Copy the required bytes to our array
	SetMemPatchable(pCopiedBytes, iBytesToCopy + JMP_SIZE);
	copy_bytes(pTarget, pCopiedBytes, JMP_SIZE);
	
	// Write a jump after the copied bytes to the function/bridge + number of bytes to copy
	WriteJMP(pCopiedBytes + iBytesToCopy, pTarget + iBytesToCopy);

	// Save the trampoline
	m_pTrampoline = (void *) pCopiedBytes;

	// Create the bridge function
	m_pBridge = CreateBridge(this);

	// Write a jump to the bridge
	WriteJMP((unsigned char *) pFunc, m_pBridge);

}

CHook::~CHook()
{
	// Free the return register buffer
	free(m_pRetReg);

	// Delete the return parameter struct
	delete m_pRetParam;

	// Delete all parameters
	Param_t* pNext = m_pParams;
	while (pNext)
	{
		Param_t* temp = pNext->m_pNext;
		delete pNext;
		pNext = temp;
	}

	// Copy back the previously copied bytes
	copy_bytes((unsigned char *) m_pTrampoline, (unsigned char *) m_pFunc, JMP_SIZE);

	// Free the trampoline array
	free(m_pTrampoline);

	// Free the asm bridge and new return address
	MemoryManager::getGlobal()->free(m_pBridge);
	MemoryManager::getGlobal()->free(m_pNewRetAddr);
}

void CHook::AddCallback(HookType_t eHookType, void* pCallback)
{
	if (!pCallback)
		return;

	if (!IsCallbackRegistered(eHookType, pCallback))
		m_Callbacks[eHookType].push_back(pCallback);
}

void CHook::RemoveCallback(HookType_t eHookType, void* pCallback)
{
	if (IsCallbackRegistered(eHookType, pCallback))
		m_Callbacks[eHookType].remove(pCallback);
}

bool CHook::IsCallbackRegistered(HookType_t eHookType, void* pCallback)
{
	std::list<void *> callbacks = m_Callbacks[eHookType];
	for(std::list<void *>::iterator it=callbacks.begin(); it != callbacks.end(); it++)
	{
		if (*it == pCallback)
			return true;
	}
	return false;
}

int CHook::GetPopSize()
{
#ifdef _WIN32
	if (m_eConvention == CONV_THISCALL || m_eConvention == CONV_STDCALL)
	{
		Param_t* pParam = GetArgument(GetArgumentCount() - 1);
		return pParam->m_iOffset + pParam->m_iSize;
	}
#endif
	return 0;
}

int CHook::GetArgumentCount()
{
	int count = 0;
	Param_t* temp = m_pParams;
	while(temp)
	{
		count++;
		temp = temp->m_pNext;
	}
	return count - 1;
}

Param_t* CHook::GetArgument(int iIndex)
{
	Param_t* temp = m_pParams;
	while(temp && iIndex > 0)
	{
		iIndex--;
		temp = temp->m_pNext;
	}
	return temp;
}


// ============================================================================
// >> GetHookManager
// ============================================================================
namespace DynamicHooks
{
	CHookManager* GetHookManager()
	{
		static CHookManager* s_pManager = new CHookManager;
		return s_pManager;
	}
}


// ============================================================================
// >> HookHandler
// ============================================================================
int HookHandler(HookType_t eHookType, CHook* pHook)
{
	bool bOverride = false;
	std::list<void *> callbacks = pHook->m_Callbacks[eHookType];
	for(std::list<void *>::iterator it=callbacks.begin(); it != callbacks.end(); it++)
	{
		bool result = ((HookFn) *it)(eHookType, pHook);
		if (result)
			bOverride = true;
	}
	return bOverride;
}


// ============================================================================
// >> CreateBridge
// ============================================================================
void Write_SaveRegisters(Assembler& a, CHook* pHook)
{
	a.mov(dword_ptr_abs(&pHook->m_pESP), esp);

#ifdef _WIN32
	if (pHook->m_eConvention == CONV_THISCALL)
		// On Windows the this pointer is saved in ecx
		a.mov(dword_ptr_abs(&pHook->m_pECX), ecx);
#endif
}

void Write_SaveReturnValue(Assembler& a, CHook* pHook)
{
	char type = pHook->m_pRetParam->m_cParam;
	if (type == SIGCHAR_FLOAT)
		a.fst(dword_ptr_abs(pHook->m_pRetReg));
	else if (type == SIGCHAR_DOUBLE)
		a.fst(qword_ptr_abs(pHook->m_pRetReg));
	else
		a.mov_ptr(pHook->m_pRetReg, eax);
}

void Write_RestoreReturnValue(Assembler& a, CHook* pHook)
{
	char type = pHook->m_pRetParam->m_cParam;
	if (type == SIGCHAR_FLOAT)
		a.fld(dword_ptr_abs(pHook->m_pRetReg));
	else if (type == SIGCHAR_DOUBLE)
		a.fld(qword_ptr_abs(pHook->m_pRetReg));
	else
		a.mov_ptr(eax, pHook->m_pRetReg);
}

void Write_CallHandler(Assembler& a, CHook* pHook, HookType_t eHookType)
{
	a.push(imm((sysint_t) pHook));
	a.push(eHookType);
	a.call((void *) &HookHandler);
	a.add(esp, 8);
}

void* CreatePostCallback(CHook* pHook)
{
	Assembler a;
	
	// Save the return value for later access
	Write_SaveReturnValue(a, pHook);
	
	// On Windows thiscalls and stdcalls we have to subtract the pop size, so
	// the post-hook callbacks can access the arguments again
	Imm iBytesToPop = imm(pHook->GetPopSize());

	// Subtract the previous added bytes
	a.sub(esp, iBytesToPop);

	// Call the post-hook handler
	Write_CallHandler(a, pHook, HOOKTYPE_POST);

	// Add them again to the stack
	a.add(esp, iBytesToPop);

	// Use the new return value
	Write_RestoreReturnValue(a, pHook);

	// Jump to the original return address
	a.jmp(dword_ptr_abs(&pHook->m_pRetAddr));

	// Generate the code
	return a.make();
}

void Write_ModifyReturnAddress(Assembler& a, CHook* pHook)
{
	// Unfortunately, there is no Assembler::mov(Mem, Mem). That's why this
	// workaround is required.
	void* pEDX = NULL;

	// Store edx in pEDX
	a.mov(dword_ptr_abs(&pEDX), edx);

	// Store the return address in edx
	a.mov(edx, dword_ptr(esp));

	// Store return address in m_pRetAddr, so we can access it later
	a.mov(dword_ptr_abs(&pHook->m_pRetAddr), edx);

	// Restore edx
	a.mov(edx, dword_ptr_abs(&pEDX));

	// Override the return address. This is a redirect to our post-hook code
	void* pNewRetAddr = CreatePostCallback(pHook);
	pHook->m_pNewRetAddr = pNewRetAddr;
	a.mov(dword_ptr(esp), imm((sysint_t) pNewRetAddr));
}

void* CreateBridge(CHook* pHook)
{
	Assembler a;
	Label label_override = a.newLabel();

	// Write a redirect to the post-hook code
	Write_ModifyReturnAddress(a, pHook);

	// Save esp and ecx (on Windows) for later access (arguments and thisptr)
	Write_SaveRegisters(a, pHook);
	
	// Call the pre-hook handler and jump to label_override if true was returned
	Write_CallHandler(a, pHook, HOOKTYPE_PRE);
	a.cmp(eax, true);
	a.je(label_override);

#ifdef _WIN32
	// Restore ecx, because it changes either in the global hook handler ('cause of the loop)
	// or in a registered hook handler (through SetArgument). That's a problem if the trampoline
	// requires a valid this pointer -- e.g. if it uses virtual functions of its class.
	// TODO: Add a Write_RestorePreservedRegisters()
	if (pHook->m_eConvention == CONV_THISCALL)
		a.mov(ecx, dword_ptr_abs(&pHook->m_pECX));
#endif

	// Jump to the trampoline
	a.jmp(pHook->m_pTrampoline);

	// This code will be executed if a pre-hook returns true
	a.bind(label_override);

	// Use the new return value
	Write_RestoreReturnValue(a, pHook);

	// Finally, return to the caller
	a.ret(imm(pHook->GetPopSize()));

	return a.make();
}