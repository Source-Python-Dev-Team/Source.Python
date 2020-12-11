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


//---------------------------------------------------------------------------------
// External variables to use.
//---------------------------------------------------------------------------------
extern INetworkStringTableContainer *networkstringtable;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
int find_game_rules_property_offset(const char* name)
{
	ServerClass* cls = ServerClassExt::find_server_class(find_game_rules_proxy_name());
	int offset = SendTableSharedExt::find_offset(cls->m_pTable, name);

	if (offset == -1)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find property '%s'.", name)

	return offset;
}

const char* find_game_rules_proxy_name()
{
	static const char* s_proxy_name = NULL;



	return NULL;
}

SendTableProxyFn find_game_rules_proxy_function()
{
	SendTableProxyFn s_proxy_func = NULL;
	if (s_proxy_func)
		return s_proxy_func;

	ServerClass* cls = ServerClassExt::find_server_class(find_game_rules_proxy_name());
	SendTable* table = cls->m_pTable;

	for (int i=0; i < table->GetNumProps(); i++)
	{
		SendProp* prop = table->GetProp(i);
		if (!V_stristr(prop->GetName(), "gamerules_data"))
		{
			continue;
		}

		SendTableProxyFn s_proxy_func = prop->GetDataTableProxyFn();
		if (!s_proxy_func)
		{
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Game rules proxy function is NULL.");
		}
	}

	return NULL;
}

CGameRulesWrapper* find_game_rules()
{
	SendTableProxyFn proxy_func = find_game_rules_proxy_function();
	static CSendProxyRecipients recipients;

	return (CGameRulesWrapper*) proxy_func(NULL, NULL, NULL, &recipients, 0);
}
