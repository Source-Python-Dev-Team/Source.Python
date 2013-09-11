// ============================================================================
// >> INCLUDES
// ============================================================================
#include "conv_interface.h"
#include "detour_class.h"
#include "func_class.h"
#include "func_stack.h"
#include "func_types.h"
#include "register_class.h"

#include "memory_hooks.h"
#include "memory_tools.h"
#include "utility/wrap_macros.h"
#include "utility/call_python.h"
#include "utility/sp_util.h"

#include "boost/python.hpp"
using namespace boost::python;


// ============================================================================
// >> Helper functions to read, set and convert addresses
// ============================================================================
template<class T>
object ReadAddr(void* pAddr)
{
	return object(*(T *) pAddr);
}

template<class T>
void SetAddr(void* pAddr, object value)
{
	*(T *) pAddr = extract<T>(value);
}

// ============================================================================
// >> CCallbackManager
// ============================================================================
void CCallbackManager::Add(void* pFunc, eHookType type)
{
	if (!pFunc)
		return;

	PyObject* callable = (PyObject *) pFunc;
	switch (type)
	{
		case TYPE_PRE:  m_PreCalls.push_front(callable); break;
		case TYPE_POST: m_PostCalls.push_front(callable); break;
	}
}

void CCallbackManager::Remove(void* pFunc, eHookType type)
{
	if (!pFunc)
		return;

	PyObject* callable = (PyObject *) pFunc;
	switch (type)
	{
		case TYPE_PRE:  m_PreCalls.remove(callable); break;
		case TYPE_POST: m_PostCalls.remove(callable); break;
	}
}

HookRetBuf_t* CCallbackManager::DoPreCalls(CDetour* pDetour)
{
	if (!pDetour)
		return NULL;

	HookRetBuf_t* buffer = new HookRetBuf_t;
	buffer->eRes = HOOKRES_NONE;
	buffer->pRetBuf = NULL;

	CStackData stackdata = CStackData(pDetour);
	void* pRetReg = pDetour->GetAsmBridge()->GetConv()->GetRegisters()->r_retreg;
	for (std::list<PyObject *>::iterator iter=m_PreCalls.begin(); iter != m_PreCalls.end(); iter++)
	{
		BEGIN_BOOST_PY()

		object retval = CALL_PY_FUNC(*iter, stackdata);
		if (!retval.is_none())
		{
			buffer->eRes = HOOKRES_OVERRIDE;
			switch(pDetour->GetFuncObj()->GetRetType()->GetType())
			{
				case TYPE_BOOL:			SetAddr<bool>(pRetReg, retval); break;
				case TYPE_CHAR:			SetAddr<char>(pRetReg, retval); break;
				case TYPE_UCHAR:		SetAddr<unsigned >(pRetReg, retval); break;
				case TYPE_SHORT:		SetAddr<short>(pRetReg, retval); break;
				case TYPE_USHORT:		SetAddr<unsigned short>(pRetReg, retval); break;
				case TYPE_INT:			SetAddr<int>(pRetReg, retval); break;
				case TYPE_UINT:			SetAddr<unsigned int>(pRetReg, retval); break;
				case TYPE_LONG:			SetAddr<long>(pRetReg, retval); break;
				case TYPE_ULONG:		SetAddr<unsigned long>(pRetReg, retval); break;
				case TYPE_LONGLONG:		SetAddr<long long>(pRetReg, retval); break;
				case TYPE_ULONGLONG:	SetAddr<unsigned long long>(pRetReg, retval); break;
				case TYPE_FLOAT:		SetAddr<float>(pRetReg, retval); break;
				case TYPE_DOUBLE:		SetAddr<double>(pRetReg, retval); break;
				case TYPE_POINTER:
				{
					unsigned long retptr = ExtractPyPtr(retval);
					memcpy(pRetReg, &retptr, sizeof(unsigned long));
				} break;
				case TYPE_STRING:		SetAddr<const char*>(pRetReg, retval); break;
				default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.")
			}
		}

		END_BOOST_PY_NORET()
	}
	return buffer;
}

