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

#ifndef __MRECIPIENT_FILTER_WRAP_H__
#define __MRECIPIENT_FILTER_WRAP_H__

#include "mrecipientfilter.h"

class CMRecipientFilter : public MRecipientFilter
{
public:
	CMRecipientFilter();
	virtual ~CMRecipientFilter();

	virtual bool is_reliable( void ) const;
	virtual bool is_init_message( void ) const;

	virtual int get_recipient_count( void ) const;
	virtual int get_recipient_index( int slot ) const;
	void add_all_players();
	void add_recipient(int iPlayer);
	void remove_all_players();
	void remove_recipient(int iPlayer);
	bool has_recipient(int iPlayer);
};

#endif // __MRECIPIENT_FILTER_WRAP_H__