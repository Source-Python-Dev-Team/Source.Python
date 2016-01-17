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

#ifndef _MEMORY_SCANNER_H
#define _MEMORY_SCANNER_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <list>
#include "export_main.h"
#include "memory_tools.h"

struct Signature_t
{
	unsigned char* m_szSignature;
	unsigned long  m_ulAddr;
};


class CBinaryFile
{
public:
	CBinaryFile(unsigned long ulAddr, unsigned long ulSize);

	CPointer* FindSignatureRaw(object oSignature);

	CPointer* FindSignature(object oSignature);
	CPointer* FindSymbol(char* szSymbol);
	CPointer* FindPointer(object oIdentifier, int iOffset, unsigned int iLevel);
	CPointer* FindAddress(object oIdentifier);


private:
	void AddSignatureToCache(unsigned char* sigstr, int iLength, unsigned int ulAddr);

	bool SearchSigInCache(unsigned char* sigstr, CPointer*& result);
	bool SearchSigInBinary(object oSignature, int iLength, unsigned char* sigstr, CPointer*& result);
	bool SearchSigHooked(object oSignature, int iLength, unsigned char* sigstr, CPointer*& result);

public:
	unsigned long          m_ulAddr;
	unsigned long          m_ulSize;
	std::list<Signature_t> m_Signatures;
};


class CBinaryManager
{
public:
	CBinaryFile* FindBinary(char* szPath, bool bSrvCheck = true);

private:
	std::list<CBinaryFile*> m_Binaries;
};

static CBinaryManager* s_pBinaryManager = new CBinaryManager();

CBinaryFile* FindBinary(char* szPath, bool bSrvCheck = true);

#endif // _MEMORY_SCANNER_H
