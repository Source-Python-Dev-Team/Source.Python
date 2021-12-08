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
#include "modules/entities/entities_collisions.h"
#include "utilities/conversions.h"

#ifdef __linux__
	#include "modules/memory/memory_rtti.h"
#endif


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IEngineTrace *enginetrace;
extern IServerTools *servertools;
extern CGlobalVars *gpGlobals;


//-----------------------------------------------------------------------------
// CCollisionManager class.
//-----------------------------------------------------------------------------
CCollisionManager::CCollisionManager():
	m_bInitialized(false),
	m_uiRefCount(0),
	m_nTickCount(-1)
{
	m_pCollisionHooks = new CListenerManager();
}

CCollisionManager::~CCollisionManager()
{
	delete m_pCollisionHooks;
}

void CCollisionManager::IncRef()
{
	if (!m_uiRefCount) {
		Initialize();
	}

	++m_uiRefCount;
}

void CCollisionManager::DecRef()
{
	if (!m_uiRefCount) {
		return;
	}

	--m_uiRefCount;

	if (!m_uiRefCount) {
		Finalize();
	}
}

void CCollisionManager::Initialize()
{
	if (m_bInitialized) {
		return;
	}

	RegisterHook(&IEngineTrace::TraceRay, 3, 2, "TraceRay");
	RegisterHook(&IEngineTrace::TraceRayAgainstLeafAndEntityList, 4, 3, "TraceRayAgainstLeafAndEntityList");
	RegisterHook(&IEngineTrace::SweepCollideable, 6, 5, "SweepCollideable");

	m_bInitialized = true;
}

void CCollisionManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	for (CollisionHooksMap_t::const_iterator it = m_mapHooks.begin(); it != m_mapHooks.end(); it++) {
		it->first->RemoveCallback(
			HOOKTYPE_PRE,
			(HookHandlerFn *)&CCollisionManager::EnterScope
		);

		it->first->RemoveCallback(
			HOOKTYPE_POST,
			(HookHandlerFn *)&CCollisionManager::ExitScope
		);
	}

	m_mapHooks.clear();

	m_bInitialized = false;
}

void CCollisionManager::RegisterFilter(CCollisionFilter *pFilter)
{
	if (m_vecFilteres.HasElement(pFilter)) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"The given filter is already registered."
		);
	}

	IncRef();
	m_vecFilteres.AddToTail(pFilter);
}

void CCollisionManager::UnregisterFilter(CCollisionFilter *pFilter)
{
	if (!m_vecFilteres.FindAndRemove(pFilter)) {
		return;
	}

	DecRef();
}

void CCollisionManager::OnNetworkedEntityCreated(object oEntity)
{
	unsigned int uiMask;

	BEGIN_BOOST_PY()
		uiMask = extract<unsigned long>(oEntity.attr("get_solid_mask")());
	END_BOOST_PY()

	if (m_setSolidMasks.find(uiMask) != m_setSolidMasks.end()) {
		return;
	}

	m_setSolidMasks.insert(uiMask);
	m_setSolidMasks.insert(uiMask | CONTENTS_TEAM1);
	m_setSolidMasks.insert(uiMask | CONTENTS_TEAM2);
}

void CCollisionManager::OnNetworkedEntityDeleted(CBaseEntity *pEntity)
{
	FOR_EACH_VEC(m_vecFilteres, i) {
		m_vecFilteres[i]->Erase(pEntity);
	}
}

void CCollisionManager::RegisterCollisionHook(object oCallback)
{
	m_pCollisionHooks->RegisterListener(oCallback.ptr());
	IncRef();
}

void CCollisionManager::UnregisterCollisionHook(object oCallback)
{
	m_pCollisionHooks->UnregisterListener(oCallback.ptr());
	DecRef();
}

list CCollisionManager::GetSolidMasks()
{
	list oMasks;

	if (!m_setSolidMasks.size()) {
		return oMasks;
	}

	static object ContentFlags = import("engines").attr("trace").attr("ContentFlags");
	for (boost::unordered_set<unsigned long>::const_iterator it=m_setSolidMasks.begin(); it != m_setSolidMasks.end(); it++) {
		oMasks.append(ContentFlags(*it));
	}

	return oMasks;
}

