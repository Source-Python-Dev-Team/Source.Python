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

#ifndef _MEMORY_WRAP_H
#define _MEMORY_WRAP_H

// ============================================================================
// >> INCLUDES
// ============================================================================
// DynamicHooks
#include "convention.h"

// Utilities
#include "memory_utilities.h"
#include "memory_rtti.h"

// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;


// ============================================================================
// >> ICallingConventionWrapper
// ============================================================================
class ICallingConventionWrapper: public ICallingConvention, public wrapper<ICallingConvention>
{
public:
	ICallingConventionWrapper(object oArgTypes, DataType_t returnType, int iAlignment=4)
		:ICallingConvention(ObjectToDataTypeVector(oArgTypes), returnType, iAlignment)
	{
	}

	virtual std::list<Register_t> GetRegisters()
	{
		override get_registers = get_override("get_registers");
		CHECK_OVERRIDE(get_registers);

		object registers = get_registers();
		std::list<Register_t> result;
		for(int i=0; i < len(registers); i++)
		{
			result.push_back(extract<Register_t>(registers[i]));
		}

		return result;
	}

	virtual int GetPopSize()
	{
		override get_pop_size = get_override("get_pop_size");
		CHECK_OVERRIDE(get_pop_size);
		return get_pop_size();
	}
	
	virtual void* GetArgumentPtr(int iIndex, CRegisters* pRegisters)
	{
		CPointer* ptr = extract<CPointer*>(GetArgumentPtrWrapper(iIndex, pRegisters));
		return (void *) ptr->m_ulAddr;
	}

	object GetArgumentPtrWrapper(int iIndex, CRegisters* pRegisters)
	{
		override get_argument_ptr = get_override("get_argument_ptr");
		CHECK_OVERRIDE(get_argument_ptr);
		return get_argument_ptr(iIndex, ptr(pRegisters));
	}

	virtual void ArgumentPtrChanged(int iIndex, CRegisters* pRegisters, void* pArgumentPtr)
	{
		override argument_ptr_changed = get_override("argument_ptr_changed");
		CHECK_OVERRIDE(argument_ptr_changed);
		argument_ptr_changed(iIndex, ptr(pRegisters), CPointer((unsigned long) pArgumentPtr));
	}

	virtual void* GetReturnPtr(CRegisters* pRegisters)
	{
		CPointer* ptr = extract<CPointer*>(GetReturnPtrWrapper(pRegisters));
		return (void *) ptr->m_ulAddr;
	}

	object GetReturnPtrWrapper(CRegisters* pRegisters)
	{
		override get_return_ptr = get_override("get_return_ptr");
		CHECK_OVERRIDE(get_return_ptr);
		return get_return_ptr(ptr(pRegisters));
	}

	virtual void ReturnPtrChanged(CRegisters* pRegisters, void* pReturnPtr)
	{
		override return_ptr_changed = get_override("return_ptr_changed");
		CHECK_OVERRIDE(return_ptr_changed);
		return_ptr_changed(ptr(pRegisters), CPointer((unsigned long) pReturnPtr));
	}

public:
	tuple GetArgTypes()
	{
		return tuple(m_vecArgTypes);
	}
};


// ============================================================================
// >> CRegisterExt
// ============================================================================
class CRegisterExt
{
public:
	static CPointer* GetAddress(CRegister& reg)
	{
		return new CPointer((unsigned long) reg.m_pAddress);
	}
};


// ============================================================================
// >> IBaseTypeIter
// ============================================================================
class IBaseTypeIter
{
public:
	IBaseTypeIter(IBaseType* pType) {
		m_pType = pType;
		m_iIndex = 0;
	}

	IBaseType* __next__() {
		if (m_iIndex >= m_pType->GetNumBaseClasses()) {
			BOOST_RAISE_EXCEPTION(PyExc_StopIteration, "No more types.");
		}

		return m_pType->GetBaseClass(m_iIndex++);
	}

private:
	IBaseType* m_pType;
	size_t m_iIndex;
};


// ============================================================================
// >> IBaseTypeExt
// ============================================================================
class IBaseTypeExt
{
public:
	static IBaseTypeIter* __iter__(IBaseType* pType)
	{
		return new IBaseTypeIter(pType);
	}
};

#endif // _MEMORY_WRAP_H
