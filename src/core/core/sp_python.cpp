/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
*
* Special thanks to Mark Chandler <mark@moddb.com>!
* Sources:
* http://code.google.com/p/sourcesdkpython/source/browse/trunk/src/game/server/py/ge_pymanager.cpp
*/

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "sp_python.h"
#include "sp_gamedir.h"
#include "eiface.h"
#include "modules/export_main.h"

//---------------------------------------------------------------------------------
// Interfaces we're going to use.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;

//---------------------------------------------------------------------------------
// Singleton
//---------------------------------------------------------------------------------
CPythonManager g_PythonManager;

//---------------------------------------------------------------------------------
// This is externed because we need to manually import ES's core library into the
// python namespace.
//---------------------------------------------------------------------------------
// PyMODINIT_FUNC PyInit_sp( void );

//---------------------------------------------------------------------------------
// Adds a path to sys.path (relative to g_GamePaths.GetSPDir()).
//---------------------------------------------------------------------------------
void AddToSysPath( const char* path )
{
	char szFolderPath[MAX_GAME_PATH];
	V_snprintf(szFolderPath, MAX_GAME_PATH, "%s%s", g_GamePaths.GetSPDir(), path);
	V_FixSlashes(szFolderPath);

	DevMsg(1, "[SP] Adding %s to path\n", szFolderPath);
	std::string szCommandString = "sys.path.append(r\"" + std::string(szFolderPath) + "\")";
	PyRun_SimpleString(szCommandString.c_str());
}

//---------------------------------------------------------------------------------
// Initializes python.
//---------------------------------------------------------------------------------
bool CPythonManager::Initialize( void )
{
	// Construct a path to the python engine directory.
	char szPythonHome[MAX_GAME_PATH];
	V_snprintf(szPythonHome, MAX_GAME_PATH, "%s/Python3", g_GamePaths.GetSPDir());
	V_FixSlashes(szPythonHome);
	DevMsg(1, "[SP] Python home path set to %s\n", szPythonHome);

	// Convert to wide char for python.
	wchar_t wszPythonHome[1024];
	V_strtowcs(szPythonHome, -1, wszPythonHome, 1024);

	// Set that as the python home directory.
 	Py_SetPythonHome(wszPythonHome);
 	Py_SetProgramName(wszPythonHome);
	Py_SetPath(wszPythonHome);

	// Initialize python and its namespaces.
	Py_Initialize();

	// Print some information
	DevMsg(1, "Python version %s initialized!\n", Py_GetVersion());

	// Make sure sys is imported.
	PyRun_SimpleString("import sys");

	// Add the Python API path.
	AddToSysPath("/packages/source-python");

	// Add operating system specific paths.
#if defined(WIN32)
	AddToSysPath("/Python3/plat-win");
#else
	AddToSysPath("/Python3/plat-linux");

	// We've got a bunch of linux shared objects here we need to load.
	AddToSysPath("/Python3/lib-dynload");
#endif

	// Site packages for any extra packages...
	AddToSysPath("/packages/site-packages");

	// Add the custom packages path.
	AddToSysPath("/packages/custom");

	// And of course, the plugins directory for script imports.
	AddToSysPath("/plugins");

	// Initialize all submodules
	modulsp_init();

	// Import the main module file.
	DevMsg(1, "[SP] Importing main module..\n");
	BEGIN_BOOST_PY()

		python::import("__init__");

	END_BOOST_PY_NORET(); // Noret because we have more stuff to do after this import.

	DevMsg(0, "[Source.Python] Loaded successfully.\n");

	return true;
}

//---------------------------------------------------------------------------------
// Shuts down python.
//---------------------------------------------------------------------------------
bool CPythonManager::Shutdown( void )
{
	return true;
}
