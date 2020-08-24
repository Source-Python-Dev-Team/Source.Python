/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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
// Includes.
//-----------------------------------------------------------------------------
#include "eiface.h"

#include "utilities/conversions.h"
#include "modules/entities/entities_transmit.h"
#include "modules/memory/memory_pointer.h"
#include "modules/memory/memory_function_info.h"


//-----------------------------------------------------------------------------
// Eternal variables.
//-----------------------------------------------------------------------------
extern IServerGameEnts *gameents;


//-----------------------------------------------------------------------------
// Static variables.
//-----------------------------------------------------------------------------
std::once_flag CTransmitManager::init_flag;
CTransmitManager* CTransmitManager::instance = nullptr;


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
CTransmitManager::CTransmitManager()
{
	initialize();

	for( auto& filters: m_vecFilters)
	{
		filters.first.SetAll();
	}
}


CTransmitManager* CTransmitManager::get_instance()
{
	std::call_once(init_flag, create);
	return instance;
}


void CTransmitManager::create()
{
	instance = new CTransmitManager;
}


void CTransmitManager::destroy()
{
	if(instance)
		instance->finalize();

	delete instance;
	instance = nullptr;
}


void CTransmitManager::initialize()
{
	CFunctionInfo *pInfo = GetFunctionInfo(&IServerGameEnts::CheckTransmit);
	if (!pInfo)
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Failed to retrieve CheckTransmit's info."
		)

	CFunction *pFunc = CPointer((unsigned long)((void *)gameents)).MakeVirtualFunction(*pInfo);
	delete pInfo;

	if (!pFunc || !pFunc->IsHookable())
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"CheckTransmit is invalid or not hookable."
		)

	m_pCheckTransmit = (void *)pFunc->m_ulAddr;
	CHook *pHook = GetHookManager()->FindHook(m_pCheckTransmit);
	if (!pHook)
	{
		pHook = GetHookManager()->HookFunction(m_pCheckTransmit, pFunc->m_pCallingConvention);
		if (!pHook)
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook CheckTransmit."
			)
	}

	pFunc->m_bAllocatedCallingConvention = false;
	delete pFunc;

	pHook->AddCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CTransmitManager::_post_check_transmit
	);
}


void CTransmitManager::finalize()
{
	CHook *pHook = GetHookManager()->FindHook(m_pCheckTransmit);
	if (pHook)
	{
		pHook->RemoveCallback(
			HOOKTYPE_POST,
			(HookHandlerFn *)&CTransmitManager::_post_check_transmit
		);
	}
}


void CTransmitManager::hide(int entity_index)
{
	if (entity_index == 0)
		return;

    for(int i = 0; i < m_vecFilters.size(); ++i)
    {
		if ((i+1) == entity_index)
			continue;

		filter_pair& filters = m_vecFilters[i];

		filters.first.Set(entity_index, false);
		filters.second.Set(entity_index, false);
    }
}


void CTransmitManager::hide_from(int entity_index, int player_index)
{
	if (entity_index == 0)
		return;

	player_index -= 1;

	if (entity_index == player_index)
		return;

	filter_pair& filters  = m_vecFilters[player_index];

	filters.first.Set(entity_index, false);
	filters.second.Set(entity_index, false);
}


void CTransmitManager::unhide(int entity_index)
{
	for( auto& filters: m_vecFilters)
	{
		filters.first.Set(entity_index, true);
		filters.second.Set(entity_index, true);
	}
}


void CTransmitManager::unhide_from(int entity_index, int player_index)
{
	player_index -= 1;

	filter_pair& filters = m_vecFilters[player_index];

	filters.first.Set(entity_index, true);
	filters.second.Set(entity_index, true);
}


void CTransmitManager::reset(int entity_index)
{
	for( auto& filters: m_vecFilters)
	{
		filters.first.Set(entity_index, true);
		filters.second.Set(entity_index, false);
	}
}


void CTransmitManager::reset_from(int entity_index, int player_index)
{
	player_index -= 1;
	m_vecFilters[player_index].first.Set(entity_index, true);
	m_vecFilters[player_index].second.Set(entity_index, false);
}


void CTransmitManager::reset_all()
{
	for( auto& filters: m_vecFilters)
	{
		filters.first.SetAll();
		filters.second.ClearAll();
	}
}


bool CTransmitManager::is_hidden(int entity_index)
{
	bool hidden_state = false;

	for( auto& filters: m_vecFilters)
	{
		hidden_state |= (!filters.first.IsBitSet(entity_index));
	}

	return hidden_state;
}


bool CTransmitManager::is_hidden_from(int entity_index, int player_index)
{
	player_index -= 1;

	return (!m_vecFilters[player_index].first.IsBitSet(entity_index));
}


tuple CTransmitManager::get_hidden_states(int entity_index)
{
	list player_list;

    for(int i = 0; i < m_vecFilters.size(); ++i)
    {
        if (!m_vecFilters[i].first.IsBitSet(entity_index))
			player_list.append(i+1);
    }

	return tuple(player_list);
}


void CTransmitManager::handle_filters(CCheckTransmitInfo* pInfo, unsigned int player_index)
{
	player_index -= 1;

	TransmitStates_t* transmit_edict = pInfo->m_pTransmitEdict;
	filter_pair& filters = m_vecFilters[player_index];

	transmit_edict->And(filters.first, transmit_edict);
	transmit_edict->Or(filters.second, transmit_edict);
}


bool CTransmitManager::_post_check_transmit(HookType_t eHookType, CHook* pHook)
{
	int nEdicts = pHook->GetArgument<int>(3);
	if (!nEdicts)
		return false;

	CCheckTransmitInfo* pInfo = pHook->GetArgument<CCheckTransmitInfo*>(1);

	unsigned int player_index;
	if (!IndexFromEdict(pInfo->m_pClientEnt, player_index))
		return false;

	instance->handle_filters(pInfo, player_index);

	return false;
}

