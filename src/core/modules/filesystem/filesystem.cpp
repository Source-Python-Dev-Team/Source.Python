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
#include "modules/memory/memory_alloc.h"
#include "utilities/wrap_macros.h"
#include "filesystem.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;

// Chunk size for InternalReadline()
#define CHUNK_SIZE 128


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool IsVPKFile(const char* file_path)
{
	struct stat buf;
	return stat(file_path, &buf) != 0 && filesystem->FileExists(file_path);
}


//---------------------------------------------------------------------------------
// SourceFile - constructor/destructor
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

SourceFile* SourceFile::Open(const char* pFileName, const char* pMode, const char* pathID)
{
	FileHandle_t handle = filesystem->Open(pFileName, pMode, pathID);
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to open file: %s", pFileName)

	return new SourceFile(handle, pMode);
}


//---------------------------------------------------------------------------------
// SourceFile - reading
//---------------------------------------------------------------------------------
PyObject* SourceFile::Read(int size)
{
	CheckClosed();
	CheckReadable();
	if (size == -1) {
		size = filesystem->Size(m_handle);
	}

	char* pOutput = new char[size+1];
	int bytesRead = filesystem->Read(pOutput, size, m_handle);
	return ConsumeBuffer(pOutput, bytesRead);
}

PyObject* SourceFile::Readline(int size)
{
	CheckClosed();
	CheckReadable();

	int bytesRead = 0;
	return InternalReadline(IsBinaryMode(), size, bytesRead);
}

PyObject* SourceFile::InternalReadline(bool binaryMode, int size, int& outBytesRead)
{
	int bytesPut = 0;
	char* buffer = (char*) UTIL_Alloc(CHUNK_SIZE);

	while (size < 0 || bytesPut < size) {
		char temp[1];

		// EOF?
		if (filesystem->Read(temp, 1, m_handle) != 1) {
			break;
		}

		// Ignore \r in text mode
		if (!binaryMode && temp[0] == '\r') {
			continue;
		}
		
		if (temp[0] == '\0') {
			buffer[bytesPut] = '\n';
		}
		else {
			buffer[bytesPut] = temp[0];
		}
		++bytesPut;
		++outBytesRead;

		if (temp[0] == '\n') {
			break;
		}

		if (bytesPut >= CHUNK_SIZE) {
			buffer = (char*) UTIL_Realloc(buffer, bytesPut + CHUNK_SIZE);
			if (buffer == NULL)
				BOOST_RAISE_EXCEPTION(PyExc_MemoryError, "Failed to reallocate the buffer.")
		}
	}
	
	PyObject* result = NULL;
	if (IsBinaryMode()) {
		result = PyBytes_FromStringAndSize(buffer, bytesPut);
	}
	else {
		result = PyUnicode_FromStringAndSize(buffer, bytesPut);
	}

	return result;
}

list SourceFile::Readlines(int hint)
{
	CheckClosed();
	CheckReadable();

	bool binaryMode = IsBinaryMode();

	list result;

	int bytesRead = 0;
	while (true) {
		PyObject* line = InternalReadline(binaryMode, -1, bytesRead);
		result.append(handle<>(borrowed(line)));
		if (EndOfFile() ||  (hint > 0 && bytesRead > hint)) {
			break;
		}
	}

	return result;
}

// internal
PyObject* SourceFile::ConsumeBuffer(char* buffer, int bytesRead)
{
	PyObject* result = NULL;
	if (IsBinaryMode()) {
		result = PyBytes_FromStringAndSize(buffer, bytesRead);
	}
	else {
		result = PyUnicode_FromStringAndSize(buffer, bytesRead);
	}

	delete buffer;
	return result;
}


//---------------------------------------------------------------------------------
// SourceFile - writing
//---------------------------------------------------------------------------------
void SourceFile::Write(PyObject* data)
{
	CheckClosed();
	CheckWriteable();

	WriteData(data);
}

void SourceFile::Writelines(list lines)
{
	CheckClosed();
	CheckWriteable();

	for (int i=0; i < len(lines); ++i) {
		object data = lines[i];
		WriteData(data.ptr());
	}
}

// internal
void SourceFile::WriteData(PyObject* data)
{	
	char* input = NULL;
	int size = 0;

	if (IsBinaryMode()) {
		if (!PyBytes_Check(data)) {
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "a bytes-like object is required, not '%s'", data->ob_type->tp_name)
		}

		input = PyBytes_AsString(data);
		size = PyBytes_Size(data);
	}
	else {
		if (!PyUnicode_Check(data)) {
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "write() argument must be str, not %s", data->ob_type->tp_name)
		}

		size = PyUnicode_GetLength(data);
		input = (char *) PyUnicode_DATA(data);
	}

	filesystem->Write((void *) input, size, m_handle);
}

int SourceFile::Truncate(int size)
{
	CheckClosed();
	CheckWriteable();

	// TODO
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented yet.")
	return 0;
}


//---------------------------------------------------------------------------------
// SourceFile - misc
//---------------------------------------------------------------------------------
void SourceFile::Save(const char* file_path)
{
	CheckClosed();
	FileHandle_t handle = filesystem->Open(file_path, "wb");
	if (handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_IOError, "Failed to open file: %s", file_path)

	int size = Size();
	void* buffer = new char[size+1];
	int bytesRead = filesystem->Read(buffer, size, m_handle);

	filesystem->Write(buffer, bytesRead, handle);
	filesystem->Close(handle);

	delete buffer;
}

void SourceFile::Close()
{
	if (m_handle != FILESYSTEM_INVALID_HANDLE) {
		filesystem->Close(m_handle);
		m_handle = NULL;
	}
}

void SourceFile::Seek(int pos, int seekType)
{
	CheckClosed();
	filesystem->Seek(m_handle, pos, (FileSystemSeek_t) seekType);
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

bool SourceFile::EndOfFile()
{
	return filesystem->EndOfFile(m_handle);
}


//---------------------------------------------------------------------------------
// SourceFile - internal
//---------------------------------------------------------------------------------
void SourceFile::CheckReadable()
{
	if (!Readable()) {
		BOOST_RAISE_EXCEPTION(PyExc_IOError, "not readable")
	}
}

void SourceFile::CheckWriteable()
{
	if (!Writeable()) {
		BOOST_RAISE_EXCEPTION(PyExc_IOError, "not writeable")
	}
}

void SourceFile::CheckClosed()
{
	if (m_handle == FILESYSTEM_INVALID_HANDLE)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "File is already closed.")
}

bool SourceFile::IsBinaryMode()
{
	return !m_mode || strchr(m_mode, 'b');
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
