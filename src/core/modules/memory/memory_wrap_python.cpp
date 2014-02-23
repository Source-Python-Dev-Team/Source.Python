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
#include "modules/export_main.h"
#include "memory_scanner.h"
#include "memory_tools.h"
#include "memory_hooks.h"
#include "memory_callback.h"

#include "dyncall.h"


//-----------------------------------------------------------------------------
// Exposes the memory_c module.
//-----------------------------------------------------------------------------
void export_binaryfile();
void export_memtools();
void export_dyncall();
void export_dynamichooks();
void export_callbacks();

DECLARE_SP_MODULE(memory_c)
{
	export_binaryfile();
	export_memtools();
	export_dyncall();
	export_dynamichooks();
	export_callbacks();
}

//-----------------------------------------------------------------------------
// Exposes CBinaryFile
//-----------------------------------------------------------------------------
// Overloads
BOOST_PYTHON_FUNCTION_OVERLOADS(find_binary_overload, FindBinary, 1, 2);

void export_binaryfile()
{
	class_<CBinaryFile, boost::noncopyable>("BinaryFile", no_init)

		// Class methods
		.def("find_pointer",
			&CBinaryFile::FindPointer,
			"Rips out a pointer from a function.",
			args("signature", "offset"),
			manage_new_object_policy()
		)

		.def("find_address",
			&CBinaryFile::FindAddress,
			"Returns the address of a signature or symbol found in memory.",
			args("identifier"),
			manage_new_object_policy()
		)

		// Special methods
		.def("__getitem__",
			&CBinaryFile::FindAddress,
			"Returns the address of a signature or symbol found in memory.",
			args("identifier"),
			manage_new_object_policy()
		)
	
		// Attributes
		.def_readwrite("address",
			&CBinaryFile::m_ulAddr,
			"Base address of the binary."
		)

		.def_readwrite("size",
			&CBinaryFile::m_ulSize,
			"Size of the binary."
		)
	;

	def("find_binary",
		&FindBinary,
		find_binary_overload(
			args("path", "srv_check"),
			"Returns a BinaryFile object or None.")[reference_existing_object_policy()]
	);
}


//-----------------------------------------------------------------------------
// Exposes CPointer
//-----------------------------------------------------------------------------
// These two macros ease the exposing part of get_<type> methods
#define OVERLOAD_GET_TYPE(name, type) \
	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_##name##_overload, CPointer::Get<type>, 0, 1)

#define EXPOSE_GET_TYPE(name, type) \
	def("get_" XSTRINGIFY(name), \
		&CPointer::Get<type>, \
		get_##name##_overload( \
			args("offset"), \
			"Returns the value at the given memory location." \
		) \
	)

// These two macros ease the exposing part of set_<type> methods
#define OVERLOAD_SET_TYPE(name, type) \
	BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_##name##_overload, CPointer::Set<type>, 1, 2)

#define EXPOSE_SET_TYPE(name, type) \
	def("set_" XSTRINGIFY(name), \
		&CPointer::Set<type>, \
		set_##name##_overload( \
			args("value", "offset"), \
			"Sets the value at the given memory location." \
		) \
	)
	
// get_<type> overloads
OVERLOAD_GET_TYPE(bool, bool)
OVERLOAD_GET_TYPE(char, char)
OVERLOAD_GET_TYPE(uchar, unsigned char)
OVERLOAD_GET_TYPE(short, short)
OVERLOAD_GET_TYPE(ushort, unsigned short)
OVERLOAD_GET_TYPE(int, int)
OVERLOAD_GET_TYPE(uint, unsigned int)
OVERLOAD_GET_TYPE(long, long)
OVERLOAD_GET_TYPE(ulong, unsigned long)
OVERLOAD_GET_TYPE(long_long, long long)
OVERLOAD_GET_TYPE(ulong_long, unsigned long long)
OVERLOAD_GET_TYPE(float, float)
OVERLOAD_GET_TYPE(double, double)
OVERLOAD_GET_TYPE(string_ptr, const char*)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_ptr_overload, GetPtr, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(get_string_array_overload, GetStringArray, 0, 1)
	