template<typename T>
void CCollisionManager::RegisterHook(T tFunc, unsigned int uiFilterIndex, unsigned int uiMaskIndex, const char *szDebugName)
{
	CFunctionInfo *pInfo = GetFunctionInfo(tFunc);
	if (!pInfo) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Failed to retrieve IEngineTrace::%s's info.",
			szDebugName
		)
	}

	CFunction *pFunc = CPointer((unsigned long)((void *)enginetrace)).MakeVirtualFunction(*pInfo);
	delete pInfo;

	if (!pFunc || !pFunc->IsHookable()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"IEngineTrace::%s's function is invalid or not hookable.",
			szDebugName
		)
	}

	static CHookManager *pHookManager = GetHookManager();
	void *pAddr = (void *)pFunc->m_ulAddr;
	CHook *pHook = pHookManager->FindHook(pAddr);
	if (!pHook) {
		pHook = pHookManager->HookFunction(pAddr, pFunc->m_pCallingConvention);
		if (!pHook) {
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to hook IEngineTrace::%s.",
				szDebugName
			)
		}
	}

	delete pFunc;

	CollisionHooksMap_t::const_iterator it = m_mapHooks.find(pHook);
	if (it != m_mapHooks.end()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_RuntimeError,
			"Collision hook \"%s\" is already registered.",
			szDebugName
		)
	}

	CollisionHookData_t hookData;
	hookData.m_uiFilterIndex = uiFilterIndex;
	hookData.m_uiMaskIndex = uiMaskIndex;
	m_mapHooks[pHook] = hookData;

	pHook->AddCallback(
		HOOKTYPE_PRE,
		(HookHandlerFn *)&CCollisionManager::EnterScope
	);

	pHook->AddCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CCollisionManager::ExitScope
	);
}

bool CCollisionManager::EnterScope(HookType_t eHookType, CHook *pHook)
{
	static CCollisionManager *pManager = GetCollisionManager();

	CollisionScope_t scope;
	scope.m_bSkip = true;

	CollisionHookData_t hookData = pManager->m_mapHooks[pHook];

	int nMask = pHook->GetArgument<int>(hookData.m_uiMaskIndex);

	bool bSolidContents = true;
	if (pManager->m_setSolidMasks.find(nMask) == pManager->m_setSolidMasks.end()) {
		if (!pManager->m_pCollisionHooks->GetCount()) {
			pManager->m_vecScopes.push_back(scope);
			return false;
		}

		bSolidContents = false;
	}

	CTraceFilterSimpleWrapper *pWrapper = NULL;
	ITraceFilter *pFilter = pHook->GetArgument<ITraceFilter *>(hookData.m_uiFilterIndex);

#ifdef _WIN32
	pWrapper = (CTraceFilterSimpleWrapper *)dynamic_cast<CTraceFilterSimple *>(pFilter);
#elif __linux__
	static boost::unordered_map<void *, bool> s_mapTables;
	void *pTable = *(void **)pFilter;
	boost::unordered_map<void *, bool>::const_iterator it = s_mapTables.find(pTable);
	if (it == s_mapTables.end()) {
		if (GetType(pFilter)->IsDerivedFrom("CTraceFilterSimple")) {
			s_mapTables[pTable] = true;
			pWrapper = reinterpret_cast<CTraceFilterSimpleWrapper *>(pFilter);
		}
		else {
			s_mapTables[pTable] = false;
		}
	}
	else if (it->second) {
		pWrapper = reinterpret_cast<CTraceFilterSimpleWrapper *>(pFilter);
	}
#endif

	if (!pWrapper) {
		pManager->m_vecScopes.push_back(scope);
		return false;
	}

	if (!pWrapper->m_pPassEnt) {
		pManager->m_vecScopes.push_back(scope);
		return false;
	}

	const CBaseHandle &pHandle = pWrapper->m_pPassEnt->GetRefEHandle();
	if (!pHandle.IsValid()) {
		pManager->m_vecScopes.push_back(scope);
		return false;
	}

	unsigned int uiIndex = pHandle.GetEntryIndex();
	if (uiIndex >= MAX_EDICTS) {
		pManager->m_vecScopes.push_back(scope);
		return false;
	}

	scope.m_pFilter = pWrapper;
	scope.m_nMask = nMask;
	scope.m_bSolidContents = bSolidContents;
	scope.m_uiIndex = uiIndex;
	scope.m_bIsPlayer = ((CBaseEntityWrapper *)pWrapper->m_pPassEnt)->IsPlayer();
	scope.m_pCache = pManager->GetCache(uiIndex);
	scope.m_pExtraShouldHitCheckFunction = pWrapper->m_pExtraShouldHitCheckFunction;
	pWrapper->m_pExtraShouldHitCheckFunction = (ShouldHitFunc_t)CCollisionManager::ShouldHitEntity;

	scope.m_bSkip = false;
	pManager->m_vecScopes.push_back(scope);

	return false;
}

