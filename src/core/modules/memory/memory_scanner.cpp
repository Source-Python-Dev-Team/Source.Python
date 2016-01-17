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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <fcntl.h>
	#include <link.h>
	#include <sys/mman.h>
#endif

#include "dynload.h"

#include "memory_scanner.h"
#include "memory_tools.h"
#include "utilities/sp_util.h"
#include "utilities/call_python.h"
#include "sp_main.h"
#include "eiface.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IVEngineServer* engine;


//-----------------------------------------------------------------------------
// BinaryFile class
//-----------------------------------------------------------------------------
CBinaryFile::CBinaryFile(unsigned long ulAddr, unsigned long ulSize)
{
	m_ulAddr = ulAddr;
	m_ulSize = ulSize;
}

CPointer* CBinaryFile::FindSignatureRaw(object oSignature)
{
	unsigned char* sigstr = (unsigned char *) PyBytes_AsString(oSignature.ptr());
	if (!sigstr)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to read the given signature.");

	int iLength = len(oSignature);

	unsigned char* base = (unsigned char *) m_ulAddr;
	unsigned char* end  = (unsigned char *) (base + m_ulSize - iLength);

	while(base < end)
	{
		int i = 0;
		for(; i < iLength; i++)
		{
			if (sigstr[i] == '\x2A')
				continue;

			if (sigstr[i] != base[i])
				break;
		}

		if (i == iLength)
		{
			return new CPointer((unsigned long) base);
		}
		base++;
	}
	return new CPointer();
}

void CBinaryFile::AddSignatureToCache(unsigned char* sigstr, int iLength, unsigned int ulAddr)
{
	Signature_t sig_t = {new unsigned char[iLength+1], ulAddr};
	strcpy((char*) sig_t.m_szSignature, (char*) sigstr);
	m_Signatures.push_back(sig_t);
}

bool CBinaryFile::SearchSigInCache(unsigned char* sigstr, CPointer*& result)
{
	PythonLog(4, "Searching for a cached signature...");
	for (std::list<Signature_t>::iterator iter=m_Signatures.begin(); iter != m_Signatures.end(); iter++)
	{
		Signature_t sig = *iter;
		if (strcmp((const char *) sig.m_szSignature, (const char *) sigstr) == 0)
		{
			PythonLog(4, "Found a cached signature!");
			result = new CPointer(sig.m_ulAddr);
			return true;
		}
	}
	PythonLog(4, "Could not find a cached signature.");
	return false;
}

bool CBinaryFile::SearchSigInBinary(object oSignature, int iLength, unsigned char* sigstr, CPointer*& result)
{
	PythonLog(4, "Searching in the binary...");
	CPointer* pPtr = FindSignatureRaw(oSignature);
	if (pPtr->IsValid())
	{
		PythonLog(4, "Found a signature in the binary!");
		AddSignatureToCache(sigstr, iLength, pPtr->m_ulAddr);
		result = pPtr;
		return true;
	}
	delete pPtr;
	PythonLog(4, "Could not find the signature in the binary.");
	return false;
}

bool CBinaryFile::SearchSigHooked(object oSignature, int iLength, unsigned char* sigstr, CPointer*& result)
{
	CPointer* pPtr = FindSignatureRaw(oSignature);
	if (!pPtr->IsValid())
	{
		PythonLog(4, "Could not find a hooked signature.");
		delete pPtr;
		return false;
	}

	PythonLog(4, "Found a hooked signature!");
	PythonLog(4, "Checking if it's unique...");

	// Add iLength, so we start searching after the match
	CPointer new_ptr = CPointer(pPtr->m_ulAddr + len(oSignature));

	// Got another match after the first one?
	CPointer* pNext = new_ptr.SearchBytes(oSignature, (m_ulAddr + m_ulSize) - new_ptr.m_ulAddr);
	bool bIsValid = pNext->IsValid();
	delete pNext;

	if (bIsValid)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Found more than one hooked signatures. Please pass more bytes.");

	PythonLog(4, "Signature is unique!");
	AddSignatureToCache(sigstr, iLength, pPtr->m_ulAddr);
	result = pPtr;
	return true;
}

