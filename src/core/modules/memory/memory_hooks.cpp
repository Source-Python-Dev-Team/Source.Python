// ============================================================================
// >> INCLUDES
// ============================================================================
#include "memory_hooks.h"
#include "memory_tools.h"
#include "utilities/wrap_macros.h"
#include "utilities/call_python.h"
#include "utilities/sp_util.h"

#include "boost/python.hpp"
using namespace boost::python;


// ============================================================================
// >> GLOBAL VARIABLES
// ============================================================================
// g_mapCallbacks[<CHook *>][<HookType_t>] -> [<PyObject *>, <PyObject *>, ...]
std::map<CHook *, std::map<DynamicHooks::HookType_t, std::list<PyObject *> > > g_mapCallbacks;


// ============================================================================
// >> HELPER FUNCTIONS
// ============================================================================
template<class T>
void SetReturnValue(CHook* pHook, object value)
{
	T val = extract<T>(value);
	pHook->SetReturnValue<T>(val);
}

template<class T>
object GetReturnValue(CHook* pHook)
{
	return object(pHook->GetReturnValue<T>());
}

template<class T>
void SetArgument(CHook* pHook, int iIndex, object value)
{
	T val = extract<T>(value);
	pHook->SetArgument<T>(iIndex, val);
}

template<class T>
object GetArgument(CHook* pHook, int iIndex)
{
	return object(pHook->GetArgument<T>(iIndex));
}


// ============================================================================
// >> SP_HookHandler
// ============================================================================
bool SP_HookHandler(DynamicHooks::HookType_t eHookType, CHook* pHook)
{
	std::list<PyObject *> callbacks = g_mapCallbacks[pHook][eHookType];

	// No need to do all this stuff, if there is no callback registered
	if (callbacks.empty())
		return false;

	object retval;
	if (eHookType == HOOKTYPE_POST)
	{
		switch(pHook->m_pRetParam->m_cParam)
		{
			case DATA_TYPE_VOID:		retval = object(); break;
			case DATA_TYPE_BOOL:		retval = GetReturnValue<bool>(pHook); break;
			case DATA_TYPE_CHAR:		retval = GetReturnValue<char>(pHook); break;
			case DATA_TYPE_UCHAR:		retval = GetReturnValue<unsigned char>(pHook); break;
			case DATA_TYPE_SHORT:		retval = GetReturnValue<short>(pHook); break;
			case DATA_TYPE_USHORT:	retval = GetReturnValue<unsigned short>(pHook); break;
			case DATA_TYPE_INT:		retval = GetReturnValue<int>(pHook); break;
			case DATA_TYPE_UINT:		retval = GetReturnValue<unsigned int>(pHook); break;
			case DATA_TYPE_LONG:		retval = GetReturnValue<long>(pHook); break;
			case DATA_TYPE_ULONG:		retval = GetReturnValue<unsigned long>(pHook); break;
			case DATA_TYPE_LONGLONG:	retval = GetReturnValue<long long>(pHook); break;
			case DATA_TYPE_ULONGLONG:	retval = GetReturnValue<unsigned long long>(pHook); break;
			case DATA_TYPE_FLOAT:		retval = GetReturnValue<float>(pHook); break;
			case DATA_TYPE_DOUBLE:	retval = GetReturnValue<double>(pHook); break;
			case DATA_TYPE_POINTER:	retval = object(ptr(new CPointer(pHook->GetReturnValue<unsigned long>()))); break;
			case DATA_TYPE_STRING:	retval = GetReturnValue<const char *>(pHook); break;
			default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.");
		}
	}
	
	CStackData stackdata = CStackData(pHook);
	bool bOverride = false;
	for (std::list<PyObject *>::iterator it=callbacks.begin(); it != callbacks.end(); it++)
	{
		BEGIN_BOOST_PY()
			object pyretval;
			if (eHookType == HOOKTYPE_PRE)
				pyretval = CALL_PY_FUNC(*it, stackdata);
			else
				pyretval = CALL_PY_FUNC(*it, stackdata, retval);

			if (!pyretval.is_none())
			{
				bOverride = true;
				switch(pHook->m_pRetParam->m_cParam)
				{
					case DATA_TYPE_VOID:		break;
					case DATA_TYPE_BOOL:		SetReturnValue<bool>(pHook, pyretval); break;
					case DATA_TYPE_CHAR:		SetReturnValue<char>(pHook, pyretval); break;
					case DATA_TYPE_UCHAR:		SetReturnValue<unsigned >(pHook, pyretval); break;
					case DATA_TYPE_SHORT:		SetReturnValue<short>(pHook, pyretval); break;
					case DATA_TYPE_USHORT:	SetReturnValue<unsigned short>(pHook, pyretval); break;
					case DATA_TYPE_INT:		SetReturnValue<int>(pHook, pyretval); break;
					case DATA_TYPE_UINT:		SetReturnValue<unsigned int>(pHook, pyretval); break;
					case DATA_TYPE_LONG:		SetReturnValue<long>(pHook, pyretval); break;
					case DATA_TYPE_ULONG:		SetReturnValue<unsigned long>(pHook, pyretval); break;
					case DATA_TYPE_LONGLONG:	SetReturnValue<long long>(pHook, pyretval); break;
					case DATA_TYPE_ULONGLONG:	SetReturnValue<unsigned long long>(pHook, pyretval); break;
					case DATA_TYPE_FLOAT:		SetReturnValue<float>(pHook, pyretval); break;
					case DATA_TYPE_DOUBLE:	SetReturnValue<double>(pHook, pyretval); break;
					case DATA_TYPE_POINTER:
					{
						CPointer* pPtr = extract<CPointer *>(pyretval);
						pHook->SetReturnValue<unsigned long>(pPtr->m_ulAddr);
					} break;
					case DATA_TYPE_STRING:		SetReturnValue<const char*>(pHook, pyretval); break;
					default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.")
				}
			}
		END_BOOST_PY_NORET()
	}
	return bOverride;
}


