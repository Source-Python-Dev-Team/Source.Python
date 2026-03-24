/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2023 Source Python Development Team.  All rights reserved.
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
#include "threads.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Namespaces.
//-----------------------------------------------------------------------------
using namespace boost::python;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_thread_yielder(scope);


//-----------------------------------------------------------------------------
// Declare the _threads module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_threads)
{
	export_thread_yielder(_threads);
}


//-----------------------------------------------------------------------------
// Exports CThreadYielder.
//-----------------------------------------------------------------------------
void export_thread_yielder(scope _threads)
{
	class_<CThreadYielder> ThreadYielder(
		"ThreadYielder",
		"When in context, yields remaining cycles to Python threads every frame.\n"
		"\n"
		".. note::\n"
		"\n"
		"	:data:`threads.sp_thread_yielding` must be enabled for it to be effective."
	);

	// Special methods...
	ThreadYielder.def("__enter__", &CThreadYielder::__enter__);
	ThreadYielder.def("__exit__", &CThreadYielder::__exit__);

	// Methods...
	ThreadYielder.def(
		"is_implemented",
		&CThreadYielder::IsImplemented,
		"Returns whether thread yielding is implemented on the current game."
	).staticmethod("is_implemented");

	// Add memory tools...
	ThreadYielder ADD_MEM_TOOLS(CThreadYielder);
}
