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
*/
#ifndef _COMMANDS_H
#define _COMMANDS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "convar.h"


//-----------------------------------------------------------------------------
// Return values.
//-----------------------------------------------------------------------------
enum CommandReturn
{
	BLOCK = 0,
	CONTINUE
};


//-----------------------------------------------------------------------------
// CCommand extension class.
//-----------------------------------------------------------------------------
class CCommandExt
{
public:
	static PyObject* GetArg(CCommand& command, unsigned int iIndex)
	{
		if ((int) iIndex >= command.ArgC())
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");

		const char* szValue = command[iIndex];
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static PyObject* ArgS(CCommand& command)
	{
		const char* szValue = command.ArgS();
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static PyObject* GetCommandString(CCommand& command)
	{
		const char* szValue = command.GetCommandString();
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}
};


#endif // _COMMAND_H
