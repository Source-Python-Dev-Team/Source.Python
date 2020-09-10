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
#include <algorithm>
#include <array>
#include <utility>
#include <vector>

#ifdef _WIN32
	#include <intrin.h>
#endif

// Source SDK
#include "public/bitvec.h"

// Source.Python
#include "utilities/baseentity.h"
#include "modules/memory/memory_function.h"

// Boost.Python
#include "boost/python.hpp"


//-----------------------------------------------------------------------------
// TransmitStates_t definition.
//-----------------------------------------------------------------------------
typedef CBitVec<MAX_EDICTS> TransmitStates_t;


//-----------------------------------------------------------------------------
// Filter struct.
//-----------------------------------------------------------------------------
struct Filter{
	TransmitStates_t hide;
	TransmitStates_t show;
};


//-----------------------------------------------------------------------------
// CTransmitManager class.
//-----------------------------------------------------------------------------
class CTransmitManager
{
private:
	CTransmitManager();
	~CTransmitManager();

	void handle_filters(TransmitStates_t* pTransmitEdict, unsigned int player_index);
	static bool _post_check_transmit(HookType_t eHookType, CHook *pHook);

public:
	static CTransmitManager* get_instance();

	static void create();
	//static void destroy();

	void add_player(edict_t* pEdict, unsigned int player_index);
	void remove_player(edict_t* pEdict, unsigned int player_index);

	void hide(unsigned int entity_index);
	void hide_from(unsigned int entity_index, unsigned int player_index);

	void show(unsigned int entity_index);
	void show_from(unsigned int entity_index, unsigned int player_index);

	void reset(unsigned int entity_index);
	void reset_from(unsigned int entity_index, unsigned int player_index);
	void reset_player(unsigned int player_index);
	void reset_all();

	bool is_hidden(unsigned int entity_index);
	bool is_hidden_from(unsigned int entity_index, unsigned int player_index);

	boost::python::tuple get_hidden_player(unsigned int entity_index);
	boost::python::tuple get_hidden_entity(unsigned int player_index);

	inline bool find_filter_index(unsigned int player_index, unsigned char& filter_index)
	{
		for (unsigned int i = 1; i <= m_arrayFilterIndexes.size(); ++i)
		{
			auto it = std::find(m_arrayFilterIndexes.begin(), m_arrayFilterIndexes.end(), i);
			if (it == m_arrayFilterIndexes.end())
			{
				filter_index = i;
				return true;
			}
		}
		return false;
	}

	inline bool get_filter_index(unsigned int player_index, unsigned char& filter_index)
	{
		filter_index = m_arrayFilterIndexes[player_index-1];
		if (filter_index)
			return true;
		else
			return false;
	}

private:
	static CTransmitManager* instance;

	//void* m_pCheckTransmit;

	unsigned int current_clients;
	std::vector<Filter> m_vecFilters;
	std::array<unsigned char, 64> m_arrayFilterIndexes;
};


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
inline unsigned int bit_ceil(unsigned int x)
{
#ifdef _WIN32
	unsigned long ret;
	_BitScanReverse(&ret, (x-1));
	return 1 << (32 - (31 ^ ret));//(1 << (32 - __lzcnt(x))))
#else
	return 1 << (32 - __builtin_clz(x-1));
#endif
}


#endif // _ENTITIES_TRANSMIT_H
