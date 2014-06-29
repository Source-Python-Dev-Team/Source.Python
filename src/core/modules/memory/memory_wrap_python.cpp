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
void export_sizes();

DECLARE_SP_MODULE(_memory)
{
	export_binaryfile();
	export_memtools();
	export_dyncall();
	export_dynamichooks();
	export_callbacks();
	export_sizes();
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
			("signature", "offset", arg("level")=0),
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
// Exposes CPointer and CFunction
//-----------------------------------------------------------------------------
#define EXPOSE_SET_TYPE(name, type) \
	.def("set_" XSTRINGIFY(name), \
		&CPointer::Set<type>, \
		"Sets the value at the given memory location.", \
		("value", arg("offset")=0) \
	)

#define EXPOSE_GET_TYPE(name, type) \
	.def("get_" XSTRINGIFY(name), \
		&CPointer::Get<type>, \
		"Returns the value at the given memory location.", \
		(arg("offset")=0) \
	)

#define EXPOSE_GET_SET_TYPE(name, type) \
	EXPOSE_SET_TYPE(name, type) \
	EXPOSE_GET_TYPE(name, type)

void export_memtools()
{
	class_<CPointer, boost::noncopyable>("Pointer", init< optional<unsigned long, bool> >())
		.def(init<CPointer&>())

		// get/set_<type> methods
		EXPOSE_GET_SET_TYPE(bool, bool)
		EXPOSE_GET_SET_TYPE(char, char)
		EXPOSE_GET_SET_TYPE(uchar, unsigned char)
		EXPOSE_GET_SET_TYPE(short, short)
		EXPOSE_GET_SET_TYPE(ushort, unsigned short)
		EXPOSE_GET_SET_TYPE(int, int)
		EXPOSE_GET_SET_TYPE(uint, unsigned int)
		EXPOSE_GET_SET_TYPE(long, long)
		EXPOSE_GET_SET_TYPE(ulong, unsigned long)
		EXPOSE_GET_SET_TYPE(long_long, long long)
		EXPOSE_GET_SET_TYPE(ulong_long, unsigned long long)
		EXPOSE_GET_SET_TYPE(float, float)
		EXPOSE_GET_SET_TYPE(double, double)
		EXPOSE_GET_SET_TYPE(string_pointer, const char*)

		.def("get_pointer",
			&CPointer::GetPtr,
			"Returns the value at the given memory location.",
			(arg("offset")=0),
			manage_new_object_policy()
		)

		.def("get_string_array",
			&CPointer::GetStringArray,
			"Returns the value at the memory location.",
			(arg("offset")=0)
		)
		
		.def("set_pointer",
			&CPointer::SetPtr,
			"Sets the value at the given memory location.",
			("value", arg("offset")=0)
		)

		.def("set_string_array",
			&CPointer::SetStringArray,
			"Sets the value at the given memory location.",
			("value",arg( "offset")=0)
		)

		// Other methods
		.def("get_virtual_func",
			&CPointer::GetVirtualFunc,
			"Returns the address (as a Pointer instance) of a virtual function at the given index.",
			args("index"),
			manage_new_object_policy()
		)

		.def("make_function",
			&CPointer::MakeFunction,
			"Creates a new Function instance.",
			args("convention", "arguments", "return_type"),
			manage_new_object_policy()
		)

		.def("make_virtual_function",
			&CPointer::MakeVirtualFunction,
			"Creates a new Function instance.",
			args("index", "convention", "arguments", "return_type"),
			manage_new_object_policy()
		)

		.def("realloc",
			&CPointer::PreRealloc,
			"Reallocates a memory block.",
			args("size")
		)

		.def("dealloc",
			&CPointer::PreDealloc,
			"Deallocates a memory block."
		)

		.def("__del__",
			&CPointer::__del__
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
		.def("__int__", make_getter(&CPointer::m_ulAddr))

        .def(!self)

        .def(self == other<unsigned long>())
        .def(self == self)

        .def(self != other<unsigned long>())
        .def(self != self)

        .def(self += int())
        .def(self += self)
		
        .def("__add__", &CPointer::operator+<unsigned long>, manage_new_object_policy())
        .def("__add__", &CPointer::operator+<CPointer>, manage_new_object_policy())

        .def("__radd__", &CPointer::operator+<unsigned long>, manage_new_object_policy())
        .def("__radd__", &CPointer::operator+<CPointer>, manage_new_object_policy())

        .def(self -= int())
        .def(self -= self)
		
        .def("__sub__", &CPointer::operator-<unsigned long>, manage_new_object_policy())
        .def("__sub__", &CPointer::operator-<CPointer>, manage_new_object_policy())

        .def("__rsub__", &CPointer::operator-<unsigned long>, manage_new_object_policy())
        .def("__rsub__", &CPointer::operator-<CPointer>, manage_new_object_policy())

		// Attributes
		.def_readwrite("address", 
			&CPointer::m_ulAddr
		)

		.def_readwrite("auto_dealloc",
			&CPointer::m_bAutoDealloc
		)
	;

	class_<CFunction, bases<CPointer>, boost::noncopyable >("Function", init<unsigned long, Convention_t, tuple, object>())
		.def(init<CFunction&>())

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
		.def_readwrite("arguments",
			&CFunction::m_Args
		)

		.def_readwrite("return_type",
			&CFunction::m_oReturnType
		)

		.def_readwrite("convention",
			&CFunction::m_eConv
		)
	;
	
	def("alloc",
		Alloc,
		"Allocates a memory block.",
		("size", arg("auto_dealloc")=true),
		manage_new_object_policy()
	);

	def("get_object_pointer",
		&GetObjectPointer,
		args("object"),
		"Returns the pointer of the C++ object"
	);

	def("make_object",
		&MakeObject,
		"Wraps a pointer using an exposed class."
	);

	def("get_size",
		&GetSize,
		"Returns the size of the class object or instance of its C++ class."
	);

	scope().attr("NULL") = object(ptr(new CPointer()));
}

//-----------------------------------------------------------------------------
// Exposes DynCall
//-----------------------------------------------------------------------------
void export_dyncall()
{
	enum_<Argument_t>("Argument")
		.value("BOOL", ARG_BOOL)
		.value("CHAR", ARG_CHAR)
		.value("UCHAR", ARG_UCHAR)
		.value("SHORT", ARG_SHORT)
		.value("USHORT", ARG_USHORT)
		.value("INT", ARG_INT)
		.value("UINT", ARG_UINT)
		.value("LONG", ARG_LONG)
		.value("ULONG", ARG_ULONG)
		.value("LONGLONG", ARG_LONGLONG)
		.value("FLOAT", ARG_FLOAT)
		.value("DOUBLE", ARG_DOUBLE)
		.value("POINTER", ARG_POINTER)
		.value("STRING", ARG_STRING)
	;

	enum_<ReturnType_t>("Return")
		.value("VOID", RET_VOID)
		.value("BOOL", RET_BOOL)
		.value("CHAR", RET_CHAR)
		.value("UCHAR", RET_UCHAR)
		.value("SHORT", RET_SHORT)
		.value("USHORT", RET_USHORT)
		.value("INT", RET_INT)
		.value("UINT", RET_UINT)
		.value("LONG", RET_LONG)
		.value("ULONG", RET_ULONG)
		.value("LONGLONG", RET_LONGLONG)
		.value("FLOAT", RET_FLOAT)
		.value("DOUBLE", RET_DOUBLE)
		.value("POINTER", RET_POINTER)
		.value("STRING", RET_STRING)
	;

	enum_<Convention_t>("Convention")
		.value("CDECL", CONV_CDECL)
		.value("STDCALL", CONV_STDCALL)
		.value("THISCALL", CONV_THISCALL)
	;

	enum_<HookType_t>("HookType")
		.value("PRE", HOOKTYPE_PRE)
		.value("POST", HOOKTYPE_POST)
	;
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
			make_function(&CStackData::GetESP, manage_new_object_policy()),
			"Stack pointer register."
		)
	;
}

//-----------------------------------------------------------------------------
// Exposes CCallback
//-----------------------------------------------------------------------------
void export_callbacks()
{
	class_< CCallback, bases< CFunction >, boost::noncopyable >("Callback", init< object, Convention_t, tuple, object, optional<bool> >())
        .def_readwrite("callback",
            &CCallback::m_oCallback,
            "The Python function that gets called by the C++ callback"
        )
    ;
}


//-----------------------------------------------------------------------------
// Exposes TYPE_SIZES
//-----------------------------------------------------------------------------
#define ADD_NATIVE_TYPE_SIZE(name, type) \
	scope().attr("TYPE_SIZES")[name] = sizeof(type);

#define ADD_SIZE(type) \
	ADD_NATIVE_TYPE_SIZE(XSTRINGIFY(type), type)

void export_sizes()
{
	scope().attr("EXPOSED_CLASSES") = g_oExposedClasses;

	// Don't remove this! It's required for the ADD_NATIVE_TYPE_SIZE macro.
	scope().attr("TYPE_SIZES") = dict();

	// Native types
	ADD_NATIVE_TYPE_SIZE("BOOL", bool)
	ADD_NATIVE_TYPE_SIZE("CHAR", char)
	ADD_NATIVE_TYPE_SIZE("UCHAR", unsigned char)
	ADD_NATIVE_TYPE_SIZE("SHORT", short)
	ADD_NATIVE_TYPE_SIZE("USHORT", unsigned short)
	ADD_NATIVE_TYPE_SIZE("INT", int)
	ADD_NATIVE_TYPE_SIZE("UINT", unsigned int)
	ADD_NATIVE_TYPE_SIZE("LONG", long)
	ADD_NATIVE_TYPE_SIZE("ULONG", unsigned long)
	ADD_NATIVE_TYPE_SIZE("LONG_LONG", long long)
	ADD_NATIVE_TYPE_SIZE("ULONG_LONG", unsigned long long)
	ADD_NATIVE_TYPE_SIZE("FLOAT", float)
	ADD_NATIVE_TYPE_SIZE("DOUBLE", double)
	ADD_NATIVE_TYPE_SIZE("POINTER", void*)
	ADD_NATIVE_TYPE_SIZE("STRING", char*)
}
