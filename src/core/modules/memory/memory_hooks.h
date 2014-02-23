#ifndef MEMORY_HOOKS_H
#define MEMORY_HOOKS_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include <list>
#include <map>

#include "boost/python.hpp"
using namespace boost::python;

#include "memory_tools.h"
#include "DynamicHooks.h"


//---------------------------------------------------------------------------------
// Classes
//---------------------------------------------------------------------------------
class CStackData
{
public:
	CStackData(CHook* pHook);

	object   GetItem(unsigned int iIndex);
	void     SetItem(unsigned int iIndex, object value);

	CPointer GetESP()
	{ return CPointer((unsigned long) m_pHook->m_pESP); }

protected:
	CHook*                m_pHook;
	std::map<int, object> m_mapCache;
};


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool SP_HookHandler(DynamicHooks::HookType_t eHookType, CHook* pHook);

#endif // MEMORY_HOOKS_H
