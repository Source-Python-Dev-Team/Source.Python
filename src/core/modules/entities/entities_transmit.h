/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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
#include "public/const.h"
#include "public/bitvec.h"
#include "public/tier1/utlvector.h"

#include "modules/listeners/listeners_manager.h"
#include "modules/memory/memory_function.h"


//-----------------------------------------------------------------------------
// TransmitStates_t definition.
//-----------------------------------------------------------------------------
typedef CBitVec<MAX_EDICTS> TransmitStates_t;


//-----------------------------------------------------------------------------
// ETransmitTarget enumeration.
//-----------------------------------------------------------------------------
enum ETransmitTarget
{
	TRANSMIT_TARGET_ENTITY,
	TRANSMIT_TARGET_PLAYER
};


//-----------------------------------------------------------------------------
// CBaseTransmitCriteria class.
//-----------------------------------------------------------------------------
class CBaseTransmitCriteria: public TransmitStates_t
{
public:
	CBaseTransmitCriteria(ETransmitTarget eTarget = TRANSMIT_TARGET_ENTITY);

public:
	ETransmitTarget m_eTarget;
};


//-----------------------------------------------------------------------------
// ETransmitType enumeration.
//-----------------------------------------------------------------------------
enum ETransmitType
{
	TRANSMIT_IN,
	TRANSMIT_OUT
};


//-----------------------------------------------------------------------------
// CBaseTransmitFilter class.
//-----------------------------------------------------------------------------
class CBaseTransmitFilter
{
public:
	CBaseTransmitFilter(ETransmitType eType = TRANSMIT_OUT, CBaseTransmitCriteria *pCriteria = NULL, object oOverride = object());

public:
	bool get_override();
	void set_override(object oOverride);
	bool has_override();

	object get_callback();
	void set_callback(object oCallback);

private:
	bool m_bOverride;
	bool m_bHasOverride;

	object m_oCallback;

public:
	ETransmitType m_eType;
	CBaseTransmitCriteria *m_pCriteria;
};


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
class CTransmitManager
{
private:
	CTransmitManager();

private:
	void initialize();
	static bool _post_check_transmit(HookType_t eHookType, CHook *pHook);
	bool handle_filters(ETransmitType eType, int iIndex, unsigned int uiPlayer);

public:
	static CTransmitManager *GetSingleton();

	void register_filter(CBaseTransmitFilter *pFilter);
	void unregister_filter(CBaseTransmitFilter *pFilter);

private:
	bool m_bInitialized;
	CUtlVector<CBaseTransmitFilter *> m_vecFilters;
};

inline CTransmitManager *GetTransmitManager() { return CTransmitManager::GetSingleton(); }


#endif // _ENTITIES_TRANSMIT_H
