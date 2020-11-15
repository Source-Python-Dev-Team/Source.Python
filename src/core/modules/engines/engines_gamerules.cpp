/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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
#include "engines_gamerules.h"
#include "modules/entities/entities_factories.h"
#include "modules/entities/entities_entity.h"
#include "modules/entities/entities_props.h"
#include "utilities/wrap_macros.h"

// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;

// SDK
#include "strtools.h"


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
int find_game_rules_property_offset(const char* name)
{
	// TODO: I guess it's save to cache the server class...

	CBaseEntityWrapper* proxy = (CBaseEntityWrapper*) CBaseEntityWrapper::find_or_create(find_game_rules_proxy_name());

	ServerClass* cls = proxy->GetServerClass();
	if (!cls)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve the server class.")

	int offset;
	offset = SendTableSharedExt::find_offset(cls->m_pTable, name);
	if (offset == -1)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find property '%s'.", name)

	return offset;
}

const char* find_game_rules_proxy_name()
{
	static const char* s_proxy_name = NULL;
	int i = 0;

	if (s_proxy_name)
	{
		return s_proxy_name;
	}

	CEntityFactoryDictionary* factory_dictionary = GetEntityFactoryDictionary();
	
	while (factory_dictionary->m_Factories.IsValidIndex(i))
	{
		const char* classname = factory_dictionary->m_Factories.GetElementName(i);
		if (V_stristr(classname, "gamerules"))
		{
			// Cache the result
			s_proxy_name = classname;
			return classname;
		}

		++i;
	}

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find game rules proxy name.");
	return NULL;
}

CGameRulesWrapper* find_game_rules()
{
	// TODO: Can we cache the result?

	CBaseEntityWrapper* proxy = (CBaseEntityWrapper*) CBaseEntityWrapper::find_or_create(find_game_rules_proxy_name());

	ServerClass* cls = proxy->GetServerClass();
	while (cls)
	{
		SendTable* table = cls->m_pTable;
		for (int i=0; i < table->GetNumProps(); i++)
		{
			SendProp* prop = table->GetProp(i);
			if (!V_stristr(prop->GetName(), "gamerules_data"))
			{
				continue;
			}

			SendTableProxyFn proxy_func = prop->GetDataTableProxyFn();
			if (!proxy_func)
			{
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Found game rules proxy entity, but proxy function is NULL.");
			}

			// Pass the recipients. Some game's proxy functions require it for
			// the game rules proxy.
			CSendProxyRecipients recipients;
			return (CGameRulesWrapper*) proxy_func(NULL, NULL, NULL, &recipients, 0);
		}

		cls = cls->m_pNext;
	}

	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Found game rules proxy entity, but proxy function is NULL.");
	return NULL;
}
