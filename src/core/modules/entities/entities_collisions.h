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

#ifndef _ENTITIES_COLLISIONS_H
#define _ENTITIES_COLLISIONS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "sp_main.h"
#include "utilities/wrap_macros.h"
#include "modules/listeners/listeners_manager.h"
#include "modules/memory/memory_pointer.h"
#include "modules/memory/memory_function_info.h"

// SDK
#include "engine/IEngineTrace.h"


//---------------------------------------------------------------------------------
// Externals.
//---------------------------------------------------------------------------------
extern IEngineTrace *enginetrace;


//-----------------------------------------------------------------------------
// Typedefs.
//-----------------------------------------------------------------------------
typedef bool (*ShouldHitFunc_t)(IHandleEntity *pEntity, int iMask);


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
class CEntityCollisionListenerManager;
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager();


//-----------------------------------------------------------------------------
// CTraceFilterSimpleWrapper class.
//-----------------------------------------------------------------------------
class CTraceFilterSimpleWrapper : public CTraceFilter
{
public:
	const IHandleEntity *m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};


//-----------------------------------------------------------------------------
// TraceRayScope_t structure.
//-----------------------------------------------------------------------------
struct TraceRayScope_t
{
	unsigned int m_uiIndex;
	CTraceFilterSimpleWrapper *m_pFilter;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};


//-----------------------------------------------------------------------------
// CEntityCollisionListenerManager class.
//-----------------------------------------------------------------------------
class CEntityCollisionListenerManager: public CListenerManager
{
public:
	CEntityCollisionListenerManager():
		m_pHook(NULL)
	{

	}

	virtual void Initialize()
	{
		if (m_pHook) {
			return;
		}

		CFunctionInfo *pInfo = GetFunctionInfo(&IEngineTrace::TraceRay);
		if (!pInfo) {
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"Failed to retrieve IEngineTrace::TraceRay's info."
			)
		}

		CFunction *pFunc = CPointer((unsigned long)((void *)enginetrace)).MakeVirtualFunction(*pInfo);
		delete pInfo;

		if (!pFunc || !pFunc->IsHookable()) {
			BOOST_RAISE_EXCEPTION(
				PyExc_ValueError,
				"IEngineTrace::TraceRay's function is invalid or not hookable."
			)
		}

		void *pAddr = (void *)pFunc->m_ulAddr;
		m_pHook = GetHookManager()->FindHook(pAddr);
		if (!m_pHook) {
			m_pHook = GetHookManager()->HookFunction(pAddr, pFunc->m_pCallingConvention);
			if (!m_pHook) {
				BOOST_RAISE_EXCEPTION(
					PyExc_ValueError,
					"Failed to hook IEngineTrace::TraceRay."
				)
			}
		}

		delete pFunc;
		m_pHook->AddCallback(
			HOOKTYPE_PRE,
			(HookHandlerFn *)&CEntityCollisionListenerManager::_pre_trace_ray
		);

		m_pHook->AddCallback(
			HOOKTYPE_POST,
			(HookHandlerFn *)&CEntityCollisionListenerManager::_post_trace_ray
		);
	}

	virtual void Finalize()
	{
		if (!m_pHook) {
			return;
		}

		m_pHook->RemoveCallback(
			HOOKTYPE_PRE,
			(HookHandlerFn *)&CEntityCollisionListenerManager::_pre_trace_ray
		);

		m_pHook->RemoveCallback(
			HOOKTYPE_POST,
			(HookHandlerFn *)&CEntityCollisionListenerManager::_post_trace_ray
		);

		m_pHook = NULL;
	}

	static bool _extract_and_validate_filter(CHook *pHook, CTraceFilterSimpleWrapper *&pResult)
	{
		CTraceFilterSimple *pFilter = dynamic_cast<CTraceFilterSimple *>(pHook->GetArgument<ITraceFilter *>(3));
		if (!pFilter) {
			return false;
		}

		CTraceFilterSimpleWrapper *pWrapper = (CTraceFilterSimpleWrapper *)pFilter;
		if (!pWrapper->m_pPassEnt) {
			return false;
		}

		const CBaseHandle &pHandle = pWrapper->m_pPassEnt->GetRefEHandle();
		if (!pHandle.IsValid()) {
			return false;
		}

		pResult = pWrapper;
		return true;
	}

	static bool _pre_trace_ray(HookType_t eHookType, CHook *pHook)
	{
		
		CTraceFilterSimpleWrapper *pFilter;
		if (!_extract_and_validate_filter(pHook, pFilter)) {
			return false;
		}

		TraceRayScope_t scope;
		scope.m_uiIndex = pFilter->m_pPassEnt->GetRefEHandle().GetEntryIndex();
		scope.m_pFilter = pFilter;
		scope.m_pExtraShouldHitCheckFunction = scope.m_pFilter->m_pExtraShouldHitCheckFunction;
		scope.m_pFilter->m_pExtraShouldHitCheckFunction = (ShouldHitFunc_t)CEntityCollisionListenerManager::_should_collide;

		static CEntityCollisionListenerManager *pManager = GetOnEntityCollisionListenerManager();
		pManager->m_vecScopes.push_back(scope);

		return false;
	}

	static bool _should_collide(IHandleEntity *pHandleEntity, int iMask)
	{
		static CEntityCollisionListenerManager *pManager = GetOnEntityCollisionListenerManager();
		if (pManager->m_vecScopes.empty()) {
			return true;
		}

		const CBaseHandle &pHandle = pHandleEntity->GetRefEHandle();
		if (!pHandle.IsValid()) {
			return true;
		}

		TraceRayScope_t scope = pManager->m_vecScopes.back();
		if (scope.m_pExtraShouldHitCheckFunction) {
			if (scope.m_pExtraShouldHitCheckFunction != scope.m_pFilter->m_pExtraShouldHitCheckFunction) {
				if (!(scope.m_pExtraShouldHitCheckFunction(pHandleEntity, iMask))) {
					return false;
				}
			}
		}

		static object Entity = import("entities").attr("entity").attr("Entity");
		object oEntity = Entity(scope.m_uiIndex);
		object oOther = Entity(pHandle.GetEntryIndex());

		bool bResult = true;
		FOR_EACH_VEC(pManager->m_vecCallables, i) {
			BEGIN_BOOST_PY()
				object oResult = pManager->m_vecCallables[i](oEntity, oOther);
				if (!oResult.is_none() && !extract<bool>(oResult)) {
					bResult = false;
				}
			END_BOOST_PY_NORET()
		}

		return bResult;
	}

	static bool _post_trace_ray(HookType_t eHookType, CHook *pHook)
	{
		static CEntityCollisionListenerManager *pManager = GetOnEntityCollisionListenerManager();
		if (pManager->m_vecScopes.empty()) {
			return false;
		}

		CTraceFilterSimpleWrapper *pFilter;
		if (!_extract_and_validate_filter(pHook, pFilter)) {
			return false;
		}

		TraceRayScope_t scope = pManager->m_vecScopes.back();
		pManager->m_vecScopes.pop_back();

		if (scope.m_pExtraShouldHitCheckFunction) {
			scope.m_pFilter->m_pExtraShouldHitCheckFunction = scope.m_pExtraShouldHitCheckFunction;
			scope.m_pExtraShouldHitCheckFunction = NULL;
		}

		return false;
	}

public:
	CHook *m_pHook;
	std::vector<TraceRayScope_t> m_vecScopes;
};

// Singleton accessor.
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager();


#endif // _ENTITIES_COLLISIONS_H
