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



// Really hacky way to access the private member function RecursiveCopyKeyValues.
// It's done by replacing the declaration of RecursiveCopyKeyValues with a dummy
// method, a friend function declaration and the actual declaration that got
// replaced.
// This is done, so we don't need to patch KeyValues.h. #define private public
// doesn't work here, because of undefined symbols (KeyValues.cpp compiles the
// function as a private symbol name, and keyvalues_wrap.cpp would try to look up
// the function using its public symbol name).
/*
	void RecursiveCopyKeyValues( KeyValues& src );

	REPLACED BY:

	void JustFinishTheDeclaration() {} 
	friend void RecursiveCopyKeyValuesHack(KeyValues* pThis, KeyValues& src); 
	void RecursiveCopyKeyValues( KeyValues& src );
*/
#if defined(ENGINE_ORANGEBOX)
	#define CopyKeyValuesFromRecursive \
		JustFinishTheDeclaration() {} \
		friend void RecursiveCopyKeyValuesHack(KeyValues* pThis, KeyValues& src); \
		void CopyKeyValuesFromRecursive
#else
	#define RecursiveCopyKeyValues \
		JustFinishTheDeclaration() {} \
		friend void RecursiveCopyKeyValuesHack(KeyValues* pThis, KeyValues& src); \
		void RecursiveCopyKeyValues
#endif

#include "tier1/KeyValues.h"

// Now, remove the replacement, so the friend function can call the member function.
#define RecursiveCopyKeyValues RecursiveCopyKeyValues
#define CopyKeyValuesFromRecursive CopyKeyValuesFromRecursive

void RecursiveCopyKeyValuesHack(KeyValues* pThis, KeyValues& src)
{
#if defined(ENGINE_ORANGEBOX)
	pThis->CopyKeyValuesFromRecursive(src);
#else
	pThis->RecursiveCopyKeyValues(src);
#endif
}



#include "modules/keyvalues/keyvalues.h"


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_keyvalues(scope);
void export_keyvalues_types(scope);
void export_keyvalues_iter(scope);


//---------------------------------------------------------------------------------
// Declare the _keyvalues module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(_keyvalues)
{
	export_keyvalues(_keyvalues);
	export_keyvalues_types(_keyvalues);
	export_keyvalues_iter(_keyvalues);
}


