// =======================================================================
// File: hook_handler.h
// Purpose: Main callback for handling hooks. Every single detour registered
//	using DynDetours will call this callback!
// =======================================================================
#ifndef _HOOK_HANDLER_H
#define _HOOK_HANDLER_H

// =======================================================================
// Includes
// =======================================================================
#include "hook_types.h"

// =======================================================================
// Forward declarations.
// =======================================================================
class CDetour;

// =======================================================================
// Pre-callback hooks.
// =======================================================================
eHookRes Dyn_PreHandler( CDetour* pDetour );
eHookRes Dyn_PostHandler( CDetour* pDetour );

#endif // _HOOK_HANDLER_H
