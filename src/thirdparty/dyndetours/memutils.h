/**
* ======================================================
* DynDetours
* Copyright (C) 2009 Deniz Sezen
* All rights reserved.
* ======================================================
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
* SetMemPatchable and ProtectMemory taken entirely from CSSDM by
* David "BAILOPAN" Anderson.
*/

#ifndef _MEMUTILS_H
#define _MEMUTILS_H

// ==================================================================
// Includes
// ==================================================================
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>

// ==================================================================
// Memory functions.
// ==================================================================
extern void ProtectMemory(void *addr, int length, int prot);
extern void SetMemPatchable(void *address, size_t size);

// ==================================================================
// Misc detour functions.
// ==================================================================
extern void WriteJMP(unsigned char* src, void* dest);

#endif
