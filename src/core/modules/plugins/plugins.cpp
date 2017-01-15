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
// C++
#ifdef _WIN32
	#include "Windows.h"
#else
	#include "link.h"
#endif

// Source.Python
#include "plugins.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IServerPluginHelpers* helpers;


//-----------------------------------------------------------------------------
// GetServerPlugin.
//-----------------------------------------------------------------------------
CServerPlugin* GetServerPlugin()
{
	static CServerPlugin* plugin = dynamic_cast<CServerPlugin*>(helpers);
	if (!plugin)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to retrieve CServerPlugin singleton.")

	return plugin;
}


//-----------------------------------------------------------------------------
// CPlugin.
//-----------------------------------------------------------------------------
object CPlugin::GetModuleName()
{
	static object Path = import("path").attr("Path");

#ifdef _WIN32
	char file_name[1024];
	if (!GetModuleFileName((HMODULE) m_pPluginModule, file_name, 1024))
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve module file name.")

	return Path(str(file_name));
#else
	link_map* dlmap = (link_map*) m_pPluginModule;
	return Path(str((const char*) dlmap->l_name));
#endif
}

const char* CPlugin::GetName()
{
	return m_szName;
}


//-----------------------------------------------------------------------------
// CPluginPlugin.
//-----------------------------------------------------------------------------
CPluginIter* CServerPlugin::GetLoadedPlugins()
{
	return new CPluginIter(&m_Plugins);
}


//-----------------------------------------------------------------------------
// CPluginIter
//-----------------------------------------------------------------------------
CPluginIter::CPluginIter(CUtlVector<CPlugin*>* plugins) 
{
	this->plugins = plugins;
	this->current_index = 0;
}

object CPluginIter::__iter__(PyObject* self)
{
	return object(handle<>(borrowed(self)));
}

CPlugin* CPluginIter::__next__()
{
	if (current_index >= plugins->Count())
		BOOST_RAISE_EXCEPTION(PyExc_StopIteration, "No more plugins.")
		
	return plugins->Element(current_index++);
}
