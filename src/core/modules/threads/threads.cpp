/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2023 Source Python Development Team.  All rights reserved.
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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "sp_main.h"
#include "threads.h"
#include "utilities.h"
#include "utilities/wrap_macros.h"

#include "dbg.h"
#include "eiface.h"
#include "convar.h"
#include "tier0/threadtools.h"

#include ENGINE_INCLUDE_PATH(threads.h)


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern CGlobalVars *gpGlobals;


//-----------------------------------------------------------------------------
// ConVar "sp_thread_yielding" registration.
//-----------------------------------------------------------------------------
ConVar sp_thread_yielding(
	"sp_thread_yielding", "0", FCVAR_NONE,
#ifndef TY_NotImplemented
	"If enabled, yields remaining cycles to Python threads every frame."
#else
	"Thread yielding is not implemented on '" XSTRINGIFY(SOURCE_ENGINE) "' at this time."
#endif
);


//-----------------------------------------------------------------------------
// CThreadYielder initialization.
//-----------------------------------------------------------------------------
unsigned long CThreadYielder::s_nRefCount = 0;


//-----------------------------------------------------------------------------
// CThreadYielder class.
//-----------------------------------------------------------------------------
PyObject *CThreadYielder::__enter__(PyObject *pSelf)
{
	if (!sp_thread_yielding.GetBool()) {
		return incref(pSelf);
	}

#ifndef TY_NotImplemented
	++s_nRefCount;

	static bool s_bHooked = false;
	if (!s_bHooked) {
		WriteJMP((unsigned char *)::TY_Hook, (void *)CThreadYielder::ThreadSleep);
		s_bHooked = true;
	}
#else
	static object warn = import("warnings").attr("warn");
	warn("Thread yielding is not implemented on '" XSTRINGIFY(SOURCE_ENGINE) "' at this time.");
#endif

	return incref(pSelf);
}

void CThreadYielder::__exit__(PyObject *, PyObject *, PyObject *, PyObject *)
{
#ifndef TY_NotImplemented
	if (!s_nRefCount) {
		return;
	}

	--s_nRefCount;
#endif
}

void CThreadYielder::ThreadSleep(unsigned nTime)
{
#ifndef TY_NotImplemented
	if (!s_nRefCount || !ThreadInMainThread() || !PyGILState_Check()) {
		TY_Sleep(nTime);
	}
	else {
		Py_BEGIN_ALLOW_THREADS;
		TY_Yield();
		TY_Sleep(nTime);
		Py_END_ALLOW_THREADS;
		DevMsg(2, MSG_PREFIX "Yielded %d%s on tick %d!\n", nTime, XSTRINGIFY(TY_Unit), gpGlobals->framecount);
	}
#endif
}

bool CThreadYielder::IsImplemented()
{
#ifndef TY_NotImplemented
	return true;
#else
	return false;
#endif
}
