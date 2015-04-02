/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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
#include "modules/export_main.h"
#include "core/sp_main.h"

extern CSourcePython g_SourcePythonPlugin;


//-----------------------------------------------------------------------------
// Declare the _core module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_core)
{
	// Constants...
	_core.attr("SOURCE_ENGINE") = XSTRINGIFY(SOURCE_ENGINE);
	_core.attr("SOURCE_ENGINE_BRANCH") = XSTRINGIFY(SOURCE_ENGINE_BRANCH);

	// IServerPluginCallbacks, public IGameEventListener2, public IEntityListener
	class_<IEntityListener>("EntityListener");
	class_<CSourcePython, bases<IEntityListener>, boost::noncopyable>("SourcePython", no_init);

	_core.attr("_sp_plugin") = boost::ref(g_SourcePythonPlugin);
}
