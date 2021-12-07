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
extern IPhysics *physics;
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

void CCollisionManager::RegisterRules(ICollisionRules *pRules)
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

void CCollisionManager::UnregisterRules(ICollisionRules *pRules)
{
	if (!m_vecRules.FindAndRemove(pRules)) {
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
	FOR_EACH_VEC(m_vecRules, i) {
		m_vecRules[i]->OnEntityDeleted(pEntity);
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

	FOR_EACH_VEC(pManager->m_vecRules, i) {
		if (!pManager->m_vecRules[i]->ShouldHitEntity((CBaseEntity *)scope.m_pFilter->m_pPassEnt, (CBaseEntity *)pHandleEntity)) {
			scope.m_pCache->SetResult(uiIndex, false);
			return false;
		}
	}

	if (scope.m_bIsPlayer) {
		static CCollisionListenerManager *OnPlayerCollision = GetOnPlayerCollisionListenerManager();
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

	static CCollisionListenerManager *OnEntityCollision = GetOnEntityCollisionListenerManager();
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
// ICollisionRules class.
//-----------------------------------------------------------------------------
ICollisionRules::ICollisionRules(bool bRegister)
{
	if (!bRegister) {
		return;
	}

	static CCollisionManager *pManager = GetCollisionManager();
	pManager->RegisterRules(this);
}

ICollisionRules::~ICollisionRules()
{
	UnloadInstance();
}

void ICollisionRules::UnloadInstance()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->UnregisterRules(this);
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
// CCollisionHash class.
//-----------------------------------------------------------------------------
CCollisionHash::CCollisionHash()
{
	m_pHash = physics->CreateObjectPairHash();

	if (!m_pHash) {
		BOOST_RAISE_EXCEPTION(
			PyExc_RuntimeError,
			"Failed to create a collision hash."
		)
	}
}

CCollisionHash::~CCollisionHash()
{
	if (!m_pHash) {
		return;
	}

	physics->DestroyObjectPairHash(m_pHash);
}

void CCollisionHash::OnEntityDeleted(CBaseEntity *pEntity)
{
	RemovePairs(pEntity);
}

bool CCollisionHash::ShouldHitEntity(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	return !HasPair(pEntity, pOther);
}

void CCollisionHash::AddPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther)
{
	if (!pEntity->IsNetworked() || !pOther->IsNetworked()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Given entity is not networked."
		)
	}

	m_pHash->AddObjectPair(pEntity, pOther);
}

void CCollisionHash::RemovePair(void *pObject, void *pOther)
{
	m_pHash->RemoveObjectPair(pObject, pOther);
}

void CCollisionHash::RemovePairs(void *pObject)
{
	m_pHash->RemoveAllPairsForObject(pObject);
}

bool CCollisionHash::Contains(void *pObject)
{
	return m_pHash->IsObjectInHash(pObject);
}

bool CCollisionHash::HasPair(void *pObject, void *pOther)
{
	return m_pHash->IsObjectPairInHash(pObject, pOther);
}

int CCollisionHash::GetCount(void *pObject)
{
	return m_pHash->GetPairCountForObject(pObject);
}

list CCollisionHash::GetPairs(void *pObject)
{
	list oObjects;
	int nCount = m_pHash->GetPairCountForObject(pObject);

	if (!nCount) {
		return oObjects;
	}

	void **ppObjects = (void **)stackalloc(nCount * sizeof(void *));
	m_pHash->GetPairListForObject(pObject, nCount, ppObjects);

	for (int i = 0; i < nCount; ++i) {
		pObject = ppObjects[i];
		if (!pObject) {
			continue;
		}

		oObjects.append(pObject);
	}

	return oObjects;
}


//-----------------------------------------------------------------------------
// CCollisionSet class.
//-----------------------------------------------------------------------------
CCollisionSet::CCollisionSet():
	ICollisionRules()
{

}

CCollisionSet::CCollisionSet(bool bRegister):
	ICollisionRules(bRegister)
{

}

void CCollisionSet::Add(CBaseEntityWrapper *pEntity)
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

	m_pSet.insert(pEntity);
}

void CCollisionSet::Remove(void *pObject)
{
	m_pSet.erase(pObject);
}

bool CCollisionSet::Contains(void *pObject)
{
	return m_pSet.find(pObject) != m_pSet.end();
}

void CCollisionSet::Clear(void *pObject)
{
	m_pSet.clear();
}

unsigned int CCollisionSet::GetSize()
{
	return m_pSet.size();
}

bool CCollisionSet::HasElements()
{
	return !m_pSet.empty();
}

object CCollisionSet::Iterate()
{
	list oElements;

	if (HasElements()) {
		for (boost::unordered_set<void *>::const_iterator it = m_pSet.begin(); it != m_pSet.end(); it++) {
			oElements.append(*it);
		}
	}

	return oElements.attr("__iter__")();
}

void CCollisionSet::OnEntityDeleted(CBaseEntity *pEntity)
{
	Remove(pEntity);
};

bool CCollisionSet::ShouldHitEntity(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	return !(Contains(pEntity) || Contains(pOther));
};


//-----------------------------------------------------------------------------
// CCollisionMap class.
//-----------------------------------------------------------------------------
boost::shared_ptr<CCollisionSet> CCollisionMap::Find(CBaseEntityWrapper *pEntity)
{
	boost::shared_ptr<CCollisionSet> spSet;
	spSet = m_mapSets[pEntity];

	if (!get_pointer(spSet)) {
		if (!pEntity->IsNetworked()) {
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Given entity is not networked."
			)
		}

		spSet = boost::shared_ptr<CCollisionSet>(new CCollisionSet(false));
		m_mapSets[pEntity] = spSet;
	}

	return spSet;
}

void CCollisionMap::Remove(void *pObject)
{
	m_mapSets.erase(pObject);
}

void CCollisionMap::Clear()
{
	m_mapSets.clear();
}

bool CCollisionMap::Contains(void *pObject)
{
	return m_mapSets.find(pObject) != m_mapSets.end();
}

unsigned int CCollisionMap::GetSize()
{
	return m_mapSets.size();
}

bool CCollisionMap::HasElements()
{
	return !m_mapSets.empty();
}

object CCollisionMap::Iterate()
{
	list oElements;

	if (HasElements()) {
		for (boost::unordered_map<void *, boost::shared_ptr<CCollisionSet> >::const_iterator it = m_mapSets.begin();
				it != m_mapSets.end(); it++) {
			oElements.append(make_tuple(object(it->first), object(it->second)));
		}
	}

	return oElements.attr("__iter__")();
}

void CCollisionMap::OnEntityDeleted(CBaseEntity *pEntity)
{
	for (boost::unordered_map<void *, boost::shared_ptr<CCollisionSet> >::const_iterator it = m_mapSets.begin();
			it != m_mapSets.end(); it++) {
		it->second->OnEntityDeleted(pEntity);
	}

	Remove(pEntity);
}

bool CCollisionMap::ShouldHitEntity(CBaseEntity *pEntity, CBaseEntity *pOther)
{
	CCollisionSet *pSet = get_pointer(m_mapSets[pEntity]);
	if (!pSet) {
		return true;
	}

	return !pSet->Contains(pOther);
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
// Singleton accessors.
//-----------------------------------------------------------------------------
static CCollisionListenerManager s_OnEntityCollision;
CCollisionListenerManager *GetOnEntityCollisionListenerManager()
{
	return &s_OnEntityCollision;
}

static CCollisionListenerManager s_OnPlayerCollision;
CCollisionListenerManager *GetOnPlayerCollisionListenerManager()
{
	return &s_OnPlayerCollision;
}
