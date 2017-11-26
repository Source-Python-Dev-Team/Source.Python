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
#ifndef _LOADER_DEFINITIONS_H
#define _LOADER_DEFINITIONS_H

//---------------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------------
#define PYLIB_NAME_WIN32	"Python3/plat-win/python36.dll"
#define PYLIB_NAME_LINUX	"Python3/plat-linux/libpython3.6m.so.1.0"

#define CORE_NAME_WIN32		"bin/core.dll"
#define CORE_NAME_LINUX		"bin/core.so"

#if defined(_WIN32)
#	define CORE_NAME  CORE_NAME_WIN32
#else
#	define CORE_NAME  CORE_NAME_LINUX
#endif

#if defined(_WIN32)
#	define PYLIB_NAME PYLIB_NAME_WIN32
#	define VCRUNTIME_LIB "Python3/plat-win/vcruntime140.dll"
#elif defined(LINUX)
#	define PYLIB_NAME PYLIB_NAME_LINUX
#endif

#define MAX_PATH_LENGTH 1024
#define MAX_ERROR_LENGTH 1024
#define MSG_PREFIX "[Source.Python] "

#endif // _LOADER_DEFINITIONS_H
