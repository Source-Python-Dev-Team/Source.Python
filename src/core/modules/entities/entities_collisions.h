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
#include "modules/memory/memory_hooks.h"

// Boost
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

// SDK
#include "bitvec.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
struct CollisionHookData_t;
class CCollisionSet;
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

typedef std::pair<CBaseEntityWrapper *, CBaseEntityWrapper *> CollisionPair_t;
typedef boost::unordered_map<CBaseEntityWrapper *, boost::shared_ptr<CCollisionSet> > CollisionMap_t;

typedef boost::unordered_set<CBaseEntityWrapper *> CollisionSet_t;


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
	object m_oMask;
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
// ECollisionMode enumeration.
//-----------------------------------------------------------------------------
enum ECollisionMode
{
	COLLISION_MODE_ALLOW,
	COLLISION_MODE_PREVENT
};


//-----------------------------------------------------------------------------
// ICollisionRules class.
//-----------------------------------------------------------------------------
class ICollisionRules
{
public:
	virtual ~ICollisionRules();

	virtual void OnEntityDeleted(CBaseEntityWrapper *pEntity) = 0;
	virtual bool ShouldCollide(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther) = 0;

	virtual void UnloadInstance();

	ECollisionMode GetMode();
	void SetMode(ECollisionMode eMode);

private:
	ECollisionMode m_eMode;
};


//-----------------------------------------------------------------------------
// CollisionPairs_t definition and specializations.
//-----------------------------------------------------------------------------
namespace boost {
	inline std::size_t hash_value(CollisionPair_t const &p) {
		std::size_t s = 0;
		boost::hash_combine(s, p.first < p.second ? p.first : p.second);
		boost::hash_combine(s, p.first < p.second ? p.second : p.first);
		return s;
	}

	struct hash_equals {
		inline bool operator()(const CollisionPair_t &a, const CollisionPair_t &b) const {
			return (a == b) || (a.first == b.second && a.second == b.first); 
		}
	};
}

typedef boost::unordered_set<CollisionPair_t, boost::hash<CollisionPair_t>, boost::hash_equals> CollisionPairs_t;


//-----------------------------------------------------------------------------
// CCollisionHash class.
//-----------------------------------------------------------------------------
class CCollisionHash : public ICollisionRules
{
public:
	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldCollide(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);

	void AddPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);
	void RemovePair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);
	void RemovePairs(CBaseEntityWrapper *pEntity);

	void Clear();

	bool Contains(CBaseEntityWrapper *pEntity);
	bool HasPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);

	unsigned int GetCount(CBaseEntityWrapper *pEntity);
	list GetPairs(CBaseEntityWrapper *pEntity);

	unsigned int GetSize();
	bool HasElements();

	object Iterate();

private:
	CollisionPairs_t m_setPairs;
};


//-----------------------------------------------------------------------------
// CCollisionSet class.
//-----------------------------------------------------------------------------
class CCollisionSet : public ICollisionRules
{
public:
	void Add(CBaseEntityWrapper *pEntity);
	void Remove(CBaseEntityWrapper *pEntity);
	void Clear();

	bool Contains(CBaseEntityWrapper *pEntity);
	unsigned int GetSize();
	bool HasElements();

	object Iterate();

	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldCollide(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);

private:
	CollisionSet_t m_pSet;
};


//-----------------------------------------------------------------------------
// CCollisionMap class.
//-----------------------------------------------------------------------------
class CCollisionMap: public ICollisionRules
{
public:
	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldCollide(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);

	boost::shared_ptr<CCollisionSet> Find(CBaseEntityWrapper *pEntity);

	void Remove(CBaseEntityWrapper *pEntity);
	void Clear();

	bool Contains(CBaseEntityWrapper *pEntity);
	unsigned int GetSize();
	bool HasElements();

	object Iterate();

private:
	CollisionMap_t m_mapSets;
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

	void RegisterRules(ICollisionRules *pRules);
	void UnregisterRules(ICollisionRules *pRules);

	void OnNetworkedEntityCreated(object oEntity);
	void OnNetworkedEntityDeleted(CBaseEntityWrapper *pEntity);

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
	CUtlVector<ICollisionRules *> m_vecRules;
	CollisionHooksMap_t m_mapHooks;
	std::vector<CollisionScope_t> m_vecScopes;

	boost::unordered_set<unsigned int> m_setSolidMasks;

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
// ICollisionRules extension class.
//-----------------------------------------------------------------------------
class ICollisionRulesExt
{
public:
	template<class T>
	static boost::shared_ptr<T> Construct(ECollisionMode eMode = COLLISION_MODE_PREVENT)
	{
		return boost::shared_ptr<T>(new T, &Finalize<T>);
	}

	template<class T>
	static void Initialize(T *pSelf, object self, ECollisionMode eMode = COLLISION_MODE_PREVENT)
	{
		pSelf->SetMode(eMode);

		static CCollisionManager *pManager = GetCollisionManager();
		pManager->RegisterRules(pSelf);
	}

	template<class T>
	static void Finalize(T *pSelf)
	{
		static CCollisionManager *pManager = GetCollisionManager();
		pManager->UnregisterRules(pSelf);
		delete pSelf;
	}
};


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

// Singleton accessors.
CCollisionListenerManager *GetOnPlayerCollisionListenerManager();
CCollisionListenerManager *GetOnEntityCollisionListenerManager();


#endif // _ENTITIES_COLLISIONS_H
