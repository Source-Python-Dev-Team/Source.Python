/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _STUDIO_WRAP_H
#define _STUDIO_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "studio.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// studiohdr_t extension class.
//-----------------------------------------------------------------------------
class HeaderExt
{
};


//-----------------------------------------------------------------------------
// mstudiobone_t extension class.
//-----------------------------------------------------------------------------
class BoneExt
{
public:
	static const char *get_name(mstudiobone_t *pBone)
	{
		return (const char *)pBone->pszName();
	}

	static CPointer *get_procedure(mstudiobone_t *pBone)
	{
		return new CPointer((unsigned long)pBone->pProcedure());
	}

	static const char *get_surface_name(mstudiobone_t *pBone)
	{
		return (const char *)pBone->pszSurfaceProp();
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
class EventExt
{
public:
	static const char *get_name(mstudioevent_t *pEvent)
	{
		return (const char *)pEvent->pszEventName();
	}
};


//-----------------------------------------------------------------------------
// mstudioattachment_t extension class.
//-----------------------------------------------------------------------------
class AttachmentExt
{
public:
	static const char *get_name(mstudioattachment_t *pAttachment)
	{
		return (const char *)pAttachment->pszName();
	}
};


#endif // _STUDIO_CACHE_WRAP_H
