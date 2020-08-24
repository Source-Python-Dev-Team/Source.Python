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
			"",
			("entity_index")
		)

		.def("hide_from",
			&CTransmitManager::hide_from,
			"",
			("entity_index", "player_index")
		)


		.def("unhide",
			&CTransmitManager::unhide,
			"",
			("entity_index")
		)

		.def("unhide_from",
			&CTransmitManager::unhide_from,
			"",
			("entity_index", "player_index")
		)


		.def("reset",
			&CTransmitManager::reset,
			"",
			("entity_index")
		)

		.def("reset_from",
			&CTransmitManager::reset_from,
			"",
			("entity_index", "player_index")
		)

		.def("reset_all",
			&CTransmitManager::reset_all,
			""
		)


		.def("is_hidden",
			&CTransmitManager::is_hidden,
			"",
			("entity_index")
		)

		.def("is_hidden_from",
			&CTransmitManager::is_hidden_from,
			"",
			("entity_index", "player_index")
		)

		.def("get_hidden_states",
			&CTransmitManager::get_hidden_states,
			"",
			("entity_index")
		)


		// Add memory tools...
		ADD_MEM_TOOLS(CTransmitManager)
	;

	// Singleton...
	_transmit.attr("transmit_manager") = object(ptr(GetTransmitManager()));
}

