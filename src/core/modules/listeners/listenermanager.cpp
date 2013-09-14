//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "listenermanager.h"
#include "utility/call_python.h"

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
// FSS No C++11 suport? In neither, VS2010 and VS2012?
// template<typename... Types> void CListenerManager::call_listeners(typename... args)
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
