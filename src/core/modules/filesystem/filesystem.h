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

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "public/filesystem.h"


//---------------------------------------------------------------------------------
// SourceFile class.
//---------------------------------------------------------------------------------
class SourceFile
{
public:
	SourceFile(FileHandle_t handle);
	SourceFile(FileHandle_t handle, const char* mode);
	~SourceFile();

	// File-like methods
	PyObject*		Read(int size=-1);
	void			Write(PyObject* data);
	void			Close();
	void			Seek(int pos, int seekType=FILESYSTEM_SEEK_HEAD);
	unsigned int	Tell();
	unsigned int	Size();
	void			Flush();
	bool			IsAtty();
	bool			Seekable();
	int				Fileno();
	bool			Closed();
	bool			Readable();
	bool			Writeable();
	PyObject*		Readline(int size=-1);
	// TODO
	//__iter__
	// __next__
	// __enter__
	// __exit__
	list			Readlines(int hint=-1);
	void			Writelines(list lines);
	int				Truncate(int size=-1);

	// Others
	FileHandle_t	GetHandle();
	void			Save(const char* file_path);
	// TODO
	// void			Delete();
	// void			Rename();
	bool			EndOfFile();

	static SourceFile* Open(const char* pFileName, const char* pMode, const char* pathID=0);

private:
	PyObject*		ConsumeBuffer(char* buffer, int bytesRead);
	void			CheckClosed();
	void			WriteData(PyObject* data);
	bool			IsBinaryMode();
	void			CheckReadable();
	void			CheckWriteable();
	PyObject*		InternalReadline(bool binaryMode, int size, int& outBytesRead);

private:
	FileHandle_t m_handle;
	char* m_mode;
};


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool IsVPKFile(const char* file_path);

#endif // _FILESYSTEM_H