HookRetBuf_t* CCallbackManager::DoPostCalls(CDetour* pDetour)
{
	if (!pDetour)
		return NULL;

	HookRetBuf_t* buffer = new HookRetBuf_t;
	buffer->eRes = HOOKRES_NONE;
	buffer->pRetBuf = NULL;

	CStackData stackdata = CStackData(pDetour);

	void* pRetReg = pDetour->GetAsmBridge()->GetConv()->GetRegisters()->r_retreg;
	object retval;
	switch(pDetour->GetFuncObj()->GetRetType()->GetType())
	{
		case TYPE_VOID:			retval = object(); break;
		case TYPE_BOOL:			retval = ReadAddr<bool>(pRetReg); break;
		case TYPE_CHAR:			retval = ReadAddr<char>(pRetReg); break;
		case TYPE_UCHAR:		retval = ReadAddr<unsigned char>(pRetReg); break;
		case TYPE_SHORT:		retval = ReadAddr<short>(pRetReg); break;
		case TYPE_USHORT:		retval = ReadAddr<unsigned short>(pRetReg); break;
		case TYPE_INT:			retval = ReadAddr<int>(pRetReg); break;
		case TYPE_UINT:			retval = ReadAddr<unsigned int>(pRetReg); break;
		case TYPE_LONG:			retval = ReadAddr<long>(pRetReg); break;
		case TYPE_ULONG:		retval = ReadAddr<unsigned long>(pRetReg); break;
		case TYPE_LONGLONG:		retval = ReadAddr<long long>(pRetReg); break;
		case TYPE_ULONGLONG:	retval = ReadAddr<unsigned long long>(pRetReg); break;
		case TYPE_FLOAT:		retval = ReadAddr<float>(pRetReg); break;
		case TYPE_DOUBLE:		retval = ReadAddr<double>(pRetReg); break;
		case TYPE_POINTER:		retval = object(CPointer(*(unsigned long *) pRetReg)); break;
		case TYPE_STRING:		retval = ReadAddr<const char *>(pRetReg); break;
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.");
	}
	
	for (std::list<PyObject *>::iterator iter=m_PostCalls.begin(); iter != m_PostCalls.end(); iter++)
	{
		BEGIN_BOOST_PY()

		object pyretval = CALL_PY_FUNC(*iter, stackdata, retval);
		if (!pyretval.is_none())
		{
			buffer->eRes = HOOKRES_OVERRIDE;
			switch(pDetour->GetFuncObj()->GetRetType()->GetType())
			{
				case TYPE_BOOL:			SetAddr<bool>(pRetReg, pyretval); break;
				case TYPE_CHAR:			SetAddr<char>(pRetReg, pyretval); break;
				case TYPE_UCHAR:		SetAddr<unsigned >(pRetReg, pyretval); break;
				case TYPE_SHORT:		SetAddr<short>(pRetReg, pyretval); break;
				case TYPE_USHORT:		SetAddr<unsigned short>(pRetReg, pyretval); break;
				case TYPE_INT:			SetAddr<int>(pRetReg, pyretval); break;
				case TYPE_UINT:			SetAddr<unsigned int>(pRetReg, pyretval); break;
				case TYPE_LONG:			SetAddr<long>(pRetReg, pyretval); break;
				case TYPE_ULONG:		SetAddr<unsigned long>(pRetReg, pyretval); break;
				case TYPE_LONGLONG:		SetAddr<long long>(pRetReg, pyretval); break;
				case TYPE_ULONGLONG:	SetAddr<unsigned long long>(pRetReg, pyretval); break;
				case TYPE_FLOAT:		SetAddr<float>(pRetReg, pyretval); break;
				case TYPE_DOUBLE:		SetAddr<double>(pRetReg, pyretval); break;
				case TYPE_POINTER:
				{
					unsigned long retptr = ExtractPyPtr(pyretval);
					memcpy(pRetReg, &retptr, sizeof(unsigned long));
				} break;
				case TYPE_STRING:		SetAddr<const char*>(pRetReg, pyretval); break;
				default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.")
			}
		}

		END_BOOST_PY_NORET()
	}
	return buffer;
}


// ============================================================================
// >> CStackData
// ============================================================================
CStackData::CStackData(CDetour* pDetour)
{
	m_pRegisters = pDetour->GetAsmBridge()->GetConv()->GetRegisters();
	m_pFunction  = pDetour->GetFuncObj();
	m_pStack     = m_pFunction->GetStack();
}

unsigned int CStackData::get_arg_num()
{
	int argnum = m_pFunction->GetNumArgs();
	if (m_pFunction->GetConvention() == CONV_THISCALL)
		argnum++;

	return argnum;
}

object CStackData::get_item(unsigned int iIndex)
{
	if (iIndex >= get_arg_num())
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	// Argument already cached?
	object retval = m_mapCache[iIndex];
	if (retval)
		return retval;

	if (m_pFunction->GetConvention() == CONV_THISCALL)
	{
		if (iIndex == 0)
		{
		#ifdef __linux__
			unsigned long thisptr = *(unsigned long *) (m_pRegisters->r_esp + 4);
		#else
			unsigned long thisptr = m_pRegisters->r_ecx;
		#endif
			retval = m_mapCache[0] = object(CPointer(thisptr));
			return retval;
		}
	}

