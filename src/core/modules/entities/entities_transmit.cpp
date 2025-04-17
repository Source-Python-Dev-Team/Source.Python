/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2021 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "modules/entities/entities_transmit.h"
#include "modules/memory/memory_function_info.h"
#include "utilities/conversions.h"
#include "modules/players/players_entity.h"

// Boost
#include "boost/foreach.hpp"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IServerGameEnts *gameents;
extern CGlobalVars *gpGlobals;


//-----------------------------------------------------------------------------
// CTransmitStates class.
//-----------------------------------------------------------------------------
bool CTransmitStates::__getitem__(int iBit)
{
	if (iBit < 0 || iBit > GetNumBits()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_IndexError,
			"Index out of range."
		)
	}

	return IsBitSet(iBit);
}

void CTransmitStates::__setitem__(int iBit, bool bValue)
{
	if (iBit < 0 || iBit > GetNumBits()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_IndexError,
			"Index out of range."
		)
	}

	return Set(iBit, bValue);
}


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
CTransmitManager::CTransmitManager():
	m_bInitialized(false),
	m_uiRefCount(0),
	m_pHook(NULL)
{
	m_pTransmitHooks = new CListenerManager();
}

CTransmitManager::~CTransmitManager()
{
	delete m_pTransmitHooks;
}

void CTransmitManager::IncRef()
{
	if (!m_uiRefCount) {
		Initialize();
	}

	++m_uiRefCount;
}

void CTransmitManager::DecRef()
{
	if (!m_uiRefCount) {
		return;
	}

	--m_uiRefCount;

	if (!m_uiRefCount) {
		Finalize();
	}
}

void CTransmitManager::Initialize()
{
	if (m_bInitialized) {
		return;
	}

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
	m_pHook = GetHookManager()->FindHook(pAddr);
	if (!m_pHook)
	{
		m_pHook = GetHookManager()->HookFunction(pAddr, pFunc->m_pCallingConvention);
		if (!m_pHook) {
			delete pFunc;
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook CheckTransmit."
			)
		}
	}

	delete pFunc;
	m_pHook->AddCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CTransmitManager::CheckTransmit
	);

	m_bInitialized = true;
}

void CTransmitManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	if (m_pHook) {
		m_pHook->RemoveCallback(
			HOOKTYPE_POST,
			(HookHandlerFn *)&CTransmitManager::CheckTransmit
		);
	}

	BOOST_FOREACH(TransmitCacheMap_t::value_type it, m_mapCache) {
		delete it.second;
	}

	m_mapCache.clear();
	m_bInitialized = false;
}

void CTransmitManager::RegisterRules(ITransmitRules *pRules)
{
	if (m_vecRules.HasElement(pRules)) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"The given rules are already registered."
		);
	}

	IncRef();
	m_vecRules.AddToTail(pRules);
}

void CTransmitManager::UnregisterRules(ITransmitRules *pRules)
{
	if (!m_vecRules.FindAndRemove(pRules)) {
		return;
	}

	DecRef();
}

void CTransmitManager::OnNetworkedEntityDeleted(CBaseEntityWrapper *pEntity, unsigned int uiEntity)
{
	FOR_EACH_VEC(m_vecRules, i) {
		m_vecRules[i]->OnEntityDeleted(pEntity);
	}

	BOOST_FOREACH(TransmitCacheMap_t::value_type it, m_mapCache) {
		it.second->DelResult(uiEntity);
	}

	TransmitCacheMap_t::const_iterator it = m_mapCache.find(uiEntity);
	if (it != m_mapCache.end()) {
		m_mapCache.erase(uiEntity);
		delete it->second;
	}
}

void CTransmitManager::OnLevelShutdown()
{
	FOR_EACH_VEC(m_vecRules, i) {
		m_vecRules[i]->Clear();
	}
}

void CTransmitManager::RegisterTransmitHook(object oCallback)
{
	m_pTransmitHooks->RegisterListener(oCallback.ptr());
	IncRef();
}