//---------------------------------------------------------------------------------
// Exports KeyValues.
//---------------------------------------------------------------------------------
void export_keyvalues(scope _keyvalues)
{
	class_<KeyValues,  boost::shared_ptr<KeyValues>, boost::noncopyable>("KeyValues", no_init)
		.def("__init__", make_constructor(&KeyValuesExt::__init__1))
		.def("__init__", make_constructor(&KeyValuesExt::__init__2))
		.def("__init__", make_constructor(&KeyValuesExt::__init__3))
		.def("__init__", make_constructor(&KeyValuesExt::__init__4))
		.def("__init__", make_constructor(&KeyValuesExt::__init__5))

		.def("from_dict", &KeyValuesExt::from_dict).staticmethod("from_dict")

		.add_property("name",
			&KeyValues::GetName,
			&KeyValues::SetName,
			"Returns the name of the keyvalues object."
		)

		.add_property("name_symbol",
			&KeyValues::GetNameSymbol,
			"Gets the name as a unique integer."
		)

		.def("uses_escape_sequences",
			&KeyValues::UsesEscapeSequences,
			"Sets whether or not this keyvalues object uses escape sequences.",
			(arg("uses_escape_sequences"))
		)

		.def("from_file",
			&KeyValuesExt::FromFile,
			"Load KeyValues data from a file and return a new KeyValues instance on success.",
			(arg("file_name"), arg("encoding")="utf-8", arg("errors")="strict", arg("uses_escape_sequences")=false)
		).staticmethod("from_file")

		.def("from_file_in_place",
			&KeyValuesExt::FromFileInPlace,
			"Load KeyValues data from a file into an existing KeyValues instance.",
			(arg("file_name"), arg("encoding")="utf-8", arg("errors")="strict")
		)

		.def("from_buffer",
			&KeyValuesExt::FromBuffer,
			"Load KeyValues data from a buffer and return a new KeyValues instance on success.",
			(arg("buffer"))
		).staticmethod("from_buffer")

		.def("from_buffer_in_place",
			&KeyValuesExt::FromBufferInPlace,
			"Load KeyValues data from a buffer into an existing KeyValues instance.",
			(arg("buffer"))
		)

		.def("save_to_file",
			&KeyValuesExt::SaveToFile,
			(arg("file_name")),
			"Saves the data in this KeyValues instance to the given file path."
		)

		.def("find_key",
			GET_METHOD(KeyValues *, KeyValues, FindKey, const char *, bool),
			"Finds a KeyValue. Creates it if create_key is set.",
			("key_name", arg("create_key")=false),
			reference_existing_object_policy()
		)

		.def("find_key_by_symbol",
			GET_CONST_METHOD(KeyValues*, KeyValues, FindKey, int),
			"Finds a subkey by an integer identifier.",
			(arg("key")),
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
			(arg("sub_key"))
		)

		.def("remove_sub_key",
			&KeyValues::RemoveSubKey,
			"Removes a subkey from the list. DOES NOT DELETE IT!",
			(arg("sub_key"))
		)

		.add_property("first_sub_key",
			make_function(
				&KeyValues::GetFirstSubKey,
				reference_existing_object_policy()
			),
			"Returns the first subkey in the list. Will iterate over the keys AND the values."
		)

		.add_property("next_key",
			make_function(
				GET_METHOD(KeyValues*, KeyValues, GetNextKey),
				reference_existing_object_policy()
			),
			&KeyValues::SetNextKey,
			"Returns the next subkey. Will iterate the keys AND the values."
		)

		.add_property("first_true_sub_key",
			make_function(
				&KeyValues::GetFirstTrueSubKey,
				reference_existing_object_policy()
			),
			"Returns the first subkey (both key and value)."
		)

		.add_property("next_true_sub_key",
			make_function(
				&KeyValues::GetNextTrueSubKey,
				reference_existing_object_policy()
			),
			"Returns the next subkey (both key and value)."
		)

		.add_property("first_value",
			make_function(
				&KeyValues::GetFirstValue,
				reference_existing_object_policy()
			),
			"Returns the first value in the tree."
		)

		.add_property("next_value",
			make_function(
				&KeyValues::GetNextValue,
				reference_existing_object_policy()
			),
			"Returns the next value in the tree."
		)

		.def("make_copy",
			&KeyValuesExt::make_copy,
			"Create a copy of the instance."
		)

		.def("get_int",
			GET_METHOD(int, KeyValues, GetInt, const char *, int),
			"Returns the integer value for the given key name.",
			(arg("key_name")=object(), arg("default_value")=0)
		)

		.def("get_uint64",
			GET_METHOD(uint64, KeyValues, GetUint64, const char *, uint64),
			"Gets the 64-bit integer value for the given key name.",
			(arg("key_name")=object(), arg("default_value")=0)
		)

		.def("get_float",
			GET_METHOD(float, KeyValues, GetFloat, const char *, float),
			"Returns the floating point value for the given key name.",
			(arg("key_name")=object(), arg("default_value")=0.0f)
		)

		.def("get_string",
			GET_METHOD(const char *, KeyValues, GetString, const char *, const char *),
			"Returns the string value for the given key name.",
			(arg("key_name")=object(), arg("default_value")="")
		)

		.def("get_color",
			&KeyValuesExt::GetColor,
			"Returns the color value of the given key name.",
			(arg("key_name")=object(), arg("default_value")=object())
		)

		.def("get_bool",
			&KeyValuesExt::GetBool,
			"Returns the boolean value for the given key name.",
			(arg("key_name")=object(), arg("default_value")=false)
		)

		.def("is_empty",
			GET_METHOD(bool, KeyValues, IsEmpty, const char *),
			"Returns true if this key or the given key is empty.",
			(arg("key_name")=object())
		)

		.def("set_string",
			&KeyValues::SetString,
			"Sets the given key's string value.",
			(arg("key_name"), arg("value"))
		)

		.def("set_int",
			&KeyValues::SetInt,
			"Sets the given key's integer value.",
			(arg("key_name"), arg("value"))
		)

		.def("set_uint64",
			&KeyValues::SetUint64,
			"Sets the given key's 64-bit integer value.",
			(arg("key_name"), arg("value"))
		)

		.def("set_float",
			&KeyValues::SetFloat,
			"Sets the given key's floating point value.",
			(arg("key_name"), arg("value"))
		)

		.def("set_color",
			&KeyValues::SetColor,
			"Sets the given key's color value.",
			(arg("key_name"), arg("value"))
		)

		.def("set_bool",
			&KeyValuesExt::SetBool,
			"Sets the given key's boolean value.",
			(arg("key_name"), arg("value"))
		)

		.def("get_data_type",
			&KeyValues::GetDataType,
			"Return the data type of this KeyValue object or the given key."
		)

		.def("__getitem__",
			&KeyValuesExt::__getitem__,
			"Return the value of the given key."
		)

		.def("__setitem__",
			&KeyValuesExt::__setitem__,
			"Set the value of the given key."
		)

		.def("__delitem__",
			&KeyValuesExt::__delitem__,
			"Remove the given key."
		)

		.def("__iter__",
			&KeyValuesExt::__iter__,
			"Return an iterator that will iterate over all keys."
		)
			
		.def("as_dict",
			&KeyValuesExt::as_dict,
			"Return the KeyValues object as a dict."
		)

		.def("recursive_copy",
			&RecursiveCopyKeyValuesHack,
			"Recursively copy the given KeyValues into this KeyValues instance."
		)

		ADD_MEM_TOOLS(KeyValues)
	;
}


