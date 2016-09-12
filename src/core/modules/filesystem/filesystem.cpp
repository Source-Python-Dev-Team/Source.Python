/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
// Source.Python
#include "utilities/wrap_macros.h"
#include "filesystem.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool IsVPKFile(const char* file_path)
{
	struct stat buf;
	return stat(file_path, &buf) != 0 && filesystem->FileExists(file_path);
}


//---------------------------------------------------------------------------------
// SourceFile
//---------------------------------------------------------------------------------
SourceFile::SourceFile(FileHandle_t handle)
{
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Handle is invalid.")

	m_handle = handle;
	m_mode = NULL;
}

SourceFile::SourceFile(FileHandle_t handle, const char* mode)
{
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Handle is invalid.")

	m_handle = handle;
	m_mode = strdup(mode);
}

SourceFile::~SourceFile()
{
	if (m_mode)
		free(m_mode);
}

void SourceFile::Save(const char* file_path)
{
	CheckClosed();
	FileHandle_t handle = filesystem->Open(file_path, "wb");
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_IOError, "Failed to open file: %s", file_path)

	int size = Size();
	void* buffer = new char[size];
	int bytesRead = filesystem->Read(buffer, size, m_handle);

	filesystem->Write(buffer, bytesRead, handle);
	filesystem->Close(handle);

	delete buffer;
}

PyObject* SourceFile::Read(int size)
{
	CheckClosed();
	if (size == -1) {
		size = filesystem->Size(m_handle);
	}

	void* pOutput = new char[size+1];
	int bytesRead = filesystem->Read(pOutput, size, m_handle);
	return ConsumeBuffer(pOutput, bytesRead);
}

object SourceFile::Readline(int size)
{
	CheckClosed();
	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
	return object();
}

list SourceFile::Readlines(int hint)
{
	CheckClosed();
	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
	return list();
}

PyObject* SourceFile::ConsumeBuffer(void* buffer, int bytesRead)
{	
	PyObject* result = NULL;
	if (IsBinaryMode()) {
		result = PyBytes_FromStringAndSize((char*) buffer, bytesRead);
	}
	else {
		result = PyUnicode_FromStringAndSize((char*) buffer, bytesRead);
	}

	delete buffer;
	return result;
}

int SourceFile::Write(object data)
{
	CheckClosed();
	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
	return 0;
}

void SourceFile::Writelines(list lines)
{
	CheckClosed();
	for (int i=0; i < len(lines); ++i) {
		Writeline(lines[i]);
	}
}

bool SourceFile::IsBinaryMode()
{
	return !m_mode || strchr(m_mode, 'b');
}

void SourceFile::Writeline(object line)
{
	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
}

int SourceFile::Truncate(int size)
{
	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
	return 0;
}

void SourceFile::Close()
{
	if (m_handle != FILESYSTEM_INVALID_HANDLE) {
		filesystem->Close(m_handle);
		m_handle = NULL;
	}
}

void SourceFile::Seek(int pos, FileSystemSeek_t seekType)
{
	CheckClosed();
	filesystem->Seek(m_handle, pos, seekType);
}

unsigned int SourceFile::Tell()
{
	CheckClosed();
	return filesystem->Tell(m_handle);
}

unsigned int SourceFile::Size()
{
	CheckClosed();
	return filesystem->Size(m_handle);
}

void SourceFile::Flush()
{
	CheckClosed();
	filesystem->Flush(m_handle);
}

FileHandle_t SourceFile::GetHandle()
{
	return m_handle;
}

bool SourceFile::IsAtty()
{
	CheckClosed();
	return false;
}

bool SourceFile::Seekable()
{
	CheckClosed();
	return true;
}

int SourceFile::Fileno()
{
	CheckClosed();
	BOOST_RAISE_EXCEPTION(PyExc_IOError, "File does not use a file descriptor.")
	return 0;
}

bool SourceFile::Closed()
{
	return m_handle == NULL;
}

bool SourceFile::Readable()
{
	return m_mode != NULL && (
		strchr(m_mode, 'r') 
		|| strchr(m_mode, '+')
	);
}

bool SourceFile::Writeable()
{
	return m_mode != NULL && (
		strchr(m_mode, 'w') 
		|| strchr(m_mode, 'x') 
		|| strchr(m_mode, 'a') 
		|| strchr(m_mode, '+')
	);
}

void SourceFile::CheckClosed()
{
	if (m_handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "File is already closed.")
}

SourceFile* SourceFile::Open(const char* pFileName, const char* pMode, const char* pathID)
{
	FileHandle_t handle = filesystem->Open(pFileName, pMode, pathID);
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to open file: %s", pFileName)

	return new SourceFile(handle, pMode);
}