CPointer* CBinaryFile::FindSignature(object oSignature)
{
	unsigned char* sigstr = (unsigned char *) PyBytes_AsString(oSignature.ptr());
	if (!sigstr)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to read the given signature.");
	
	CPointer* result = NULL;
	if (SearchSigInCache(sigstr, result))
		return result;
	
	int iLength = len(oSignature);
	if (SearchSigInBinary(oSignature, iLength, sigstr, result))
		return result;
	
	PythonLog(4, "Searching for a hooked signature (relative jump)...");
	if (iLength <= 6)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Signature is too short to search for a hooked signature (relative jump).");

	oSignature = import("binascii").attr("unhexlify")("E92A2A2A2A") + oSignature.slice(5, _);
	if (SearchSigHooked(oSignature, iLength, sigstr, result))
		return result;
	
	PythonLog(4, "Searching for a hooked signature (absolute jump)...");
	if (iLength <= 7)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Signature is too short to search for a hooked signature (absolute jump).");

	oSignature = import("binascii").attr("unhexlify")("FF252A2A2A2A") + oSignature.slice(6, _);
	if (SearchSigHooked(oSignature, iLength, sigstr, result))
		return result;

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Could not find signature.");
	return new CPointer(); // To fix a warning. This will never get called.
}

CPointer* CBinaryFile::FindSymbol(char* szSymbol)
{
#ifdef _WIN32
	return new CPointer((unsigned long) GetProcAddress((HMODULE) m_ulAddr, szSymbol));

#elif defined(__linux__)
	// -----------------------------------------
	// We need to use mmap now that VALVe has
	// made them all private!
	// Thank you to DamagedSoul from AlliedMods
	// for the following code.
	// It can be found at:
	// http://hg.alliedmods.net/sourcemod-central/file/dc361050274d/core/logic/MemoryUtils.cpp
	// -----------------------------------------
	struct link_map *dlmap;
	struct stat dlstat;
	int dlfile;
	uintptr_t map_base;
	Elf32_Ehdr *file_hdr;
	Elf32_Shdr *sections, *shstrtab_hdr, *symtab_hdr, *strtab_hdr;
	Elf32_Sym *symtab;
	const char *shstrtab, *strtab;
	uint16_t section_count;
	uint32_t symbol_count;

	dlmap = (struct link_map *) m_ulAddr;
	symtab_hdr = NULL;
	strtab_hdr = NULL;

	dlfile = open(dlmap->l_name, O_RDONLY);
	if (dlfile == -1 || fstat(dlfile, &dlstat) == -1)
	{
		close(dlfile);
		return new CPointer();
	}

	/* Map library file into memory */
	file_hdr = (Elf32_Ehdr *)mmap(NULL, dlstat.st_size, PROT_READ, MAP_PRIVATE, dlfile, 0);
	map_base = (uintptr_t)file_hdr;
	if (file_hdr == MAP_FAILED)
	{
		close(dlfile);
		return new CPointer();
	}
	close(dlfile);

	if (file_hdr->e_shoff == 0 || file_hdr->e_shstrndx == SHN_UNDEF)
	{
		munmap(file_hdr, dlstat.st_size);
		return new CPointer();
	}

	sections = (Elf32_Shdr *)(map_base + file_hdr->e_shoff);
	section_count = file_hdr->e_shnum;
	/* Get ELF section header string table */
	shstrtab_hdr = &sections[file_hdr->e_shstrndx];
	shstrtab = (const char *)(map_base + shstrtab_hdr->sh_offset);

	/* Iterate sections while looking for ELF symbol table and string table */
	for (uint16_t i = 0; i < section_count; i++)
	{
		Elf32_Shdr &hdr = sections[i];
		const char *section_name = shstrtab + hdr.sh_name;

		if (strcmp(section_name, ".symtab") == 0)
			symtab_hdr = &hdr;

		else if (strcmp(section_name, ".strtab") == 0)
			strtab_hdr = &hdr;
	}

	/* Uh oh, we don't have a symbol table or a string table */
	if (symtab_hdr == NULL || strtab_hdr == NULL)
	{
		munmap(file_hdr, dlstat.st_size);
		return new CPointer();
	}

	symtab = (Elf32_Sym *)(map_base + symtab_hdr->sh_offset);
	strtab = (const char *)(map_base + strtab_hdr->sh_offset);
	symbol_count = symtab_hdr->sh_size / symtab_hdr->sh_entsize;
	void* sym_addr = NULL;

	/* Iterate symbol table starting from the position we were at last time */
	for (uint32_t i = 0; i < symbol_count; i++)
	{
		Elf32_Sym &sym = symtab[i];
		unsigned char sym_type = ELF32_ST_TYPE(sym.st_info);
		const char *sym_name = strtab + sym.st_name;

		/* Skip symbols that are undefined or do not refer to functions or objects */
		if (sym.st_shndx == SHN_UNDEF || (sym_type != STT_FUNC && sym_type != STT_OBJECT))
			continue;

		if (strcmp(szSymbol, sym_name) == 0)
		{
			sym_addr = (void *)(dlmap->l_addr + sym.st_value);
			break;
		}
	}

	// Unmap the file now.
	munmap(file_hdr, dlstat.st_size);
	return new CPointer((unsigned long) sym_addr);

#else
#error "BinaryFile::FindSymbol() is not implemented on this OS"
#endif
}

