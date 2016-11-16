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
#include "export_main.h"
#include "sp_main.h"
#include "core.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern CSourcePython g_SourcePythonPlugin;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_source_python_plugin(scope);
static void export_constants(scope);
static void export_functions(scope);


//-----------------------------------------------------------------------------
// Declare the _core module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_core)
{
	export_source_python_plugin(_core);
	export_constants(_core);
	export_functions(_core);
}


//-----------------------------------------------------------------------------
// Expose CSourcePython.
//-----------------------------------------------------------------------------
void export_source_python_plugin(scope _core)
{
	class_<CSourcePython, bases<IEntityListener>, boost::noncopyable>("_SourcePythonPlugin", no_init);

	// Singleton...
	_core.attr("_sp_plugin") = boost::ref(g_SourcePythonPlugin);
}


//-----------------------------------------------------------------------------
// Expose constants.
//-----------------------------------------------------------------------------
void export_constants(scope _core)
{
	_core.attr("SOURCE_ENGINE") = XSTRINGIFY(SOURCE_ENGINE);
	_core.attr("SOURCE_ENGINE_BRANCH") = XSTRINGIFY(SOURCE_ENGINE_BRANCH);
}

//-----------------------------------------------------------------------------
// Expose functions.
//-----------------------------------------------------------------------------
void export_functions(scope _core)
{
	def(
		"console_message",
		&ConsoleMessage,
		"Output a message to the server console."
	);
}
