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
#ifndef _ENTITIES_PROPS_H
#define _ENTITIES_PROPS_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "utlhash.h"
#include "dt_send.h"

//---------------------------------------------------------------------------------
// Every SendProp has a name and an offset. We'll use this for the prop offset
// hash table.
//---------------------------------------------------------------------------------
class CPropOffset
{
public:
	const char*		prop_name;
	SendProp*			prop;
	int					prop_offset;
};

//---------------------------------------------------------------------------------
// The SendProp offset manager. Caches prop offsets with their name so that we
// don't have to repeatedly look them up every call.
//---------------------------------------------------------------------------------
class CSendPropHashTable
{
private:
	CUtlHash<CPropOffset> m_prop_table;

public:
	// Instantiates the prop table.
	CSendPropHashTable();

	// Need to free prop names.
	~CSendPropHashTable();

public:
	// Hash table comparison function.
	static bool hash_compare( const CPropOffset& a, const CPropOffset& b );

	// Returns the index of a given element.
	static unsigned int hash_key( const CPropOffset& a );

public:
	// Returns a prop offset by name.
	// unsigned int get_offset( const char* prop_name, bool& was_found );
	SendProp* get_prop( const char* prop_name, int &iOffset );

	// Inserts a prop offset into the hash table.
	void insert_offset( const char* name, SendProp* prop, int iOffset );

	// Removes a prop offset by name.
	void remove_offset( const char* prop_name );
};

//---------------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------------
SendProp* UTIL_FindSendProp( SendTable* send_table, const char* prop_name, int &iOffset );

#endif // _ENTITIES_PROPS_H
