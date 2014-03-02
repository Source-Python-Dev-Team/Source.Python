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

#ifndef _MEMORY_CALLBACK_H
#define _MEMORY_CALLBACK_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_tools.h"
#include "utility/sp_util.h"


// ============================================================================
// >> FUNCTIONS
// ============================================================================
// Forward declarations
class CCallback;

object CallCallback(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX);

template<class T>
T CallbackCaller(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX)
{
    return extract<T>(CallCallback(pCallback, ulEBP, ulECX));
}

template<>
inline void CallbackCaller(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX)
{
    CallCallback(pCallback, ulEBP, ulECX);
}

template<>
inline void* CallbackCaller(CCallback* pCallback, unsigned long ulEBP, unsigned long ulECX)
{
	CPointer* pPtr = extract<CPointer *>(CallCallback(pCallback, ulEBP, ulECX));
    return (void *) pPtr->m_ulAddr;
}


// ============================================================================
// >> CLASSES
// ============================================================================
class CCallback: public CFunction
{
public:
    CCallback(object oCallback, Convention_t eConv, tuple args, ReturnType_t return_type);
	virtual ~CCallback();

    int          GetPopSize();
    int          GetArgumentOffset(int iIndex);

	virtual void Dealloc();
	virtual void Realloc(int iSize);

public:
    object m_oCallback;
};

#endif // _MEMORY_CALLBACK_H