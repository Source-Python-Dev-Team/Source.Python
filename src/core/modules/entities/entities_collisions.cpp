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

	BEGIN_BOOST_PY()
		static boost::python::tuple oSolidMasks(import("entities.collisions").attr("SOLID_MASKS"));
		for (int i = 0; i < len(oSolidMasks); i++) {
			m_setSolidMasks.insert(extract<unsigned long>(object(oSolidMasks[i])));
		}
	END_BOOST_PY()

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
	m_setSolidMasks.clear();

	m_bInitialized = false;
}

void CCollisionManager::RegisterHash(ICollisionHash *pHash)
{
	if (m_vecHashes.HasElement(pHash)) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"The given hash is already registered."
		);
	}

	IncRef();
	m_vecHashes.AddToTail(pHash);
}

void CCollisionManager::UnregisterHash(ICollisionHash *pHash)
{
	if (!m_vecHashes.FindAndRemove(pHash)) {
		return;
	}

	DecRef();
}

void CCollisionManager::OnNetworkedEntityDeleted(CBaseEntity *pEntity)
{
	FOR_EACH_VEC(m_vecHashes, i) {
		m_vecHashes[i]->RemovePairs((void *)pEntity);
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
		scope.m_pExtraShouldHitCheckFunction = NULL;
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

		FOR_EACH_VEC(pManager->m_pCollisionHooks->m_vecCallables, i) {
			BEGIN_BOOST_PY()
				object oResult = pManager->m_pCollisionHooks->m_vecCallables[i](oEntity, oOther, oFilter, scope.m_nMask);
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

	FOR_EACH_VEC(pManager->m_vecHashes, i) {
		if (pManager->m_vecHashes[i]->HasPair((void *)scope.m_pFilter->m_pPassEnt, (void *)pHandleEntity)) {
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

	CollisionCacheMap_t::const_iterator it = m_mapCache.find(uiIndex);
	if (it != m_mapCache.end()) {
		return it->second;
	}

	CCollisionCache *pCache = new CCollisionCache();
	m_mapCache[uiIndex] = pCache;

	return pCache;
}


//-----------------------------------------------------------------------------
// ICollisionHash class.
//-----------------------------------------------------------------------------
ICollisionHash::ICollisionHash()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->RegisterHash(this);
}

ICollisionHash::~ICollisionHash()
{
	UnloadInstance();
}

void ICollisionHash::UnloadInstance()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->UnregisterHash(this);
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

void CCollisionHash::AddPair(void *pEntity, void *pOther)
{
	if (!IsValidNetworkedEntityPointer(pEntity) || !IsValidNetworkedEntityPointer(pOther)) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Given entity pointer invalid or not networked."
		)
	}

	m_pHash->AddObjectPair(pEntity, pOther);
}

void CCollisionHash::RemovePair(void *pEntity, void *pOther)
{
	m_pHash->RemoveObjectPair(pEntity, pOther);
}

void CCollisionHash::RemovePairs(void *pEntity)
{
	m_pHash->RemoveAllPairsForObject(pEntity);
}

bool CCollisionHash::Contains(void *pEntity)
{
	return m_pHash->IsObjectInHash(pEntity);
}

bool CCollisionHash::HasPair(void *pEntity, void *pOther)
{
	return m_pHash->IsObjectPairInHash(pEntity, pOther);
}

int CCollisionHash::GetCount(void *pEntity)
{
	return m_pHash->GetPairCountForObject(pEntity);
}

list CCollisionHash::GetPairs(void *pEntity)
{
	list oEntities;
	int nCount = m_pHash->GetPairCountForObject(pEntity);

	if (!nCount) {
		return oEntities;
	}

	void **ppEntities = (void **)stackalloc(nCount * sizeof(void *));
	m_pHash->GetPairListForObject(pEntity, nCount, ppEntities);

	for (int i = 0; i < nCount; ++i) {
		pEntity = ppEntities[i];
		if (!pEntity) {
			continue;
		}

		oEntities.append(pEntity);
	}

	return oEntities;
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

	GetCollisionManager()->IncRef();
	m_bInitialized = true;
}

void CCollisionListenerManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	GetCollisionManager()->DecRef();
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
