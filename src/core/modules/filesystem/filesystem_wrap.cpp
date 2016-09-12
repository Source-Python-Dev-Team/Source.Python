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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "filesystem.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_functions(scope);
static void export_source_file(scope);
static void export_seek_type(scope);


//-----------------------------------------------------------------------------
// Declare the _filesystem module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_filesystem)
{
	export_functions(_filesystem);
	export_seek_type(_filesystem);
	export_source_file(_filesystem);
}


//-----------------------------------------------------------------------------
// Exports functions
//-----------------------------------------------------------------------------
void export_functions(scope _filesystem)
{
	def("is_vpk_file", &IsVPKFile);
}


//-----------------------------------------------------------------------------
// Exports SourceFile
//-----------------------------------------------------------------------------
void export_source_file(scope _filesystem)
{
	class_<SourceFile> _SourceFile("SourceFile", init<FileHandle_t>());

	// File-like methods
	_SourceFile.def(
		"save",
		&SourceFile::Save,
		(arg("file_path"))
	);

	_SourceFile.def(
		"read",
		&SourceFile::Read,
		(arg("size")=-1)
	);

	_SourceFile.def(
		"readline",
		&SourceFile::Readline,
		(arg("size")=-1)
	);

	_SourceFile.def(
		"readlines",
		&SourceFile::Readlines,
		(arg("hint")=-1)
	);

	_SourceFile.def(
		"write",
		&SourceFile::Write
	);

	_SourceFile.def(
		"writelines",
		&SourceFile::Writelines
	);

	_SourceFile.def(
		"truncate",
		&SourceFile::Truncate,
		(arg("size")=-1)
	);

	_SourceFile.def(
		"close", 
		&SourceFile::Close
	);

	_SourceFile.def(
		"seek", 
		&SourceFile::Seek,
		(arg("offset"), arg("whence")=FILESYSTEM_SEEK_HEAD)
	);

	_SourceFile.def(
		"tell", 
		&SourceFile::Tell
	);

	_SourceFile.def(
		"flush", 
		&SourceFile::Flush
	);

	_SourceFile.def(
		"isatty", 
		&SourceFile::IsAtty
	);

	_SourceFile.def(
		"seekable",
		&SourceFile::Seekable
	);

	_SourceFile.def(
		"fileno",
		&SourceFile::Fileno
	);

	_SourceFile.def(
		"readable",
		&SourceFile::Readable
	);

	_SourceFile.def(
		"writeable",
		&SourceFile::Writeable
	);

	_SourceFile.add_property(
		"closed",
		&SourceFile::Closed
	);

	// Others
	_SourceFile.add_property(
		"size",
		&SourceFile::Size
	);

	_SourceFile.add_property(
		"handle",
		make_function(&SourceFile::GetHandle, return_by_value_policy())
	);

	_SourceFile.add_property(
		"eof",
		&SourceFile::EndOfFile
	);
	
	_SourceFile.def(
		"open",
		&SourceFile::Open,
		(arg("file_path"), arg("mode")="rt", arg("path_id")=object()),
		manage_new_object_policy()
	).staticmethod("open");
}

void export_seek_type(scope _filesystem)
{
	enum_<FileSystemSeek_t> SeekType("SeekType");

	SeekType.value("HEAD", FILESYSTEM_SEEK_HEAD);
	SeekType.value("CURRENT", FILESYSTEM_SEEK_CURRENT);
	SeekType.value("TAIL", FILESYSTEM_SEEK_TAIL);
}