void CTransmitManager::UnregisterTransmitHook(object oCallback)
{
	m_pTransmitHooks->UnregisterListener(oCallback.ptr());
	DecRef();
}

bool CTransmitManager::CheckTransmit(HookType_t eHookType, CHook *pHook)
{
	int nEdicts = pHook->GetArgument<int>(3);
	if (!nEdicts) {
		return false;
	}

	CCheckTransmitInfo *pInfo = pHook->GetArgument<CCheckTransmitInfo *>(1);

	IPlayerInfo *pPlayerInfo = playerinfomanager->GetPlayerInfo(pInfo->m_pClientEnt);
	if (!pPlayerInfo || pPlayerInfo->IsFakeClient()) {
		return false;
	}

	unsigned int uiPlayer;
	if (!IndexFromEdict(pInfo->m_pClientEnt, uiPlayer)) {
		return false;
	}

	static CTransmitManager *pManager = GetTransmitManager();
	object oPlayer;

	unsigned short *pIndexes = pHook->GetArgument<unsigned short *>(2);

	if (pManager->m_pTransmitHooks->GetCount()) {
		oPlayer = GetEntityObject(uiPlayer);
		object oStates = object(ptr((CTransmitStates *)pInfo->m_pTransmitEdict));

		list oIndexes;
		for (int i=0; i < nEdicts; i++) {
			oIndexes.append((unsigned int)pIndexes[i]);
		}

		CALL_LISTENERS_WITH_MNGR(pManager->m_pTransmitHooks, oPlayer, oIndexes, oStates)
	}

	for (int i=0; i < nEdicts; i++)
	{
		unsigned int uiEntity = (unsigned int)pIndexes[i];
		if (uiEntity == WORLD_ENTITY_INDEX ||
				uiPlayer == uiEntity ||
				!pInfo->m_pTransmitEdict->IsBitSet(uiEntity)) {
			continue;
		}

		bool bResult = true;
		FOR_EACH_VEC(pManager->m_vecRules, j) {
			if (!pManager->m_vecRules[j]->ShouldTransmit(uiPlayer, uiEntity)) {
				bResult = false;
				break;
			}
		}

		if (!bResult) {
			pInfo->m_pTransmitEdict->Set((int)uiEntity, false);
			continue;
		}

#if defined(ENGINE_ORANGEBOX) || defined(ENGINE_BMS) || defined(ENGINE_GMOD)
		edict_t *pEdict = engine->PEntityOfEntIndex(uiEntity);
#else
		edict_t *pEdict = (edict_t *)(gpGlobals->pEdicts + uiEntity);
#endif

		if (!pEdict) {
			continue;
		}

		if (uiEntity <= (unsigned int)gpGlobals->maxClients) {
			static CTransmitListenerManager *OnPlayerTransmit = GetOnPlayerTransmitListenerManager();

			if (!OnPlayerTransmit->GetCount() ||
					((PlayerMixin *)pEdict->GetUnknown()->GetBaseEntity())->GetLifeState() != LIFE_ALIVE) {
				continue;
			}

			if (oPlayer.is_none()) {
				oPlayer = GetEntityObject(uiPlayer);
			}

			if (!OnPlayerTransmit->CallCallbacks(
					oPlayer,
					GetEntityObject(uiEntity))) {
				pInfo->m_pTransmitEdict->Set((int)uiEntity, false);
			}

			continue;
		}

		static CTransmitListenerManager *OnEntityTransmit = GetOnEntityTransmitListenerManager();

		if (!OnEntityTransmit->GetCount() ||
				pEdict->m_fStateFlags & FL_EDICT_ALWAYS) {
			continue;
		}

		CTransmitCache *pCache = pManager->GetCache(uiPlayer);

		if (!(pEdict->m_fStateFlags & FL_EDICT_CHANGED) && pCache->HasResult(uiEntity)) {
			pInfo->m_pTransmitEdict->Set((int)uiEntity, pCache->GetResult(uiEntity));
			continue;
		}

		if (oPlayer.is_none()) {
			oPlayer = GetEntityObject(uiPlayer);
		}

		if (!OnEntityTransmit->CallCallbacks(oPlayer, GetEntityObject(uiEntity))) {
			pCache->SetResult(uiEntity, false);
			pInfo->m_pTransmitEdict->Set((int)uiEntity, false);
		}
		else {
			pCache->SetResult(uiEntity, true);
		}
	}

	return false;
}

