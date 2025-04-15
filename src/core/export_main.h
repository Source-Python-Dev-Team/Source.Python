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
#ifndef _EXPORT_MAIN_H
#define _EXPORT_MAIN_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "boost/python.hpp"
#include "utilities/wrap_macros.h"

//---------------------------------------------------------------------------------
// Maximum number of modules we can have.
//---------------------------------------------------------------------------------
#define MAX_SOURCEPYTHON_MODULES 64

//---------------------------------------------------------------------------------
// Python init function typedef.
//---------------------------------------------------------------------------------
typedef void (*ModuleInitFn)( scope );

//---------------------------------------------------------------------------------
// This declares a Source.Python module.
//---------------------------------------------------------------------------------
#define DECLARE_SP_MODULE( name ) \
	void PyInit_##name( scope ); \
	static CSPModule g_##name##_Init(XSTRINGIFY(name), &PyInit_##name); \
	void PyInit_##name( scope name )

#define DECLARE_SP_SUBMODULE( package, name ) \
	void PyInit_##package##_##name( scope ); \
	static CSPModule g_##package##_##name##_Init( XSTRINGIFY(package.name), &PyInit_##package##_##name ); \
	void PyInit_##package##_##name( scope name )

//---------------------------------------------------------------------------------
// A deferred version of boost::python::dict. Thanks to spitice!
// There was a change in Python that results in an access violation when Python is
// not ready, yet.
//---------------------------------------------------------------------------------
class DeferredDict {
private:
	std::shared_ptr<dict> _pDict;
public:
	dict& get();
};

//---------------------------------------------------------------------------------
// This is a module definition structure. The way our module system is going to
// work is you declare a module via BOOST_PYTHON_MODULE. Then the CPythonManager
// class will iterate over the module list and add it to the import table.
//---------------------------------------------------------------------------------
struct SourcePythonModule_t
{
	char*		 szName;
	ModuleInitFn initFunc;
};

//---------------------------------------------------------------------------------
// Global module array.
//---------------------------------------------------------------------------------
extern SourcePythonModule_t g_SourcePythonModules[];

//---------------------------------------------------------------------------------
// This helper class will do some static initialization.
//---------------------------------------------------------------------------------
class CSPModule
{
	public:
		static int nextFreeModule;

	public:
		CSPModule(char* szName, ModuleInitFn initFunc) {
			if( nextFreeModule < MAX_SOURCEPYTHON_MODULES ) {
				g_SourcePythonModules[nextFreeModule].initFunc = (ModuleInitFn)initFunc;
				g_SourcePythonModules[nextFreeModule].szName = szName;

				nextFreeModule++;

				g_SourcePythonModules[nextFreeModule].initFunc = NULL;
				g_SourcePythonModules[nextFreeModule].szName = NULL;
			}
		}
};

//---------------------------------------------------------------------------------
// Exports everything we're exposing from boost to python.
//---------------------------------------------------------------------------------
extern bool modulsp_init( void );

#endif // _EXPORT_MAIN_H
