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

#ifndef _CORE_H
#define _CORE_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dbg.h"
#include "interface.h"
#include "dynload.h"

// Maximum console message size including null terminating char
#define MAX_CON_MSG 1024


//-----------------------------------------------------------------------------
// MessageSeverity
//-----------------------------------------------------------------------------
enum MessageSeverity
{
	SEVERITY_MESSAGE = 0,
	SEVERITY_WARNING,
	SEVERITY_ASSERT,
	SEVERITY_ERROR,
	SEVERITY_LOG,
};


//-----------------------------------------------------------------------------
// OutputReturn
//-----------------------------------------------------------------------------
enum OutputReturn
{
	OUTPUT_BLOCK = 0,
	OUTPUT_CONTINUE
};


//-----------------------------------------------------------------------------
// ConMsg wrapper
//-----------------------------------------------------------------------------
inline void ConsoleMessage(const char* msg)
{
	char* pMsg = (char*) msg;
	int iLen = strlen(msg);

	while(iLen > 0) {
		ConMsg(pMsg);
		pMsg += MAX_CON_MSG-1;
		iLen -= MAX_CON_MSG-1;
	}
}

inline void* GetInterface(const char* library, const char* interface_name)
{
	DLLib* lib = dlLoadLibrary(library);
	if (!lib)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to load library '%s'.", library)

	CreateInterfaceFn pCreateInterface = (CreateInterfaceFn) dlFindSymbol(lib, CREATEINTERFACE_PROCNAME);
	dlFreeLibrary(lib);

	if (!pCreateInterface)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve interface function '%s'.", CREATEINTERFACE_PROCNAME)

	int return_code;
	void* result = pCreateInterface(interface_name, &return_code);
	if (!result)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find interface '%s'. Return code: %i.", interface_name, return_code)

	return result;
}

#endif // _CORE_H
