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
#ifndef _COMMANDS_H
#define _COMMANDS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "convar.h"
#include "utilities/ipythongenerator.h"
#include "boost/typeof/typeof.hpp" 


//-----------------------------------------------------------------------------
// Return values.
//-----------------------------------------------------------------------------
enum CommandReturn
{
	BLOCK = 0,
	CONTINUE
};


//-----------------------------------------------------------------------------
// CCommand extension class.
//-----------------------------------------------------------------------------
class CCommandExt
{
public:
	static PyObject* GetArg(CCommand& command, unsigned int iIndex)
	{
		if ((int) iIndex >= command.ArgC())
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");

		const char* szValue = command[iIndex];
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static PyObject* ArgS(CCommand& command)
	{
		const char* szValue = command.ArgS();
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static PyObject* GetCommandString(CCommand& command)
	{
		const char* szValue = command.GetCommandString();
		return PyUnicode_DecodeUTF8(szValue, strlen(szValue), "ignore");
	}

	static bool Tokenize(CCommand& command, const char* szCommand)
	{
		return command.Tokenize(szCommand);
	}
};


//-----------------------------------------------------------------------------
// ConCommandBase extension class.
//-----------------------------------------------------------------------------
class ConCommandBaseExt
{
public:
	static int GetFlags(ConCommandBase* command)
	{
		return command->m_nFlags;
	}

	static void SetFlags(ConCommandBase* command, int flags)
	{
		command->m_nFlags = flags;
	}

	static void AddFlags(ConCommandBase* command, int flags)
	{
		command->m_nFlags |= flags;
	}

	static void RemoveFlags(ConCommandBase* command, int flags)
	{
		command->m_nFlags &= ~flags;
	}
};


//-----------------------------------------------------------------------------
// CommandGenerator template
//-----------------------------------------------------------------------------
template<class CommandMap>
class CommandGenerator: public IPythonGenerator<const char>
{
public:
	CommandGenerator(PyObject* self)
		: IPythonGenerator<const char>(self)
	{
	}
	
	CommandGenerator(PyObject* self, const CommandGenerator<CommandMap>& rhs)
		: IPythonGenerator<const char>(self)
	{
		m_Begin = rhs.m_Begin;
		m_End = rhs.m_End;
	}

	virtual ~CommandGenerator()
	{
	}

protected:
	virtual const char* getNext()
	{
		if (m_Begin == m_End) {
			return NULL;
		}

		return (*m_Begin++).first.c_str();
	}

protected:
	typename CommandMap::const_iterator m_Begin;
	typename CommandMap::const_iterator m_End;
};


// Macro to generate the subclass that implements the CommandGenerator
#define COMMAND_GENERATOR(classname, command_map) \
	class classname: public CommandGenerator<BOOST_TYPEOF(command_map)> \
	{ \
	public: \
		classname(PyObject* self) \
			: CommandGenerator<BOOST_TYPEOF(command_map)>(self) \
		{ \
			m_Begin = command_map.cbegin(); \
			m_End = command_map.cend(); \
		} \
		\
		classname(PyObject* self, const classname& rhs) \
			: CommandGenerator<BOOST_TYPEOF(command_map)>(self, rhs) {} \
	}; \
	\
	BOOST_SPECIALIZE_HAS_BACK_REFERENCE(classname)

#define EXPOSE_COMMAND_GENERATOR(classname) \
	class_<classname>(XSTRINGIFY(classname)) \
		.def("__iter__", &classname::iter) \
		.def("__next__", &classname::next) \
	;

#endif // _COMMAND_H