// set_<type> overloads
OVERLOAD_SET_TYPE(bool, bool)
OVERLOAD_SET_TYPE(char, char)
OVERLOAD_SET_TYPE(uchar, unsigned char)
OVERLOAD_SET_TYPE(short, short)
OVERLOAD_SET_TYPE(ushort, unsigned short)
OVERLOAD_SET_TYPE(int, int)
OVERLOAD_SET_TYPE(uint, unsigned int)
OVERLOAD_SET_TYPE(long, long)
OVERLOAD_SET_TYPE(ulong, unsigned long)
OVERLOAD_SET_TYPE(long_long, long long)
OVERLOAD_SET_TYPE(ulong_long, unsigned long long)
OVERLOAD_SET_TYPE(float, float)
OVERLOAD_SET_TYPE(double, double)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_ptr_overload, SetPtr, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_string_ptr_overload, SetStringPtr, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_string_array_overload, SetStringArray, 1, 2)

void export_memtools()
{
	class_<CPointer>("Pointer", init< optional<unsigned long> >())
		// get_<type> methods
		.EXPOSE_GET_TYPE(bool, bool)
		.EXPOSE_GET_TYPE(char, char)
		.EXPOSE_GET_TYPE(uchar, unsigned char)
		.EXPOSE_GET_TYPE(short, short)
		.EXPOSE_GET_TYPE(ushort, unsigned short)
		.EXPOSE_GET_TYPE(int, int)
		.EXPOSE_GET_TYPE(uint, unsigned int)
		.EXPOSE_GET_TYPE(long, long)
		.EXPOSE_GET_TYPE(ulong, unsigned long)
		.EXPOSE_GET_TYPE(long_long, long long)
		.EXPOSE_GET_TYPE(ulong_long, unsigned long long)
		.EXPOSE_GET_TYPE(float, float)
		.EXPOSE_GET_TYPE(double, double)
		.EXPOSE_GET_TYPE(string_ptr, const char*)

		.def("get_ptr",
			&CPointer::GetPtr,
			get_ptr_overload(
				"Returns the value at the given memory location.",
				args("offset")
				)[manage_new_object_policy()]
		)

		.def("get_string_array",
			&CPointer::GetStringArray,
			get_string_array_overload(
				"Returns the value at the memory location.",
				args("offset")
			)
		)
		
		// set_<type> methods
		.EXPOSE_SET_TYPE(bool, bool)
		.EXPOSE_SET_TYPE(char, char)
		.EXPOSE_SET_TYPE(uchar, unsigned char)
		.EXPOSE_SET_TYPE(short, short)
		.EXPOSE_SET_TYPE(ushort, unsigned short)
		.EXPOSE_SET_TYPE(int, int)
		.EXPOSE_SET_TYPE(uint, unsigned int)
		.EXPOSE_SET_TYPE(long, long)
		.EXPOSE_SET_TYPE(ulong, unsigned long)
		.EXPOSE_SET_TYPE(long_long, long long)
		.EXPOSE_SET_TYPE(ulong_long, unsigned long long)
		.EXPOSE_SET_TYPE(float, float)
		.EXPOSE_SET_TYPE(double, double)
		
		.def("set_ptr",
			&CPointer::SetPtr,
			set_ptr_overload(
				"Sets the value at the given memory location.",
				args("value", "offset")
			)
		)

		.def("set_string_ptr",
			&CPointer::SetStringPtr,
			set_string_ptr_overload(
				"Sets the value at the given memory location.",
				args("value", "offset")
			)
		)

		.def("set_string_array",
			&CPointer::SetStringArray,
			set_string_array_overload(
				"Sets the value at the given memory location.",
				args("value", "offset")
			)
		)

		// Other methods
		.def("get_virtual_func",
			&CPointer::GetVirtualFunc,
			"Returns the address (as a CPointer instance) of a virtual function at the given index.",
			args("index"),
			manage_new_object_policy()
		)

		.def("make_function",
			&CPointer::MakeFunction,
			"Creates a new Function instance.",
			args("convention", "parameters"),
			manage_new_object_policy()
		)

		.def("make_virtual_function",
			&CPointer::MakeVirtualFunction,
			"Creates a new Function instance.",
			args("index", "convention", "parameters"),
			manage_new_object_policy()
		)

		.def("realloc",
			&CPointer::Realloc,
			"Reallocates a memory block.",
			args("size")
		)

		.def("dealloc",
			&CPointer::Dealloc,
			"Deallocates a memory block."
		)

		 .def("get_size",
			&CPointer::GetSize,
			"Returns the size of the memory block."
		)

		.def("compare",
			&CPointer::Compare,
			"Compares the first <num> bytes of both pointers. Returns 0 if they are equal. A value greater than zero indicates that the first "\
			"byte that does not match in both pointers has a greater value in <self> than in <other>. A value less than zero indicates the opposite.",
			args("other", "num")
		)

		.def("is_overlapping",
			&CPointer::IsOverlapping,
			"Returns True if the pointers are overlapping each other.",
			args("destination", "num_bytes")
		)

		.def("search_bytes",
			&CPointer::SearchBytes,
			"Searches within the first <num_bytes> of this memory block for the first occurence of <bytes> and returns a pointer it.",
			args("bytes", "num_bytes"),
			manage_new_object_policy()
		)

		.def("copy",
			&CPointer::Copy,
			"Copies <num_bytes> from <self> to the pointer <destination>. Overlapping is not allowed!",
			args("destination", "num_bytes")
		)

		.def("move",
			&CPointer::Move,
			"Copies <num_bytes> from <self> to the pointer <destination>. Overlapping is allowed!",
			args("destination", "num_bytes")
		)

		// Special methods
		.def(int_(self))

        .def(!self)

        .def(self == other<unsigned long>())
        .def(self == self)

        .def(self != other<unsigned long>())
        .def(self != self)

        .def(self += int())
        .def(self += self)

        .def(self + int())
        .def(self + self)
        .def(int() + self)

        .def(self -= int())
        .def(self -= self)

        .def(self - int())
        .def(self - self)
        .def(int() - self)

		// Attributes
		.def_readwrite("address", 
			&CPointer::m_ulAddr
		)
	;

	def("alloc",
		Alloc,
		"Allocates a memory block.",
		args("size"),
		manage_new_object_policy()
	);

	class_<CFunction, bases<CPointer> >("Function", init<unsigned long, Convention_t, char*>())
		.def("__call__",
			raw_method(&CFunction::Call),
			"Calls the function dynamically."
		)

		.def("call_trampoline",
			raw_method(&CFunction::CallTrampoline),
			"Calls the trampoline function dynamically."
		)

		.def("add_hook",
			&CFunction::AddHook,
			"Adds a hook callback.",
			args("hook_type", "callback")
		)

		.def("remove_hook",
			&CFunction::RemoveHook,
			"Removes a hook callback.",
			args("hook_type", "callback")
		)

		.def("add_pre_hook",
			&CFunction::AddPreHook,
			"Adds a pre-hook callback."
		)

		.def("add_post_hook",
			&CFunction::AddPostHook,
			"Adds a post-hook callback."
		)

		.def("remove_pre_hook",
			&CFunction::RemovePreHook,
			"Removes a pre-hook callback."
		)

		.def("remove_post_hook",
			&CFunction::RemovePostHook,
			"Removes a post-hook callback."
		)

		// Attributes
		.def_readwrite("parameters",
			&CFunction::m_szParams
		)

		.def_readwrite("convention",
			&CFunction::m_eConv
		)
	;
}

