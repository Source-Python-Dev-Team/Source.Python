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

#ifndef _ENGINES_SERVER_H
#define _ENGINES_SERVER_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;

// SDK
#include "iserver.h"


//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
class IServerExt
{
public:
	static tuple GetNetStats(IServer* pServer);
};


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
// Example: execute_server_command('echo', 'hi', 123, 4.5)
object execute_server_command(tuple args, dict kwargs);

// Example: queue_server_command('echo', 'hi', 123, 4.5)
object queue_server_command(tuple args, dict kwargs);

// Example: queue_command_string('blah hi 123 4.5')
void queue_command_string(const char* szCommand);

// Example: insert_server_command('echo', 'hi', 123, 4.5)
object insert_server_command(tuple args, dict kwargs);

// Example: insert_command_string('blah hi 123 4.5')
void insert_command_string(const char* szCommand);

#endif // _ENGINES_SERVER_H
