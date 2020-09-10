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
#include "strtools.h"

#include "utilities/conversions.h"
#include "modules/entities/entities_transmit.h"
#include "modules/memory/memory_pointer.h"
#include "modules/memory/memory_function_info.h"


//-----------------------------------------------------------------------------
// Eternal variables.
//-----------------------------------------------------------------------------
extern IServerGameEnts *gameents;
extern CGlobalVars *gpGlobals;
extern IPlayerInfoManager *playerinfomanager;


//-----------------------------------------------------------------------------
// Static variables.
//-----------------------------------------------------------------------------
CTransmitManager* CTransmitManager::instance = nullptr;


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
CTransmitManager::CTransmitManager()
	:m_arrayFilterIndexes()
{
	CFunctionInfo *pFunctionInfo = GetFunctionInfo(&IServerGameEnts::CheckTransmit);
	if (!pFunctionInfo)
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Failed to retrieve CheckTransmit's info."
		)

	CFunction *pFunction = CPointer((unsigned long)((void *)gameents)).MakeVirtualFunction(*pFunctionInfo);
	delete pFunctionInfo;

	if (!pFunction || !pFunction->IsHookable())
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"CheckTransmit is invalid or not hookable."
		)

	void *pAddr = (void *)pFunction->m_ulAddr;
	CHook *pHook = GetHookManager()->FindHook(pAddr);
	if (!pHook)
	{
		pHook = GetHookManager()->HookFunction(pAddr, pFunction->m_pCallingConvention);
		if (!pHook)
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook CheckTransmit."
			)
	}

	//m_pCheckTransmit = (void *)pFunction->m_ulAddr;
	delete pFunction;

	pHook->AddCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CTransmitManager::_post_check_transmit
	);

	current_clients = 0;

	IPlayerInfo* pPlayerInfo = nullptr;

	for (unsigned int i=1; i <= (unsigned int) gpGlobals->maxClients; ++i)
	{
		if (!PlayerInfoFromIndex(i, pPlayerInfo))
			continue;

		if (pPlayerInfo->IsFakeClient() || V_strstr(pPlayerInfo->GetNetworkIDString(), "BOT"))
			continue;

		++current_clients;
		m_arrayFilterIndexes[i-1] = current_clients;
	}

	unsigned int filter_size = bit_ceil(current_clients);
	if (filter_size < 4)
		filter_size = 4;

	m_vecFilters.resize(filter_size);

	for(auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		it->hide.SetAll();
	}
}


CTransmitManager::~CTransmitManager()
{
/*
	CHook *pHook = GetHookManager()->FindHook(m_pCheckTransmit);
	if (pHook)
		pHook->RemoveCallback(HOOKTYPE_POST, (HookHandlerFn *)&CTransmitManager::_post_check_transmit);
*/
}


CTransmitManager* CTransmitManager::get_instance()
{
	if (!instance)
		create();

	return instance;
}


void CTransmitManager::create()
{
	instance = new CTransmitManager;
}


/*
void CTransmitManager::destroy()
{
	delete instance;
	instance = nullptr;
}
*/


void CTransmitManager::add_player(edict_t* pEdict, unsigned int player_index)
{
	static ConVar* sv_replaybots = cvar->FindVar("sv_replaybots");
	static ConVar* sv_stressbots = cvar->FindVar("sv_stressbots");
	IPlayerInfo* pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);

	if ((pPlayerInfo &&
		(pPlayerInfo->IsFakeClient() || V_strstr(pPlayerInfo->GetNetworkIDString(), "BOT"))) && (
		(!sv_replaybots || !sv_replaybots->GetBool()) ||
		(!sv_stressbots || !sv_stressbots->GetBool())))
		return;

	unsigned int filter_size = m_vecFilters.size();
	if (current_clients >= filter_size)
		m_vecFilters.resize(filter_size*2);

	unsigned char filter_index;
	if (find_filter_index(player_index, filter_index))
	{
		++current_clients;
		m_arrayFilterIndexes[player_index-1] = filter_index;

		Filter& filter = m_vecFilters[filter_index-1];

		filter.hide.SetAll();
		filter.show.ClearAll();
	}
}


void CTransmitManager::remove_player(edict_t* pEdict, unsigned int player_index)
{
	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		--current_clients;
		m_arrayFilterIndexes[player_index-1] = 0;
	}
}


