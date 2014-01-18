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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "KeyValues.h"
#include "filesystem.h"
#include "modules/export_main.h"

//---------------------------------------------------------------------------------
// Externals
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;

//---------------------------------------------------------------------------------
// Keyvalues module definition.
//---------------------------------------------------------------------------------
void export_keyvalues();

DECLARE_SP_MODULE(keyvalues_c)
{
	export_keyvalues();
}

//---------------------------------------------------------------------------------
// Exposes KeyValues functionality.
//---------------------------------------------------------------------------------
class KeyValuesExt
{
public:
	static bool GetBool(KeyValues* pKeyValues, const char * szName = NULL, bool bDefault = false)
	{
		return pKeyValues->GetInt(szName, bDefault);
	}

	static void SetBool(KeyValues* pKeyValues, const char * szName, bool bValue)
	{
		pKeyValues->SetInt(szName, bValue);
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(load_from_file_overload, LoadFromFile, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(save_to_file_overload, SaveToFile, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(find_key_overload, FindKey, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_int_overload, GetInt, 0, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_uint64_overload, GetUint64, 0, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_float_overload, GetFloat, 0, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_string_overload, GetString, 0, 2);
BOOST_PYTHON_FUNCTION_OVERLOADS(get_bool_overload, KeyValuesExt::GetBool, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(is_empty_overload, IsEmpty, 0, 1);


void export_keyvalues()
{
	// TODO: rename
	// TODO: Add constructors. There are two possibilities:
	// 1. Patch SDK and make destructor public. Then deleteThis() isn't required anymore
	// 2. Create functions which construct a new object
	class_<KeyValues, boost::noncopyable>("CKeyValues", no_init)//init<const char *>())
		/*
		.def(init<const char *, const char *, const char *>())
		.def(init<const char *, const char *, int>())
		.def(init<const char *, const char *, const char *, const char *, const char *>())
		.def(init<const char *, const char *, int, const char *, int>())
		*/

		.def("delete",
			&KeyValues::deleteThis,
			"Ensures that KeyValues object is deleted from correct heap"
		)

		.def("get_name",
			&KeyValues::GetName,
			"Returns the name of the keyvalues object."
		)

		.def("set_name", 
			&KeyValues::SetName,
			"Sets the name of the keyvalues object.",
			args("name")
		)

		.def("get_name_symbol",
			&KeyValues::GetNameSymbol,
			"Gets the name as a unique integer.",
			args("key_symbol")
		)

		.def("uses_escape_sequences",
			&KeyValues::UsesEscapeSequences,
			"Sets whether or not this keyvalues object uses escape sequences.",
			args("uses_escape_sequences")
		)

		.def("load_from_file",
			&KeyValues::LoadFromFile,
			load_from_file_overload(
				"Loads KeyValues data from a file into this CKeyValues instance.",
				args("filesystem", "resource_name", "path_id")
			)
		)
		
		.def("save_to_file",
			&KeyValues::SaveToFile,
			save_to_file_overload(
				args("filesystem", "resource_name", "path_id"),
				"Saves the data in this CKeyValues instance to the given file path."
			)
		)

		.def("find_key",
			GET_METHOD(KeyValues *, KeyValues, FindKey, const char *, bool),
			find_key_overload(
				"Finds a KeyValue. Creates it if create_key is set.",
				args("key_name", "create_key")
			)[reference_existing_object_policy()]
		)
			
		.def("find_key_by_symbol",
			static_cast< KeyValues* (KeyValues::*)( int ) const >(&KeyValues::FindKey),
			"Finds a subkey by an integer identifier.",
			args("key"),
			reference_existing_object_policy()
		)
		
		.def("create_new_key",
			&KeyValues::CreateNewKey,
			"Creates a new child key with an autogenerated name. The name is guaranteed to be\
			an integer, of value 1 higher than the highest other integer key name.",
			reference_existing_object_policy()
		)
		
		.def("add_sub_key",
			&KeyValues::AddSubKey,
			"Adds a sub key. Make sure the subkey isn't a child of some other KeyValues.",
			args("sub_key")
		)

		.def("remove_sub_key",
			&KeyValues::RemoveSubKey,
			"Removes a subkey from the list. DOES NOT DELETE IT!",
			args("sub_key")
		)

		.def("get_first_sub_key",
			&KeyValues::GetFirstSubKey,
			"Returns the first subkey in the list. Will iterate over the keys AND the\
			values.",
			reference_existing_object_policy()
		)

		.def("get_next_key",
			&KeyValues::GetNextKey,
			"Returns the next subkey. Will iterate the keys AND the values.",
			reference_existing_object_policy()
		)

		.def("get_first_true_sub_key",
			&KeyValues::GetFirstTrueSubKey,
			"Returns the first subkey (both key and value).",
			reference_existing_object_policy()
		)

		.def("get_next_true_sub_key",
			&KeyValues::GetNextTrueSubKey,
			"Returns the next subkey (both key and value).",
			reference_existing_object_policy()
		)

		.def("get_first_value",
			&KeyValues::GetFirstValue,
			"Returns the first value in the tree.",
			reference_existing_object_policy()
		)

		.def("get_next_value",
			&KeyValues::GetNextValue,
			"Returns the next value in the tree.",
			reference_existing_object_policy()
		)

		.def("get_int",
			GET_METHOD(int, KeyValues, GetInt, const char *, int),
			get_int_overload(
				"Returns the integer value for the given key name.",
				args("key_name", "default_value")
			)
		)

		.def("get_uint64",
			GET_METHOD(uint64, KeyValues, GetUint64, const char *, uint64),
			get_uint64_overload(
				"Gets the 64-bit integer value for the given key name.",
				args("key_name", "default_value")
			)
		)

		.def("get_float",
			GET_METHOD(float, KeyValues, GetFloat, const char *, float),
			get_float_overload(
				"Returns the floating point value for the given key name.",
				args("key_name", "default_value")
			)
		)

		.def("get_string",
			GET_METHOD(const char *, KeyValues, GetString, const char *, const char *),
			get_string_overload(
				"Returns the string value for the given key name.",
				args("key_name", "default_value")
			)
		)

		.def("get_bool",
			&KeyValuesExt::GetBool,
			get_bool_overload(
				"Returns the boolean value for the given key name.",
				args("key_name", "default_value")
			)
		)

		.def("is_empty",
			GET_METHOD(bool, KeyValues, IsEmpty, const char *),
			is_empty_overload(
				"Returns true if this key or the given key is empty.",
				args("key_name")
			)
		)

		.def("set_string",
			&KeyValues::SetString,
			"Sets the given key's string value.",
			args("key_name", "value")
		)

		.def("set_int",
			&KeyValues::SetInt,
			"Sets the given key's integer value.",
			args("key_name", "value")
		)

		.def("set_uint64",
			&KeyValues::SetUint64,
			"Sets the given key's 64-bit integer value.",
			args("key_name", "value")
		)

		.def("set_float",
			&KeyValues::SetFloat,
			"Sets the given key's floating point value.",
			args("key_name", "value")
		)

		.def("set_bool",
			&KeyValuesExt::SetBool,
			"Sets the given key's boolean value.",
			args("key_name", "value")
		)
	;
}

