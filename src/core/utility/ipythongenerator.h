/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/
#ifndef _PYTHON_GENERATOR_H
#define _PYTHON_GENERATOR_H

#include <boost/python/has_back_reference.hpp>
#include <boost/python/handle.hpp>

using namespace boost::python;

//---------------------------------------------------------------------------------
// An abstract class for exposing Python generators.
// iter() should bind to the magic method __iter__().
// next() should bind to the magic method __next__().
//
// Implementation Details:
// Implement getNext() to return the next object in the sequence.
// Returning NULL in getNext() will mean the generator will raise a StopIteration.
//---------------------------------------------------------------------------------
template<class T>
class IPythonGenerator
{
public:
	IPythonGenerator(PyObject* self);
	virtual ~IPythonGenerator() = 0;
	handle<> iter();
	T* next();
protected:
	virtual T* getNext() = 0;
	IPythonGenerator(const IPythonGenerator& rhs);
private:
	PyObject* m_pSelf;
};

//---------------------------------------------------------------------------------
// Constructor.
//---------------------------------------------------------------------------------
template<class T>
IPythonGenerator<T>::IPythonGenerator(PyObject* self):
	m_pSelf(self)
{
}

//---------------------------------------------------------------------------------
// [protected] Copy-Constructor.
//---------------------------------------------------------------------------------
template<class T>
IPythonGenerator<T>::IPythonGenerator(const IPythonGenerator<T>& rhs):
	m_pSelf(rhs.m_pSelf)
{
}

//---------------------------------------------------------------------------------
// [virtual] Destructor
//---------------------------------------------------------------------------------
template<class T>
IPythonGenerator<T>::~IPythonGenerator()
{
}

//---------------------------------------------------------------------------------
// Maps to __iter__(). Just returns itself.
//---------------------------------------------------------------------------------
template<class T>
handle<> IPythonGenerator<T>::iter()
{
	return handle<>(borrowed(m_pSelf));
}

//---------------------------------------------------------------------------------
// Maps to __next__(). Calls getNext() and returns the next object, or raises
// StopIteration if getNext() returned NULL.
//---------------------------------------------------------------------------------
template<class T>
T* IPythonGenerator<T>::next()
{
	T* pValue = getNext();
	if (!pValue)
	{
		BOOST_RAISE_EXCEPTION(PyExc_StopIteration, "Iteration stops here.");
		return NULL;
	}

	return pValue;
}

//---------------------------------------------------------------------------------
// Use this macro with classes inheriting from IPythonGenerator to automatically
// bind the magic methods for __iter__() and __next__().
// You should still use BOOST_CLASS_END() to end the class definition.
//---------------------------------------------------------------------------------
#define BOOST_GENERATOR_CLASS( classname ) \
	BOOST_CLASS(classname) \
		CLASS_METHOD_SPECIAL(classname, \
			"__iter__", \
			iter, \
			"Returns the iterable object." \
		) \
		CLASS_METHOD_SPECIAL(classname, \
			"__next__", \
			next, \
			"Returns the next valid instance.", \
			reference_existing_object_policy() \
		)

#endif // _PYTHON_GENERATOR_H
