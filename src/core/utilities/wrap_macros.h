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
#ifndef _WRAP_MACROS_H
#define _WRAP_MACROS_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "boost/function.hpp"
#include "boost/python.hpp"
using namespace boost::python;


//---------------------------------------------------------------------------------
// Define checks
//---------------------------------------------------------------------------------
#if !defined(SOURCE_ENGINE)
#error("SOURCE_ENGINE define must be globally defined, did we forget?");
#elif !defined(SOURCE_ENGINE_BRANCH)
#error("SOURCE_ENGINE_BRANCH define must be globally defined, did we forget?");
#endif

//---------------------------------------------------------------------------------
// Macros to expose function typedefs
//---------------------------------------------------------------------------------
template<class Function>
inline void* GetFuncPtr(Function func)
{
	return (void*) func.functor.members.func_ptr;
}

// Example typedef:
// typedef int (*MultiplyFn)(int, int);
// 
// Example usage:
// BOOST_FUNCTION_TYPEDEF(int (int, int), BoostMultiplyFn)
#define BOOST_FUNCTION_TYPEDEF(sig, cpp_name) \
	typedef boost::function< sig> cpp_name;

// Example usage:
// EXPOSE_FUNCTION_TYPEDEF(BoostMultiplyFn, "MultiplyFn")
#define EXPOSE_FUNCTION_TYPEDEF(cpp_name, py_name) \
	class_<cpp_name>(py_name, no_init) \
		.def("__call__", &cpp_name::operator()) \
		.def(GET_PTR_NAME, &GetFuncPtr<cpp_name>, return_by_value_policy()) \
	;

#define EXPOSE_FUNCTION_TYPEDEF_RET(cpp_name, py_name, policy) \
	class_<cpp_name>(py_name, no_init) \
		.def("__call__", &cpp_name::operator(), policy) \
		.def(GET_PTR_NAME, &GetFuncPtr<cpp_name>, return_by_value_policy()) \
	;

//---------------------------------------------------------------------------------
// Surround boost python statements with this macro in order to handle exceptions.
//---------------------------------------------------------------------------------
#define BEGIN_BOOST_PY() \
	try {

#define END_BOOST_PY( ... ) \
	} catch( ... ) { \
		PyErr_Print(); \
		PyErr_Clear(); \
		return __VA_ARGS__; \
	}

#define END_BOOST_PY_NORET( ... ) \
	} catch( ... ) { \
		PyErr_Print(); \
		PyErr_Clear(); \
	}

//---------------------------------------------------------------------------------
// Use this macro to raise a Python exception.
//---------------------------------------------------------------------------------
#define BOOST_RAISE_EXCEPTION( exceptionName, exceptionString, ... ) \
    { \
		PyErr_Format(exceptionName, exceptionString, ##__VA_ARGS__); \
		throw_error_already_set(); \
    }

//---------------------------------------------------------------------------------
// This macro will turn input into a string.
//---------------------------------------------------------------------------------
#define XSTRINGIFY(s) STRINGIFY(s)
#define STRINGIFY(s) #s

//---------------------------------------------------------------------------------
// This macro returns the given overloaded method
//---------------------------------------------------------------------------------
#define GET_METHOD(return_type, class_name, method, ...) \
	static_cast< return_type (class_name::*)( __VA_ARGS__ ) >(&class_name::method)

#define GET_CONST_METHOD(return_type, class_name, method, ...) \
	static_cast< return_type (class_name::*)( __VA_ARGS__ ) const >(&class_name::method)

//---------------------------------------------------------------------------------
// This macro returns the given overloaded function
//---------------------------------------------------------------------------------
#define GET_FUNCTION(return_type, function, ...) \
	static_cast< return_type(*)( __VA_ARGS__ ) >(&function)


//---------------------------------------------------------------------------------
// Use this template to create variadic class methods
//---------------------------------------------------------------------------------
template<class T>
object raw_method(T method)
{
	return eval("lambda method: lambda *args, **kw: method(args[0], args[1:], kw)")(make_function(method));
}

//---------------------------------------------------------------------------------
// Use this macro to define a function or class method that raises a
// NotImplementedError. This is quite hacky, but saves a lot work!
//---------------------------------------------------------------------------------
inline object _NotImplementedOnThisEngine(boost::python::tuple args, boost::python::dict kw)
{
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Not implemented on this engine.");
	return object();
}

#define NOT_IMPLEMENTED(name) \
	def(name, raw_function(&_NotImplementedOnThisEngine), "Not implemented on this engine.")

#define NOT_IMPLEMENTED_ATTR(name) \
	add_property(name, raw_function(&_NotImplementedOnThisEngine), raw_function(&_NotImplementedOnThisEngine), "Not implemented on this engine.")

//---------------------------------------------------------------------------------
// Use this macro to default a not implemented value to None.
//---------------------------------------------------------------------------------
#define NOT_IMPLEMENTED_VALUE(classname, name) \
	PyDict_SetItemString( \
		converter::registry::query(typeid(classname))->m_class_object->tp_dict, \
		name, \
		object().ptr() \
	);

//---------------------------------------------------------------------------------
// Use this macro to check the return value of a get_override(...) call.
//---------------------------------------------------------------------------------
#define CHECK_OVERRIDE(override) \
	if (override.is_none()) \
		BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "Method must be implemented by a subclass.")

//---------------------------------------------------------------------------------
// Use this macro to add a specialization for a class to hold back-references.
//---------------------------------------------------------------------------------
#define BOOST_SPECIALIZE_HAS_BACK_REFERENCE( classname ) \
	namespace boost { namespace python { \
		template<> \
		struct has_back_reference<classname> : mpl::true_ \
		{}; \
	}}

//---------------------------------------------------------------------------------
// These macros allow us to include engine / branch specific files
//---------------------------------------------------------------------------------
// Current working directory macros
#define JOIN_PATH(folder, file_path)			XSTRINGIFY(folder/file_path)
#define ENGINE_INCLUDE_PATH(file_path)			JOIN_PATH(SOURCE_ENGINE, file_path)
#define ENGINE_BRANCH_INCLUDE_PATH(file_path)	JOIN_PATH(SOURCE_ENGINE_BRANCH, file_path)

//---------------------------------------------------------------------------------
// These typedefs save some typing. Use this policy for any functions that return
// a newly allocated instance of a class which you need to delete yourself.
//---------------------------------------------------------------------------------
typedef return_value_policy<manage_new_object> manage_new_object_policy;

//---------------------------------------------------------------------------------
// Use this policy for objects that someone else will free.
//---------------------------------------------------------------------------------
typedef return_value_policy<reference_existing_object> reference_existing_object_policy;

//---------------------------------------------------------------------------------
// Use this policy for functions that return const objects.
//---------------------------------------------------------------------------------
typedef return_value_policy<copy_const_reference> copy_const_reference_policy;

typedef return_value_policy<return_by_value> return_by_value_policy;

#endif // _WRAP_MACROS_H
