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
* asm.h/c from devmaster.net (thanks cybermind) edited by pRED* to handle gcc
* -fPIC thunks correctly
*/

#ifndef __ASM_H__
#define __ASM_H__

#define OP_JMP				0xE9
#define OP_JMP_SIZE			5

#define OP_NOP				0x90
#define OP_NOP_SIZE			1

#define OP_PREFIX			0xFF
#define OP_JMP_SEG			0x25

#define OP_JMP_BYTE			0xEB
#define OP_JMP_BYTE_SIZE	2

#ifdef __cplusplus
extern "C" {
#endif

	void check_thunks(unsigned char *dest, unsigned char *pc);

	//if dest is NULL, returns minimum number of bytes needed to be copied
	//if dest is not NULL, it will copy the bytes to dest as well as fix CALLs and JMPs
	//http://www.devmaster.net/forums/showthread.php?t=2311
	int copy_bytes(unsigned char *func, unsigned char* dest, int required_len);

	//insert a specific JMP instruction at the given location
	void inject_jmp(void* src, void* dest);

	//fill a given block with NOPs
	void fill_nop(void* src, unsigned int len);

	//evaluate a JMP at the target
	void* eval_jump(void* src);

#ifdef __cplusplus
}
#endif

#endif //__ASM_H__
