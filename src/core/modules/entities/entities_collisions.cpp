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

#ifdef __linux__
	#include "modules/memory/memory_rtti.h"
#endif


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IEngineTrace *enginetrace;
extern IPhysics *physics;


//-----------------------------------------------------------------------------
// CCollisionManager class.
//-----------------------------------------------------------------------------
CCollisionManager::CCollisionManager():
	m_bInitialized(false),
	m_uiRefCount(0)
{

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

	UnregisterHook(&IEngineTrace::TraceRay, "TraceRay");
	UnregisterHook(&IEngineTrace::TraceRayAgainstLeafAndEntityList, "TraceRayAgainstLeafAndEntityList");
	UnregisterHook(&IEngineTrace::SweepCollideable, "SweepCollideable");

	m_bInitialized = false;
}

void CCollisionManager::RegisterHash(CCollisionHash *pHash)
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

void CCollisionManager::UnregisterHash(CCollisionHash *pHash)
{
	if (!m_vecHashes.FindAndRemove(pHash)) {
		return;
	}

	DecRef();
}

template<typename T>
CHook *CCollisionManager::GetHook(T tFunc, const char *szDebugName)
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
	return pHook;
}

template<typename T>
void CCollisionManager::RegisterHook(T tFunc, unsigned int uiFilterIndex, unsigned int uiMaskIndex, const char *szDebugName)
{
	CHook *pHook = GetHook(tFunc, szDebugName);
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

template<typename T>
void CCollisionManager::UnregisterHook(T tFunc, const char *szDebugName)
{
	CHook *pHook = GetHook(tFunc, szDebugName);
	CollisionHooksMap_t::const_iterator it = m_mapHooks.find(pHook);
	if (it == m_mapHooks.end()) {
		BOOST_RAISE_EXCEPTION(
			PyExc_RuntimeError,
			"Collision hook \"%s\" is not registered.",
			szDebugName
		)
	}

	pHook->RemoveCallback(
		HOOKTYPE_PRE,
		(HookHandlerFn *)&CCollisionManager::EnterScope
	);

	pHook->RemoveCallback(
		HOOKTYPE_POST,
		(HookHandlerFn *)&CCollisionManager::ExitScope
	);

	m_mapHooks.erase(it);
}

bool CCollisionManager::EnterScope(HookType_t eHookType, CHook *pHook)
{
	static CCollisionManager *pManager = GetCollisionManager();

	CollisionScope_t scope;
	scope.m_bSkip = true;

	CollisionHookData_t hookData = pManager->m_mapHooks[pHook];

	int nMask = pHook->GetArgument<int>(hookData.m_uiMaskIndex);

#if defined(ENGINE_CSGO)
	if ((nMask & CONTENTS_EMPTY ||
		nMask & CONTENTS_AUX ||
		nMask & CONTENTS_SLIME ||
		nMask & CONTENTS_WATER ||
		nMask & CONTENTS_BLOCKLOS ||
		nMask & CONTENTS_OPAQUE ||
		nMask & CONTENTS_IGNORE_NODRAW_OPAQUE ||
		nMask & CONTENTS_CURRENT_0 || // CONTENTS_BRUSH_PAINT
		nMask & CONTENTS_DEBRIS ||
		nMask & CONTENTS_DETAIL ||
		nMask & CONTENTS_TRANSLUCENT ||
		nMask & CONTENTS_HITBOX) &&
		!(nMask & CONTENTS_CURRENT_90) // CONTENTS_GRENADECLIP
	) {
#else
	if (nMask != MASK_SOLID &&
		nMask != MASK_PLAYERSOLID &&
		nMask != MASK_NPCSOLID
	) {
#endif
		pManager->m_vecScopes.push_back(scope);
		return false;
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
		s_mapTables[pTable] = GetType(pFilter)->IsDerivedFrom("CTraceFilterSimple");
	}

	if (s_mapTables[pTable]) {
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
	scope.m_uiIndex = uiIndex;
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

	FOR_EACH_VEC(pManager->m_vecHashes, i) {
		if (pManager->m_vecHashes[i]->HasPair((void *)scope.m_pFilter->m_pPassEnt, (void *)pHandleEntity)) {
			return false;
		}
	}

	static CEntityCollisionListenerManager *pListener = GetOnEntityCollisionListenerManager();
	if (!pListener->GetCount()) {
		return true;
	}

	static object Entity = import("entities").attr("entity").attr("Entity");

	object oEntity = Entity(scope.m_uiIndex);
	object oOther = Entity(uiIndex);

	FOR_EACH_VEC(pListener->m_vecCallables, i) {
		BEGIN_BOOST_PY()
			object oResult = pListener->m_vecCallables[i](oEntity, oOther);
			if (!oResult.is_none() && !extract<bool>(oResult)) {
				return false;
			}
		END_BOOST_PY_NORET()
	}

	return true;
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

	static CCollisionManager *pManager = GetCollisionManager();
	pManager->RegisterHash(this);
}

CCollisionHash::~CCollisionHash()
{
	if (!m_pHash) {
		return;
	}

	static CCollisionManager *pManager = GetCollisionManager();
	pManager->UnregisterHash(this);
	physics->DestroyObjectPairHash(m_pHash);
}

void CCollisionHash::AddPair(void *pObject, void *pOther)
{
	m_pHash->AddObjectPair(pObject, pOther);
}

void CCollisionHash::RemovePair(void *pObject, void *pOther)
{
	m_pHash->RemoveObjectPair(pObject, pOther);
}

void CCollisionHash::RemovePairs(void *pObject)
{
	m_pHash->RemoveAllPairsForObject(pObject);
}

bool CCollisionHash::IsInHash(void *pObject)
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

void CCollisionHash::UnloadInstance()
{
	static CCollisionManager *pManager = GetCollisionManager();
	pManager->UnregisterHash(this);
}


//-----------------------------------------------------------------------------
// CEntityCollisionListenerManager class.
//-----------------------------------------------------------------------------
CEntityCollisionListenerManager::CEntityCollisionListenerManager():
	m_bInitialized(false)
{

}

void CEntityCollisionListenerManager::Initialize()
{
	if (m_bInitialized) {
		return;
	}

	GetCollisionManager()->IncRef();
	m_bInitialized = true;
}

void CEntityCollisionListenerManager::Finalize()
{
	if (!m_bInitialized) {
		return;
	}

	GetCollisionManager()->DecRef();
	m_bInitialized = false;
}

// Singleton accessor.
static CEntityCollisionListenerManager s_OnEntityCollision;
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager()
{
	return &s_OnEntityCollision;
}
