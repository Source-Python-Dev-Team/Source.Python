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
	extern int PAGE_SIZE;
	#define PAGE_ALIGN_UP(x) ((x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#endif

#include "dynload.h"

#include "memory_scanner.h"
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
CBinaryFile::CBinaryFile(unsigned long ulModule, unsigned long ulBase, unsigned long ulSize)
{
	m_ulModule = ulModule;
	m_ulBase = ulBase;
	m_ulSize = ulSize;
}

CPointer* CBinaryFile::FindSignatureRaw(object oSignature)
{
	unsigned char* sigstr = (unsigned char *) PyBytes_AsString(oSignature.ptr());
	if (!sigstr)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to read the given signature.");

	int iLength = len(oSignature);

	unsigned char* base = (unsigned char *) m_ulBase;
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
	for (std::list<Signature_t>::iterator iter=m_Signatures.begin(); iter != m_Signatures.end(); ++iter)
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
	CPointer* pNext = new_ptr.SearchBytes(oSignature, (m_ulBase + m_ulSize) - new_ptr.m_ulAddr);
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

	object oHexSig = oSignature.attr("hex")();
	const char* szHexSig = extract<const char*>(oHexSig);
	
	PythonLog(4, "Searching for a hooked signature (relative jump)...");
	if (iLength <= 6)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Signature is too short to search for a hooked signature (relative jump): %s", szHexSig);

	static object unhexlify = import("binascii").attr("unhexlify");
	oSignature = unhexlify("E92A2A2A2A") + oSignature.slice(5, _);
	if (SearchSigHooked(oSignature, iLength, sigstr, result))
		return result;
	
	PythonLog(4, "Searching for a hooked signature (absolute jump)...");
	if (iLength <= 7)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Signature is too short to search for a hooked signature (absolute jump): %s", szHexSig);

	oSignature = import("binascii").attr("unhexlify")("FF252A2A2A2A") + oSignature.slice(6, _);
	if (SearchSigHooked(oSignature, iLength, sigstr, result))
		return result;
	
	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Could not find signature: %s", szHexSig);
	return new CPointer(); // To fix a warning. This will never get called.
}

CPointer* CBinaryFile::FindSymbol(char* szSymbol)
{
#ifdef _WIN32
	void* pAddr = GetProcAddress((HMODULE) m_ulModule, szSymbol);
	if (!pAddr)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Could not find symbol: %s", szSymbol)

	return new CPointer((unsigned long) pAddr);

#elif defined(__linux__)
	dlerror();
	void* pResult = dlsym((void*) m_ulModule, szSymbol);
	if (!dlerror())
		return new CPointer((unsigned long) pResult);

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

	dlmap = (struct link_map *) m_ulModule;
	symtab_hdr = NULL;
	strtab_hdr = NULL;

	dlfile = open(dlmap->l_name, O_RDONLY);
	if (dlfile == -1 || fstat(dlfile, &dlstat) == -1)
	{
		close(dlfile);
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to open file. Symbol: %s", szSymbol)
	}

	/* Map library file into memory */
	file_hdr = (Elf32_Ehdr *)mmap(NULL, dlstat.st_size, PROT_READ, MAP_PRIVATE, dlfile, 0);
	map_base = (uintptr_t)file_hdr;
	close(dlfile);
	if (file_hdr == MAP_FAILED)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to map file. Symbol: %s", szSymbol)

	if (file_hdr->e_shoff == 0 || file_hdr->e_shstrndx == SHN_UNDEF)
	{
		munmap(file_hdr, dlstat.st_size);
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "No section header string table has been found. Symbol: %s", szSymbol)
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
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "No symbol table or string table found. Symbol: %s", szSymbol)
	}

	symtab = (Elf32_Sym *)(map_base + symtab_hdr->sh_offset);
	strtab = (const char *)(map_base + strtab_hdr->sh_offset);
	symbol_count = symtab_hdr->sh_size / symtab_hdr->sh_entsize;

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
			pResult = (void *)(dlmap->l_addr + sym.st_value);
			break;
		}
	}

	// Unmap the file now.
	munmap(file_hdr, dlstat.st_size);
	if (!pResult)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Could not find symbol: %s", szSymbol)

	return new CPointer((unsigned long) pResult);
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
			ptr->m_ulAddr = GetPtrHelper(ptr->m_ulAddr);
			iLevel = iLevel - 1;
		}
	}
	return ptr;
}