void CTransmitManager::hide(unsigned int entity_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	for(auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		it->hide.Set((int) entity_index, false);
		it->show.Set((int) entity_index, false);
	}

	reset_from(entity_index, entity_index);
}


void CTransmitManager::hide_from(unsigned int entity_index, unsigned int player_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return;

	if (entity_index == player_index)
		return;

	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		filter.hide.Set((int) entity_index, false);
		filter.show.Set((int) entity_index, false);
	}
}


void CTransmitManager::show(unsigned int entity_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	for( auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		it->hide.Set((int) entity_index, true);
		it->show.Set((int) entity_index, true);
	}
}


void CTransmitManager::show_from(unsigned int entity_index, unsigned int player_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return;

	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		filter.hide.Set((int) entity_index, true);
		filter.show.Set((int) entity_index, true);
	}
}


void CTransmitManager::reset(unsigned int entity_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	for( auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		it->hide.Set((int) entity_index, true);
		it->show.Set((int) entity_index, false);
	}
}


void CTransmitManager::reset_from(unsigned int entity_index, unsigned int player_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return;

	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return;

	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		filter.hide.Set((int) entity_index, true);
		filter.show.Set((int) entity_index, false);
	}
}


void CTransmitManager::reset_player(unsigned int player_index)
{
	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return;

	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		filter.hide.SetAll();
		filter.show.ClearAll();
	}
}


void CTransmitManager::reset_all()
{
	for( auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		it->hide.SetAll();
		it->show.ClearAll();
	}
}


bool CTransmitManager::is_hidden(unsigned int entity_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return false;

	bool hidden_state = false;

	for( auto it = m_vecFilters.begin(); it != m_vecFilters.begin()+current_clients; ++it)
	{
		hidden_state |= (!it->hide.IsBitSet((int) entity_index));
	}

	return hidden_state;
}


bool CTransmitManager::is_hidden_from(unsigned int entity_index, unsigned int player_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return false;

	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return false;

	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		return (!filter.hide.IsBitSet((int) entity_index));
	}

	return false;
}


boost::python::tuple CTransmitManager::get_hidden_player(unsigned int entity_index)
{
	if (entity_index < 1 || entity_index >= MAX_EDICTS)
		return boost::python::tuple();

	static boost::python::object Player = boost::python::import("players.entity").attr("Player");

	boost::python::list player_list;

	for (unsigned int i=1; i <= (unsigned int) gpGlobals->maxClients; ++i)
	{
		if (is_hidden_from(entity_index, i))
			player_list.append(Player(i));
	}

	return boost::python::tuple(player_list);
}


boost::python::tuple CTransmitManager::get_hidden_entity(unsigned int player_index)
{
	if (player_index < 1 || player_index > (unsigned int) gpGlobals->maxClients)
		return boost::python::tuple();

	static boost::python::object Entity = boost::python::import("entities.entity").attr("Entity");

	boost::python::list entity_list;

	for (unsigned int i=1; i < MAX_EDICTS; ++i)
	{
		if (is_hidden_from(i, player_index))
			entity_list.append(Entity(i));
	}

	return boost::python::tuple(entity_list);
}


void CTransmitManager::handle_filters(TransmitStates_t* pTransmitEdict, unsigned int player_index)
{
	unsigned char filter_index;
	if (get_filter_index(player_index, filter_index))
	{
		Filter& filter = m_vecFilters[filter_index-1];

		uint32* base = pTransmitEdict->Base();
		const uint32* hide = filter.hide.Base();
		const uint32* show = filter.show.Base();
		for (int i = 0; i < CalcNumIntsForBits(MAX_EDICTS); ++i)
		{
			base[i] = (base[i] & hide[i]) | show[i];
		}
	}
}


bool CTransmitManager::_post_check_transmit(HookType_t eHookType, CHook* pHook)
{
	int nEdicts = pHook->GetArgument<int>(3);
	if (!nEdicts)
		return false;

	CCheckTransmitInfo* pTransmitInfo = pHook->GetArgument<CCheckTransmitInfo*>(1);

	unsigned int player_index;
	if (!IndexFromEdict(pTransmitInfo->m_pClientEnt, player_index))
		return false;

	instance->handle_filters(pTransmitInfo->m_pTransmitEdict, player_index);

	return false;
}

