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
ISimpleEntityHook::ISimpleEntityHook(const char* func_name, HookHandlerFn* hook_handler, bool pre, bool post)
{
	this->func_name = func_name;
	this->hook_handler = hook_handler;
	this->pre = pre;
	this->post = post;
}

bool ISimpleEntityHook::Initialize(CBaseEntity* pEntity)
{
	if (!Test(pEntity))
	{
		return false;
	}

	unsigned int index;
	if (!IndexFromBaseEntity(pEntity, index))
	{
		return false;
	}
	
	PythonLog(3, "Initializing core hook (%s)...", this->func_name);

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

		PythonLog(0, "Failed to initialize the core hook.");

		return true;
	}

	CHook* pHook = GetHookManager()->FindHook((void*) func->m_ulAddr);
	if (!pHook)
	{
		pHook = GetHookManager()->HookFunction(
			(void*) func->m_ulAddr,
			func->m_pCallingConvention);

		if (!pHook)
		{
			PythonLog(0, "Could create a hook.");
			return false;
		}
	}

	if (this->pre)
	{
		PythonLog(4, "Adding a pre hook.");
		pHook->AddCallback(HOOKTYPE_PRE, this->hook_handler);
	}

	if (this->post)
	{
		PythonLog(4, "Adding a post hook.");
		pHook->AddCallback(HOOKTYPE_POST, this->hook_handler);
	}
	
	PythonLog(3, "Core hook has been initialized.");
	return true;
}


//---------------------------------------------------------------------------------
// PlayerHook
//---------------------------------------------------------------------------------
PlayerHook::PlayerHook(const char* func_name, HookHandlerFn* hook_handler, bool pre, bool post)
	:ISimpleEntityHook(func_name, hook_handler, pre, post)
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
		int buttons = extract<int>(player.attr("buttons"));
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
