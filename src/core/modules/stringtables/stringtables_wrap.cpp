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
#include "export_main.h"
#include "modules/memory/memory_tools.h"

#include "utilities/wrap_macros.h"
#include "networkstringtabledefs.h"
#include "eiface.h"

//---------------------------------------------------------------------------------
// External variables to use.
//---------------------------------------------------------------------------------
extern INetworkStringTableContainer *networkstringtable;
extern IVEngineServer* engine;

//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_stringtable(scope);
void export_stringtable_container(scope);

//---------------------------------------------------------------------------------
// Declares the stringtables_c module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(_stringtables)
{
	export_stringtable(_stringtables);
	export_stringtable_container(_stringtables);
	
	_stringtables.attr("INVALID_STRING_TABLE") = INVALID_STRING_TABLE;
	_stringtables.attr("INVALID_STRING_INDEX") = INVALID_STRING_INDEX;
}

//---------------------------------------------------------------------------------
// Add a string to the specified table.
//---------------------------------------------------------------------------------
int AddString( INetworkStringTable *pTable, const char *string, const char *user_data, int length, bool is_server, bool auto_unlock )
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

//---------------------------------------------------------------------------------
// Sets the user data of the given string index.
//---------------------------------------------------------------------------------
void SetStringIndexUserData( INetworkStringTable *pTable, int string_index, const char *user_data, int length )
{
	if (length == -1)
	{
		length = strlen(user_data) + 1;
	}
	pTable->SetStringUserData(string_index, length, user_data);
}

