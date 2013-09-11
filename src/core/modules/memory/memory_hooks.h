#ifndef MEMORY_HOOKS_H
#define MEMORY_HOOKS_H

#include <list>

#include "callback_manager.h"
#include "func_class.h"
#include "func_stack.h"
#include "register_class.h"

#include "boost/python.hpp"

#include "memory_tools.h"

#include <map>

using namespace boost::python;


class CCallbackManager: public ICallbackManager
{
private:
	std::list<PyObject *> m_PreCalls;
	std::list<PyObject *> m_PostCalls;

public:
	virtual void Add(void* pFunc, eHookType type);
	virtual void Remove(void* pFunc, eHookType type);

	virtual HookRetBuf_t* DoPreCalls(CDetour* pDetour);
	virtual HookRetBuf_t* DoPostCalls(CDetour* pDetour);

	virtual const char* GetLang() { return "Python"; }
};

class CStackData
{
public:
	CStackData(CDetour* pDetour);

	CPointer* get_esp() { return new CPointer(m_pRegisters->r_esp); }
	CPointer* get_ecx() { return new CPointer(m_pRegisters->r_ecx); }
	CPointer* get_ebp() { return new CPointer(m_pRegisters->r_ebp); }
	CPointer* get_edx() { return new CPointer(m_pRegisters->r_edx); }

	object get_item(unsigned int iIndex);
	void   set_item(unsigned int iIndex, object value);

	unsigned int get_arg_num();

private:
	CRegisterObj*         m_pRegisters;
	CFuncObj*             m_pFunction;
	CFuncStack*           m_pStack;
	map<int, object>      m_mapCache;
};

#endif // MEMORY_HOOKS_H
