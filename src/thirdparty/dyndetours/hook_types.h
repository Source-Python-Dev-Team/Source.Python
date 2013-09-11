// =======================================================================
// File: hook_types.h
// Purpose: Defines some useful constants for the hook system.
// =======================================================================
#ifndef _HOOK_TYPES_H
#define _HOOK_TYPES_H

// =======================================================================
// Constants for callback execution.
// =======================================================================
enum eHookType
{
	TYPE_PRE,  // Execute callback before original.
	TYPE_POST, // Execute callback after original.
};

// =======================================================================
// Constants for the hooking system.
// =======================================================================
enum eHookRes
{
	HOOKRES_ERROR=-1, // Problem occured.
	HOOKRES_NONE=0,	  // Call orig like normal.
	HOOKRES_NEWPARAMS,// Parameters modified
	HOOKRES_OVERRIDE, // Don't call orig function.
};

// =======================================================================
// Hook result buffer
// =======================================================================
struct HookRetBuf_t
{
	eHookRes eRes;	// Result action
	void* pRetBuf;  // Dynamically allocated space for the return value.
};

#endif // _HOOK_CONSTANTS_H
