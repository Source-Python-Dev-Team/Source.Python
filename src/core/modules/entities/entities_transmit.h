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

#ifndef _ENTITIES_TRANSMIT_H
#define _ENTITIES_TRANSMIT_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "modules/entities/entities_entity.h"
#include "modules/memory/memory_hooks.h"
#include "modules/listeners/listeners_manager.h"

// Boost
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
class CTransmitSet;
class CTransmitCache;


//-----------------------------------------------------------------------------
// Typedefs.
//-----------------------------------------------------------------------------
typedef boost::unordered_set<unsigned int> TransmitSet_t;
typedef boost::unordered_map<unsigned int, boost::shared_ptr<CTransmitSet> > TransmitMap_t;

typedef CBitVec<MAX_EDICTS> TransmitCache_t;
typedef boost::unordered_map<unsigned int, CTransmitCache *> TransmitCacheMap_t;

typedef std::pair<unsigned int, unsigned int> TransmitPair_t;

typedef CBitVec<MAX_EDICTS> TransmitStates_t;


//-----------------------------------------------------------------------------
// CTransmitStates class.
//-----------------------------------------------------------------------------
class CTransmitStates : public TransmitStates_t
{
public:
	bool __getitem__(int iIndex);
	void __setitem__(int iIndex, bool bValue);
};


//-----------------------------------------------------------------------------
// ETransmitMode enumeration.
//-----------------------------------------------------------------------------
enum ETransmitMode
{
	TRANSMIT_MODE_ALLOW,
	TRANSMIT_MODE_PREVENT
};


//-----------------------------------------------------------------------------
// ITransmitRules class.
//-----------------------------------------------------------------------------
class ITransmitRules
{
public:
	virtual ~ITransmitRules();

	virtual void OnEntityDeleted(CBaseEntityWrapper *pEntity) = 0;
	virtual bool ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity) = 0;

	virtual bool ShouldTransmit(CBaseEntityWrapper *pPlayer, CBaseEntityWrapper *pEntity);

	virtual void Clear() = 0;
	virtual void UnloadInstance();

	ETransmitMode GetMode();
	void SetMode(ETransmitMode eMode);

private:
	ETransmitMode m_eMode;
};


//-----------------------------------------------------------------------------
// CTransmitCache class.
//-----------------------------------------------------------------------------
class CTransmitCache : private TransmitCache_t
{
public:
	bool HasResult(unsigned int uiIndex);
	bool GetResult(unsigned int uiIndex);
	void SetResult(unsigned int uiIndex, bool bResult);
	void DelResult(unsigned int uiIndex);

private:
	TransmitCache_t m_vecCache;
};


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
class CTransmitManager
{
public:
	friend CTransmitManager *GetTransmitManager();
	friend class CTransmitListenerManager;

private:
	CTransmitManager();
	~CTransmitManager();

public:
	void Initialize();
	void Finalize();

	void RegisterRules(ITransmitRules *pRules);
	void UnregisterRules(ITransmitRules *pRules);

	void OnNetworkedEntityDeleted(CBaseEntityWrapper *pEntity, unsigned int uiEntity);
	void OnLevelShutdown();

	void RegisterTransmitHook(object oCallback);
	void UnregisterTransmitHook(object oCallback);

private:
	static bool CheckTransmit(HookType_t eHookType, CHook *pHook);

	CTransmitCache *GetCache(unsigned int uiIndex);

protected:
	void IncRef();
	void DecRef();

private:
	bool m_bInitialized;
	unsigned int m_uiRefCount;
	CUtlVector<ITransmitRules *> m_vecRules;

	CHook *m_pHook;
	TransmitCacheMap_t m_mapCache;

	CListenerManager *m_pTransmitHooks;
};

// Singleton accessor.
inline CTransmitManager *GetTransmitManager()
{
	static CTransmitManager *s_pEntityTransmitManager = new CTransmitManager;
	return s_pEntityTransmitManager;
}


//-----------------------------------------------------------------------------
// TransmitPairs_t definition and specializations.
//-----------------------------------------------------------------------------
struct TransmitPairHash_t
{
	inline std::size_t operator()(TransmitPair_t const &p) const {
		std::size_t s = 0;
		boost::hash_combine(s, p.first < p.second ? p.first : p.second);
		boost::hash_combine(s, p.first < p.second ? p.second : p.first);
		return s;
	}
};

