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

#ifndef _PLUGINS_H
#define _PLUGINS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// SDK
#include "eiface.h"

// Source.Python
#include "utilities/wrap_macros.h"


//-----------------------------------------------------------------------------
// CPlugin.
//-----------------------------------------------------------------------------
class CPlugin
{
public:
	char m_szName[128];
	bool m_bDisable;
	IServerPluginCallbacks* m_pPlugin;
	int	m_iPluginInterfaceVersion;
	CSysModule* m_pPluginModule;

	object GetModuleName();
	const char* GetName();
};


//-----------------------------------------------------------------------------
// CServerPlugin.
//-----------------------------------------------------------------------------
class CPluginIter;

class CServerPlugin: public IServerPluginHelpers
{
public:
	CUtlVector<CPlugin*> m_Plugins;

	CPluginIter* GetLoadedPlugins();
};


//-----------------------------------------------------------------------------
// CPluginIter.
//-----------------------------------------------------------------------------
class CPluginIter
{
public:
	CPluginIter(CUtlVector<CPlugin*>* plugins);
	static object	__iter__(PyObject* self);
	CPlugin*		__next__();

private:
	CUtlVector<CPlugin*>* plugins;
	int current_index;
};


//-----------------------------------------------------------------------------
// GetServerPlugin.
//-----------------------------------------------------------------------------
CServerPlugin* GetServerPlugin();

#endif // _PLUGINS_H
