// =======================================================================
// File: detour_class.h
// Purpose: Handles detour creation.
// =======================================================================
#ifndef _DETOURMAN_CLASS_H
#define _DETOURMAN_CLASS_H

//========================================================================
// Includes
//========================================================================
#include <vector>
#include "hook_types.h"
#include "func_types.h"

//========================================================================
// Forward declarations.
//========================================================================
class CDetour;

//========================================================================
// Detour Manager class
//========================================================================
class CDetourManager
{
	private:
		/* This vector will store a list of detours and their addresses. */
		std::vector<CDetour *> m_DetourList;

	public:

	   /* @brief Destructor. Removes all detours and frees up memory
		*	taken up by them.
		*/
		~CDetourManager( void );

	   /* @brief Adds a detour to our internal list of detours.
		* @param pTarget  - Pointer to a callback function.
		* @param szParams - Parameter format string.
		* @param conv     - Calling convention of hooked function.
		* @return A detour instance for the target address.
		*/
		CDetour* Add_Detour( void* pTarget, const char* szParams, eCallConv conv );

	   /* @brief Removes a detour at the target address.
		* @param pTarget - The target address the detour is bound to.
		* @return True if the detour was found and removed successfully.
		*/
		bool     Remove_Detour( void* pTarget );

	   /* @brief Finds a detour in our list.
		* @param pTarget - The address that the detour has hooked.
		* @return A CDetour pointer that is tied to the target function.
		*/
		CDetour* Find_Detour( void* pTarget );
};

extern CDetourManager g_DetourManager;

#endif // _DETOURMAN_CLASS_H

