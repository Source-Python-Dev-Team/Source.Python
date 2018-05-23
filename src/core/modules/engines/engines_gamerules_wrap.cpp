/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
// Source.Python
#include "export_main.h"
#include "sp_main.h"

// SDK
#include "game/shared/gamerules.h"


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_gamerules(scope);


//---------------------------------------------------------------------------------
// Declare the _sound module.
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _gamerules)
{
	export_gamerules(_gamerules);
}


//-----------------------------------------------------------------------------
// Expose gamerules.
//-----------------------------------------------------------------------------
void export_gamerules(scope _gamerules)
{
	// IGameSystem
	class_<IGameSystem, boost::noncopyable> GameSystem("GameSystem", no_init);
	
	GameSystem.add_property(
		"name", 
		&IGameSystem::Name);

	GameSystem ADD_MEM_TOOLS(IGameSystem);


	// IGameSystemPerFrame
	class_<IGameSystemPerFrame, bases<IGameSystem>, boost::noncopyable> GameSystemPerFrame("GameSystemPerFrame", no_init);

	GameSystemPerFrame ADD_MEM_TOOLS(IGameSystemPerFrame);


	// CBaseGameSystemPerFrame
	class_<CBaseGameSystemPerFrame, bases<IGameSystemPerFrame>, boost::noncopyable> BaseGameSystemPerFrame("BaseGameSystemPerFrame", no_init);

	BaseGameSystemPerFrame ADD_MEM_TOOLS(CBaseGameSystemPerFrame);


	// CAutoGameSystemPerFrame
	class_<CAutoGameSystemPerFrame, bases<CBaseGameSystemPerFrame>, boost::noncopyable> AutoGameSystemPerFrame("AutoGameSystemPerFrame", no_init);

	AutoGameSystemPerFrame.add_property(
		"next", 
		make_getter(&CAutoGameSystemPerFrame::m_pNext, reference_existing_object_policy()));

	AutoGameSystemPerFrame ADD_MEM_TOOLS(CAutoGameSystemPerFrame);


	// CGameRules
	class_<CGameRules, bases<CAutoGameSystemPerFrame>, boost::noncopyable> GameRules("GameRules", no_init);

	GameRules.add_property(
		"game_description",
		&CGameRules::GetGameDescription);

	GameRules.def(
		"should_collide",
		&CGameRules::ShouldCollide);

	GameRules.def(
		"get_tagged_convar_list",
		&CGameRules::GetTaggedConVarList);

	GameRules ADD_MEM_TOOLS(CGameRules);

	BEGIN_CLASS_INFO(CGameRules)
		FUNCTION_INFO(DeathNotice)
	END_CLASS_INFO()
}