bool CCollisionManager::ExitScope(HookType_t eHookType, CHook *pHook)
{
	static CCollisionManager *pManager = GetCollisionManager();
	if (pManager->m_vecScopes.empty()) {
		return false;
	}

	CollisionScope_t scope = pManager->m_vecScopes.back();
	pManager->m_vecScopes.pop_back();

	if (!scope.m_bSkip && scope.m_pExtraShouldHitCheckFunction) {
		scope.m_pFilter->m_pExtraShouldHitCheckFunction = scope.m_pExtraShouldHitCheckFunction;
	}

	return false;
}

bool CCollisionManager::ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask)
{
	static CCollisionManager *pManager = GetCollisionManager();
	if (pManager->m_vecScopes.empty()) {
		return true;
	}

	CollisionScope_t scope = pManager->m_vecScopes.back();
	if (scope.m_bSkip) {
		return true;
	}

	if (!pHandleEntity) {
		return true;
	}

	const CBaseHandle &pHandle = pHandleEntity->GetRefEHandle();
	if (!pHandle.IsValid()) {
		return true;
	}

	unsigned int uiIndex = pHandle.GetEntryIndex();
	if (uiIndex >= MAX_EDICTS) {
		return true;
	}

	if (scope.m_pExtraShouldHitCheckFunction) {
		if (scope.m_pExtraShouldHitCheckFunction != scope.m_pFilter->m_pExtraShouldHitCheckFunction) {
			if (!(scope.m_pExtraShouldHitCheckFunction(pHandleEntity, contentsMask))) {
				return false;
			}
		}
	}

	static object Entity = import("entities").attr("entity").attr("Entity");

	object oEntity;
	object oOther;

	if (pManager->m_pCollisionHooks->GetCount()) {
		oEntity = Entity(scope.m_uiIndex);
		oOther = Entity(uiIndex);

		object oFilter = object(ptr((ITraceFilter *)scope.m_pFilter));

		static object ContentFlags = import("engines").attr("trace").attr("ContentFlags");
		object oMask = ContentFlags(scope.m_nMask);

		FOR_EACH_VEC(pManager->m_pCollisionHooks->m_vecCallables, i) {
			BEGIN_BOOST_PY()
				object oResult = pManager->m_pCollisionHooks->m_vecCallables[i](oEntity, oOther, oFilter, oMask);
				if (!oResult.is_none() && !extract<bool>(oResult)) {
					scope.m_pCache->SetResult(uiIndex, false);
					return false;
				}
			END_BOOST_PY_NORET()
		}
	}

	if (scope.m_pCache->HasResult(uiIndex)) {
		return scope.m_pCache->GetResult(uiIndex);
	}

	if (!scope.m_bSolidContents) {
		return true;
	}

	FOR_EACH_VEC(pManager->m_vecFilteres, i) {
		if (pManager->m_vecFilteres[i]->IsSet(scope.m_uiIndex, uiIndex))  {
			scope.m_pCache->SetResult(uiIndex, false);
			return false;
		}
	}

	if (scope.m_bIsPlayer) {
		static CPlayerCollisionListenerManager *OnPlayerCollision = GetOnPlayerCollisionListenerManager();
		if (!OnPlayerCollision->GetCount()) {
			scope.m_pCache->SetResult(uiIndex, true);
			return true;
		}

		static object Player = import("players.entity").attr("Player");
		object oPlayer = Player(scope.m_uiIndex);

		if (oOther.is_none()) {
			oOther = Entity(uiIndex);
		}

		if (!OnPlayerCollision->CallCallbacks(oPlayer, oOther)) {
			scope.m_pCache->SetResult(uiIndex, false);
			return false;
		}

		scope.m_pCache->SetResult(uiIndex, true);
		return true;
	}

	static CEntityCollisionListenerManager *OnEntityCollision = GetOnEntityCollisionListenerManager();
	if (!OnEntityCollision->GetCount()) {
		scope.m_pCache->SetResult(uiIndex, true);
		return true;
	}

	if (oEntity.is_none()) {
		oEntity = Entity(scope.m_uiIndex);
	}

	if (oOther.is_none()) {
		oOther = Entity(uiIndex);
	}

	if (!OnEntityCollision->CallCallbacks(oEntity, oOther)) {
		scope.m_pCache->SetResult(uiIndex, false);
		return false;
	}

	scope.m_pCache->SetResult(uiIndex, true);
	return true;
}

