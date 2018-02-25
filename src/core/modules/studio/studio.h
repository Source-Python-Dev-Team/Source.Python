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

#ifndef _STUDIO_H
#define _STUDIO_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "modules/memory/memory_tools.h"
#include "utilities/wrap_macros.h"
#include "utilities/sp_util.h"

// SDK
#include "public/studio.h"
#include "datacache/imdlcache.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IMDLCache *modelcache;


//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------
#define INVALID_ATTACHMENT_INDEX -1
#define INVALID_BONE_INDEX -1


//-----------------------------------------------------------------------------
// studiohdr_t extension class.
//-----------------------------------------------------------------------------
class ModelHeaderExt
{
public:
	static boost::shared_ptr<studiohdr_t> __init__(const char *szModelName)
	{
		MDLHandle_t usModelIndex = modelcache->FindMDL(szModelName);

		if (usModelIndex == MDLHANDLE_INVALID)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "\"%s\" is not cached.", szModelName);

		return boost::shared_ptr<studiohdr_t>(modelcache->GetStudioHdr(usModelIndex), &NeverDeleteDeleter<studiohdr_t *>);
	}
};


//-----------------------------------------------------------------------------
// mstudiobone_t extension class.
//-----------------------------------------------------------------------------
class ModelBoneExt
{
public:
	static const char *get_name(mstudiobone_t *pModelBone)
	{
		return (const char *)pModelBone->pszName();
	}

	static CPointer *get_procedure(mstudiobone_t *pModelBone)
	{
		return new CPointer((unsigned long)pModelBone->pProcedure());
	}

	static const char *get_surface_name(mstudiobone_t *pModelBone)
	{
		return (const char *)pModelBone->pszSurfaceProp();
	}
};


//-----------------------------------------------------------------------------
// mstudiohitboxset_t extension class.
//-----------------------------------------------------------------------------
class HitboxSetExt
{
public:
	static const char *get_name(mstudiohitboxset_t *pHitboxSet)
	{
		return (const char *)pHitboxSet->pszName();
	}
};


//-----------------------------------------------------------------------------
// mstudiohitboxset_t extension class.
//-----------------------------------------------------------------------------
class AnimationExt
{
public:
	static const char *get_name(mstudioanimdesc_t *pAnimation)
	{
		return (const char *)pAnimation->pszName();
	}

	static mstudiomovement_t *get_movement(mstudioanimdesc_t *pAnimation, int iMovementIndex)
	{
		return reinterpret_cast<mstudiomovement_t *>(pAnimation->pMovement(iMovementIndex));
	}
};


//-----------------------------------------------------------------------------
// mstudioseqdesc_t extension class.
//-----------------------------------------------------------------------------
class SequenceExt
{
public:
	static const char *get_label(mstudioseqdesc_t *pSequence)
	{
		return (const char *)pSequence->pszLabel();
	}

	static const char *get_activity(mstudioseqdesc_t *pSequence)
	{
		return (const char *)pSequence->pszActivityName();
	}
};


//-----------------------------------------------------------------------------
// mstudioevent_t extension class.
//-----------------------------------------------------------------------------
class SequenceEventExt
{
public:
	static const char *get_name(mstudioevent_t *pSequenceEvent)
	{
		return (const char *)pSequenceEvent->pszEventName();
	}
};


//-----------------------------------------------------------------------------
// mstudioattachment_t extension class.
//-----------------------------------------------------------------------------
class ModelAttachmentExt
{
public:
	static const char *get_name(mstudioattachment_t *pModelAttachment)
	{
		return (const char *)pModelAttachment->pszName();
	}
};


#endif // _STUDIO_H