CTransmitCache *CTransmitManager::GetCache(unsigned int uiIndex)
{
	TransmitCacheMap_t::const_iterator it = m_mapCache.find(uiIndex);
	if (it != m_mapCache.end()) {
		return it->second;
	}

	CTransmitCache *pCache = new CTransmitCache();
	m_mapCache[uiIndex] = pCache;

	return pCache;
}


//-----------------------------------------------------------------------------
// CTransmitCache class.
//-----------------------------------------------------------------------------
bool CTransmitCache::HasResult(unsigned int uiIndex)
{
	return IsBitSet(uiIndex);
}

bool CTransmitCache::GetResult(unsigned int uiIndex)
{
	return m_vecCache.IsBitSet(uiIndex);
}

void CTransmitCache::SetResult(unsigned int uiIndex, bool bResult)
{
	Set(uiIndex);
	m_vecCache.Set((int)uiIndex, bResult);
}

void CTransmitCache::DelResult(unsigned int uiIndex)
{
	Set((int)uiIndex, false);
}


//-----------------------------------------------------------------------------
// ITransmitRules class.
//-----------------------------------------------------------------------------
ITransmitRules::~ITransmitRules()
{
	UnloadInstance();
}

void ITransmitRules::UnloadInstance()
{
	static CTransmitManager *pManager = GetTransmitManager();
	pManager->UnregisterRules(this);
}

bool ITransmitRules::ShouldTransmit(CBaseEntityWrapper *pPlayer, CBaseEntityWrapper *pEntity)
{
	return ShouldTransmit(pPlayer->GetIndex(), pEntity->GetIndex());
}

ETransmitMode ITransmitRules::GetMode()
{
	return m_eMode;
}

void ITransmitRules::SetMode(ETransmitMode eMode)
{
	m_eMode = eMode;
}


//-----------------------------------------------------------------------------
// CTransmitHash class.
//-----------------------------------------------------------------------------
void CTransmitHash::OnEntityDeleted(CBaseEntityWrapper *pEntity)
{
	RemovePairs(pEntity);
}

bool CTransmitHash::ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity)
{
	if (!HasElements()) {
		return true;
	}

	bool bResult = !HasPair(uiPlayer, uiEntity);
	return GetMode() == TRANSMIT_MODE_ALLOW ? !bResult : bResult;
}

void CTransmitHash::AddPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther)
{
	if (!pEntity->IsPlayer() && !pOther->IsPlayer()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"At least one of the entity needs to be a player."
		)
	}

	m_setPairs.insert(TransmitPair_t(pEntity->GetIndex(), pOther->GetIndex()));
}

void CTransmitHash::RemovePair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther)
{
	m_setPairs.erase(TransmitPair_t(pEntity->GetIndex(), pOther->GetIndex()));
}

void CTransmitHash::RemovePairs(CBaseEntityWrapper *pEntity)
{
	unsigned int uiEntity = pEntity->GetIndex();
	for (TransmitPairs_t::const_iterator it = m_setPairs.begin(); it != m_setPairs.end(); ) {
		if (it->first == uiEntity || it->second == uiEntity) {
			it = m_setPairs.erase(it);
			continue;
		}

		++it;
	}
}

void CTransmitHash::Clear()
{
	m_setPairs.clear();
}

