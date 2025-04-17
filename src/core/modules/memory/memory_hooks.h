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

#ifndef MEMORY_HOOKS_H
#define MEMORY_HOOKS_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include <list>
#include <map>

#include "boost/python.hpp"
using namespace boost::python;

// DynamicHooks
#include "hook.h"

//---------------------------------------------------------------------------------
// Classes
//---------------------------------------------------------------------------------
class CStackData
{
public:
	CStackData(CHook* pHook);

	object		GetItem(unsigned int iIndex);
	void		SetItem(unsigned int iIndex, object value);

	CRegisters* GetRegisters()
	{ return m_pHook->GetRegisters(); }

	str	__repr__()
	{ return str(boost::python::tuple(ptr(this))); }

	void* GetReturnAddress()
	{
		void* pESP = m_pHook->GetRegisters()->m_esp->GetValue<void*>();
		if (m_pHook->m_RetAddr.count(pESP) == 0) {
			return NULL;
		}
		return m_pHook->m_RetAddr[pESP].back();
	}

	bool GetUsePreRegister()
	{
		return m_pHook->m_bUsePreRegisters;
	}

	void SetUsePreRegisters(bool value)
	{
		m_pHook->m_bUsePreRegisters = value;
	}

protected:
	CHook*                m_pHook;
	std::map<int, object> m_mapCache;
};


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool SP_HookHandler(HookType_t eHookType, CHook* pHook);

extern bool g_HooksDisabled;

inline void SetHooksDisabled(bool value)
{
	g_HooksDisabled = value;
}

inline bool GetHooksDisabled()
{
	return g_HooksDisabled;
}

#endif // MEMORY_HOOKS_H
