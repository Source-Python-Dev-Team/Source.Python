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
*
* Special thanks to Mark Chandler <mark@moddb.com>!
* Sources:
* http://code.google.com/p/sourcesdkpython/source/browse/trunk/src/game/server/py/ge_pymanager.cpp
*/

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "sp_python.h"
#include "sp_main.h"
#include "eiface.h"
#include "utilities/shared_utils.h"
#include "export_main.h"
#include "modules/entities/entities_entity.h"
#include "icommandline.h"


//---------------------------------------------------------------------------------
// Interfaces we're going to use.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;


//---------------------------------------------------------------------------------
// Singleton
//---------------------------------------------------------------------------------
CPythonManager g_PythonManager;


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void InitConverters();


//---------------------------------------------------------------------------------
// Returns the path to the ../source-python/addon/ directory.
//---------------------------------------------------------------------------------
const char *GetSourcePythonDir()
{
	static char szGameDir[MAX_PATH_LENGTH];
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);
	GenerateSymlink(szGameDir);
	static char szAddonDir[MAX_PATH_LENGTH];
	V_snprintf(szAddonDir, MAX_PATH_LENGTH, "%s%s", szGameDir, "/addons/source-python");
	return szAddonDir;
}


//---------------------------------------------------------------------------------
// Adds a path to sys.path (relative to GetSourcePythonDir()).
//---------------------------------------------------------------------------------
void AddToSysPath( const char* path )
{
	char szFolderPath[MAX_PATH_LENGTH];
	V_snprintf(szFolderPath, MAX_PATH_LENGTH, "%s%s", GetSourcePythonDir(), path);
	V_FixSlashes(szFolderPath);

	DevMsg(1, MSG_PREFIX "Adding %s to path\n", szFolderPath);
	std::string szCommandString = "sys.path.append(r\"" + std::string(szFolderPath) + "\")";
	PyRun_SimpleString(szCommandString.c_str());
}


//---------------------------------------------------------------------------------
// Initializes python.
//---------------------------------------------------------------------------------
bool CPythonManager::Initialize( void )
{
	// Construct a path to the python engine directory.
	char szPythonHome[MAX_PATH_LENGTH];
	V_snprintf(szPythonHome, MAX_PATH_LENGTH, "%s/Python3", GetSourcePythonDir());
	V_FixSlashes(szPythonHome);
	DevMsg(1, MSG_PREFIX "Python home path set to %s\n", szPythonHome);

	// Convert to wide char for python.
	wchar_t wszPythonHome[MAX_PATH_LENGTH];
	V_strtowcs(szPythonHome, -1, wszPythonHome, MAX_PATH_LENGTH);

	// Get the full path to the shared python library
	char szProgramName[MAX_PATH_LENGTH];
	V_snprintf(szProgramName, MAX_PATH_LENGTH, "%s/%s", GetSourcePythonDir(), PYLIB_NAME);
	V_FixSlashes(szProgramName);
	DevMsg(1, MSG_PREFIX "sys.executable set to %s\n", szProgramName);

	// Convert to wide char for python.
	wchar_t wszProgramName[MAX_PATH_LENGTH];
	V_strtowcs(szProgramName, -1, wszProgramName, MAX_PATH_LENGTH);

	// Set that as the python home directory.
 	Py_SetPythonHome(wszPythonHome);
 	Py_SetProgramName(wszProgramName);
	Py_SetPath(wszPythonHome);

	// Initialize python and its namespaces.
	Py_Initialize();

	// Print some information
	DevMsg(1, MSG_PREFIX "Python version %s initialized!\n", Py_GetVersion());
	
	// Set sys.argv and update sys.path
	DevMsg(1, MSG_PREFIX "Setting sys.argv...\n");
	ICommandLine* pCommandLine = CommandLine();

	int iParamCount = pCommandLine->ParmCount();
	wchar_t** argv = new wchar_t*[iParamCount];
	for (int i=0; i < iParamCount; i++)
	{
		const char* szParam = pCommandLine->GetParm(i);
		int iParamLength = strlen(szParam);

		wchar_t* wszParam = new wchar_t[iParamLength+1];
		// Not sure what's wrong with V_strtowcs, but it seems like it
		// doesn't convert the string correctly on Linux
		mbstowcs(wszParam, szParam, iParamLength+1);

		argv[i] = wszParam;
	}
	PySys_SetArgv(iParamCount, argv);

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

	// Initialize all converters
	InitConverters();

	// Initialize all submodules
	if (!modulsp_init())
	{
		Msg(MSG_PREFIX "Failed to initialize internal modules.\n");
		return false;
	}

// Patch for issues #175.
// For unknown reasons the streams might be unable to connect due to io.OpenWrapper failing
//	to get their handle from their fileno and internally raise an "OSError":
//
//	[WinError 6] The handle is invalid"...
//
// Same error occurs when trying to retrieve a python file object from their descriptor
//	using PyFile_FromFd. This is kinda weird since GetStdHandle is returning valid handles for them
//	but oh well... reconnecting them seems to fix the issues from my testings.
#ifdef _WIN32
	// This fix currently only works for dedicated servers, but crashes on listen servers.
	if (engine->IsDedicatedServer()) {
		object sys = python::import("sys");
		object io_open = python::import("io").attr("open");
	
		object stdin_ = sys.attr("stdin");
		if (stdin_.is_none())
		{
			DevMsg(1, MSG_PREFIX "stdin is None... reconnecting.\n");
			sys.attr("stdin") = sys.attr("__stdin__") = io_open("CONIN$", "rt");
		}

		object stdout_ = sys.attr("stdout");
		if (stdout_.is_none())
		{
			DevMsg(1, MSG_PREFIX "stdout is None... reconnecting.\n");
			sys.attr("stdout") = sys.attr("__stdout__") = io_open("CONOUT$", "wt");
		}

		object stderr_ = sys.attr("stderr");
		if (stderr_.is_none())
		{
			DevMsg(1, MSG_PREFIX "stderr is None... reconnecting.\n");

			// Use CONOUT$, because CONERR$ has no effect:
			// https://github.com/Source-Python-Dev-Team/Source.Python/issues/237
			sys.attr("stderr") = sys.attr("__stderr__") = io_open("CONOUT$", "wt");
		}
	}
#endif

	// Import the main module file.
	DevMsg(1, MSG_PREFIX "Loading main module...\n");

	try {
		python::import("__init__").attr("load")();
	}
	catch( ... ) {
		Msg(MSG_PREFIX "Failed to load the main module due to following exception:\n");

		// Don't use PyErr_Print() here because our sys.excepthook (might) has not been installed
		// yet so let's just format and output to the console ourself.
		if (PyErr_Occurred())
		{
			PyObject *pType;
			PyObject *pValue;
			PyObject *pTraceback;
			PyErr_Fetch(&pType, &pValue, &pTraceback);
			PyErr_NormalizeException(&pType, &pValue, &pTraceback);

			handle<> hType(pType);
			handle<> hValue(allow_null(pValue));
			handle<> hTraceback(allow_null(pTraceback));

			object format_exception = import("traceback").attr("format_exception");
			const char* pMsg = extract<const char *>(str("\n").join(format_exception(hType, hValue, hTraceback)));

			// Send the message in chunks, because it can get quite big.
			ChunkedMsg(pMsg);

			PyErr_Clear();
		}

		return false;
	}

	return true;
}


