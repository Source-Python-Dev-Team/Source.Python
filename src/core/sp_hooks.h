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
#ifndef _SP_HOOKS_H
#define _SP_HOOKS_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
// C++
#include <vector>

// DynamicHooks
#include "hook.h"


//---------------------------------------------------------------------------------
// IEntityHook
//---------------------------------------------------------------------------------
class CBaseEntity;

class IEntityHook
{
public:
	/* This method is called for every existing entity and all future entities
	*  that are created. If true is returned, this object is removed from the
	*  queue and deleted.
	*/
	virtual bool Initialize(CBaseEntity* pEntity) = 0;
	virtual ~IEntityHook() {}
};

extern std::vector<IEntityHook*> g_EntityHooks;

//---------------------------------------------------------------------------------
// ISimpleEntityHook
//---------------------------------------------------------------------------------
class ISimpleEntityHook: public IEntityHook
{
public:
	ISimpleEntityHook(const char* func_name, HookHandlerFn* hook_handler, HookType_t hook_type);
	
	virtual bool Initialize(CBaseEntity* pEntity);
	virtual bool Test(CBaseEntity* pEntity) = 0;

public:
	const char* func_name;
	HookHandlerFn* hook_handler;
	HookType_t hook_type;
};

//---------------------------------------------------------------------------------
// PlayerHook
//---------------------------------------------------------------------------------
class PlayerHook: public ISimpleEntityHook
{
public:
	PlayerHook(const char* func_name, HookHandlerFn* hook_handler, HookType_t hook_type);

	virtual bool Test(CBaseEntity* pEntity);
};


//---------------------------------------------------------------------------------
// FUNCTIONS
//---------------------------------------------------------------------------------
void InitHooks();
void InitHooks(CBaseEntity* pEntity);


//---------------------------------------------------------------------------------
// HOOKS
//---------------------------------------------------------------------------------
bool PrePlayerRunCommand(HookType_t hook_type, CHook* pHook);


#endif // _SP_HOOKS_H