//---------------------------------------------------------------------------------
// Exports KeyValues types.
//---------------------------------------------------------------------------------
void export_keyvalues_types(scope _keyvalues)
{
	enum_<KeyValues::types_t> KeyValuesType("KeyValueType");
	
	KeyValuesType.value("NONE", KeyValues::TYPE_NONE);
	KeyValuesType.value("STRING", KeyValues::TYPE_STRING);
	KeyValuesType.value("INT", KeyValues::TYPE_INT);
	KeyValuesType.value("FLOAT", KeyValues::TYPE_FLOAT);
	KeyValuesType.value("PTR", KeyValues::TYPE_PTR);
	KeyValuesType.value("WSTRING", KeyValues::TYPE_WSTRING);
	KeyValuesType.value("COLOR", KeyValues::TYPE_COLOR);
	KeyValuesType.value("UNINT64", KeyValues::TYPE_UINT64);
	
#ifdef ENGINE_CSGO
	// TODO: Move this to a engine specific file
	KeyValuesType.value("COMPILED_INT_BYTE", KeyValues::TYPE_COMPILED_INT_BYTE);
	KeyValuesType.value("COMPILED_INT_0", KeyValues::TYPE_COMPILED_INT_0);
	KeyValuesType.value("COMPILED_INT_1", KeyValues::TYPE_COMPILED_INT_1);
#endif

	KeyValuesType.value("NUM_TYPES", KeyValues::TYPE_NUMTYPES);
}


//---------------------------------------------------------------------------------
// Exports KeyValues iter.
//---------------------------------------------------------------------------------
void export_keyvalues_iter(scope _keyvalues)
{
	class_<KeyValuesIter> _KeyValuesIter("KeyValuesIter", init<KeyValues*>());

	_KeyValuesIter.def(
		"__next__",
		&KeyValuesIter::__next__
	);
}