struct TransmitPairEquals_t
{
	inline bool operator()(const TransmitPair_t &a, const TransmitPair_t &b) const {
		return (a == b) || (a.first == b.second && a.second == b.first); 
	}
};

typedef boost::unordered_set<TransmitPair_t, TransmitPairHash_t, TransmitPairEquals_t> TransmitPairs_t;


//-----------------------------------------------------------------------------
// CTransmitHash class.
//-----------------------------------------------------------------------------
class CTransmitHash : public ITransmitRules
{
public:
	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity);

	void AddPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);
	void RemovePair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);
	void RemovePairs(CBaseEntityWrapper *pEntity);

	void Clear();

	bool Contains(CBaseEntityWrapper *pEntity);
	bool HasPair(unsigned int uiEntity, unsigned int uiOther);
	bool HasPair(CBaseEntityWrapper *pEntity, CBaseEntityWrapper *pOther);

	unsigned int GetCount(CBaseEntityWrapper *pEntity);
	list GetPairs(CBaseEntityWrapper *pEntity);

	unsigned int GetSize();
	bool HasElements();

	object Iterate();

private:
	TransmitPairs_t m_setPairs;
};


//-----------------------------------------------------------------------------
// CTransmitSet class.
//-----------------------------------------------------------------------------
class CTransmitSet : public ITransmitRules
{
public:
	void Add(CBaseEntityWrapper *pEntity);
	void Remove(CBaseEntityWrapper *pEntity);
	void Clear();

	bool Contains(unsigned int uiEntity);
	bool Contains(CBaseEntityWrapper *pEntity);
	unsigned int GetSize();
	bool HasElements();

	object Iterate();

	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity);

private:
	TransmitSet_t m_pSet;
};


//-----------------------------------------------------------------------------
// CTransmitMap class.
//-----------------------------------------------------------------------------
class CTransmitMap: public ITransmitRules
{
public:
	void OnEntityDeleted(CBaseEntityWrapper *pEntity);
	bool ShouldTransmit(unsigned int uiPlayer, unsigned int uiEntity);

	boost::shared_ptr<CTransmitSet> Find(unsigned int uiEntity);
	boost::shared_ptr<CTransmitSet> Find(CBaseEntityWrapper *pEntity);

	void Remove(CBaseEntityWrapper *pEntity);
	void Clear();

	bool Contains(unsigned int uiEntity);
	bool Contains(CBaseEntityWrapper *pEntity);
	unsigned int GetSize();
	bool HasElements();

	object Iterate();

private:
	TransmitMap_t m_mapSets;
};


//-----------------------------------------------------------------------------
// ITransmitRules extension class.
//-----------------------------------------------------------------------------
class ITransmitRulesExt
{
public:
	template<class T>
	static boost::shared_ptr<T> Construct(ETransmitMode eMode = TRANSMIT_MODE_PREVENT)
	{
		return boost::shared_ptr<T>(new T, &Finalize<T>);
	}

	template<class T>
	static void Initialize(T *pSelf, object self, ETransmitMode eMode = TRANSMIT_MODE_PREVENT)
	{
		pSelf->SetMode(eMode);

		static CTransmitManager *pManager = GetTransmitManager();
		pManager->RegisterRules(pSelf);
	}

	template<class T>
	static void Finalize(T *pSelf)
	{
		static CTransmitManager *pManager = GetTransmitManager();
		pManager->UnregisterRules(pSelf);
		delete pSelf;
	}
};


//-----------------------------------------------------------------------------
// CTransmitListenerManager class.
//-----------------------------------------------------------------------------
class CTransmitListenerManager : public CListenerManager
{
public:
	CTransmitListenerManager();

	void Initialize();
	void Finalize();

	bool CallCallbacks(object oPlayer, object oEntity);

private:
	bool m_bInitialized;
};

// Singleton accessors.
CTransmitListenerManager *GetOnEntityTransmitListenerManager();
CTransmitListenerManager *GetOnPlayerTransmitListenerManager();


#endif // _ENTITIES_TRANSMIT_H
