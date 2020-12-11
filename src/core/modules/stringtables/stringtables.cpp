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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "stringtables.h"

//---------------------------------------------------------------------------------
// External variables to use.
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;


//-----------------------------------------------------------------------------
// INetworkStringTableExt
//-----------------------------------------------------------------------------
bool INetworkStringTableExt::__contains__( INetworkStringTable *pTable, const char *string )
{
	return pTable->FindStringIndex(string) != INVALID_STRING_INDEX;
}

const char* INetworkStringTableExt::GetString(INetworkStringTable& table, int index)
{
	if ((index < 0) || (index >= table.GetNumStrings()))
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	return table.GetString(index);
}

int INetworkStringTableExt::AddString( INetworkStringTable *pTable, const char *string, const char *user_data, int length, bool is_server, bool auto_unlock )
{
	int index = INVALID_STRING_INDEX;
	bool locked = false;
	if (auto_unlock)
	{
		locked = engine->LockNetworkStringTables(false);
	}
	if (user_data && pTable->FindStringIndex(string) == INVALID_STRING_INDEX && length == -1)
	{
		length = strlen(user_data) + 1;
	}
	index = pTable->AddString(is_server, string, length, (const void *)user_data);
	if (locked && auto_unlock)
	{
		engine->LockNetworkStringTables(locked);
	}
	return index;
}

void INetworkStringTableExt::SetStringIndexUserData( INetworkStringTable *pTable, int string_index, const char *user_data, int length )
{
	if (length == -1)
	{
		length = strlen(user_data) + 1;
	}
	pTable->SetStringUserData(string_index, length, user_data);
}

void INetworkStringTableExt::SetStringUserData( INetworkStringTable *pTable, const char *string, const char *user_data, int length )
{
	int string_index = pTable->FindStringIndex(string);
	if (string_index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	SetStringIndexUserData(pTable, string_index, user_data, length);
}

const char* INetworkStringTableExt::GetStringIndexUserData( INetworkStringTable *pTable, int string_index )
{
	return (const char *)pTable->GetStringUserData(string_index, NULL);
}

const char* INetworkStringTableExt::GetStringUserData( INetworkStringTable *pTable, const char *string )
{
	int index = pTable->FindStringIndex(string);
	if (index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	return GetStringIndexUserData(pTable, index);
}

int INetworkStringTableExt::GetStringIndexUserDataLength( INetworkStringTable *pTable, int string_index )
{
	int length = 0;
	const void * user_data = pTable->GetStringUserData(string_index, &length);
	return length;
}

int INetworkStringTableExt::GetStringUserDataLength( INetworkStringTable *pTable, const char *string )
{
	int index = pTable->FindStringIndex(string);
	if (index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	return GetStringIndexUserDataLength(pTable, index);
}

//---------------------------------------------------------------------------------
// INetworkStringTableContainerExt
//---------------------------------------------------------------------------------
INetworkStringTable* INetworkStringTableContainerExt::GetTable(INetworkStringTableContainer& table_container, TABLEID table_id)
{
	if ((table_id < 0) || (table_id >= table_container.GetNumTables()))
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	return table_container.GetTable(table_id);
}