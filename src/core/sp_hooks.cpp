/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2018 Source Python Development Team.  All rights reserved.
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
// Includes
//---------------------------------------------------------------------------------
// Boost.Python
#include "boost/python.hpp"
using namespace boost::python;

// SDK
#include "edict.h"
#include "game/shared/shareddefs.h"
#include "game/shared/usercmd.h"

// Source.Python
#include "sp_hooks.h"
#include "utilities/conversions.h"
#include "utilities/call_python.h"
#include "modules/entities/entities_entity.h"
#include "modules/listeners/listeners_manager.h"


//---------------------------------------------------------------------------------
// GLOBAL VARIABLES
//---------------------------------------------------------------------------------
std::vector<IEntityHook*> g_EntityHooks;


//---------------------------------------------------------------------------------
// ISimpleEntityHook
//---------------------------------------------------------------------------------
ISimpleEntityHook::ISimpleEntityHook(const char* func_name, HookHandlerFn* hook_handler, HookType_t hook_type)
{
	this->func_name = func_name;
	this->hook_handler = hook_handler;
	this->hook_type = hook_type;
}

bool ISimpleEntityHook::Initialize(CBaseEntity* pEntity)
{
	if (!Test(pEntity))
	{
		return false;
	}

	PythonLog(4, "Initializing core hook (%s)...", this->func_name);

	unsigned int index;
	if (!IndexFromBaseEntity(pEntity, index))
	{
		PythonLog(0, "Failed to convert the entity pointer to an index (%s)", this->func_name);
		return true;
	}

	CFunction* func = NULL;
	try
	{
		static object Entity = import("entities.entity").attr("Entity");

		object entity = Entity(index);
		func = extract<CFunction*>(entity.attr(this->func_name));
	}
	catch (...)
	{
		PyErr_Print();
		PyErr_Clear();

		PythonLog(0, "Failed to import entities.entity.Entity or to retrieve %s.", this->func_name);
		return true;
	}

	if (!func->AddHook(this->hook_type, this->hook_handler))
	{
		PythonLog(0, "Could not create a hook for %s.", this->func_name);
		return true;
	}

	PythonLog(3, "Core hook (%s) has been initialized.", this->func_name);
	return true;
}


//---------------------------------------------------------------------------------
// PlayerHook
//---------------------------------------------------------------------------------
PlayerHook::PlayerHook(const char* func_name, HookHandlerFn* hook_handler, HookType_t hook_type)
	:ISimpleEntityHook(func_name, hook_handler, hook_type)
{
}

bool PlayerHook::Test(CBaseEntity* pEntity)
{
	CBaseEntityWrapper* pWrapper = (CBaseEntityWrapper*) pEntity;
	return pWrapper->IsPlayer();
}


//---------------------------------------------------------------------------------
// FUNCTIONS
//---------------------------------------------------------------------------------
void InitHooks()
{
	CBaseEntity* pEntity = (CBaseEntity *) servertools->FirstEntity();
	while (pEntity)
	{
		InitHooks(pEntity);
		pEntity = (CBaseEntity *) servertools->NextEntity(pEntity);
	}
}

void InitHooks(CBaseEntity* pEntity)
{
	if (!pEntity)
		return;

	std::vector<IEntityHook*>::iterator it = g_EntityHooks.begin();
	while (it != g_EntityHooks.end())
	{
		IEntityHook* pHook = *it;

		if (pHook->Initialize(pEntity))
		{
			it = g_EntityHooks.erase(it);
			delete pHook;
		}
		else
		{
			++it;
		}
	}
}

//---------------------------------------------------------------------------------
// HOOKS
//---------------------------------------------------------------------------------
bool PrePlayerRunCommand(HookType_t hook_type, CHook* pHook)
{
	GET_LISTENER_MANAGER(OnPlayerRunCommand, run_command_manager);
	GET_LISTENER_MANAGER(OnButtonStateChanged, button_state_manager);

	if (!run_command_manager->GetCount() && !button_state_manager->GetCount())
		return false;

	static object Player = import("players.entity").attr("Player");

	CBaseEntity* pEntity = pHook->GetArgument<CBaseEntity*>(0);
	unsigned int index;
	if (!IndexFromBaseEntity(pEntity, index))
		return false;
	
	// https://github.com/Source-Python-Dev-Team/Source.Python/issues/149
#if defined(ENGINE_BRANCH_TF2)
	CUserCmd cmd = *pHook->GetArgument<CUserCmd*>(1);
	CUserCmd* pCmd = &cmd;
#else
	CUserCmd* pCmd = pHook->GetArgument<CUserCmd*>(1);
#endif

	object player = Player(index);
	CALL_LISTENERS(OnPlayerRunCommand, player, ptr(pCmd));

	if (button_state_manager->GetCount())
	{
		CBaseEntityWrapper* pWrapper = (CBaseEntityWrapper*) pEntity;
		static int offset = pWrapper->FindDatamapPropertyOffset("m_nButtons");

		int buttons = pWrapper->GetDatamapPropertyByOffset<int>(offset);
		if (buttons != pCmd->buttons)
		{
			CALL_LISTENERS(OnButtonStateChanged, player, buttons, pCmd->buttons);
		}
	}
	
#if defined(ENGINE_BRANCH_TF2)
	CUserCmd* pRealCmd = pHook->GetArgument<CUserCmd*>(1);
	memcpy(pRealCmd, pCmd, sizeof(CUserCmd));
#endif

	return false;
}
