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

#include "mrecipientfilter_wrap.h"


CMRecipientFilter::CMRecipientFilter()
{
}

CMRecipientFilter::~CMRecipientFilter()
{
}

bool CMRecipientFilter::is_reliable( void ) const
{
	return IsReliable();
}

bool CMRecipientFilter::is_init_message( void ) const
{
	return IsInitMessage();
}

int CMRecipientFilter::get_recipient_count( void ) const
{
	return GetRecipientCount();
}

int CMRecipientFilter::get_recipient_index( int slot ) const
{
	return GetRecipientIndex(slot);
}

void CMRecipientFilter::add_all_players()
{
	AddAllPlayers();
}

void CMRecipientFilter::add_recipient( int iPlayer )
{
	AddRecipient(iPlayer);
}

void CMRecipientFilter::remove_all_players()
{
	RemoveAllPlayers();
}

void CMRecipientFilter::remove_recipient( int iPlayer )
{
	RemoveRecipient(iPlayer);
}

bool CMRecipientFilter::has_recipient( int iPlayer )
{
	return HasRecipient(iPlayer);
}