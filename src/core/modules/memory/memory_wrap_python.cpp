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
#include "export_main.h"
#include "memory_scanner.h"
#include "memory_tools.h"
#include "memory_hooks.h"

#include "dyncall.h"

// DynamicHooks
#include "registers.h"


//-----------------------------------------------------------------------------
// Exposes the memory_c module.
//-----------------------------------------------------------------------------
void export_binaryfile(scope);
void export_memtools(scope);
void export_dyncall(scope);
void export_dynamichooks(scope);
void export_sizes(scope);

DECLARE_SP_MODULE(_memory)
{
	export_binaryfile(_memory);
	export_memtools(_memory);
	export_dyncall(_memory);
	export_dynamichooks(_memory);
	export_sizes(_memory);
}

//-----------------------------------------------------------------------------
// Exposes CBinaryFile
//-----------------------------------------------------------------------------
void export_binaryfile(scope _memory)
{
	class_<CBinaryFile, boost::noncopyable>("BinaryFile", no_init)

		// Class methods
		.def("find_pointer",
			&CBinaryFile::FindPointer,
			"Rips out a pointer from a function.",
			("signature", arg("offset")=0, arg("level")=0),
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
		"Returns a BinaryFile object or None.",
		("path", arg("srv_check")=true),
		reference_existing_object_policy()
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

void export_memtools(scope _memory)
{
	class_<CPointer, CPointer *>("Pointer", init< optional<unsigned long, bool> >())
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
			args("size"),
			manage_new_object_policy()
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

	class_<CFunction, bases<CPointer>, boost::noncopyable >("Function", init<unsigned long, object, object, object>())
		.def(init<CFunction&>())
		.def("__call__",
			raw_method(&CFunction::Call),
			"Calls the function dynamically."
		)

		.def("is_callable",
			&CFunction::IsCallable,
			"Return True if the function is callable."
		)

		.def("is_hookable",
			&CFunction::IsHookable,
			"Return True if the function is hookable."
		)

		.def("is_hooked",
			&CFunction::IsHooked,
			"Return True if the function is hooked."
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

		.def("_delete_hook",
			&CFunction::DeleteHook,
			"Removes all hooks and restores the original function."
		)

		// Attributes
		.def_readonly("arguments",
			&CFunction::m_tArgs
		)

		.def_readonly("return_type",
			&CFunction::m_oConverter
		)

		.def_readonly("convention",
			&CFunction::m_eCallingConvention
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

	_memory.attr("NULL") = object(CPointer());
}

//-----------------------------------------------------------------------------
// Exposes DynCall
//-----------------------------------------------------------------------------
void export_dyncall(scope _memory)
{
	enum_<DataType_t>("DataType")
		.value("VOID", DATA_TYPE_VOID)
		.value("BOOL", DATA_TYPE_BOOL)
		.value("CHAR", DATA_TYPE_CHAR)
		.value("UCHAR", DATA_TYPE_UCHAR)
		.value("SHORT", DATA_TYPE_SHORT)
		.value("USHORT", DATA_TYPE_USHORT)
		.value("INT", DATA_TYPE_INT)
		.value("UINT", DATA_TYPE_UINT)
		.value("LONG", DATA_TYPE_LONG)
		.value("ULONG", DATA_TYPE_ULONG)
		.value("LONG_LONG", DATA_TYPE_LONG_LONG)
		.value("ULONG_LONG", DATA_TYPE_ULONG_LONG)
		.value("FLOAT", DATA_TYPE_FLOAT)
		.value("DOUBLE", DATA_TYPE_DOUBLE)
		.value("POINTER", DATA_TYPE_POINTER)
		.value("STRING", DATA_TYPE_STRING)
	;

	enum_<Convention_t>("Convention")
		.value("CUSTOM", CONV_CUSTOM)
		.value("CDECL", CONV_CDECL)
		.value("STDCALL", CONV_STDCALL)
		.value("THISCALL", CONV_THISCALL)
	;

	enum_<HookType_t>("HookType")
		.value("PRE", HOOKTYPE_PRE)
		.value("POST", HOOKTYPE_POST)
	;
}


//-----------------------------------------------------------------------------
// Exposes DynamicHooks
//-----------------------------------------------------------------------------
class ICallingConventionWrapper: public ICallingConvention, public wrapper<ICallingConvention>
{
public:
	ICallingConventionWrapper(object oArgTypes, DataType_t returnType, int iAlignment=4)
		:ICallingConvention(ObjectToDataTypeVector(oArgTypes), returnType, iAlignment)
	{
	}

	virtual std::list<Register_t> GetRegisters()
	{
		object registers = get_override("get_registers")();
		std::list<Register_t> result;
		for(int i=0; i < len(registers); i++)
		{
			result.push_back(extract<Register_t>(registers[i]));
		}

		return result;
	}

	virtual int GetPopSize()
	{
		return get_override("get_pop_size")();
	}
	
	virtual void* GetArgumentPtr(int iIndex, CRegisters* pRegisters)
	{
		CPointer* ptr = extract<CPointer*>(GetArgumentPtrWrapper(iIndex, pRegisters));
		return (void *) ptr->m_ulAddr;
	}

	object GetArgumentPtrWrapper(int iIndex, CRegisters* pRegisters)
	{
		return get_override("get_argument_ptr")(iIndex, ptr(pRegisters));
	}

	virtual void ArgumentPtrChanged(int iIndex, CRegisters* pRegisters, void* pArgumentPtr)
	{
		get_override("argument_ptr_changed")(iIndex, ptr(pRegisters), CPointer((unsigned long) pArgumentPtr));
	}

	virtual void* GetReturnPtr(CRegisters* pRegisters)
	{
		CPointer* ptr = extract<CPointer*>(GetReturnPtrWrapper(pRegisters));
		return (void *) ptr->m_ulAddr;
	}

	object GetReturnPtrWrapper(CRegisters* pRegisters)
	{
		return get_override("get_return_ptr")(ptr(pRegisters));
	}

	virtual void ReturnPtrChanged(CRegisters* pRegisters, void* pReturnPtr)
	{
		get_override("return_ptr_changed")(ptr(pRegisters), CPointer((unsigned long) pReturnPtr));
	}

public:
	tuple GetArgTypes()
	{
		return tuple(m_vecArgTypes);
	}
};


class CRegisterExt
{
public:
	static CPointer* GetAddress(CRegister& reg)
	{
		return new CPointer((unsigned long) reg.m_pAddress);
	}
};

void export_dynamichooks(scope _memory)
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

		.def("__repr__",
			&CStackData::__repr__
		)

		.add_property("registers",
			make_function(&CStackData::GetRegisters, reference_existing_object_policy())
		)
	;

	enum_<Register_t>("Register")
		// ========================================================================
		// >> 8-bit General purpose registers
		// ========================================================================
		.value("AL", AL)
		.value("CL", CL)
		.value("DL", DL)
		.value("BL", BL)

		.value("AH", AH)
		.value("CH", CH)
		.value("DH", DH)
		.value("BH", BH)

		// ========================================================================
		// >> 16-bit General purpose registers
		// ========================================================================
		.value("AX", AX)
		.value("CX", CX)
		.value("DX", DX)
		.value("BX", BX)
		.value("SP", SP)
		.value("BP", BP)
		.value("SI", SI)
		.value("DI", DI)

		// ========================================================================
		// >> 32-bit General purpose registers
		// ========================================================================
		.value("EAX", EAX)
		.value("ECX", ECX)
		.value("EDX", EDX)
		.value("EBX", EBX)
		.value("ESP", ESP)
		.value("EBP", EBP)
		.value("ESI", ESI)
		.value("EDI", EDI)

		// ========================================================================
		// >> 64-bit MM (MMX) registers
		// ========================================================================
		.value("MM0", MM0)
		.value("MM1", MM1)
		.value("MM2", MM2)
		.value("MM3", MM3)
		.value("MM4", MM4)
		.value("MM5", MM5)
		.value("MM6", MM6)
		.value("MM7", MM7)

		// ========================================================================
		// >> 128-bit XMM registers
		// ========================================================================
		.value("XMM0", XMM0)
		.value("XMM1", XMM1)
		.value("XMM2", XMM2)
		.value("XMM3", XMM3)
		.value("XMM4", XMM4)
		.value("XMM5", XMM5)
		.value("XMM6", XMM6)
		.value("XMM7", XMM7)

		// ========================================================================
		// >> 16-bit Segment registers
		// ========================================================================
		.value("CS", CS)
		.value("SS", SS)
		.value("DS", DS)
		.value("ES", ES)
		.value("FS", FS)
		.value("GS", GS)

		// ========================================================================
		// >> 80-bit FPU registers
		// ========================================================================
		.value("ST0", ST0)
		.value("ST1", ST1)
		.value("ST2", ST2)
		.value("ST3", ST3)
		.value("ST4", ST4)
		.value("ST5", ST5)
		.value("ST6", ST6)
		.value("ST7", ST7)
	;

	class_<CRegister, CRegister*, boost::noncopyable>("ProcessorRegister", no_init)
		.add_property("size",
			make_getter(&CRegister::m_iSize)
		)

		.add_property("address",
			make_function(&CRegisterExt::GetAddress, manage_new_object_policy())
		)
	;

	class_<CRegisters, boost::noncopyable>("Registers", no_init)
		.def_readonly("al", &CRegisters::m_al)
		.def_readonly("cl", &CRegisters::m_cl)
		.def_readonly("dl", &CRegisters::m_dl)
		.def_readonly("bl", &CRegisters::m_bl)

		.def_readonly("ah", &CRegisters::m_ah)
		.def_readonly("ch", &CRegisters::m_ch)
		.def_readonly("dh", &CRegisters::m_dh)
		.def_readonly("bh", &CRegisters::m_bh)
		
		.def_readonly("ax", &CRegisters::m_ax)
		.def_readonly("cx", &CRegisters::m_cx)
		.def_readonly("dx", &CRegisters::m_dx)
		.def_readonly("bx", &CRegisters::m_bx)
		.def_readonly("sp", &CRegisters::m_sp)
		.def_readonly("bp", &CRegisters::m_bp)
		.def_readonly("si", &CRegisters::m_si)
		.def_readonly("di", &CRegisters::m_di)
		
		.def_readonly("eax", &CRegisters::m_eax)
		.def_readonly("ecx", &CRegisters::m_ecx)
		.def_readonly("edx", &CRegisters::m_edx)
		.def_readonly("ebx", &CRegisters::m_ebx)
		.def_readonly("esp", &CRegisters::m_esp)
		.def_readonly("ebp", &CRegisters::m_ebp)
		.def_readonly("esi", &CRegisters::m_esi)
		.def_readonly("edi", &CRegisters::m_edi)
		
		.def_readonly("mm0", &CRegisters::m_mm0)
		.def_readonly("mm1", &CRegisters::m_mm1)
		.def_readonly("mm2", &CRegisters::m_mm2)
		.def_readonly("mm3", &CRegisters::m_mm3)
		.def_readonly("mm4", &CRegisters::m_mm4)
		.def_readonly("mm5", &CRegisters::m_mm5)
		.def_readonly("mm6", &CRegisters::m_mm6)
		.def_readonly("mm7", &CRegisters::m_mm7)
		
		.def_readonly("xmm0", &CRegisters::m_xmm0)
		.def_readonly("xmm1", &CRegisters::m_xmm1)
		.def_readonly("xmm2", &CRegisters::m_xmm2)
		.def_readonly("xmm3", &CRegisters::m_xmm3)
		.def_readonly("xmm4", &CRegisters::m_xmm4)
		.def_readonly("xmm5", &CRegisters::m_xmm5)
		.def_readonly("xmm6", &CRegisters::m_xmm6)
		.def_readonly("xmm7", &CRegisters::m_xmm7)
		
		.def_readonly("cs", &CRegisters::m_cs)
		.def_readonly("ss", &CRegisters::m_ss)
		.def_readonly("ds", &CRegisters::m_ds)
		.def_readonly("es", &CRegisters::m_es)
		.def_readonly("fs", &CRegisters::m_fs)
		.def_readonly("gs", &CRegisters::m_gs)
		
		.def_readonly("st0", &CRegisters::m_st0)

		// The following registers are currently not supported by DynamicHooks
		/*
		.def_readonly("st1", &CRegisters::m_st1)
		.def_readonly("st2", &CRegisters::m_st2)
		.def_readonly("st3", &CRegisters::m_st3)
		.def_readonly("st4", &CRegisters::m_st4)
		.def_readonly("st5", &CRegisters::m_st5)
		.def_readonly("st6", &CRegisters::m_st6)
		.def_readonly("st7", &CRegisters::m_st7)
		*/
	;
	
	class_<ICallingConventionWrapper, boost::noncopyable>("CallingConvention", init< object, DataType_t, optional<int> >())
		.def("get_registers",
			pure_virtual(&ICallingConventionWrapper::GetRegisters)
		)

		.def("get_pop_size",
			pure_virtual(&ICallingConventionWrapper::GetPopSize)
		)

		.def("get_argument_ptr",
			pure_virtual(&ICallingConventionWrapper::GetArgumentPtrWrapper)
		)

		.def("argument_ptr_changed",
			pure_virtual(&ICallingConventionWrapper::ArgumentPtrChanged)
		)

		.def("get_return_ptr",
			pure_virtual(&ICallingConventionWrapper::GetReturnPtrWrapper)
		)

		.def("return_ptr_changed",
			pure_virtual(&ICallingConventionWrapper::ReturnPtrChanged)
		)

		.add_property("argument_types",
			&ICallingConventionWrapper::GetArgTypes
		)

		.def_readonly("return_type",
			&ICallingConventionWrapper::m_returnType
		)

		.def_readonly("alignment",
			&ICallingConventionWrapper::m_iAlignment
		)

		ADD_MEM_TOOLS_WRAPPER(ICallingConventionWrapper, ICallingConvention)
	;

	def("get_data_type_size",
		&GetDataTypeSize,
		("data_type", arg("alignment")=4)
	);
}


//-----------------------------------------------------------------------------
// Exposes TYPE_SIZES
//-----------------------------------------------------------------------------
#define ADD_NATIVE_TYPE_SIZE(name, type) \
	scope().attr("TYPE_SIZES")[name] = sizeof(type);

#define ADD_SIZE(type) \
	ADD_NATIVE_TYPE_SIZE(XSTRINGIFY(type), type)

void export_sizes(scope _memory)
{
	_memory.attr("EXPOSED_CLASSES") = g_oExposedClasses;

	// Don't remove this! It's required for the ADD_NATIVE_TYPE_SIZE macro.
	_memory.attr("TYPE_SIZES") = dict();

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