CCollisionCache *CCollisionManager::GetCache(unsigned int uiIndex)
{
	if (gpGlobals->tickcount != m_nTickCount) {
		for (CollisionCacheMap_t::const_iterator it = m_mapCache.begin(); it != m_mapCache.end(); it++) {
			delete it->second;
		}

		m_mapCache.clear();
		m_nTickCount = gpGlobals->tickcount;
	}
	else {
		CollisionCacheMap_t::const_iterator it = m_mapCache.find(uiIndex);
		if (it != m_mapCache.end()) {
			return it->second;
		}
	}

	CCollisionCache *pCache = new CCollisionCache();
	m_mapCache[uiIndex] = pCache;

	return pCache;
}


//-----------------------------------------------------------------------------
// CCollisionCache class.
//-----------------------------------------------------------------------------
bool CCollisionCache::HasResult(unsigned int uiIndex)
{
	return IsBitSet(uiIndex);
}

bool CCollisionCache::GetResult(unsigned int uiIndex)
{
	return m_vecCache.IsBitSet(uiIndex);
}

void CCollisionCache::SetResult(unsigned int uiIndex, bool bResult)
{
	Set(uiIndex);
	m_vecCache.Set((int)uiIndex, bResult);
}


//-----------------------------------------------------------------------------
// CCollisionTable class.
//-----------------------------------------------------------------------------
CCollisionTable::CCollisionTable(unsigned int uiActivatorIndex, CBaseEntity *pActivator):
	m_uiActivatorIndex(uiActivatorIndex)
{
	if (((CBaseEntityWrapper *)pActivator)->IsPlayer())
	{
		static object Player = import("players.entity").attr("Player");
		m_oActivator = Player(uiActivatorIndex);
	}
	else
	{
		static object Entity = import("entities.entity").attr("Entity");
		m_oActivator = Entity(uiActivatorIndex);
	}
}

bool CCollisionTable::__getitem__(unsigned int uiIndex)
{
	edict_t* pEdict;
	if(!EdictFromIndex(uiIndex, pEdict))
		return false;

	return IsBitSet((int)uiIndex);
}

bool CCollisionTable::__setitem__(unsigned int uiIndex, bool bDisable)
{
	if (uiIndex == m_uiActivatorIndex)
		return false;

	edict_t* pEdict;
	if(!EdictFromIndex(uiIndex, pEdict))
		return false;

	Set((int)uiIndex, bDisable);
	return true;
}

void CCollisionTable::Disable(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	if (uiIndex == m_uiActivatorIndex)
		return;

	Set((int)uiIndex, true);
}

