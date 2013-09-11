// =======================================================================
// File: detour_class.h
// Purpose: This is where the magic begins. An instance of the CDetour
//	class is instantiated for every NEW target function that the library
//	recieves. This means only one CDetour instance per target. This class
//	also stores a list of all callback functions to call before and after
//	the original function is executed.
// =======================================================================
#ifndef _DETOUR_CLASS_H
#define _DETOUR_CLASS_H

// =======================================================================
// Includes
// =======================================================================
#include <vector>
#include "hook_types.h"
#include "func_class.h"
#include "asmbridge_class.h"
#include "trampoline_class.h"
#include "callback_manager.h"

// =======================================================================
// The CDetour class.
// =======================================================================
class CDetour
{
	private:
		// ------------------------------------
		// This points to an array of saved
		// bytes from the original function.
		// ------------------------------------
		CTrampoline* m_pTrampoline;

		// ------------------------------------
		// This contains the ASM bridge which
		// connects the original function to
		// our detour.
		// ------------------------------------
		CASMBridge* m_pBridge;

		// ------------------------------------
		// The function object representing
		// the target function.
		// ------------------------------------
		CFuncObj* m_pFuncObj;

		// ------------------------------------
		// The status of this detour.
		// ------------------------------------
		bool m_bInitialized;

		// ------------------------------------
		// TEMPORARY! Stores pointers to all of
		// our pre-execution callbacks.
		// ------------------------------------
		std::vector<ICallbackManager *> m_vecPreCallbacks;
		
		// ------------------------------------
		// TEMPORARY! Stores pointers to all of
		// our post-execution callbacks.
		// ------------------------------------
		std::vector<ICallbackManager *> m_vecPostCallbacks;

		// ------------------------------------
		// @brief Parses and creates a function
		//	object from the parameter list.
		// ------------------------------------
		CFuncObj* CreateFromString( const char* szParamList, 
			eCallConv eConv, void* pTarget );

	public:

		// ------------------------------------
		// Constructor
		// ------------------------------------
		CDetour( void* pTarget, const char* szParamList, 
			eCallConv eConv );

		// ------------------------------------
		// Destructor
		// ------------------------------------
		~CDetour( void );

		// ------------------------------------
		// Adds a callback manager.
		// ------------------------------------
		void AddManager( ICallbackManager* pManager, eHookType type );

		// ------------------------------------
		// Callback manager accessor.
		// ------------------------------------
		ICallbackManager* GetManager( const char* lang, eHookType type );

		// ------------------------------------
		// Processes callbacks.
		// ------------------------------------
		HookRetBuf_t* DoCallbacks( eHookType type );

		// ------------------------------------
		// Returns the ASM bridge for this
		// detour.
		// ------------------------------------
		CASMBridge* GetAsmBridge( void ) { return m_pBridge; }

		// ------------------------------------
		// Trampoline accessor.
		// ------------------------------------
		void* GetTrampoline( void ) { return m_pTrampoline->GetBase(); }

		// ------------------------------------
		// Function object accessor.
		// ------------------------------------
		CFuncObj* GetFuncObj( void ) { return m_pFuncObj; }
};

#endif // _DETOUR_CLASS_H

