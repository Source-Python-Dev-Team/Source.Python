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
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "modules/entities/entities_transmit.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_transmit_manager(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._transmit module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _transmit)
{
	export_transmit_manager(_transmit);
}


//-----------------------------------------------------------------------------
// Exports CTransmitManager.
//-----------------------------------------------------------------------------
void export_transmit_manager(scope _transmit)
{
	class_<CTransmitManager, CTransmitManager *, boost::noncopyable>("TransmitManager", no_init)

		// Class methods
		.def("hide",
			&CTransmitManager::hide,
			"Hide the entity from all players.",
			("entity_index")
		)

		.def("hide_from",
			&CTransmitManager::hide_from,
			"Hide the entity from player.",
			("entity_index", "player_index")
		)


		.def("show",
			&CTransmitManager::show,
			"Show the entity to all players.",
			("entity_index")
		)

		.def("show_from",
			&CTransmitManager::show_from,
			"Show the entity to player.",
			("entity_index", "player_index")
		)


		.def("reset",
			&CTransmitManager::reset,
			"Reset the entity's hidden/shown state.",
			("entity_index")
		)

		.def("reset_from",
			&CTransmitManager::reset_from,
			"Reset the player's entity hidden/shown state.",
			("entity_index", "player_index")
		)

		.def("reset_player",
			&CTransmitManager::reset_player,
			"Reset the player's hidden/shown state on all entities.",
			("player_index")
		)

		.def("reset_all",
			&CTransmitManager::reset_all,
			"Reset all entities' hidden/shown state."
		)


		.def("is_hidden",
			&CTransmitManager::is_hidden,
			"Return True if the entity is hidden from any player.",
			("entity_index")
		)

		.def("is_hidden_from",
			&CTransmitManager::is_hidden_from,
			"Return True if the entity is hidden from the player.",
			("entity_index", "player_index")
		)


		.def("get_hidden_player",
			&CTransmitManager::get_hidden_player,
			"Get the players where the entity is hidden.",
			("entity_index")
		)

		.def("get_hidden_entity",
			&CTransmitManager::get_hidden_entity,
			"Get the entities that are hidden from the player.",
			("player_index")
		)


		// Add memory tools...
		ADD_MEM_TOOLS(CTransmitManager)
	;

	// Singleton...
	_transmit.attr("TransmitManager") = object(ptr(CTransmitManager::get_instance()));
}