void CCollisionTable::Reset(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);
	Set((int)uiIndex, false);
}

bool CCollisionTable::IsDisabled(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);
	return IsBitSet((int)uiIndex);
}

void CCollisionTable::DisableAll()
{
	CBaseEntity *pBaseEntity = (CBaseEntity *)servertools->FirstEntity();
	while (pBaseEntity)
	{
		unsigned int uiIndex;
		if (IndexFromBaseEntity(pBaseEntity, uiIndex) && uiIndex != m_uiActivatorIndex)
			Set((int)uiIndex, true);

		pBaseEntity = (CBaseEntity *)servertools->NextEntity(pBaseEntity);
	}
}

int CCollisionTable::Count()
{
	int count = 0;
	for (int i = 0; i < MAX_EDICTS; ++i)
	{
		if (IsBitSet(i))
			++count;
	}

	return count;
}

list CCollisionTable::GetDisabled()
{
	list oEntities;

	if (IsAllClear())
		return oEntities;

	for (int i = 0; i < MAX_EDICTS; ++i)
	{
		CBaseEntity *pBaseEntity;
		if (IsBitSet(i) && BaseEntityFromIndex((unsigned int)i, pBaseEntity))
		{
			if (((CBaseEntityWrapper *)pBaseEntity)->IsPlayer())
			{
				static object Player = import("players.entity").attr("Player");
				oEntities.append(Player((unsigned int)i));
			}
			else
			{
				static object Entity = import("entities.entity").attr("Entity");
				oEntities.append(Entity((unsigned int)i));
			}
		}
	}

	return oEntities;
}


//-----------------------------------------------------------------------------
// CCollisionFilter class.
//-----------------------------------------------------------------------------
CCollisionFilter::CCollisionFilter()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->RegisterFilter(this);
}

CCollisionFilter::~CCollisionFilter()
{
	UnloadInstance();
}

boost::shared_ptr<CCollisionTable> CCollisionFilter::GetCollisionTable(CBaseEntity *pActivator)
{
	unsigned int uiActivatorIndex = ExcIndexFromBaseEntity(pActivator);

	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiActivatorIndex);
	if (it != m_mapFilter.end())
		return it->second;

	boost::shared_ptr<CCollisionTable> pCollisionTable(new CCollisionTable(uiActivatorIndex, pActivator));
	m_mapFilter.insert(std::make_pair(uiActivatorIndex, pCollisionTable));

	return pCollisionTable;
}

boost::shared_ptr<CCollisionTable> CCollisionFilter::GetCollisionTable(unsigned int uiActivatorIndex)
{
	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiActivatorIndex);
	if (it != m_mapFilter.end())
		return it->second;

	CBaseEntity *pActivator = ExcBaseEntityFromIndex(uiActivatorIndex);

	boost::shared_ptr<CCollisionTable> pCollisionTable(new CCollisionTable(uiActivatorIndex, pActivator));
	m_mapFilter.insert(std::make_pair(uiActivatorIndex, pCollisionTable));

	return pCollisionTable;
}

void CCollisionFilter::DisableFrom(CBaseEntity *pEntity, CBaseEntity *pActivator)
{
	GetCollisionTable(pActivator)->Disable(pEntity);
}

void CCollisionFilter::ResetFrom(CBaseEntity *pEntity, CBaseEntity *pActivator)
{
	GetCollisionTable(pActivator)->Reset(pEntity);
}

bool CCollisionFilter::IsDisabledFrom(CBaseEntity *pEntity, CBaseEntity *pActivator)
{
	return GetCollisionTable(pActivator)->IsDisabled(pEntity);
}

