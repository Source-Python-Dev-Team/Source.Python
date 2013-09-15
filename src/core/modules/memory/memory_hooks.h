#ifndef MEMORY_HOOKS_H
#define MEMORY_HOOKS_H

#include <list>
#include <map>

#include "boost/python.hpp"
using namespace boost::python;

#include "memory_tools.h"
#include "DynamicHooks.h"

class CStackData
{
public:
	CStackData(CHook* pHook);

	object get_item(unsigned int iIndex);
	void   set_item(unsigned int iIndex, object value);

	CPointer* get_esp();

private:
	CHook*                m_pHook;
	std::map<int, object> m_mapCache;
};


bool SP_HookHandler(DynamicHooks::HookType_t eHookType, CHook* pHook);

#endif // MEMORY_HOOKS_H
