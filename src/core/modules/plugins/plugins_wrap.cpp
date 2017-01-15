/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2017 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "export_main.h"
#include "modules/memory/memory_utilities.h"
#include "plugins.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_plugin(scope);
static void export_server_plugin(scope);
static void export_plugin_iter(scope);


//-----------------------------------------------------------------------------
// Declare the _plugins module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_plugins)
{
	export_plugin(_plugins);
	export_server_plugin(_plugins);
	export_plugin_iter(_plugins);

	_plugins.attr("server_plugin_manager") = ptr(GetServerPlugin());
}


//-----------------------------------------------------------------------------
// Exports CPlugin.
//-----------------------------------------------------------------------------
void export_plugin(scope _plugins)
{
	class_<CPlugin, boost::noncopyable> ServerPlugin("ServerPlugin", no_init);

	ServerPlugin.add_property(
		"name",
		&CPlugin::GetName,
		"Return the name/description of the plugin."
	);

	ServerPlugin.add_property(
		"module_name",
		&CPlugin::GetModuleName,
		"Return the path to the plugin's shared library."
	);

	ServerPlugin.def_readwrite(
		"interface_version",
		&CPlugin::m_iPluginInterfaceVersion,
		"Return the version of the IServerPluginCallbacks interface the plugin is built on."
	);
		
	ServerPlugin ADD_MEM_TOOLS(CPlugin);
}


//-----------------------------------------------------------------------------
// Exports CServerPlugin.
//-----------------------------------------------------------------------------
void export_server_plugin(scope _plugins)
{
	class_<CServerPlugin, boost::noncopyable> ServerPluginManager("ServerPluginManager", no_init);

	ServerPluginManager.add_property(
		"loaded_plugins",
		make_function(&CServerPlugin::GetLoadedPlugins, manage_new_object_policy()),
		"Return a generator to iterate over all loaded server plugins."
	);
		
	ServerPluginManager ADD_MEM_TOOLS(CServerPlugin);
}


//-----------------------------------------------------------------------------
// Exports CPluginIter.
//-----------------------------------------------------------------------------
void export_plugin_iter(scope _plugins)
{
	class_<CPluginIter> PluginIter("PluginIter", no_init);

	PluginIter.def("__iter__",
		&CPluginIter::__iter__
	);

	PluginIter.def(
		"__next__",
		&CPluginIter::__next__,
		reference_existing_object_policy()
	);
}
