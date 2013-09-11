// =======================================================================
// File: ICallbackManager.h
// Purpose: The callback manager interface. Each language should implement
//	a callback manager to manage callback processing.
// =======================================================================
#ifndef _ICALLBACKMANAGER_H
#define _ICALLBACKMANAGER_H

// =======================================================================
// Includes
// =======================================================================
#include "hook_types.h"

// =======================================================================
// Forward declarations.
// =======================================================================
class CDetour;

// =======================================================================
// The ICallbackManager interface.
// =======================================================================
class ICallbackManager
{
	public:
		// ------------------------------------
		// Virtual destructor.
		// ------------------------------------
		virtual ~ICallbackManager() {};

		// ------------------------------------
		// Callback adding / removal. pFuncObj
		// is a blackbox for whatever your
		// language uses to represent functions.
		// ------------------------------------
		virtual void Add( void* pFuncObj, eHookType type ) = 0;
		virtual void Remove( void* pFuncObj, eHookType type ) = 0;

		// ------------------------------------
		// Callback processing.
		// ------------------------------------
		virtual HookRetBuf_t* DoPreCalls( CDetour* pDet ) = 0;
		virtual HookRetBuf_t* DoPostCalls( CDetour* pDet ) = 0;

		// ------------------------------------
		// Returns the language this callback
		// is implemented by.
		// ------------------------------------
		virtual const char* GetLang( void ) = 0;
};

#endif // _ICALLBACKMANAGER_H