//---------------------------------------------------------------------------------
// Sets the user data of the given string.
//---------------------------------------------------------------------------------
void SetStringUserData( INetworkStringTable *pTable, const char *string, const char *user_data, int length )
{
	int string_index = pTable->FindStringIndex(string);
	if (string_index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	SetStringIndexUserData(pTable, string_index, user_data, length);
}

//---------------------------------------------------------------------------------
// Returns the user data of the given string index.
//---------------------------------------------------------------------------------
const char *GetStringIndexUserData( INetworkStringTable *pTable, int string_index )
{
	return (const char *)pTable->GetStringUserData(string_index, NULL);
}

//---------------------------------------------------------------------------------
// Returns the user data of the given string.
//---------------------------------------------------------------------------------
const char *GetStringUserData( INetworkStringTable *pTable, const char *string )
{
	int index = pTable->FindStringIndex(string);
	if (index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	return GetStringIndexUserData(pTable, index);
}

//---------------------------------------------------------------------------------
// Returns the length of the user data of the given string index.
//---------------------------------------------------------------------------------
int GetStringIndexUserDataLength( INetworkStringTable *pTable, int string_index )
{
	int length = 0;
	const void * user_data = pTable->GetStringUserData(string_index, &length);
	return length;
}

//---------------------------------------------------------------------------------
// Returns the user data of the given string.
//---------------------------------------------------------------------------------
int GetStringUserDataLength( INetworkStringTable *pTable, const char *string )
{
	int index = pTable->FindStringIndex(string);
	if (index == INVALID_STRING_INDEX)
	{
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Given string not found.")
	}
	return GetStringIndexUserDataLength(pTable, index);
}

//---------------------------------------------------------------------------------
// Returns whether or not the given string is in the table
//---------------------------------------------------------------------------------
bool __contains__( INetworkStringTable *pTable, const char *string )
{
	return pTable->FindStringIndex(string) != INVALID_STRING_INDEX;
}

//---------------------------------------------------------------------------------
// Exposes INetworkStringTable.
//---------------------------------------------------------------------------------
class INetworkStringTableExt
{
public:
	static const char* GetString(INetworkStringTable& table, int index)
	{
		if ((index < 0) || (index >= table.GetNumStrings()))
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

		return table.GetString(index);
	}
};

void export_stringtable(scope _stringtables)
{
	class_<INetworkStringTable, boost::noncopyable>("StringTable", no_init)
		
		.add_property("name",
			&INetworkStringTable::GetTableName,
			"Returns the name of the table (read-only)."
		)
		
		.add_property("index",
			&INetworkStringTable::GetTableId,
			"Returns the index of the table (read-only)."
		)
		
		.def("__len__",
			&INetworkStringTable::GetNumStrings,
			"Returns the length of the table."
		)
		
		.def("__contains__",
			&__contains__,
			"Returns whether or not the given string is contained in the table."
		)

		.add_property("max_strings",
			&INetworkStringTable::GetMaxStrings,
			"Returns the max length of the table (read-only)."
		)
		
		.add_property("entry_bits",
			&INetworkStringTable::GetEntryBits,
			"Returns the maximum entries of the table (read-only)."
		)
		
		.def("set_tick",
			&INetworkStringTable::SetTick,
			"Set the tick on which the table has been last modified.",
			("tick")
		)
		
		.def("changed_since_tick",
			&INetworkStringTable::SetTick,
			"Returns True if the table has been modified since the given tick, False otherwise.",
			("tick")
		)
		
		.def("add_string",
			&AddString,
			"Adds the given string to the table.",
			("string", arg("user_data")=object(), arg("length")=-1, arg("is_server")=true, arg("auto_unlock")=true)
		)
		
		.def("__getitem__",
			&INetworkStringTableExt::GetString,
			"Returns the string at the given index.",
			("string_index")
		)
		
		.def("set_user_data",
			&SetStringIndexUserData,
			"Sets the user data of the given string index.",
			("string_index", "user_data", arg("length")=-1)
		)
		
		.def("set_user_data",
			&SetStringUserData,
			"Sets the user data of the given string.",
			("string", "user_data", arg("length")=-1)
		)
		
		.def("get_user_data",
			&GetStringIndexUserData,
			"Returns the user data of the given string index.",
			("string_index")
		)
		
		.def("get_user_data",
			&GetStringUserData,
			"Returns the user data of the given string.",
			("string")
		)
		
		.def("__getitem__",
			&INetworkStringTable::FindStringIndex,
			"Returns the index of the given string (returns INVALID_STRING_INDEX if not found).",
			("string")
		)
		
		.def("get_user_data_length",
			&GetStringIndexUserDataLength,
			"Returns the length of the user data of the given string index.",
			("string_index")
		)
		
		.def("get_user_data_length",
			&GetStringUserDataLength,
			"Returns the length of the user data of the given string.",
			("string")
		)

		ADD_MEM_TOOLS(INetworkStringTable)
	;
}

//---------------------------------------------------------------------------------
// Exposes INetworkStringTableContainer.
//---------------------------------------------------------------------------------
class INetworkStringTableContainerExt
{
public:
	static INetworkStringTable* GetTable(INetworkStringTableContainer& table_container, TABLEID table_id)
	{
		if ((table_id < 0) || (table_id >= table_container.GetNumTables()))
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

		return table_container.GetTable(table_id);
	}
};

void export_stringtable_container(scope _stringtables)
{
	class_<INetworkStringTableContainer, boost::noncopyable>("_StringTables", no_init)
		
		.def("__getitem__",
			&INetworkStringTableContainerExt::GetTable,
			"Returns the StringTable instance of the given table ID.",
			("table_index"),
			reference_existing_object_policy()
		)
		
		.def("__getitem__",
			&INetworkStringTableContainer::FindTable,
			"Returns the StringTable instance of the given table name.",
			("table_name"),
			reference_existing_object_policy()
		)
		
		.def("__getattr__",
			&INetworkStringTableContainer::FindTable,
			"Returns the StringTable instance of the given table name.",
			("table_name"),
			reference_existing_object_policy()
		)
		
		.def("__len__",
			&INetworkStringTableContainer::GetNumTables,
			"Returns the length of the container."
		)
		
		// Do we really need this?
		.def("set_allow_client_side_add_string",
			&INetworkStringTableContainer::SetAllowClientSideAddString,
			"Allows clients to add strings to the given table.",
			("table", "state")
		)

		ADD_MEM_TOOLS(INetworkStringTableContainer)
	;
	
	// Singleton.
	_stringtables.attr("string_tables") = object(ptr(networkstringtable));
	
}
