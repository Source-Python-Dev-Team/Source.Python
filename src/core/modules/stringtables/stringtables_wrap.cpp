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
#include "stringtables.h"

//---------------------------------------------------------------------------------
// External variables to use.
//---------------------------------------------------------------------------------
extern INetworkStringTableContainer *networkstringtable;


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
// Exposes INetworkStringTable.
//---------------------------------------------------------------------------------
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
			&INetworkStringTableExt::__contains__,
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
			&INetworkStringTable::ChangedSinceTick,
			"Returns True if the table has been modified since the given tick, False otherwise.",
			("tick")
		)
		
		.def("add_string",
			&INetworkStringTableExt::AddString,
			"Adds the given string to the table.",
			("string", arg("user_data")=object(), arg("length")=-1, arg("is_server")=true, arg("auto_unlock")=true)
		)
		
		.def("__getitem__",
			&INetworkStringTableExt::GetString,
			"Returns the string at the given index.",
			("string_index")
		)
		
		.def("set_user_data",
			&INetworkStringTableExt::SetStringIndexUserData,
			"Sets the user data of the given string index.",
			("string_index", "user_data", arg("length")=-1)
		)
		
		.def("set_user_data",
			&INetworkStringTableExt::SetStringUserData,
			"Sets the user data of the given string.",
			("string", "user_data", arg("length")=-1)
		)
		
		.def("get_user_data",
			&INetworkStringTableExt::GetStringIndexUserData,
			"Returns the user data of the given string index.",
			("string_index")
		)
		
		.def("get_user_data",
			&INetworkStringTableExt::GetStringUserData,
			"Returns the user data of the given string.",
			("string")
		)
		
		.def("__getitem__",
			&INetworkStringTable::FindStringIndex,
			"Returns the index of the given string (returns INVALID_STRING_INDEX if not found).",
			("string")
		)
		
		.def("get_user_data_length",
			&INetworkStringTableExt::GetStringIndexUserDataLength,
			"Returns the length of the user data of the given string index.",
			("string_index")
		)
		
		.def("get_user_data_length",
			&INetworkStringTableExt::GetStringUserDataLength,
			"Returns the length of the user data of the given string.",
			("string")
		)

		ADD_MEM_TOOLS(INetworkStringTable)
	;
}

//---------------------------------------------------------------------------------
// Exposes INetworkStringTableContainer.
//---------------------------------------------------------------------------------
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
