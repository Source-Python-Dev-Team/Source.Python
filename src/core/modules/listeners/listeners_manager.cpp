//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "listeners_manager.h"


//-----------------------------------------------------------------------------
// Adds a callable to the end of the CListenerManager vector.
//-----------------------------------------------------------------------------
void CListenerManager::RegisterListener(PyObject* pCallable)
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
void CListenerManager::UnregisterListener(PyObject* pCallable)
{
	// Get the object instance of the callable
	object oCallable = object(handle<>(borrowed(pCallable)));

	// Remove the callback from the ServerCommandManager instance
	m_vecCallables.FindAndRemove(oCallable);
}

void CListenerManager::Notify(tuple args, dict kwargs)
{
	for(int i = 0; i < m_vecCallables.Count(); i++)
	{
		BEGIN_BOOST_PY()
			eval("lambda func, args, kwargs: func(*args, **kwargs)")(m_vecCallables[i], args, kwargs);
		END_BOOST_PY_NORET()
	}
}