//-----------------------------------------------------------------------------
// Exposes DynCall
//-----------------------------------------------------------------------------
void export_dyncall()
{
	enum_<Convention_t>("Convention")
		.value("CDECL", CONV_CDECL)
		.value("STDCALL", CONV_STDCALL)
		.value("THISCALL", CONV_THISCALL)
	;

	// Other constants that are very useful.
	scope().attr("DC_ERROR_NONE") = DC_ERROR_NONE;
	scope().attr("DC_ERROR_UNSUPPORTED_MODE") = DC_ERROR_UNSUPPORTED_MODE;

	def("get_error",
		GetError,
		"Returns the last DynCall error ID."
	);
}

void export_dynamichooks()
{
	class_<CStackData>("StackData", init<CHook*>())
		// Special methods
		.def("__getitem__",
			&CStackData::GetItem,
			"Returns the argument at the specified index."
		)

		.def("__setitem__",
			&CStackData::SetItem,
			"Sets the argument at the specified index."
		)

		// Properties
		.add_property("esp",
			&CStackData::GetESP,
			"Stack pointer register."
		)
	;
}

void export_callbacks()
{
    class_< CCallback, bases< CFunction > >("Callback", init< object, Convention_t, char * >())
        .def("free",
            &CCallback::Free,
            "Frees the callback. Don't use dealloc()!"
        )

        .def_readwrite("callback",
            &CCallback::m_oCallback,
            "The Python function that gets called by the C++ callback"
        )
    ;
}
