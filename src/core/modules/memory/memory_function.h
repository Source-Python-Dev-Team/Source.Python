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

#ifndef _MEMORY_FUNCTION_H
#define _MEMORY_FUNCTION_H

// ============================================================================
// >> INCLUDES
// ============================================================================
// Memory
#include "memory_pointer.h"

// DynamicHooks
#include "manager.h"


// ============================================================================
// >> Convention_t
// ============================================================================
enum Convention_t
{
	CONV_CUSTOM,
	CONV_CDECL,
	CONV_THISCALL,
	CONV_STDCALL,
	CONV_FASTCALL
};


// ============================================================================
// >> CFunction
// ============================================================================
class CFunction: public CPointer, private boost::noncopyable
{
public:
	CFunction(unsigned long ulAddr, object oCallingConvention, object oArgs, object oReturnType);
	CFunction(unsigned long ulAddr, Convention_t eCallingConvention, int iCallingConvention,
		boost::python::tuple tArgs, DataType_t eReturnType, object oConverter);

	~CFunction();

	bool IsCallable();
	bool IsHookable();

	bool IsHooked();

	CFunction* GetTrampoline();

	object Call(boost::python::tuple args, dict kw);
	object CallTrampoline(boost::python::tuple args, dict kw);
	object SkipHooks(boost::python::tuple args, dict kw);

	void AddHook(HookType_t eType, PyObject* pCallable);
	void RemoveHook(HookType_t eType, PyObject* pCallable);

	void AddPreHook(PyObject* pCallable)
	{ return AddHook(HOOKTYPE_PRE, pCallable); }

	void AddPostHook(PyObject* pCallable)
	{ return AddHook(HOOKTYPE_POST, pCallable); }

	void RemovePreHook(PyObject* pCallable)
	{ RemoveHook(HOOKTYPE_PRE, pCallable); }

	void RemovePostHook(PyObject* pCallable)
	{ RemoveHook(HOOKTYPE_POST, pCallable);	}

	void DeleteHook();

	bool AddHook(HookType_t eType, HookHandlerFn* pFunc);

public:
	boost::python::tuple	m_tArgs;
	object					m_oConverter;
	DataType_t				m_eReturnType;

	// Shared built-in calling convention identifier
	Convention_t			m_eCallingConvention;

	// DynCall calling convention
	int						m_iCallingConvention;

	// DynamicHooks calling convention (built-in and custom)
	ICallingConvention*		m_pCallingConvention;
	bool					m_bAllocatedCallingConvention;

	// Custom calling convention
	object					m_oCallingConvention;
};


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
ICallingConvention* MakeDynamicHooksConvention(Convention_t eConv, std::vector<DataType_t> vecArgTypes, DataType_t returnType, int iAlignment=4);

#endif // _MEMORY_FUNCTION_H