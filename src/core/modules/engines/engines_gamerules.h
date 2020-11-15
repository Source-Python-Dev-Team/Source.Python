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

#ifndef _ENGINES_GAMERULES_H
#define _ENGINES_GAMERULES_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// SDK
#include "strtools.h"


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
class CGameRulesWrapper;

int find_game_rules_property_offset(const char* name);
const char* find_game_rules_proxy_name();
CGameRulesWrapper* find_game_rules();


//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
class CGameRulesWrapper
{
public:

	// Getter methods
	template<class T>
	T GetProperty(const char* name)
	{
		return GetPropertyByOffset<T>(find_game_rules_property_offset(name));
	}

	template<class T>
	T GetPropertyByOffset(int offset)
	{
		return *(T *) (((unsigned long) this) + offset);
	}

	const char* GetPropertyStringArray(const char* name)
	{
		return GetPropertyStringArrayByOffset(find_game_rules_property_offset(name));
	}

	const char* GetPropertyStringArrayByOffset(int offset)
	{
		return (const char*) (((unsigned long) this) + offset);
	}

	// Setter methods
	template<class T>
	void SetProperty(const char* name, T value)
	{
		SetPropertyByOffset<T>(find_game_rules_property_offset(name), value);
	}

	template<class T>
	void SetPropertyByOffset(int offset, T value)
	{
		*(T *) (((unsigned long) this) + offset) = value;
	}

	void SetPropertyStringArray(const char* name, const char* value)
	{
		SetPropertyStringArrayByOffset(find_game_rules_property_offset(name), value);
	}

	void SetPropertyStringArrayByOffset(int offset, const char* value)
	{
		strcpy((char*) (((unsigned long) this) + offset), value);
	}
};

#endif // _ENGINES_GAMERULES_H