CPointer* CBinaryFile::FindPointer(object oIdentifier, int iOffset, unsigned int iLevel)
{
	CPointer* ptr = FindAddress(oIdentifier);
	if (ptr->IsValid())
	{
		ptr->m_ulAddr += iOffset;
		while (iLevel > 0)
		{
			ptr = ptr->GetPtr();
			iLevel = iLevel - 1;
		}
	}
	return ptr;
}

CPointer* CBinaryFile::FindAddress(object oIdentifier)
{
#ifdef _WIN32
	if(CheckClassname(oIdentifier, "bytes"))
		return FindSignature(oIdentifier);
#endif
	
	return FindSymbol(extract<char*>(oIdentifier));
}

//-----------------------------------------------------------------------------
// CBinaryManager class
//-----------------------------------------------------------------------------
// Small helper function
bool str_ends_with(const char *szString, const char *szSuffix)
{
	int stringlen = strlen(szString);
	int suffixlen = strlen(szSuffix);
	if (suffixlen >  stringlen)
		return false;

	return strncmp(szString + stringlen - suffixlen, szSuffix, suffixlen) == 0;
}

CBinaryFile* CBinaryManager::FindBinary(char* szPath, bool bSrvCheck /* = true */)
{
	std::string szBinaryPath = szPath;
#ifdef __linux__
	if (bSrvCheck && !str_ends_with(szBinaryPath.data(), "_srv") && !str_ends_with(szBinaryPath.data(), ".so"))
		szBinaryPath += "_srv.so";
	else if (!str_ends_with(szBinaryPath.data(), ".so"))
		szBinaryPath += ".so";
#endif

	unsigned long ulAddr = (unsigned long) dlLoadLibrary(szBinaryPath.data());
#ifdef __linux__
	if (!ulAddr)
	{
		char szGameDir[MAX_PATH_LENGTH];
		engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

		// If the previous path failed, try the "bin" folder of the game.
		// This will allow passing e.g. "server" to this function.
		szBinaryPath = std::string(szGameDir) + "/bin/" + szBinaryPath;
		ulAddr = (unsigned long) dlLoadLibrary(szBinaryPath.data());
	}
#endif

	if (!ulAddr)
	{
		szBinaryPath = "Unable to find " + szBinaryPath;
		#ifdef _WIN32
			if (!str_ends_with(szBinaryPath.data(), ".dll"))
				szBinaryPath += ".dll";
		#endif
		BOOST_RAISE_EXCEPTION(PyExc_IOError, szBinaryPath.data())
	}

	// Search for an existing BinaryFile object
	for (std::list<CBinaryFile *>::iterator iter=m_Binaries.begin(); iter != m_Binaries.end(); iter++)
	{
		CBinaryFile* binary = *iter;
		if (binary->m_ulAddr == ulAddr)
		{
			// We don't need to open it several times
			dlFreeLibrary((DLLib *) ulAddr);
			return binary;
		}
	}

	unsigned long ulSize;

#ifdef _WIN32
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER *) ulAddr;
	IMAGE_NT_HEADERS* nt  = (IMAGE_NT_HEADERS *) ((BYTE *) dos + dos->e_lfanew);
	ulSize = nt->OptionalHeader.SizeOfImage;

#elif defined(__linux__)
	// TODO: Retrieve whole size
	struct stat buf;
	if (stat(szBinaryPath.data(), &buf) == -1)
	{
		dlFreeLibrary((DLLib *) ulAddr);
		szBinaryPath = "Unable to retrieve the binary size for " + szBinaryPath;
		BOOST_RAISE_EXCEPTION(PyExc_RuntimeError, szBinaryPath.data())
	}
	ulSize = buf.st_size;

#else
#error "BinaryManager::FindBinary() is not implemented on this OS"
#endif

	// Create a new Binary object and add it to the list
	CBinaryFile* binary = new CBinaryFile(ulAddr, ulSize);
	m_Binaries.push_front(binary);
	return binary;
}

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
CBinaryFile* FindBinary(char* szPath, bool bSrvCheck /* = true */)
{
	return s_pBinaryManager->FindBinary(szPath, bSrvCheck);
}