CPointer* CBinaryFile::FindAddress(object oIdentifier)
{
	if(CheckClassname(oIdentifier, "bytes"))
		return FindSignature(oIdentifier);
	
	return FindSymbol(extract<char*>(oIdentifier));
}

dict CBinaryFile::GetSymbols()
{
	dict result;
#ifdef _WIN32
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER) m_ulModule;
	if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve DOS header.")

	PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS) ((BYTE *) m_ulModule + dos_header->e_lfanew);
	if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve NT headers.")

	if (nt_headers->OptionalHeader.NumberOfRvaAndSizes <= 0)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid number of directories in the optional header.")

	PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY) (
		(BYTE *) m_ulModule
		+ nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	if (exports->AddressOfNames == NULL)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Address of names is NULL.")

	BYTE** symbols = (BYTE**)(m_ulModule + exports->AddressOfNames);
	for (DWORD i=0; i < exports->NumberOfNames; i++)
	{
		const char* name = (const char*) (m_ulModule + symbols[i]);

		// TODO: Don't use GetProcAddress. There is probably a faster way
		result[name] = CPointer((unsigned long) GetProcAddress((HMODULE) m_ulModule, name));
	}
#elif __linux__
	// TODO: Remove duplicated code. See also: FindSymbol()
	
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

	dlmap = (struct link_map *) m_ulModule;
	symtab_hdr = NULL;
	strtab_hdr = NULL;

	dlfile = open(dlmap->l_name, O_RDONLY);
	if (dlfile == -1 || fstat(dlfile, &dlstat) == -1)
	{
		close(dlfile);
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to open file.")
	}

	/* Map library file into memory */
	file_hdr = (Elf32_Ehdr *)mmap(NULL, dlstat.st_size, PROT_READ, MAP_PRIVATE, dlfile, 0);
	map_base = (uintptr_t)file_hdr;
	close(dlfile);
	if (file_hdr == MAP_FAILED)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to map file.")

	if (file_hdr->e_shoff == 0 || file_hdr->e_shstrndx == SHN_UNDEF)
	{
		munmap(file_hdr, dlstat.st_size);
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "No section header string table has been found.")
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
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "No symbol table or string table found.")
	}

	symtab = (Elf32_Sym *)(map_base + symtab_hdr->sh_offset);
	strtab = (const char *)(map_base + strtab_hdr->sh_offset);
	symbol_count = symtab_hdr->sh_size / symtab_hdr->sh_entsize;

	/* Iterate symbol table starting from the position we were at last time */
	for (uint32_t i = 0; i < symbol_count; i++)
	{
		Elf32_Sym &sym = symtab[i];
		unsigned char sym_type = ELF32_ST_TYPE(sym.st_info);
		const char *sym_name = strtab + sym.st_name;

		/* Skip symbols that are undefined or do not refer to functions or objects */
		if (sym.st_shndx == SHN_UNDEF || (sym_type != STT_FUNC && sym_type != STT_OBJECT))
			continue;

		result[sym_name] = CPointer((unsigned long)(dlmap->l_addr + sym.st_value));
	}

	// Unmap the file now.
	munmap(file_hdr, dlstat.st_size);
#else
	#error Unsupported platform.
#endif
	return result;
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

