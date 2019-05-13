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
// Includes.
//-----------------------------------------------------------------------------
#include "core.h"
#include "export_main.h"


void ConsoleMessage(const char* msg)
{
	char* pMsg = (char*) msg;
	int iLen = strlen(msg);

	while(iLen > 0) {
		ConMsg("%s", pMsg);
		pMsg += MAX_CON_MSG-1;
		iLen -= MAX_CON_MSG-1;
	}
}

void* GetInterface(const char* library, const char* interface_name)
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

list GetCoreModules()
{
	list result;

	for (int i=0; i < MAX_SOURCEPYTHON_MODULES; ++i)
	{
		const char* module = g_SourcePythonModules[i].szName;
		if (!module)
			break;

		result.append(module);
	}

	return result;
}
