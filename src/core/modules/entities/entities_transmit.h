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
#include <mutex>
#include <vector>
#include <utility>

#include "public/const.h"
#include "public/bitvec.h"
#include "public/tier1/utlvector.h"

#include "utilities/baseentity.h"

#include "modules/listeners/listeners_manager.h"
#include "modules/memory/memory_function.h"

// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;


//-----------------------------------------------------------------------------
// TransmitStates_t definition.
//-----------------------------------------------------------------------------
typedef CBitVec<MAX_EDICTS> TransmitStates_t;


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
class CTransmitManager
{
private:
	CTransmitManager();

	void initialize();
	void finalize();

	void handle_filters(CCheckTransmitInfo* pInfo, unsigned int player_index);
	static bool _post_check_transmit(HookType_t eHookType, CHook *pHook);

public:
	using filter_pair = std::pair<TransmitStates_t, TransmitStates_t>;

	static CTransmitManager* get_instance();

	static void create();
	static void destroy();

	void hide(int entity_index);
	void hide_from(int entity_index, int player_index);

	void unhide(int entity_index);
	void unhide_from(int entity_index, int player_index);

	void reset(int entity_index);
	void reset_from(int entity_index, int player_index);
	void reset_all();

	bool is_hidden(int entity_index);
	bool is_hidden_from(int entity_index, int player_index);

	tuple get_hidden_states(int entity_index);

private:
    static std::once_flag init_flag;
    static CTransmitManager* instance;

	void* m_pCheckTransmit = nullptr;

	std::vector<std::pair<TransmitStates_t, TransmitStates_t>> m_vecFilters = decltype(m_vecFilters)(ABSOLUTE_PLAYER_LIMIT);
};

inline CTransmitManager *GetTransmitManager() { return CTransmitManager::get_instance(); }


#endif // _ENTITIES_TRANSMIT_H