CBinaryFile* CBinaryManager::FindBinary(char* szPath, bool bSrvCheck /* = true */, bool bCheckExtension /* = true */)
{
	std::string szBinaryPath = szPath;
#ifdef __linux__
	if (bCheckExtension) {
		if (bSrvCheck && !str_ends_with(szBinaryPath.data(), "_srv") && !str_ends_with(szBinaryPath.data(), ".so"))
			szBinaryPath += "_srv.so";
		else if (!str_ends_with(szBinaryPath.data(), ".so"))
			szBinaryPath += ".so";
	}
#endif

	unsigned long ulModule = (unsigned long) dlLoadLibrary(szBinaryPath.data());
	unsigned long ulBase = 0;
#ifdef __linux__
	if (!ulModule)
	{
		char szGameDir[MAX_PATH_LENGTH];
		engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

		// If the previous path failed, try the "bin" folder of the game.
		// This will allow passing e.g. "server" to this function.
		szBinaryPath = std::string(szGameDir) + "/bin/" + szBinaryPath;
		ulModule = (unsigned long) dlLoadLibrary(szBinaryPath.data());
	}
#endif

	if (!ulModule)
	{
		szBinaryPath = "Unable to find " + szBinaryPath;
		#ifdef _WIN32
			if (bCheckExtension && !str_ends_with(szBinaryPath.data(), ".dll"))
				szBinaryPath += ".dll";
		#endif
		BOOST_RAISE_EXCEPTION(PyExc_IOError, szBinaryPath.data())
	}

	// Search for an existing BinaryFile object
	for (std::list<CBinaryFile *>::iterator iter=m_Binaries.begin(); iter != m_Binaries.end(); ++iter)
	{
		CBinaryFile* binary = *iter;
		if (binary->m_ulModule == ulModule)
		{
			// We don't need to open it several times
			dlFreeLibrary((DLLib *) ulModule);
			return binary;
		}
	}

	unsigned long ulSize;

#ifdef _WIN32
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER *) ulModule;
	IMAGE_NT_HEADERS* nt  = (IMAGE_NT_HEADERS *) ((BYTE *) dos + dos->e_lfanew);
	ulSize = nt->OptionalHeader.SizeOfImage;
	ulBase = ulModule;

#elif defined(__linux__)
	// Copied from here. Thanks!
	// https://github.com/alliedmodders/sourcemod/blob/237db0504c7a59e394828446af3e8ca3d53ef647/core/logic/MemoryUtils.cpp#L486

	Elf32_Ehdr *file;
	Elf32_Phdr *phdr;
	uint16_t phdrCount;

	struct link_map *lm = (struct link_map*) ulModule;
	ulBase = reinterpret_cast<uintptr_t>(lm->l_addr);
	file = reinterpret_cast<Elf32_Ehdr *>(ulBase);

	/* Check ELF magic */
	if (memcmp(ELFMAG, file->e_ident, SELFMAG) != 0)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "ELF magic check failed.");
	}

	/* Check ELF version */
	if (file->e_ident[EI_VERSION] != EV_CURRENT)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "ELF version check failed.");
	}

	/* Check ELF architecture, which is 32-bit/x86 right now
	 * Should change this for 64-bit if Valve gets their act together
	 */
	if (file->e_ident[EI_CLASS] != ELFCLASS32 || file->e_machine != EM_386 || file->e_ident[EI_DATA] != ELFDATA2LSB)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "ELF architecture check failed.");
	}

	/* For our purposes, this must be a dynamic library/shared object */
	if (file->e_type != ET_DYN)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Library is not a dynamic or shared object.");
	}

	phdrCount = file->e_phnum;
	phdr = reinterpret_cast<Elf32_Phdr *>(ulBase + file->e_phoff);

	for (uint16_t i = 0; i < phdrCount; i++)
	{
		Elf32_Phdr &hdr = phdr[i];

		/* We only really care about the segment with executable code */
		if (hdr.p_type == PT_LOAD && hdr.p_flags == (PF_X|PF_R))
		{
			/* From glibc, elf/dl-load.c:
			 * c->mapend = ((ph->p_vaddr + ph->p_filesz + GLRO(dl_pagesize) - 1) 
			 *             & ~(GLRO(dl_pagesize) - 1));
			 *
			 * In glibc, the segment file size is aligned up to the nearest page size and
			 * added to the virtual address of the segment. We just want the size here.
			 */
			ulSize = PAGE_ALIGN_UP(hdr.p_filesz);
			break;
		}
	}
#else
#error "BinaryManager::FindBinary() is not implemented on this OS"
#endif

	// Create a new Binary object and add it to the list
	CBinaryFile* binary = new CBinaryFile(ulModule, ulBase, ulSize);
	m_Binaries.push_front(binary);
	return binary;
}

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
CBinaryFile* FindBinary(char* szPath, bool bSrvCheck /* = true */, bool bCheckExtension /* = true */)
{
	return s_pBinaryManager->FindBinary(szPath, bSrvCheck, bCheckExtension);
}
