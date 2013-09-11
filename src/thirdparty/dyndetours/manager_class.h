// =======================================================================
// File: manager_class.h
// Purpose: This file contains the CDetourManager class which is
//	responsible for creating and removing detours.
// =======================================================================
#ifndef _MANAGER_CLASS_H
#define _MANAGER_CLASS_H

// =======================================================================
// Forward declarations
// =======================================================================
class CDetour;

// =======================================================================
// Includes
// =======================================================================
#include <map>
#include "func_types.h"

// =======================================================================
// Some typedefs to help us along.
// =======================================================================
typedef std::pair<unsigned int, CDetour*> TDetourPair;
typedef std::map<unsigned int, CDetour*> TDetourMap;

// =======================================================================
// CDetourManager class.
// =======================================================================
class CDetourManager
{
	private:
		TDetourMap m_DetourList;

	public:

		CDetour* CreateDetour(void* pTarget, void* pCallBack,
			eCallConv conv, char* szParams);

		CDetour* GetDetour(void* pAddress);
};

#endif // _MANAGER_CLASS_H
