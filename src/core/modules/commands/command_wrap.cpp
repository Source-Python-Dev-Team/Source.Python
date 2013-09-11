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
#include "command_wrap.h"
#include "core/sp_main.h"

//-----------------------------------------------------------------------------
// CICommand constructors.
//-----------------------------------------------------------------------------
CICommand::CICommand()
{
	m_CCommand_ptr = NULL;
}

CICommand::CICommand( const CCommand* command )
{
	m_CCommand_ptr = command;
}

//-----------------------------------------------------------------------------
// CICommand methods.
//-----------------------------------------------------------------------------
int CICommand::get_arg_count()
{
	return m_CCommand_ptr->ArgC();
}

const char *CICommand::get_arg_string()
{
	return m_CCommand_ptr->ArgS();
}

const char *CICommand::get_command_string()
{
	return m_CCommand_ptr->GetCommandString();
}

const char *CICommand::get_arg( int iIndex )
{
	return m_CCommand_ptr->Arg(iIndex);
}

//-----------------------------------------------------------------------------
// Registers a Filter.
//-----------------------------------------------------------------------------
void BaseFilters::register_filter(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Is the callable already in the vector?
	if( !m_vecCallables.HasElement(oCallable) )
	{
		// Add the callable to the vector
		m_vecCallables.AddToTail(oCallable);
	}
}

//-----------------------------------------------------------------------------
// Unregisters a Filter.
//-----------------------------------------------------------------------------
void BaseFilters::unregister_filter(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the vector
	m_vecCallables.FindAndRemove(oCallable);
}