void CCollisionFilter::Disable(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	CBaseEntity *pBaseEntity = (CBaseEntity *)servertools->FirstEntity();
	while (pBaseEntity)
	{
		unsigned int uiActivatorIndex;
		if (IndexFromBaseEntity(pBaseEntity, uiActivatorIndex) && uiIndex != uiActivatorIndex)
		{
			CollisionFilterMap_t::iterator it = m_mapFilter.find(uiActivatorIndex);
			if (it != m_mapFilter.end())
			{
				it->second->Set((int)uiIndex, true);
			}
			else
			{
				boost::shared_ptr<CCollisionTable> pCollisionTable(new CCollisionTable(uiActivatorIndex, pBaseEntity));
				m_mapFilter.insert(std::make_pair(uiActivatorIndex, pCollisionTable));
				pCollisionTable->Set((int)uiIndex, true);
			}
		}

		pBaseEntity = (CBaseEntity *)servertools->NextEntity(pBaseEntity);
	}
}

void CCollisionFilter::Reset(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		it->second->Set((int)uiIndex, false);
	}
}

bool CCollisionFilter::IsDisabled(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		if (it->second->IsBitSet((int)uiIndex))
			return true;
	}
	return false;
}

int CCollisionFilter::CountDisabled(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	int count = 0;
	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		if (it->second->IsBitSet((int)uiIndex))
			++count;
	}
	return count;
}

list CCollisionFilter::GetDisabledActivator(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ExcIndexFromBaseEntity(pEntity);

	list oEntities;

	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		boost::shared_ptr<CCollisionTable> pCollisionTable = it->second;
		if (pCollisionTable->IsBitSet((int)uiIndex))
			oEntities.append(pCollisionTable->m_oActivator);
	}

	return oEntities;
}

void CCollisionFilter::AddPair(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	if (pEntity == pOther)
		return;

	boost::shared_ptr<CCollisionTable> pEntityTable = GetCollisionTable(pEntity);
	boost::shared_ptr<CCollisionTable> pOtherTable = GetCollisionTable(pOther);

	pEntityTable->Set((int)pOtherTable->m_uiActivatorIndex, true);
	pOtherTable->Set((int)pEntityTable->m_uiActivatorIndex, true);
}

void CCollisionFilter::RemovePair(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	unsigned int uiEntityIndex = ExcIndexFromBaseEntity(pEntity);
	unsigned int uiOtherIndex = ExcIndexFromBaseEntity(pOther);

	CollisionFilterMap_t::iterator it;

	it = m_mapFilter.find(uiEntityIndex);
	if (it != m_mapFilter.end())
		it->second->Set((int)uiOtherIndex, false);

	it = m_mapFilter.find(uiOtherIndex);
	if (it != m_mapFilter.end())
		it->second->Set((int)uiEntityIndex, false);
}

bool CCollisionFilter::HasPair(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	unsigned int uiEntityIndex = ExcIndexFromBaseEntity(pEntity);
	unsigned int uiOtherIndex = ExcIndexFromBaseEntity(pOther);

	CollisionFilterMap_t::iterator it;

	it = m_mapFilter.find(uiEntityIndex);
	if (it == m_mapFilter.end() || !it->second->IsBitSet((int)uiOtherIndex))
		return false;

	it = m_mapFilter.find(uiOtherIndex);
	if (it == m_mapFilter.end() || !it->second->IsBitSet((int)uiEntityIndex))
		return false;

	return true;
}

void CCollisionFilter::RemovePairs(CBaseEntity *pEntity)
{
	unsigned int uiEntityIndex = ExcIndexFromBaseEntity(pEntity);

	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiEntityIndex);
	if (it != m_mapFilter.end())
	{
		boost::shared_ptr<CCollisionTable> pEntityTable = it->second;

		for (unsigned int uiOtherIndex = 0; uiOtherIndex < MAX_EDICTS; ++uiOtherIndex) {
			if (pEntityTable->IsBitSet((int)uiOtherIndex))
			{
				CollisionFilterMap_t::iterator it = m_mapFilter.find(uiOtherIndex);
				if (it != m_mapFilter.end())
				{
					boost::shared_ptr<CCollisionTable> pOtherTable = it->second;
					if (pOtherTable->IsBitSet((int)uiEntityIndex))
					{
						pEntityTable->Set((int)uiOtherIndex, false);
						pOtherTable->Set((int)uiEntityIndex, false);
					}
				}
			}
		}
	}
}