	ArgNode_t* pArgNode = m_pStack->GetArgument(m_pFunction->GetConvention() == CONV_THISCALL ? iIndex-1 : iIndex);
	int        offset   = pArgNode->m_nOffset;

	#ifdef __linux__
		if (m_pFunction->GetConvention() == CONV_THISCALL)
			// Add size of "this" pointer
			offset += 4;
	#endif

	void* pAddr = (void *) (m_pRegisters->r_esp + 4 + offset);
	switch(pArgNode->m_pArg->GetType())
	{
		case TYPE_BOOL:      retval = ReadAddr<bool>(pAddr); break;
		case TYPE_CHAR:      retval = ReadAddr<char>(pAddr); break;
		case TYPE_UCHAR:     retval = ReadAddr<unsigned char>(pAddr); break;
		case TYPE_SHORT:     retval = ReadAddr<short>(pAddr); break;
		case TYPE_USHORT:    retval = ReadAddr<unsigned short>(pAddr); break;
		case TYPE_INT:       retval = ReadAddr<int>(pAddr); break;
		case TYPE_UINT:      retval = ReadAddr<unsigned int>(pAddr); break;
		case TYPE_LONG:      retval = ReadAddr<long>(pAddr); break;
		case TYPE_ULONG:     retval = ReadAddr<unsigned long>(pAddr); break;
		case TYPE_LONGLONG:  retval = ReadAddr<long long>(pAddr); break;
		case TYPE_ULONGLONG: retval = ReadAddr<unsigned long long>(pAddr); break;
		case TYPE_FLOAT:     retval = ReadAddr<float>(pAddr); break;
		case TYPE_DOUBLE:    retval = ReadAddr<double>(pAddr); break;
		case TYPE_POINTER:   retval = object(CPointer(*(unsigned long *) pAddr)); break;
		case TYPE_STRING:    retval = ReadAddr<const char *>(pAddr); break;
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.") break;
	}

	m_mapCache[iIndex] = retval;
	return retval;
}

void CStackData::set_item(unsigned int iIndex, object value)
{
	if (iIndex >= get_arg_num())
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	// Update cache
	m_mapCache[iIndex] = value;

	// Update address
	if (m_pFunction->GetConvention() == CONV_THISCALL)
	{
		if (iIndex == 0)
		{
		#ifdef __linux__
			SetAddr<unsigned long>((void *) (m_pRegisters->r_esp + 4), value);
		#else
			m_pRegisters->r_ecx = extract<unsigned long>(value);
		#endif
			return;
		}
	}

	ArgNode_t* pArgNode = m_pStack->GetArgument(m_pFunction->GetConvention() == CONV_THISCALL ? iIndex-1 : iIndex);
	int        offset   = pArgNode->m_nOffset;

	#ifdef __linux__
		if (m_pFunction->GetConvention() == CONV_THISCALL)
			// Add size of "this" pointer
			offset += 4;
	#endif

	void* ulAddr = (void*)(m_pRegisters->r_esp + 4 + offset);
	switch(pArgNode->m_pArg->GetType())
	{
		case TYPE_BOOL:      SetAddr<bool>(ulAddr, value); break;
		case TYPE_CHAR:      SetAddr<char>(ulAddr, value); break;
		case TYPE_UCHAR:     SetAddr<unsigned char>(ulAddr, value); break;
		case TYPE_SHORT:     SetAddr<short>(ulAddr, value); break;
		case TYPE_USHORT:    SetAddr<unsigned short>(ulAddr, value); break;
		case TYPE_INT:       SetAddr<int>(ulAddr, value); break;
		case TYPE_UINT:      SetAddr<unsigned int>(ulAddr, value); break;
		case TYPE_LONG:      SetAddr<long>(ulAddr, value); break;
		case TYPE_ULONG:     SetAddr<unsigned long>(ulAddr, value); break;
		case TYPE_LONGLONG:  SetAddr<long long>(ulAddr, value); break;
		case TYPE_ULONGLONG: SetAddr<unsigned long long>(ulAddr, value); break;
		case TYPE_FLOAT:     SetAddr<float>(ulAddr, value); break;
		case TYPE_DOUBLE:    SetAddr<double>(ulAddr, value); break;
		case TYPE_POINTER:   *(unsigned long *) ulAddr = ExtractPyPtr(value); break;
		case TYPE_STRING:    SetAddr<const char *>(ulAddr, value); break;
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.")
	}
}