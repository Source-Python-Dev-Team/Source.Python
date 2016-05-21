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

#ifndef _MEMORY_ALLOC_H
#define _MEMORY_ALLOC_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "commonmacros.h" // Required for IsPowerOfTwo in memalloc.h
#include <malloc.h>
#include "memalloc.h"


// ============================================================================
// >> UTIL_GetMemSize
// ============================================================================
inline size_t UTIL_GetMemSize(void* ptr)
{
#ifndef NO_MALLOC_OVERRIDE
	return g_pMemAlloc->GetSize(ptr);
#else
	#ifdef _WIN32
		return _msize(ptr);
	#elif defined(__linux__)
		return malloc_usable_size(ptr);
	#else
		#error "Unsupported platform."
	#endif
#endif
}


// ============================================================================
// >> UTIL_Alloc
// ============================================================================
inline void* UTIL_Alloc(size_t size)
{
	void* pPtr = NULL;

#ifndef NO_MALLOC_OVERRIDE
	pPtr = MemAlloc_Alloc(size);
#else
	pPtr = malloc(size);
#endif

	memset(pPtr, 0, size);
	return pPtr;
}


// ============================================================================
// >> UTIL_Realloc
// ============================================================================
inline void* UTIL_Realloc(void* ptr, size_t size)
{
	void* pPtr = NULL;

#ifndef NO_MALLOC_OVERRIDE
	pPtr = g_pMemAlloc->Realloc(ptr, size);
#else
	pPtr = realloc(ptr, size);
#endif

	return pPtr;
}


// ============================================================================
// >> UTIL_Dealloc
// ============================================================================
inline void UTIL_Dealloc(void* ptr)
{
#ifndef NO_MALLOC_OVERRIDE
	g_pMemAlloc->Free(ptr);
#else
	free(ptr);
#endif
}

#endif // _MEMORY_ALLOC_H