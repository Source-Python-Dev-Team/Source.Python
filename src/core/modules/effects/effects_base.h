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

#ifndef _EFFECTS_BASE_H
#define _EFFECTS_BASE_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "utilities/sp_util.h"
#include "game/server/basetempentity.h"
#include "modules/memory/memory_alloc.h"


//-----------------------------------------------------------------------------
// CBaseTempEntity extension class.
//-----------------------------------------------------------------------------
class BaseTempEntityExt
{
public:
	static boost::shared_ptr<CBaseTempEntity> _wrap_ptr(CPointer *pPtr)
	{
		return boost::shared_ptr<CBaseTempEntity>((CBaseTempEntity *)(void *)pPtr->m_ulAddr, &NeverDeleteDeleter<CBaseTempEntity *>);
	}

	static boost::shared_ptr<CBaseTempEntity> _copy_base(CBaseTempEntity *pBaseTempEntity, size_t nSize)
	{
		void *pReturnValue = UTIL_Alloc(nSize);
		if (!pReturnValue)
			BOOST_RAISE_EXCEPTION(PyExc_MemoryError, "Unable to allocate memory.");

		memcpy(pReturnValue, (void *)pBaseTempEntity, nSize);
		return boost::shared_ptr<CBaseTempEntity>((CBaseTempEntity *)pReturnValue, &Deleter);
	}

	static void Deleter(CBaseTempEntity *pBaseTempEntity)
	{
		UTIL_Dealloc((void *)pBaseTempEntity);
	}
};


#endif // _EFFECTS_BASE_H