bool CTransmitHash::Contains(CBaseEntityWrapper *pEntity)
{
	unsigned int uiEntity = pEntity->GetIndex();

	BOOST_FOREACH(TransmitPair_t p, m_setPairs) {
		if (p.first == uiEntity || p.second == uiEntity) {
			return true;
		}
	}

	return false;
}

bool CTransmitHash::HasPair(unsigned int uiEntity, unsigned int uiOther)
{
	return m_setPairs.find(TransmitPair_t(uiEntity, uiOther)) != m_setPairs.end();
}

bool CTransmitHash::HasPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther)
{
	return HasPair(pEntity->GetIndex(), pOther->GetIndex());
}

unsigned int CTransmitHash::GetCount(CBaseEntityWrapper *pEntity)
{
	unsigned int nCount = 0;
	unsigned int uiEntity = pEntity->GetIndex();

	BOOST_FOREACH(TransmitPair_t p, m_setPairs) {
		if (p.first == uiEntity || p.second == uiEntity) {
			++nCount;
		}
	}

	return nCount;
}

list CTransmitHash::GetPairs(CBaseEntityWrapper *pEntity)
{
	list oObjects;
	unsigned int uiEntity = pEntity->GetIndex();

	BOOST_FOREACH(TransmitPair_t p, m_setPairs) {
		if (p.first == uiEntity) {
			oObjects.append(GetEntityObject(p.second));
		}
		else if (p.second == uiEntity) {
			oObjects.append(GetEntityObject(p.first));
		}
	}

	return oObjects;
}

unsigned int CTransmitHash::GetSize()
{
	return m_setPairs.size();
}

bool CTransmitHash::HasElements()
{
	return !m_setPairs.empty();
}

object CTransmitHash::Iterate()
{
	list oEntities;

	if (HasElements()) {
		BOOST_FOREACH(TransmitPair_t it, m_setPairs) {
			oEntities.append(make_tuple(GetEntityObject(it.first), GetEntityObject(it.second)));
		}
	}

	return oEntities.attr("__iter__")();
}


//-----------------------------------------------------------------------------
// CTransmitSet class.
//-----------------------------------------------------------------------------
void CTransmitSet::Add(CBaseEntityWrapper *pEntity)
{
	if (Contains(pEntity)) {
		return;
	}

	if (!pEntity->IsNetworked()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Given entity is not networked."
		)
	}

	m_pSet.insert(pEntity->GetIndex());
}

void CTransmitSet::Remove(CBaseEntityWrapper *pEntity)
{
	m_pSet.erase(pEntity->GetIndex());
}

bool CTransmitSet::Contains(unsigned int uiEntity)
{
	return m_pSet.find(uiEntity) != m_pSet.end();
}

bool CTransmitSet::Contains(CBaseEntityWrapper *pEntity)
{
	return Contains(pEntity->GetIndex());
}

void CTransmitSet::Clear()
{
	m_pSet.clear();
}

unsigned int CTransmitSet::GetSize()
{
	return m_pSet.size();
}

bool CTransmitSet::HasElements()
{
	return !m_pSet.empty();
}

object CTransmitSet::Iterate()
{
	list oEntities;

	if (HasElements()) {
		BOOST_FOREACH(unsigned int uiEntity, m_pSet) {
			oEntities.append(GetEntityObject(uiEntity));
		}
	}

	return oEntities.attr("__iter__")();
}

void CTransmitSet::OnEntityDeleted(CBaseEntityWrapper *pEntity)
{
	Remove(pEntity);
};

bool CTransmitSet::ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity)
{
	if (!HasElements()) {
		return true;
	}

	bool bResult = !(m_pSet.find(uiEntity) != m_pSet.end());
	return GetMode() == TRANSMIT_MODE_ALLOW ? !bResult : bResult;
};