//---------------------------------------------------------------------------------
// Shuts down python.
//---------------------------------------------------------------------------------
bool CPythonManager::Shutdown( void )
{
	DevMsg(1, MSG_PREFIX "Unloading main module...\n");
	try {
		python::import("__init__").attr("unload")();
	}
	catch( ... ) {
		PyErr_Print();
		PyErr_Clear();
		Msg(MSG_PREFIX "Failed to unload the main module.\n");
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------------
// Converters
//---------------------------------------------------------------------------------
// string_t
struct string_t_to_python
{
	string_t_to_python()
	{
		to_python_converter< string_t, string_t_to_python >();
	}

	static PyObject* convert(string_t const& s)
	{
		return incref(object(s.ToCStr()).ptr());
	}
};

// CBaseEntity*
struct baseentity_to_python
{
	baseentity_to_python()
	{
		to_python_converter< CBaseEntity*, baseentity_to_python >();
	}

	static PyObject* convert(CBaseEntity* pAddr)
	{
		return incref(object(CBaseEntityWrapper::wrap(pAddr)).ptr());
	}
};

struct baseentity_from_python
{
	baseentity_from_python()
	{
		boost::python::converter::registry::insert(
			&convert,
			boost::python::type_id<CBaseEntity>()
		);
	}

	static void* convert(PyObject* obj)
	{
		CBaseEntityWrapper* pAddr = extract<CBaseEntityWrapper*>(obj);
		return (void *) pAddr;
	}
};

// void*
struct void_ptr_to_python
{
	void_ptr_to_python()
	{
		to_python_converter< void*, void_ptr_to_python >();
	}

	static PyObject* convert(void* pPtr)
	{
		return incref(object(CPointer((unsigned long) pPtr)).ptr());
	}
};

struct void_ptr_from_python
{
	void_ptr_from_python()
	{
		boost::python::converter::registry::insert(
			&convert,
			boost::python::type_id<void>()
		);
	}

	static void* convert(PyObject* obj)
	{
		CPointer* pAddr = extract<CPointer*>(obj);
		return (void *) pAddr->m_ulAddr;
	}
};

// unsigned char*
struct unsigned_char_ptr_to_python
{
	unsigned_char_ptr_to_python()
	{
		to_python_converter< unsigned char*, unsigned_char_ptr_to_python >();
	}

	static PyObject* convert(unsigned char* pPtr)
	{
		return incref(object(CPointer((unsigned long) pPtr)).ptr());
	}
};

//---------------------------------------------------------------------------------
// Initializes all converters
//---------------------------------------------------------------------------------
void InitConverters()
{
	string_t_to_python();

	baseentity_to_python();
	baseentity_from_python();
	
	void_ptr_to_python();
	void_ptr_from_python();
	
	unsigned_char_ptr_to_python();
}
