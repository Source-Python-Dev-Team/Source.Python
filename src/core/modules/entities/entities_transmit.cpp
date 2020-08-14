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
// CBaseTransmitCriteria class.
//-----------------------------------------------------------------------------
CBaseTransmitCriteria::CBaseTransmitCriteria(ETransmitTarget eTarget)
{
	m_eTarget = eTarget;
}


//-----------------------------------------------------------------------------
// CBaseTransmitFilter class.
//-----------------------------------------------------------------------------
CBaseTransmitFilter::CBaseTransmitFilter(ETransmitType eType, CBaseTransmitCriteria *pCriteria, object oOverride):
	m_oCallback(object()),
	m_bHasOverride(false)
{
	m_eType = eType;
	m_pCriteria = pCriteria;

	set_override(oOverride);
}


object CBaseTransmitFilter::get_callback()
{
	return m_oCallback;
}

void CBaseTransmitFilter::set_callback(object oCallback)
{
	if (!oCallback.is_none() && !PyCallable_Check(oCallback.ptr()))
		BOOST_RAISE_EXCEPTION(
			PyExc_TypeError,
			"The given function is not callable."
		);

	m_oCallback = oCallback;
}


bool CBaseTransmitFilter::get_override()
{
	return m_bOverride;
}

void CBaseTransmitFilter::set_override(object oOverride)
{
	if (oOverride.is_none())
	{
		m_bOverride = oOverride;
		m_bHasOverride = false;
	}
	else
	{
		extract<bool> extract_override(oOverride);
		if (!extract_override.check())
			BOOST_RAISE_EXCEPTION(
				PyExc_TypeError,
				"The given override couldn't be evaluated as a boolean."
			);

		m_bOverride = extract_override();
		m_bHasOverride = true;
	}
}

bool CBaseTransmitFilter::has_override()
{
	return m_bHasOverride;
}


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
CTransmitManager::CTransmitManager():
	m_bInitialized(false)
{
}

CTransmitManager *CTransmitManager::GetSingleton()
{
	static CTransmitManager *s_pManager = new CTransmitManager;
	return s_pManager;
}


void CTransmitManager::initialize()
{
	if (m_bInitialized)
		return;

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

	void *pAddr = (void *)pFunc->m_ulAddr;
	CHook *pHook = GetHookManager()->FindHook(pAddr);
	if (!pHook)
	{
		pHook = GetHookManager()->HookFunction(pAddr, pFunc->m_pCallingConvention);
		if (!pHook)
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook CheckTransmit."
			)
	}

	delete pFunc;
	pHook->AddCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CTransmitManager::_post_check_transmit
	);

	m_bInitialized = true;
}


void CTransmitManager::register_filter(CBaseTransmitFilter *pFilter)
{
	if (m_vecFilters.HasElement(pFilter))
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"The given filter is already registered."
		);

	initialize();
	m_vecFilters.AddToTail(pFilter);
}

void CTransmitManager::unregister_filter(CBaseTransmitFilter *pFilter)
{
	m_vecFilters.FindAndRemove(pFilter);
}


bool CTransmitManager::handle_filters(ETransmitType eType, int iIndex, unsigned int uiPlayer)
{
	static object Entity = import("entities.entity").attr("Entity");
	object entity = Entity(iIndex);

	static object Player = import("players.entity").attr("Player");
	object player = Player(uiPlayer);

	for (int i=0;i < m_vecFilters.Count();i++)
	{
		CBaseTransmitFilter *pFilter = m_vecFilters[i];

		if (pFilter->m_eType != eType)
			continue;

		if (pFilter->m_pCriteria)
		{
			switch (pFilter->m_pCriteria->m_eTarget)
			{
				case TRANSMIT_TARGET_ENTITY:
				{
					if (!pFilter->m_pCriteria->IsBitSet(iIndex))
						continue;
					break;
				}
				case TRANSMIT_TARGET_PLAYER:
				{
					if (!pFilter->m_pCriteria->IsBitSet((int)uiPlayer))
						continue;
					break;
				}
				default:
					continue;
			}
		}

		if (pFilter->has_override())
			return pFilter->get_override();

		object callback = pFilter->get_callback();
		if (!callback.is_none() && callback(entity, player))
			return true;
	}

	return false;
}


bool CTransmitManager::_post_check_transmit(HookType_t eHookType, CHook *pHook)
{
	static CTransmitManager *pManager = GetTransmitManager();
	if (!pManager->m_vecFilters.Count())
		return false;

	int nEdicts = pHook->GetArgument<int>(3);
	if (!nEdicts)
		return false;

	CCheckTransmitInfo *pInfo = pHook->GetArgument<CCheckTransmitInfo *>(1);

	unsigned int uiIndex;
	if (!IndexFromEdict(pInfo->m_pClientEnt, uiIndex))
		return false;

	static object Player = import("players.entity").attr("Player");
	object player = Player(uiIndex);

	unsigned short *pIndexes = pHook->GetArgument<unsigned short *>(2);

	for (int i=0; i < nEdicts; i++)
	{
		BEGIN_BOOST_PY()
			int iIndex = pIndexes[i];
			if (iIndex == WORLD_ENTITY_INDEX)
				continue;

			unsigned int uiPlayer;
			IndexFromEdict(pInfo->m_pClientEnt, uiPlayer);

			if (uiPlayer == iIndex)
				continue;

			ETransmitType eType = pInfo->m_pTransmitEdict->IsBitSet(iIndex) ? TRANSMIT_OUT : TRANSMIT_IN;
			if (!pManager->handle_filters(eType, iIndex, uiPlayer))
				continue;

			pInfo->m_pTransmitEdict->Set(iIndex, !pInfo->m_pTransmitEdict->Get(iIndex));
		END_BOOST_PY_NORET()
	}

	return false;
}
