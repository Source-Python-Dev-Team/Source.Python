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
#ifndef _CVAR_WRAP_H
#define _CVAR_WRAP_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "icvar.h"
#include "convar.h"

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern ICvar* g_pCVar;

//-----------------------------------------------------------------------------
// CConVar class.
//-----------------------------------------------------------------------------
class CConVar
{
public:
	CConVar( ConVar* pConVar );

	// Get CConVar instance with just the name
	CConVar( const char *pName );

	// Get CConVar instance with name, default, and flags
	CConVar( const char *pName, const char *pDefaultValue, int flags );

	// Get CConVar instance with name, default, flags, and helpstring
	CConVar( const char *pName, const char *pDefaultValue, int flags,
		const char *pHelpString );

	// Get CConVar instance with name, default, flags, helpstring and min/max values
	CConVar( const char *pName, const char *pDefaultValue, int flags,
		const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax );

	const char* get_help_text();
	const char *get_name();

	bool is_flag_set( int iFlag );
	void add_flags( int iFlags );
	void remove_flags( int iFlags );
	int get_flags();

	bool is_command();

	float get_float();
	int get_int();
	bool get_bool();
	char const *get_string();

	void set_float( float flValue );
	void set_int( int iValue );
	void set_bool( bool bValue );
	void set_string( const char *szValue );

	void revert();
	const char *get_default();

	bool has_min();
	bool has_max();
	float get_min_value();
	float get_max_value();

private:
	const char *m_Name;
};

//-----------------------------------------------------------------------------
// CConCommandBase class.
//-----------------------------------------------------------------------------
class CConCommandBase
{
public:
	CConCommandBase( ConCommandBase* pConCommandBase );
	CConCommandBase( const char *pName, const char *pHelpString, int flags );

	bool is_command();
	bool is_registered();

	bool is_flag_set( int iFlag );
	void add_flags( int iFlags );
	void remove_flags( int iFlags );
	int get_flags();

	const char *get_name();
	const char *get_help_text();

private:
	const char *m_Name;
};

//-----------------------------------------------------------------------------
// CCvar class.
//-----------------------------------------------------------------------------
class CCvar
{
public:
	void register_con_command( CConCommandBase *pCommandBase );
	void unregister_con_command( CConCommandBase *pCommandBase );

	CConCommandBase *find_command_base( const char *szName );
	CConVar *find_var( const char *szName );
};

#endif // _CVAR_WRAP_H