int CCollisionFilter::CountPairs(CBaseEntity *pEntity)
{
	unsigned int uiEntityIndex = ExcIndexFromBaseEntity(pEntity);

	int count = 0;
	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiEntityIndex);
	if (it != m_mapFilter.end())
	{
		boost::shared_ptr<CCollisionTable> pEntityTable = it->second;

		for (unsigned int uiOtherIndex = 0; uiOtherIndex < MAX_EDICTS; ++uiOtherIndex) {
			if (pEntityTable->IsBitSet((int)uiOtherIndex))
			{
				CollisionFilterMap_t::iterator it = m_mapFilter.find(uiOtherIndex);
				if (it != m_mapFilter.end() && it->second->IsBitSet((int)uiEntityIndex))
					++count;
			}
		}
	}

	return count;
}

list CCollisionFilter::GetPairs(CBaseEntity *pEntity)
{
	unsigned int uiEntityIndex = ExcIndexFromBaseEntity(pEntity);

	list oEntities;
	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiEntityIndex);
	if (it != m_mapFilter.end())
	{
		boost::shared_ptr<CCollisionTable> pEntityTable = it->second;

		for (unsigned int uiOtherIndex = 0; uiOtherIndex < MAX_EDICTS; ++uiOtherIndex) {
			if (pEntityTable->IsBitSet((int)uiOtherIndex))
			{
				CollisionFilterMap_t::iterator it = m_mapFilter.find(uiOtherIndex);
				if (it != m_mapFilter.end())
				{
					boost::shared_ptr<CCollisionTable> pOtherTable = it->second;
					if (pOtherTable->IsBitSet((int)uiEntityIndex))
					{
						oEntities.append(pOtherTable->m_oActivator);
					}
				}
			}
		}
	}

	return oEntities;
}

void CCollisionFilter::Clear()
{
	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		it->second->ClearAll();
	}
}

inline bool CCollisionFilter::IsSet(unsigned int uiActivatorIndex, unsigned int uiOtherIndex)
{
	CollisionFilterMap_t::iterator it = m_mapFilter.find(uiActivatorIndex);
	if (it != m_mapFilter.end() && it->second->IsBitSet((int)uiOtherIndex))
		return true;

	return false;
}

inline void CCollisionFilter::Erase(CBaseEntity *pEntity)
{
	unsigned int uiIndex = ((CBaseEntityWrapper *)pEntity)->GetIndex();
	m_mapFilter.erase(uiIndex);

	for (CollisionFilterMap_t::iterator it = m_mapFilter.begin(); it != m_mapFilter.end(); ++it)
	{
		it->second->Set((int)uiIndex, false);
	}
}

void CCollisionFilter::UnloadInstance()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->UnregisterFilter(this);
}


//-----------------------------------------------------------------------------
// CCollisionListenerManager class.
//-----------------------------------------------------------------------------
CCollisionListenerManager::CCollisionListenerManager():
	m_bInitialized(false)
{

}

void CCollisionListenerManager::Initialize()
{
	if (m_bInitialized) {
		return;
	}

	static CCollisionManager *pManager = GetCollisionManager();
	pManager->IncRef();
	m_bInitialized = true;
}

void CCollisionListenerManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	static CCollisionManager *pManager = GetCollisionManager();
	pManager->DecRef();
	m_bInitialized = false;
}

bool CCollisionListenerManager::CallCallbacks(object oEntity, object oOther)
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
// CEntityCollisionListenerManager singleton accessor.
//-----------------------------------------------------------------------------
static CEntityCollisionListenerManager s_OnEntityCollision;
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager()
{
	return &s_OnEntityCollision;
}


//-----------------------------------------------------------------------------
// CPlayerCollisionListenerManager singleton accessor.
//-----------------------------------------------------------------------------
static CPlayerCollisionListenerManager s_OnPlayerCollision;
CPlayerCollisionListenerManager *GetOnPlayerCollisionListenerManager()
{
	return &s_OnPlayerCollision;
}
