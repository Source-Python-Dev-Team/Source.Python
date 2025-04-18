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
void EnableDictTraversal();


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
bool AddToSysPath( PyConfig& config, const char* path )
{
	PyStatus status;
	char szFolderPath[MAX_PATH_LENGTH];
	wchar_t wszFolderPath[MAX_PATH_LENGTH];

	V_snprintf(szFolderPath, MAX_PATH_LENGTH, "%s%s", GetSourcePythonDir(), path);
	V_FixSlashes(szFolderPath);
	V_strtowcs(szFolderPath, -1, wszFolderPath, MAX_PATH_LENGTH);

	DevMsg(1, MSG_PREFIX "Adding %s to path...\n", szFolderPath);
	config.module_search_paths_set = 1;
	status = PyWideStringList_Append(&config.module_search_paths, wszFolderPath);
	if (PyStatus_Exception(status)) {
		Msg(MSG_PREFIX "Failed to add %s to sys.path.\n", szFolderPath);
		PyConfig_Clear(&config);
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------------
// Initializes python.
//---------------------------------------------------------------------------------
bool CPythonManager::Initialize( void )
{
	char szPythonHome[MAX_PATH_LENGTH];
	wchar_t wszPythonHome[MAX_PATH_LENGTH];
	char szProgramName[MAX_PATH_LENGTH];
	wchar_t wszProgramName[MAX_PATH_LENGTH];
	PyStatus status;
	PyConfig config;

	V_snprintf(szPythonHome, MAX_PATH_LENGTH, "%s/Python3", GetSourcePythonDir());
	V_FixSlashes(szPythonHome);
	V_strtowcs(szPythonHome, -1, wszPythonHome, MAX_PATH_LENGTH);
	DevMsg(1, MSG_PREFIX "Python home path set to %s.\n", szPythonHome);

	// Get the full path to the shared python library
	V_snprintf(szProgramName, MAX_PATH_LENGTH, "%s/%s", GetSourcePythonDir(), PYLIB_NAME);
	V_FixSlashes(szProgramName);
	V_strtowcs(szProgramName, -1, wszProgramName, MAX_PATH_LENGTH);
	DevMsg(1, MSG_PREFIX "sys.executable set to %s\n", szProgramName);

	PyConfig_InitIsolatedConfig(&config);

	status = PyConfig_SetString(&config, &config.program_name, wszProgramName);
	if (PyStatus_Exception(status)) {
		Msg(MSG_PREFIX "Failed to set Python program name.\n");
		PyConfig_Clear(&config);
		return false;
	}

	status = PyConfig_SetString(&config, &config.home, wszPythonHome);
	if (PyStatus_Exception(status)) {
		Msg(MSG_PREFIX "Failed to set Python home.\n");
		PyConfig_Clear(&config);
		return false;
	}

	if (!AddToSysPath(config, "/Python3"))
		return false;

	if (!AddToSysPath(config, "/packages/source-python"))
		return false;

	// Add operating system specific paths.
#if defined(WIN32)
	if (!AddToSysPath(config, "/Python3/plat-win"))
		return false;
#else
	if (!AddToSysPath(config, "/Python3/plat-linux"))
		return false;

	if (!AddToSysPath(config, "/Python3/lib-dynload"))
		return false;
#endif

	if (!AddToSysPath(config, "/packages/site-packages"))
		return false;

	if (!AddToSysPath(config, "/packages/custom"))
		return false;

	if (!AddToSysPath(config, "/plugins"))
		return false;

	// Set sys.argv
	DevMsg(1, MSG_PREFIX "Setting sys.argv...\n");
	ICommandLine* pCommandLine = CommandLine();

	int iParamCount = pCommandLine->ParmCount();
	wchar_t** argv = new wchar_t* [iParamCount];
	for (int i = 0; i < iParamCount; i++)
	{
		const char* szParam = pCommandLine->GetParm(i);
		int iParamLength = strlen(szParam);

		wchar_t* wszParam = new wchar_t[iParamLength + 1];
		// Not sure what's wrong with V_strtowcs, but it seems like it
		// doesn't convert the string correctly on Linux
		mbstowcs(wszParam, szParam, iParamLength + 1);

		argv[i] = wszParam;
	}

	status = PyConfig_SetArgv(&config, iParamCount, argv);
	if (PyStatus_Exception(status)) {
		Msg(MSG_PREFIX "Failed to set sys.argv.\n");
		return false;
	}

	status = Py_InitializeFromConfig(&config);
	PyConfig_Clear(&config);
	if (PyStatus_Exception(status)) {
		Msg(MSG_PREFIX "Failed to initialize Python.\n");
		return false;
	}

	// Print some information
	DevMsg(1, MSG_PREFIX "Python version %s initialized!\n", Py_GetVersion());

	// Enable circular references traversal
	EnableDictTraversal();

	// Initialize all converters
	InitConverters();

	// Initialize all submodules
	if (!modulsp_init())
	{
		Msg(MSG_PREFIX "Failed to initialize internal modules.\n");
		PrintCurrentException();
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

		// Only reconnect the streams if the server was launched with a console (issue #392).
		if (pCommandLine->FindParm("-console")) {
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
		PrintCurrentException();

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
		Msg(MSG_PREFIX "Failed to unload the main module.\n");
		PrintCurrentException();
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------------
// Circular references traversal
//---------------------------------------------------------------------------------
struct dict_traversal
{
	static int is_gc(PyObject *self)
	{
		return !!downcast<objects::instance<> >(self)->dict;
	}

	static int traverse(PyObject *self, visitproc visit, void *arg)
	{
		Py_VISIT(downcast<objects::instance<> >(self)->dict);
		return 0;
	}

	static int clear(PyObject *self)
	{
		Py_CLEAR(downcast<objects::instance<> >(self)->dict);
		return 0;
	}
};

void EnableDictTraversal()
{
	PyTypeObject *type = objects::class_type().get();
	type->tp_flags |= Py_TPFLAGS_HAVE_GC;
	type->tp_is_gc = dict_traversal::is_gc;
	type->tp_traverse = dict_traversal::traverse;
	type->tp_clear = dict_traversal::clear;
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

struct baseentity_index_from_python
{
	baseentity_index_from_python()
	{
		boost::python::converter::registry::insert(
			&convert,
			boost::python::type_id<CBaseEntity>()
		);

		boost::python::converter::registry::insert(
			&convert,
			boost::python::type_id<CBaseEntityWrapper>()
		);
	}

	static void* convert(PyObject* obj)
	{
		extract<unsigned int> extractor(obj);
		if (!extractor.check()) {
			return NULL;
		}

		return (void *)ExcBaseEntityFromIndex(extractor());
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
	baseentity_index_from_python();
	
	void_ptr_to_python();
	void_ptr_from_python();
	
	unsigned_char_ptr_to_python();
}
