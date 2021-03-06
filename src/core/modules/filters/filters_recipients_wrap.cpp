/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "memalloc.h"
#include "filters_recipients.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_irecipientfilter(scope);
void export_mrecipientfilter(scope);


//-----------------------------------------------------------------------------
// Declare the _filters._recipients module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_filters, _recipients)
{
	export_irecipientfilter(_recipients);
	export_mrecipientfilter(_recipients);
}


//-----------------------------------------------------------------------------
// Exports IRecipientFilter
//-----------------------------------------------------------------------------
void export_irecipientfilter(scope _recipients)
{
	class_<IRecipientFilter, IRecipientFilter*, boost::noncopyable> BaseRecipientFilter("BaseRecipientFilter", no_init);

	// Special methods...
	BaseRecipientFilter.def("__len__",
		&IRecipientFilter::GetRecipientCount,
		"Return the recipient count."
	);

	BaseRecipientFilter.def("get_recipient_index",
		&IRecipientFilter::GetRecipientIndex,
		"Obtains the player index at the slot in the filter",
		args("slot")
	);

	BaseRecipientFilter.def("__getitem__",
		&IRecipientFilterExt::__getitem__,
		"Obtains the player index at the slot in the filter",
		args("index")
	);

	// Methods...
	BaseRecipientFilter.def("is_reliable",
		&IRecipientFilter::IsReliable,
		"Whether this recipient filter will be network reliable (sent in-order)"
	);

	BaseRecipientFilter.def("is_init_message",
		&IRecipientFilter::IsInitMessage,
		"Whether the message has been initialised?"
	);

	BaseRecipientFilter.def("get_recipient_index",
		&IRecipientFilter::GetRecipientIndex,
		"Obtains the player index at the slot in the filter",
		args("slot")
	);

	// Add memory tools...
	BaseRecipientFilter ADD_MEM_TOOLS(IRecipientFilter);
}


//-----------------------------------------------------------------------------
// Exports MRecipientFilter
//-----------------------------------------------------------------------------
void export_mrecipientfilter(scope _recipients)
{
	class_<MRecipientFilter, boost::shared_ptr<MRecipientFilter>, bases<IRecipientFilter>, boost::noncopyable > _RecipientFilter("_RecipientFilter");

	// Constructors...
	CLASSMETHOD(_RecipientFilter,
		"from_abstract_pointer",
		&MRecipientFilter::from_abstract_pointer,
		"Returns a new recipient filter instance filled with the indexes contained in the given IRecipientFilter pointer.\n"
		"\n"
		".. note::\n"
		"	No checks is done on the given pointer and it is assumed to be valid. Do not use this "
		"	constructor if you don't know what you are doing. This should really only be used to wrap "
		"	the content of recipient filters that do not have RTTI data (e.g. during the execution of certain hooks, etc.).",
		args("cls", "pointer")
	);

	// Special methods...
	_RecipientFilter.def("__contains__",
		&MRecipientFilter::HasRecipient,
		"Return True if the given index is in the recipient filter.",
		args("index")
	);

	// Methods...
	_RecipientFilter.def("add_all_players",
		&MRecipientFilter::AddAllPlayers,
		"Adds all the players on the server to the filter"
	);

	_RecipientFilter.def("add_recipient",
		&MRecipientFilter::AddRecipient,
		"Adds the index of the player to the filter",
		args("index")
	);

	_RecipientFilter.def("remove_all_players",
		&MRecipientFilter::RemoveAllPlayers,
		"Removes all the players on the server from the filter"
	);

	_RecipientFilter.def("remove_recipient",
		&MRecipientFilter::RemoveRecipient,
		"Removes the index of the player from the filter",
		args("index")
	);

	_RecipientFilter.def_readwrite("reliable",
		&MRecipientFilter::m_bReliable,
		"Get/set whether or not the filter is reliable.\n\n"
		":rtype: bool"
	);

	// Add memory tools...
	_RecipientFilter ADD_MEM_TOOLS(MRecipientFilter);
}
