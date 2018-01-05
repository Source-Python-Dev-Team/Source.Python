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

// ============================================================================
// >> INCLUDES
// ============================================================================
// Source.Python
#include "entities.h"
#include "../../modules/studio/studio.h"


// ============================================================================
// >> External variables
// ============================================================================
extern IMDLCache *modelcache;


// ============================================================================
// >> IServerUnknownExt
// ============================================================================
const char* IServerUnknownExt::GetClassname(IServerUnknown* pUnknown)
{
	IServerNetworkable* pNetworkable = pUnknown->GetNetworkable();
	if (!pNetworkable)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to get the IServerNetworkable pointer.");

	return pNetworkable->GetClassName();
}

bool IServerUnknownExt::IsNetworked(IServerUnknown* pUnknown)
{
	IServerNetworkable *pServerNetworkable = pUnknown->GetNetworkable();
	if (pServerNetworkable)
		return pServerNetworkable->GetEdict() != NULL;

	return false;
}


// ============================================================================
// >> ServerEntityExt
// ============================================================================
void ServerEntityExt::SetModelName(IServerEntity *pServerEntity, const char *szModelName)
{
	CBaseEntity *pBaseEntity = pServerEntity->GetBaseEntity();
	if (!pBaseEntity)
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "BaseEntity is NULL.");

	((CBaseEntityWrapper *)pBaseEntity)->SetKeyValue("model", szModelName);
}

MDLHandle_t ServerEntityExt::get_model_handle(IServerEntity *pServerEntity)
{
	string_t name = pServerEntity->GetModelName();
	if (!name)
		return MDLHANDLE_INVALID;

	return modelcache->FindMDL(name.ToCStr());
}

studiohdr_t* ServerEntityExt::get_model_header(IServerEntity *pServerEntity)
{
	MDLHandle_t handle = get_model_handle(pServerEntity);
	if (handle == MDLHANDLE_INVALID)
		return NULL;

	return modelcache->GetStudioHdr(handle);
}

int ServerEntityExt::lookup_attachment(IServerEntity* pEntity, const char* name)
{
	studiohdr_t* header = get_model_header(pEntity);
	if (!header)
		return INVALID_ATTACHMENT_INDEX;

	for (int i=0; i < header->numlocalattachments; ++i)
	{
		mstudioattachment_t* attachment = header->pLocalAttachment(i);
		if (V_strcmp(attachment->pszName(), name) == 0)
			return i;
	}

	return INVALID_ATTACHMENT_INDEX;
}

int ServerEntityExt::lookup_bone(IServerEntity* pEntity, const char* name)
{
	studiohdr_t* header = get_model_header(pEntity);
	if (!header)
		return INVALID_BONE_INDEX;

	for (int i=0; i < header->numbones; ++i)
	{
		mstudiobone_t* bone = header->pBone(i);
		if (V_strcmp(bone->pszName(), name) == 0)
			return i;
	}

	return INVALID_BONE_INDEX;
}