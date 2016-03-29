/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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

#ifdef _WIN32

// ============================================================================
// >> INCLUDES
// ============================================================================
// Windows
#include <Windows.h>

// Memory
#include "utilities/wrap_macros.h"

// Utilities
#include "memory_exception.h"


// ============================================================================
// >> FUNCTIONS
// ============================================================================
int ExceptionHandler(_EXCEPTION_POINTERS* info, DWORD code)
{
	if (code == EXCEPTION_ACCESS_VIOLATION) {
		EXCEPTION_RECORD* record = info->ExceptionRecord;
		char* exc_message;

		if (record->ExceptionInformation[0] == 0)
			exc_message = "Access violation while reading address '%u'.";
		else 
			exc_message = "Access violation while writing address '%u'.";

		BOOST_RAISE_EXCEPTION(PyExc_RuntimeError, exc_message, record->ExceptionInformation[1])
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

#endif // _WIN32