// ============================================================================
// >> CStackData
// ============================================================================
CStackData::CStackData(CHook* pHook)
{
	m_pHook = pHook;
}

object CStackData::GetItem(unsigned int iIndex)
{
	if (iIndex >= (unsigned int) m_pHook->GetArgumentCount())
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	// Argument already cached?
	object retval = m_mapCache[iIndex];
	if (retval)
		return retval;

	switch(m_pHook->GetArgument(iIndex)->m_cParam)
	{
		case DATA_TYPE_BOOL:      retval = GetArgument<bool>(m_pHook, iIndex); break;
		case DATA_TYPE_CHAR:      retval = GetArgument<char>(m_pHook, iIndex); break;
		case DATA_TYPE_UCHAR:     retval = GetArgument<unsigned char>(m_pHook, iIndex); break;
		case DATA_TYPE_SHORT:     retval = GetArgument<short>(m_pHook, iIndex); break;
		case DATA_TYPE_USHORT:    retval = GetArgument<unsigned short>(m_pHook, iIndex); break;
		case DATA_TYPE_INT:       retval = GetArgument<int>(m_pHook, iIndex); break;
		case DATA_TYPE_UINT:      retval = GetArgument<unsigned int>(m_pHook, iIndex); break;
		case DATA_TYPE_LONG:      retval = GetArgument<long>(m_pHook, iIndex); break;
		case DATA_TYPE_ULONG:     retval = GetArgument<unsigned long>(m_pHook, iIndex); break;
		case DATA_TYPE_LONGLONG:  retval = GetArgument<long long>(m_pHook, iIndex); break;
		case DATA_TYPE_ULONGLONG: retval = GetArgument<unsigned long long>(m_pHook, iIndex); break;
		case DATA_TYPE_FLOAT:     retval = GetArgument<float>(m_pHook, iIndex); break;
		case DATA_TYPE_DOUBLE:    retval = GetArgument<double>(m_pHook, iIndex); break;
		case DATA_TYPE_POINTER:   retval = object(ptr(new CPointer(m_pHook->GetArgument<unsigned long>(iIndex)))); break;
		case DATA_TYPE_STRING:    retval = GetArgument<const char *>(m_pHook, iIndex); break;
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.") break;
	}
	m_mapCache[iIndex] = retval;
	return retval;
}

void CStackData::SetItem(unsigned int iIndex, object value)
{
	if (iIndex >= (unsigned int) m_pHook->GetArgumentCount())
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	// Update cache
	m_mapCache[iIndex] = value;
	switch(m_pHook->GetArgument(iIndex)->m_cParam)
	{
		case DATA_TYPE_BOOL:      SetArgument<bool>(m_pHook, iIndex, value); break;
		case DATA_TYPE_CHAR:      SetArgument<char>(m_pHook, iIndex, value); break;
		case DATA_TYPE_UCHAR:     SetArgument<unsigned char>(m_pHook, iIndex, value); break;
		case DATA_TYPE_SHORT:     SetArgument<short>(m_pHook, iIndex, value); break;
		case DATA_TYPE_USHORT:    SetArgument<unsigned short>(m_pHook, iIndex, value); break;
		case DATA_TYPE_INT:       SetArgument<int>(m_pHook, iIndex, value); break;
		case DATA_TYPE_UINT:      SetArgument<unsigned int>(m_pHook, iIndex, value); break;
		case DATA_TYPE_LONG:      SetArgument<long>(m_pHook, iIndex, value); break;
		case DATA_TYPE_ULONG:     SetArgument<unsigned long>(m_pHook, iIndex, value); break;
		case DATA_TYPE_LONGLONG:  SetArgument<long long>(m_pHook, iIndex, value); break;
		case DATA_TYPE_ULONGLONG: SetArgument<unsigned long long>(m_pHook, iIndex, value); break;
		case DATA_TYPE_FLOAT:     SetArgument<float>(m_pHook, iIndex, value); break;
		case DATA_TYPE_DOUBLE:    SetArgument<double>(m_pHook, iIndex, value); break;
		case DATA_TYPE_POINTER:
		{
			CPointer* pPtr = extract<CPointer*>(value);
			SetArgument<unsigned long>(m_pHook, iIndex, object(pPtr->m_ulAddr));
		} break;
		case DATA_TYPE_STRING:    SetArgument<const char *>(m_pHook, iIndex, value); break;
		default: BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unknown type.")
	}
}
