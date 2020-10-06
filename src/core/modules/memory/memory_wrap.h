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

// Memory
#include "memory_function.h"

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
	ICallingConventionWrapper(object oArgTypes, DataType_t returnType, int iAlignment=4, Convention_t eDefaultConv=CONV_CUSTOM)
		:ICallingConvention(ObjectToDataTypeVector(oArgTypes), returnType, iAlignment)
	{
		if (eDefaultConv != CONV_CUSTOM) {
			m_pDefaultCallingConvention = MakeDynamicHooksConvention(eDefaultConv, m_vecArgTypes, m_returnType, m_iAlignment);
		}
		else {
			m_pDefaultCallingConvention = nullptr;
		}
	}

	virtual ~ICallingConventionWrapper()
	{
		// If we are still flagged as hooked, then that means DynamicHooks is done with us.
		if (m_bHooked)
		{
			// Release the Python reference we reserved for DynamicHooks.
			PyObject *pOwner = detail::wrapper_base_::owner(this);
			if (pOwner && Py_REFCNT(pOwner))
				Py_DECREF(pOwner);
		}

		delete m_pDefaultCallingConvention;
		m_pDefaultCallingConvention = nullptr;
	}

	static void Deleter(ICallingConventionWrapper *pThis)
	{
		// If we are still hooked, DynamicHooks will take care of us.
		if (pThis->m_bHooked)
			return;

		// We are not hooked, nor referenced anymore so we can be deleted.
		delete pThis;
	}

	static boost::shared_ptr<ICallingConventionWrapper> __init__(
		object oArgTypes, DataType_t returnType, int iAlignment=4, Convention_t eDefaultConv=CONV_CUSTOM)
	{
		return boost::shared_ptr<ICallingConventionWrapper>(
			new ICallingConventionWrapper(oArgTypes, returnType, iAlignment, eDefaultConv), &Deleter
		);
	}

	void Initialize(object self, object oArgTypes, DataType_t returnType, int iAlignment, Convention_t eDefaultConv)
	{
		// If we didn't receive a default convention on construction, try to resolve one from the Python instance.
		if (!m_pDefaultCallingConvention)
		{
			try
			{
				m_pDefaultCallingConvention = MakeDynamicHooksConvention(
					extract<Convention_t>(self.attr("default_convention")), m_vecArgTypes, m_returnType, m_iAlignment
				);
			}
			catch (error_already_set &)
			{
				if (!PyErr_ExceptionMatches(PyExc_AttributeError))
					throw_error_already_set();

				PyErr_Clear();
			}
		}
	}

	virtual std::list<Register_t> GetRegisters()
	{
		override get_registers = get_override("get_registers");
		if (!get_registers && m_pDefaultCallingConvention) {
			return m_pDefaultCallingConvention->GetRegisters();
		}
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
		if (!get_pop_size && m_pDefaultCallingConvention) {
			return m_pDefaultCallingConvention->GetPopSize();
		}
		CHECK_OVERRIDE(get_pop_size);

		return get_pop_size();
	}

	virtual void* GetArgumentPtr(int iIndex, CRegisters* pRegisters)
	{
		override get_argument_ptr = get_override("get_argument_ptr");
		if (!get_argument_ptr && m_pDefaultCallingConvention) {
			return m_pDefaultCallingConvention->GetArgumentPtr(iIndex, pRegisters);
		}
		CHECK_OVERRIDE(get_argument_ptr);

		object argument_ptr = get_argument_ptr(iIndex, ptr(pRegisters));
		CPointer* _ptr = extract<CPointer*>(argument_ptr);
		return (void *) _ptr->m_ulAddr;
	}

	virtual void ArgumentPtrChanged(int iIndex, CRegisters* pRegisters, void* pArgumentPtr)
	{
		override argument_ptr_changed = get_override("argument_ptr_changed");
		if (!argument_ptr_changed && m_pDefaultCallingConvention) {
			m_pDefaultCallingConvention->ArgumentPtrChanged(iIndex, pRegisters, pArgumentPtr);
			return;
		}
		CHECK_OVERRIDE(argument_ptr_changed);
		argument_ptr_changed(iIndex, ptr(pRegisters), CPointer((unsigned long) pArgumentPtr));
	}

	virtual void* GetReturnPtr(CRegisters* pRegisters)
	{
		override get_return_ptr = get_override("get_return_ptr");
		if (!get_return_ptr && m_pDefaultCallingConvention) {
			return m_pDefaultCallingConvention->GetReturnPtr(pRegisters);
		}
		CHECK_OVERRIDE(get_return_ptr)

		object return_ptr = get_return_ptr(ptr(pRegisters));
		CPointer* _ptr = extract<CPointer*>(return_ptr);
		return (void *) _ptr->m_ulAddr;
	}

	virtual void ReturnPtrChanged(CRegisters* pRegisters, void* pReturnPtr)
	{
		override return_ptr_changed = get_override("return_ptr_changed");
		if (!return_ptr_changed && m_pDefaultCallingConvention) {
			m_pDefaultCallingConvention->ReturnPtrChanged(pRegisters, pReturnPtr);
			return;
		}
		CHECK_OVERRIDE(return_ptr_changed);

		return_ptr_changed(ptr(pRegisters), CPointer((unsigned long) pReturnPtr));
	}

	tuple GetArgTypes()
	{
		list argumentTypes;
		for(std::vector<DataType_t>::iterator it=this->m_vecArgTypes.begin(); it != this->m_vecArgTypes.end(); ++it)
		{
			argumentTypes.append(*it);
		}

		return tuple(argumentTypes);
	}

public:
	ICallingConvention* m_pDefaultCallingConvention;
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
