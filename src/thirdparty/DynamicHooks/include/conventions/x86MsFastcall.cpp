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

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "x86MsFastcall.h"
#include <string.h>


// ============================================================================
// >> x86MsFastcall
// ============================================================================
x86MsFastcall::x86MsFastcall(std::vector<DataType_t> vecArgTypes, DataType_t returnType, int iAlignment) : 
	ICallingConvention(vecArgTypes, returnType, iAlignment)
{
	int iSize = GetDataTypeSize(m_returnType);
	if (iSize > 4)
	{
		m_pReturnBuffer = malloc(iSize);
	}
	else
	{
		m_pReturnBuffer = NULL;
	}
}

x86MsFastcall::~x86MsFastcall()
{
	if (m_pReturnBuffer)
	{
		free(m_pReturnBuffer);
	}
}

std::list<Register_t> x86MsFastcall::GetRegisters()
{
	std::list<Register_t> registers;
	
	registers.push_back(ESP);

	int iArgSize = m_vecArgTypes.size();

	if (iArgSize > 0)
	{
		registers.push_back(ECX);
	}

	if (iArgSize > 1)
	{
		registers.push_back(EDX);
	}

	if (m_returnType == DATA_TYPE_FLOAT || m_returnType == DATA_TYPE_DOUBLE)
	{
		registers.push_back(ST0);
	}
	else
	{
		registers.push_back(EAX);
		if (m_pReturnBuffer && iArgSize <= 1)
		{
			registers.push_back(EDX);
		}
	}

	return registers;
}

int x86MsFastcall::GetPopSize()
{
	int iPopSize = 0;

	for(unsigned int i=2; i < m_vecArgTypes.size(); i++)
	{
		iPopSize += GetDataTypeSize(m_vecArgTypes[i], m_iAlignment);
	}

	return iPopSize;
}

void* x86MsFastcall::GetArgumentPtr(int iIndex, CRegisters* pRegisters)
{
	if (iIndex == 0)
	{
		return pRegisters->m_ecx->m_pAddress;
	}

	if (iIndex == 1)
	{
		return pRegisters->m_edx->m_pAddress;
	}

	int iOffset = 4;
	for(int i=2; i < iIndex; i++)
	{
		iOffset += GetDataTypeSize(m_vecArgTypes[i], m_iAlignment);
	}

	return (void *) (pRegisters->m_esp->GetValue<unsigned long>() + iOffset);
}

void x86MsFastcall::ArgumentPtrChanged(int iIndex, CRegisters* pRegisters, void* pArgumentPtr)
{
}

void* x86MsFastcall::GetReturnPtr(CRegisters* pRegisters)
{
	if (m_returnType == DATA_TYPE_FLOAT || m_returnType == DATA_TYPE_DOUBLE)
		return pRegisters->m_st0->m_pAddress;

	if (m_pReturnBuffer)
	{
		// First half in eax, second half in edx
		memcpy(m_pReturnBuffer, pRegisters->m_eax, 4);
		memcpy((void *) ((unsigned long) m_pReturnBuffer + 4), pRegisters->m_edx, 4);
		return m_pReturnBuffer;
	}

	return pRegisters->m_eax->m_pAddress;
}

void x86MsFastcall::ReturnPtrChanged(CRegisters* pRegisters, void* pReturnPtr)
{
	if (m_returnType == DATA_TYPE_FLOAT || m_returnType == DATA_TYPE_DOUBLE)
		return;

	if (m_pReturnBuffer)
	{
		// First half in eax, second half in edx
		memcpy(pRegisters->m_eax, m_pReturnBuffer, 4);
		memcpy(pRegisters->m_edx, (void *) ((unsigned long) m_pReturnBuffer + 4), 4);
	}
}