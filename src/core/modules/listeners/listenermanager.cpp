//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "listenermanager.h"
#include "utility/call_python.h"
#include "boost/python.hpp"
#include "modules/entities/entities_wrap.h"
#include <stdarg.h> // For va_list, va_end etc

//-----------------------------------------------------------------------------
// Adds a callable to the end of the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::register_listener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Is the callable already in the vector?
	if( !m_vecCallables.HasElement(oCallable) )
	{
		// Add the callable to the vector
		m_vecCallables.AddToTail(oCallable);
	}
}

//-----------------------------------------------------------------------------
// Removes all instances of a callable from the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::unregister_listener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the ServerCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);
}

//-----------------------------------------------------------------------------
// Calls all registered listeners.
//-----------------------------------------------------------------------------

void CListenerManager::call_listeners(int argc, ...)
{
	if (argc > 0)
	{
		boost::python::dict dReturn = boost::python::dict();

		va_list varargs;

		va_start(varargs, argc);
		for (int i = 0; i < argc; i++)
		{
		    Param p = va_arg(varargs, Param);
			switch (p.type)
			{
				case CListenerManager::INT:
					dReturn[p.name] = p.int_value;
					break;
				case CListenerManager::BOOL:
					dReturn[p.name] = p.bool_value;
					break;
				case CListenerManager::CONST_CHAR_PTR:
					dReturn[p.name] = p.const_char_ptr;
					break;
				case CListenerManager::CHAR_PTR:
					dReturn[p.name] = p.char_ptr;
					break;
				case CListenerManager::EDICT_T_PTR:
					dReturn[p.name] = CEdict(p.edict_t_ptr);
			}
		}
		va_end(varargs);

		for(int i = 0; i < m_vecCallables.Count(); i++)
		{
			BEGIN_BOOST_PY()

				// Get the PyObject instance of the callable
				PyObject* pCallable = m_vecCallables[i].ptr();

				// Call the callable
				CALL_PY_FUNC(pCallable, dReturn);

			END_BOOST_PY_NORET()
		}
	}
	else // For speed reasons (i.e. ticklistener) this sperate, so it doesn't create dictionary objects if no parameters are passed
	{
		for(int i = 0; i < m_vecCallables.Count(); i++)
		{
			BEGIN_BOOST_PY()

				// Get the PyObject instance of the callable
				PyObject* pCallable = m_vecCallables[i].ptr();

				// Call the callable
				CALL_PY_FUNC(pCallable);

			END_BOOST_PY_NORET()
		}
	}
}

/*
void CListenerManager::call_listeners()
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable
			CALL_PY_FUNC(pCallable);

		END_BOOST_PY_NORET()
	}
}

void CListenerManager::call_listeners( edict_t *pEntity )
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()

			// Get the PyObject instance of the callable
			PyObject* pCallable = m_vecCallables[i].ptr();

			// Call the callable
			CALL_PY_FUNC(pCallable, pEntity);

		END_BOOST_PY_NORET()
	}
}
*/