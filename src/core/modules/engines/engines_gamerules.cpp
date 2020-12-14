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
#include "modules/entities/entities_props.h"
#include "modules/stringtables/stringtables.h"
#include "utilities/wrap_macros.h"

// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;

// SDK
#include "server_class.h"


//---------------------------------------------------------------------------------
// External variables to use.
//---------------------------------------------------------------------------------
extern INetworkStringTableContainer *networkstringtable;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
int find_game_rules_property_offset(const char* name)
{
	static SendTable* s_table = NULL;
	if (!s_table) 
	{
		ServerClass* cls = ServerClassExt::find_server_class(find_game_rules_proxy_name());
		s_table = cls->m_pTable;
	}

	int offset = SendTableSharedExt::find_offset(s_table, name);

	if (offset == -1)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find property '%s'.", name)

	return offset;
}

const char* find_game_rules_proxy_name()
{
	static std::string s_proxy_name;
	if (!s_proxy_name.empty())
		// Use cache
		return s_proxy_name.c_str();

	INetworkStringTable* table = networkstringtable->FindTable("GameRulesCreation");
	if (!table)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find string table 'GameRulesCreation'.")

	s_proxy_name = INetworkStringTableExt::GetStringUserData(table, "classname");
	if (s_proxy_name.empty())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "'classname' of string table 'GameRulesCreation' is NULL.")

	s_proxy_name += "Proxy";
	return s_proxy_name.c_str();
}

SendTableProxyFn find_game_rules_proxy_function()
{
	SendTableProxyFn s_proxy_func = NULL;
	if (s_proxy_func)
		// Use cache
		return s_proxy_func;

	ServerClass* cls = ServerClassExt::find_server_class(find_game_rules_proxy_name());
	SendTable* table = cls->m_pTable;

	for (int i=0; i < table->GetNumProps(); i++)
	{
		SendProp* prop = table->GetProp(i);
		if (!V_stristr(prop->GetName(), "gamerules_data"))
			continue;

		s_proxy_func = prop->GetDataTableProxyFn();
		break;
	}

	if (!s_proxy_func)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Game rules proxy function is NULL.");

	return s_proxy_func;
}

CGameRulesWrapper* find_game_rules()
{
	SendTableProxyFn proxy_func = find_game_rules_proxy_function();
	static CSendProxyRecipients recipients;
	
	CGameRulesWrapper* game_rules = (CGameRulesWrapper*) proxy_func(NULL, NULL, NULL, &recipients, 0);
	if (!game_rules)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Game rules pointer is NULL.");

	return game_rules;
}
