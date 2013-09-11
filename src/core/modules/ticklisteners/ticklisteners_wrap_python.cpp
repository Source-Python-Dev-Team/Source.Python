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
*/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "ticklisteners_wrap.h"

//-----------------------------------------------------------------------------
// Functions that expose tick listener functionality to us.
//-----------------------------------------------------------------------------
void export_ticklistener();

//-----------------------------------------------------------------------------
// Exposes the tick_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(tick_c)
{
	export_ticklistener();
}

//-----------------------------------------------------------------------------
// Exposes CTickListenerManager
//-----------------------------------------------------------------------------
void export_ticklistener()
{
	BOOST_ABSTRACT_CLASS(CTickListenerManager)

		CLASS_METHOD(CTickListenerManager,
			register_listener,
			"Adds the given callable to the end of the tick listener vector.",
			args("pCallable")
		)

		CLASS_METHOD(CTickListenerManager,
			unregister_listener,
			"Removes the given callable from the tick listener vector.",
			args("pCallable")
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_tick_listener_manager,
		"Returns the CTickListListenerManager instance",
		reference_existing_object_policy()
	);
}
