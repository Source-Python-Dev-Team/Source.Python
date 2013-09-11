/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
* Original source:
* https://developer.valvesoftware.com/wiki/Ingame_menu_for_server_plugins_(CS:S_only)
*/

#include "modules/export_main.h"
#include "mrecipientfilter_wrap.h"

//---------------------------------------------------------------------------------
// Namespaces to use
//---------------------------------------------------------------------------------
using namespace boost::python;

//---------------------------------------------------------------------------------
// Exposer functions.
//---------------------------------------------------------------------------------
void export_mrecipientfilter_interface();

//---------------------------------------------------------------------------------
// Exposes the engine module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(recipientfilter_c)
{
	export_mrecipientfilter_interface();
}

void export_mrecipientfilter_interface()
{
	BOOST_CLASS_NOCOPY( CMRecipientFilter )

		CLASS_METHOD(CMRecipientFilter,
			is_reliable,
			"Whether this recipient filter will be network reliable (sent in-order)"
		)

		CLASS_METHOD(CMRecipientFilter,
			is_init_message,
			"Whether the message has been initialised?"
		)

		CLASS_METHOD(CMRecipientFilter,
			get_recipient_count,
			"Obtain the amount of clients in this filter"
		)

		CLASS_METHOD(CMRecipientFilter,
			get_recipient_index,
			"Obtains the player index at the slot in the filter",
			args("slot")
		)

		CLASS_METHOD(CMRecipientFilter,
			add_all_players,
			"Adds all the players on the server to the filter"
		)

		CLASS_METHOD(CMRecipientFilter,
			add_recipient,
			"Adds the index of the player to the filter",
			args("iPlayer")
		)

		CLASS_METHOD(CMRecipientFilter,
			remove_all_players,
			"Removes all the players on the server from the filter"
		)

		CLASS_METHOD(CMRecipientFilter,
			remove_recipient,
			"Removes the index of the player from the filter",
			args("iPlayer")
		)

		CLASS_METHOD(CMRecipientFilter,
			has_recipient,
			"Returns true if the given index is in the recipient, false otherwise.",
			args("iPlayer")
		)
		
	BOOST_END_CLASS()
}