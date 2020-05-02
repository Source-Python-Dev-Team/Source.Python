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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "core_cache.h"
#include "export_main.h"


//-----------------------------------------------------------------------------
// CCachedProperty class.
//-----------------------------------------------------------------------------
CCachedProperty::CCachedProperty(
	object fget=object(), object fset=object(), object fdel=object(), const char *doc=NULL,
	bool unbound=false, boost::python::tuple args=boost::python::tuple(), dict kwargs=dict())
{
	set_getter(fget);
	set_setter(fset);
	set_deleter(fdel);

	m_szDocString = doc;
	m_bUnbound = unbound;

	m_args = args;
	m_kwargs = kwargs;
}


object CCachedProperty::_callable_check(object function, const char *szName)
{
	if (!function.is_none() && !PyCallable_Check(function.ptr()))
		BOOST_RAISE_EXCEPTION(
			PyExc_TypeError,
			"The given %s function is not callable.", szName
		);

	return function;
}

object CCachedProperty::_prepare_value(object value)
{
	if (!PyGen_Check(value.ptr()))
		return value;

	if (getattr(value, "gi_frame").is_none())
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"The given generator is exhausted."
		);

	list values;
	while (true)
	{
		try
		{
			values.append(value.attr("__next__")());
		}
		catch(...)
		{
			if (!PyErr_ExceptionMatches(PyExc_StopIteration))
				throw_error_already_set();

			PyErr_Clear();
			break;
		}
	}

	return values;
}

void CCachedProperty::_invalidate_cache(PyObject *pRef)
{
	try
	{
		m_cache[handle<>(pRef)].del();
	}
	catch (...)
	{
		if (!PyErr_ExceptionMatches(PyExc_KeyError))
			throw_error_already_set();

		PyErr_Clear();
	}
}

void CCachedProperty::_update_cache(object instance, object value)
{
	if (m_bUnbound)
		m_cache[handle<>(
			PyWeakref_NewRef(
				instance.ptr(),
				make_function(
					boost::bind(&CCachedProperty::_invalidate_cache, this, _1),
					default_call_policies(),
					boost::mpl::vector2<void, PyObject *>()
				).ptr()
			)
		)] = value;
	else
	{
		dict cache = extract<dict>(instance.attr("__dict__"));
		cache[m_name] = value;
	}
}

void CCachedProperty::_delete_cache(object instance)
{
	try
	{
		if (m_bUnbound)
			m_cache[
				handle<>(
					PyWeakref_NewRef(instance.ptr(), NULL)
				)
			].del();
		else
		{
			dict cache = extract<dict>(instance.attr("__dict__"));
			cache[m_name].del();
		}
	}
	catch (...)
	{
		if (!PyErr_ExceptionMatches(PyExc_KeyError))
			throw_error_already_set();

		PyErr_Clear();
	}
}


object CCachedProperty::get_getter()
{
	return m_fget;
}

object CCachedProperty::set_getter(object fget)
{
	m_fget = _callable_check(fget, "getter");
	return fget;
}


object CCachedProperty::get_setter()
{
	return m_fset;
}

object CCachedProperty::set_setter(object fset)
{
	m_fset = _callable_check(fset, "setter");
	return fset;
}


object CCachedProperty::get_deleter()
{
	return m_fdel;
}

object CCachedProperty::set_deleter(object fdel)
{
	m_fdel = _callable_check(fdel, "deleter");
	return fdel;
}


str CCachedProperty::get_name()
{
	return m_name;
}

object CCachedProperty::get_owner()
{
	return m_owner();
}


void CCachedProperty::__set_name__(object owner, str name)
{
	m_name = name;
	m_owner = object(handle<>(PyWeakref_NewRef(owner.ptr(), NULL)));
}

object CCachedProperty::__get__(object self, object instance, object owner=object())
{
	CCachedProperty &pSelf = extract<CCachedProperty &>(self);
	if (instance.is_none())
		return self;

	object name = pSelf.get_name();
	if (name.is_none())
		BOOST_RAISE_EXCEPTION(
			PyExc_AttributeError,
			"Unable to retrieve the value of an unbound property."
		);

	object value;

	try
	{
		if (pSelf.m_bUnbound)
			return pSelf.m_cache[
				handle<>(
					PyWeakref_NewRef(instance.ptr(), NULL)
				)
			];
		else
		{
			dict cache = extract<dict>(instance.attr("__dict__"));
			return cache[name];
		}
	}
	catch (...)
	{
		if (!PyErr_ExceptionMatches(PyExc_KeyError))
			throw_error_already_set();

		PyErr_Clear();

		object getter = pSelf.get_getter();
		if (getter.is_none())
			BOOST_RAISE_EXCEPTION(
				PyExc_AttributeError,
				"Unable to retrieve the value of a property that have no getter function."
			);

		value = pSelf._prepare_value(
			getter(
				*(make_tuple(handle<>(borrowed(instance.ptr()))) + pSelf.m_args),
				**pSelf.m_kwargs
			)
		);

		pSelf._update_cache(instance, value);
	}

	return value;
}


void CCachedProperty::__set__(object instance, object value)
{
	if (m_fset.is_none())
		BOOST_RAISE_EXCEPTION(
			PyExc_AttributeError,
			"Unable to assign the value of a property that have no setter function."
		);

	object result = m_fset(
		*(make_tuple(handle<>(borrowed(instance.ptr())), value) + m_args),
		**m_kwargs
	);

	if (!result.is_none())
		_update_cache(instance, _prepare_value(result));
	else
		_delete_cache(instance);
}

void CCachedProperty::__delete__(object instance)
{
	if (!m_fdel.is_none())
		m_fdel(
			*(make_tuple(handle<>(borrowed(instance.ptr()))) + m_args),
			**m_kwargs
		);

	_delete_cache(instance);
}

object CCachedProperty::__call__(object self, object fget)
{
	CCachedProperty &pSelf = extract<CCachedProperty &>(self);
	pSelf.set_getter(fget);
	return self;
}

object CCachedProperty::__getitem__(str item)
{
	return m_kwargs[item];
}

void CCachedProperty::__setitem__(str item, object value)
{
	m_kwargs[item] = value;
}


CCachedProperty *CCachedProperty::wrap_descriptor(
	object descriptor, object owner, str name,
	bool unbound, boost::python::tuple args, dict kwargs)
{
	CCachedProperty *pProperty = new CCachedProperty(
		descriptor.attr("__get__"), descriptor.attr("__set__"), descriptor.attr("__delete__"),
		extract<const char *>(descriptor.attr("__doc__")), unbound, args, kwargs
	);

	pProperty->__set_name__(owner, name);

	return pProperty;
}