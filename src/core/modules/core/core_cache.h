/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2019 Source Python Development Team.  All rights reserved.
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

#ifndef _CORE_CACHE_H
#define _CORE_CACHE_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "boost/python.hpp"
using namespace boost::python;


//-----------------------------------------------------------------------------
// CCachedProperty class.
//-----------------------------------------------------------------------------
class CCachedProperty
{
public:
	CCachedProperty(
		object fget, object fset, object fdel, object doc, bool unbound,
		boost::python::tuple args, object kwargs
	);

	static object _callable_check(object function, const char *szName);
	static object _prepare_value(object value);
	void _invalidate_cache(PyObject *pRef);

	object get_getter();
	object set_getter(object fget);

	object get_setter();
	object set_setter(object fget);

	object get_deleter();
	object set_deleter(object fget);

	str get_name();
	object get_owner();

	object get_cached_value(object instance);
	void set_cached_value(object instance, object value);
	void delete_cached_value(object instance);

	static object bind(object self, object owner, str name);

	void __set_name__(object owner, str name);
	static object __get__(object self, object instance, object owner);
	void __set__(object instance, object value);
	void __delete__(object instance);
	static object __call__(object self, object fget);
	object __getitem__(str item);
	void __setitem__(str item, object value);

	static CCachedProperty *wrap_descriptor(
		object descriptor, object owner=object(), str name=str(),
		bool unbound=false, boost::python::tuple args=boost::python::tuple(), object kwargs=object()
	);

private:
	object m_fget;
	object m_fset;
	object m_fdel;

	str m_name;
	object m_owner;

	bool m_bUnbound;
	dict m_cache;

public:
	object m_doc;

	boost::python::tuple m_args;
	dict m_kwargs;
};


#endif // _CORE_CACHE_H
