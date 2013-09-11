// =======================================================================
// File: cpp_manager.h
// Purpose: Declares the CCPPManager callback manager. This class is
//	responsible for adding / creating callbacks for the C++ language.
// =======================================================================
#ifndef _CPP_MANAGER_H
#define _CPP_MANAGER_H

// =======================================================================
// Includes
// =======================================================================
#include "callback_manager.h"
#include "hook_types.h"
#include "func_types.h"
#include <vector>

// =======================================================================
// Forward declarations
// =======================================================================
class CDetour;

// =======================================================================
// This is the function prototype for a C++ callback.
// =======================================================================
typedef HookRetBuf_t* (*CPPCallBack)( CDetour* );

// =======================================================================
// Creates a C++ callback for a detour.
// =======================================================================
extern bool CPP_CreateCallback( void* target, eCallConv conv, const char* szParams, 
							   CPPCallBack callback, eHookType type );

// =======================================================================
// The CCPPManager class.
// =======================================================================
class CCPPManager : public ICallbackManager
{
	private:
		// ------------------------------------
		// Callback lists.
		// ------------------------------------
		std::vector<void *> m_vecPreCalls;
		std::vector<void *> m_vecPostCalls;

	public:
		// ------------------------------------
		// Language accessor.
		// ------------------------------------
		virtual const char* GetLang( void ) {
			return "C++";
		}

		// ------------------------------------
		// Callback adding / removal.
		// ------------------------------------
		virtual void Add( void* pFuncObj, eHookType type );
		virtual void Remove( void* pFuncObj, eHookType type );

		// ------------------------------------
		// Callback processing.
		// ------------------------------------
		virtual HookRetBuf_t* DoPreCalls( CDetour* pDet );
		virtual HookRetBuf_t* DoPostCalls( CDetour* pDet );
};

#endif // _CPP_MANAGER
