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
#include "modules/physics/physics.h"
#include "modules/memory/memory_hooks.h"

// Boost
#include "boost/unordered_map.hpp"

// SDK
#include "vphysics/object_hash.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
struct CollisionHookData_t;


//-----------------------------------------------------------------------------
// Typedefs.
//-----------------------------------------------------------------------------
#if !defined(ENGINE_CSGO) && !defined(ENGINE_BLADE) && !defined(ENGINE_BMS)
	typedef bool (*ShouldHitFunc_t)( IHandleEntity *pHandleEntity, int contentsMask );
#endif

typedef boost::unordered_map<CHook *, CollisionHookData_t> CollisionHooksMap_t;


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
	CTraceFilterSimpleWrapper *m_pFilter;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
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

	virtual void AddPair(void *pObject, void *pOther) = 0;
	virtual void RemovePair(void *pObject, void *pOther) = 0;
	virtual void RemovePairs(void *pObject) = 0;

	virtual bool IsInHash(void *pObject) = 0;
	virtual bool HasPair(void *pObject, void *pOther) = 0;

	virtual int GetCount(void *pObject) = 0;
	virtual list GetPairs(void *pObject) = 0;

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

	void AddPair(void *pObject, void *pOther);
	void RemovePair(void *pObject, void *pOther);
	void RemovePairs(void *pObject);

	bool IsInHash(void *pObject);
	bool HasPair(void *pObject, void *pOther);

	int GetCount(void *pObject);
	list GetPairs(void *pObject);

private:
	IPhysicsObjectPairHash *m_pHash;
};


//-----------------------------------------------------------------------------
// CCollisionManager class.
//-----------------------------------------------------------------------------
class CCollisionManager
{
public:
	friend class CEntityCollisionListenerManager;

public:
	CCollisionManager();

	void Initialize();
	void Finalize();

	void RegisterHash(ICollisionHash *pHash);
	void UnregisterHash(ICollisionHash *pHash);

protected:
	void IncRef();
	void DecRef();

private:
	template<typename T>
	static CHook *GetHook(T tFunc, const char *szDebugName);

	template<typename T>
	void RegisterHook(T tFunc, unsigned int uiFilterIndex, unsigned int nMaskIndex, const char *szDebugName);

	template<typename T>
	void UnregisterHook(T tFunc, const char *szDebugName);

	static bool EnterScope(HookType_t eHookType, CHook *pHook);
	static bool ExitScope(HookType_t eHookType, CHook *pHook);

	static bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask);

private:
	bool m_bInitialized;
	unsigned int m_uiRefCount;
	CUtlVector<ICollisionHash *> m_vecHashes;
	CollisionHooksMap_t m_mapHooks;
	std::vector<CollisionScope_t> m_vecScopes;
};

// Singleton accessor.
inline CCollisionManager *GetCollisionManager()
{
	static CCollisionManager *s_pEntityCollisionManager = new CCollisionManager;
	return s_pEntityCollisionManager;
}


//-----------------------------------------------------------------------------
// CEntityCollisionListenerManager class.
//-----------------------------------------------------------------------------
class CEntityCollisionListenerManager: public CListenerManager
{
public:
	CEntityCollisionListenerManager();

	void Initialize();
	void Finalize();

private:
	bool m_bInitialized;
};

// Singleton accessor.
CEntityCollisionListenerManager *GetOnEntityCollisionListenerManager();


#endif // _ENTITIES_COLLISIONS_H
