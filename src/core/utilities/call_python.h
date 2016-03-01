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

#ifndef _CALL_PYTHON_H
#define _CALL_PYTHON_H

#ifdef _WIN32
	#pragma warning(disable : 4996)
#endif

// ----------------------------------------------------------------------------
// Includes.
// ----------------------------------------------------------------------------
#include "boost/python/call.hpp"
#include "utilities/wrap_macros.h"

// ----------------------------------------------------------------------------
// Namespaces
// ----------------------------------------------------------------------------
using namespace boost::python;


// ----------------------------------------------------------------------------
// This macro allows us to call a Python function OR Python method. It decides
// what we need to use.
// boost::python::object retval = CALL_PY_FUNC(PyObject*, ...);
// ----------------------------------------------------------------------------
template<class T>
T xdecref_and_return(T value)
{
	xdecref(value);
	return value;
}

#define CALL_PY_FUNC(pCallable, ...) \
	PyObject_HasAttrString(pCallable, "__self__") ? \
		boost::python::call_method<object>( \
			xdecref_and_return(PyObject_GetAttrString(pCallable, "__self__")), \
			extract<const char*>(xdecref_and_return(PyObject_GetAttrString(pCallable, "__name__"))), \
			##__VA_ARGS__ \
		) : call<object>(pCallable, ##__VA_ARGS__)


// ----------------------------------------------------------------------------
// Python Logging functions
// ----------------------------------------------------------------------------
/*
    (-1) message (should always be printed)
    (0)  critical
    (1)  exception
    (2)  warning
    (3)  info
    (4)  debug
*/
inline void PythonLog( const char* szLevel, const char* szFormat, ... )
{
	// Format the message
	char szMessage[2048];
	va_list args;
	va_start(args, szFormat);
	vsprintf(szMessage, szFormat, args);
	va_end(args);

	// Get the std::string instance of szMessage
	std::string szMethod = szLevel;

	// Get the method's name
	std::string szMethodName = "log_" + szMethod;

	BEGIN_BOOST_PY()

		// Import the loggers module
		object oLogModule = import("loggers");

		// Get the main SP Logger instance
		object oLogger = oLogModule.attr("_sp_logger");

		// Get the PyObject instance of the logger
		PyObject* poLogger = oLogger.ptr();

		// Call the given method
		call_method<void>( poLogger, szMethodName.c_str(), szMessage );

	END_BOOST_PY()
}

inline void PythonLog( int iLevel, const char* szFormat, ... )
{
	// Format the message
	char szMessage[2048];
	va_list args;
	va_start(args, szFormat);
	vsprintf(szMessage, szFormat, args);
	va_end(args);

	BEGIN_BOOST_PY()

		// Import the loggers module
		object oLogModule = import("loggers");

		// Get the main SP Logger instance
		object oLogger = oLogModule.attr("_sp_logger");

		// Get the PyObject instance logger
		PyObject* poLogger = oLogger.ptr();

		// Call the log method with the given level
		call_method<void>( poLogger, "log", iLevel, szMessage );

	END_BOOST_PY()
}

#endif // _CALL_PYTHON_H