//-----------------------------------------------------------------------------
// CTransmitMap class.
//-----------------------------------------------------------------------------
boost::shared_ptr<CTransmitSet> CTransmitMap::Find(unsigned int uiEntity)
{
	TransmitMap_t::const_iterator it = m_mapSets.find(uiEntity);
	if (it != m_mapSets.end()) {
		return it->second;
	}

	CTransmitSet *pSet = new CTransmitSet;
	pSet->SetMode(GetMode());

	boost::shared_ptr<CTransmitSet> spSet = boost::shared_ptr<CTransmitSet>(pSet);
	m_mapSets[uiEntity] = spSet;

	return spSet;
}

boost::shared_ptr<CTransmitSet> CTransmitMap::Find(CBaseEntityWrapper *pEntity)
{
	return Find(pEntity->GetIndex());
}

void CTransmitMap::Remove(CBaseEntityWrapper *pEntity)
{
	m_mapSets.erase(pEntity->GetIndex());
}

void CTransmitMap::Clear()
{
	m_mapSets.clear();
}

bool CTransmitMap::Contains(unsigned int uiEntity)
{
	return m_mapSets.find(uiEntity) != m_mapSets.end();
}

bool CTransmitMap::Contains(CBaseEntityWrapper *pEntity)
{
	return Contains(pEntity->GetIndex());
}

unsigned int CTransmitMap::GetSize()
{
	return m_mapSets.size();
}

bool CTransmitMap::HasElements()
{
	return !m_mapSets.empty();
}

object CTransmitMap::Iterate()
{
	list oEntities;

	if (HasElements()) {
		BOOST_FOREACH(TransmitMap_t::value_type it, m_mapSets) {
			oEntities.append(make_tuple(GetEntityObject(it.first), object(it.second)));
		}
	}

	return oEntities.attr("__iter__")();
}

void CTransmitMap::OnEntityDeleted(CBaseEntityWrapper *pEntity)
{
	BOOST_FOREACH(TransmitMap_t::value_type it, m_mapSets) {
		it.second->OnEntityDeleted(pEntity);
	}

	Remove(pEntity);
}

bool CTransmitMap::ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity)
{
	TransmitMap_t::const_iterator it = m_mapSets.find(uiEntity);
	if (it == m_mapSets.end()) {
		return true;
	}

	CTransmitSet *pSet = get_pointer(it->second);
	if (!pSet || !pSet->HasElements()) {
		return GetMode() != TRANSMIT_MODE_ALLOW;
	}

	bool bResult = !pSet->Contains(uiPlayer);
	return GetMode() == TRANSMIT_MODE_ALLOW ? !bResult : bResult;
}


//-----------------------------------------------------------------------------
// CTransmitListenerManager class.
//-----------------------------------------------------------------------------
CTransmitListenerManager::CTransmitListenerManager():
	m_bInitialized(false)
{

}

void CTransmitListenerManager::Initialize()
{
	if (m_bInitialized) {
		return;
	}

	static CTransmitManager *pManager = GetTransmitManager();
	pManager->IncRef();
	m_bInitialized = true;
}

void CTransmitListenerManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	static CTransmitManager *pManager = GetTransmitManager();
	pManager->DecRef();
	m_bInitialized = false;
}

bool CTransmitListenerManager::CallCallbacks(object oEntity, object oOther)
{
	FOR_EACH_VEC(m_vecCallables, i) {
		BEGIN_BOOST_PY()
			object oResult = m_vecCallables[i](oEntity, oOther);
			if (!oResult.is_none() && !extract<bool>(oResult)) {
				return false;
			}
		END_BOOST_PY_NORET()
	}

	return true;
}


//-----------------------------------------------------------------------------
// Singleton accessors.
//-----------------------------------------------------------------------------
static CTransmitListenerManager s_OnPlayerTransmit;
CTransmitListenerManager *GetOnPlayerTransmitListenerManager()
{
	return &s_OnPlayerTransmit;
}

static CTransmitListenerManager s_OnEntityTransmit;
CTransmitListenerManager *GetOnEntityTransmitListenerManager()
{
	return &s_OnEntityTransmit;
}
