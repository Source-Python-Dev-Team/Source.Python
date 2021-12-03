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
#include "modules/listeners/listeners_manager.h"
#include "modules/entities/entities_entity.h"
#include "modules/physics/physics.h"
#include "modules/memory/memory_hooks.h"

// Boost
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

// SDK
#include "vphysics/object_hash.h"
#include "bitvec.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
struct CollisionHookData_t;
class CCollisionCache;


//-----------------------------------------------------------------------------
// Typedefs.
//-----------------------------------------------------------------------------
#if !defined(ENGINE_CSGO) && !defined(ENGINE_BLADE) && !defined(ENGINE_BMS)
	typedef bool (*ShouldHitFunc_t)( IHandleEntity *pHandleEntity, int contentsMask );
#endif

typedef boost::unordered_map<CHook *, CollisionHookData_t> CollisionHooksMap_t;

typedef CBitVec<MAX_EDICTS> CollisionCache_t;
typedef boost::unordered_map<unsigned int, CCollisionCache *> CollisionCacheMap_t;


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
// CollisionScope_t structure.
//-----------------------------------------------------------------------------
struct CollisionScope_t
{
	bool m_bSkip;
	unsigned int m_uiIndex;
	bool m_bIsPlayer;
	CTraceFilterSimpleWrapper *m_pFilter;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
	CCollisionCache *m_pCache;
	int m_nMask;
	bool m_bSolidContents;
};


//-----------------------------------------------------------------------------
// CollisionHookData_t structure.
//-----------------------------------------------------------------------------
struct CollisionHookData_t
{
	unsigned int m_uiFilterIndex;
	unsigned int m_uiMaskIndex;
};


//-----------------------------------------------------------------------------
// ICollisionHash class.
//-----------------------------------------------------------------------------
class ICollisionHash
{
public:
	ICollisionHash();
	virtual ~ICollisionHash();

	virtual void AddPair(void *pEntity, void *pOther) = 0;
	virtual void RemovePair(void *pEntity, void *pOther) = 0;
	virtual void RemovePairs(void *pEntity) = 0;

	virtual bool Contains(void *pEntity) = 0;
	virtual bool HasPair(void *pEntity, void *pOther) = 0;

	virtual int GetCount(void *pEntity) = 0;
	virtual list GetPairs(void *pEntity) = 0;

	void UnloadInstance();
};


//-----------------------------------------------------------------------------
// CCollisionHash class.
//-----------------------------------------------------------------------------
class CCollisionHash : public ICollisionHash
{
public:
	CCollisionHash();
	~CCollisionHash();

	void AddPair(void *pEntity, void *pOther);
	void RemovePair(void *pEntity, void *pOther);
	void RemovePairs(void *pEntity);

	bool Contains(void *pEntity);
	bool HasPair(void *pEntity, void *pOther);

	int GetCount(void *pEntity);
	list GetPairs(void *pEntity);

private:
	IPhysicsObjectPairHash *m_pHash;
};


//-----------------------------------------------------------------------------
// CCollisionCache class.
//-----------------------------------------------------------------------------
class CCollisionCache : private CollisionCache_t
{
public:
	bool HasResult(unsigned int uiIndex);
	bool GetResult(unsigned int uiIndex);
	void SetResult(unsigned int uiIndex, bool bResult);

private:
	CollisionCache_t m_vecCache;
};


//-----------------------------------------------------------------------------
// CCollisionManager class.
//-----------------------------------------------------------------------------
class CCollisionManager
{
public:
	friend CCollisionManager *GetCollisionManager();
	friend class CCollisionListenerManager;

private:
	CCollisionManager();
	~CCollisionManager();

public:
	void Initialize();
	void Finalize();

	void RegisterHash(ICollisionHash *pHash);
	void UnregisterHash(ICollisionHash *pHash);

	void OnNetworkedEntityCreated(object oEntity);
	void OnNetworkedEntityDeleted(CBaseEntity *pEntity);

	void RegisterCollisionHook(object oCallback);
	void UnregisterCollisionHook(object oCallback);

	list GetSolidMasks();

protected:
	void IncRef();
	void DecRef();

private:
	template<typename T>
	void RegisterHook(T tFunc, unsigned int uiFilterIndex, unsigned int nMaskIndex, const char *szDebugName);

	static bool EnterScope(HookType_t eHookType, CHook *pHook);
	static bool ExitScope(HookType_t eHookType, CHook *pHook);

	static bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask);

	CCollisionCache *GetCache(unsigned int uiIndex);

private:
	bool m_bInitialized;
	unsigned int m_uiRefCount;
	CUtlVector<ICollisionHash *> m_vecHashes;
	CollisionHooksMap_t m_mapHooks;
	std::vector<CollisionScope_t> m_vecScopes;

	boost::unordered_set<unsigned long> m_setSolidMasks;

	int m_nTickCount;
	CollisionCacheMap_t m_mapCache;

	CListenerManager *m_pCollisionHooks;
};

// Singleton accessor.
inline CCollisionManager *GetCollisionManager()
{
	static CCollisionManager *s_pEntityCollisionManager = new CCollisionManager;
	return s_pEntityCollisionManager;
}


//-----------------------------------------------------------------------------
// CCollisionListenerManager class.
//-----------------------------------------------------------------------------
class CCollisionListenerManager : public CListenerManager
{
public:
	CCollisionListenerManager();

	void Initialize();
	void Finalize();

	bool CallCallbacks(object oEntity, object oOther);

private:
	bool m_bInitialized;
};


//-----------------------------------------------------------------------------
// CEntityCollisionListenerManager class.
//-----------------------------------------------------------------------------
class CEntityCollisionListenerManager: public CCollisionListenerManager
{

};

// Singleton accessor.
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager();


//-----------------------------------------------------------------------------
// CPlayerCollisionListenerManager class.
//-----------------------------------------------------------------------------
class CPlayerCollisionListenerManager: public CCollisionListenerManager
{

};

// Singleton accessor.
CPlayerCollisionListenerManager *GetOnPlayerCollisionListenerManager();


#endif // _ENTITIES_COLLISIONS_H
