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
	object fget=object(), object fset=object(), object fdel=object(), object doc=object(),
	boost::python::tuple args=boost::python::tuple(), object kwargs=object())
{
	m_doc = doc;

	set_getter(fget);
	set_setter(fset);
	set_deleter(fdel);

	m_args = args;

	if (!kwargs.is_none())
		m_kwargs = extract<dict>(kwargs);
	else
		m_kwargs = dict();
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


object CCachedProperty::get_getter()
{
	return m_fget;
}

object CCachedProperty::set_getter(object fget)
{
	m_fget = _callable_check(fget, "getter");

	if (m_doc.is_none()) {
		m_doc = m_fget.attr("__doc__");
	}

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
	return m_owner;
}


object CCachedProperty::get_cached_value(object instance)
{
	if (!m_name || m_owner.is_none())
		BOOST_RAISE_EXCEPTION(
			PyExc_AttributeError,
			"Unable to retrieve the value of an unbound property."
		);

	PyObject *pValue = NULL;
	PyObject **ppDict = _PyObject_GetDictPtr(instance.ptr());

	
	if (ppDict && *ppDict) {
		pValue = PyDict_GetItem(*ppDict, m_name.ptr());
	}

	if (!pValue) {
		const char *szName = extract<const char *>(m_name);
		BOOST_RAISE_EXCEPTION(
			PyExc_KeyError,
			"No cached value found for '%s'.",
			szName
		)
	}

	return object(handle<>(borrowed(pValue)));
}

void CCachedProperty::set_cached_value(object instance, object value)
{
	if (!m_name || m_owner.is_none())
		BOOST_RAISE_EXCEPTION(
			PyExc_AttributeError,
			"Unable to assign the value of an unbound property."
		);

	if (!PyObject_IsInstance(instance.ptr(), m_owner.ptr())) {
		const char *szOwner = extract<const char *>(m_owner.attr("__qualname__"));
		BOOST_RAISE_EXCEPTION(
			PyExc_TypeError,
			"Given instance is not of type '%s'.",
			szOwner
		)
	}

	if (PyGen_Check(value.ptr())) {
		return;
	}

	PyObject *pDict = PyObject_GenericGetDict(instance.ptr(), NULL);

	if (!pDict) {
		const char *szOwner = extract<const char *>(m_owner.attr("__qualname__"));
		BOOST_RAISE_EXCEPTION(
			PyExc_AttributeError,
			"'%s' object has no attribute '__dict__'",
			szOwner
		)
	}

	PyDict_SetItem(pDict, m_name.ptr(), value.ptr());
	Py_XDECREF(pDict);
}

void CCachedProperty::delete_cached_value(object instance)
{
	PyObject **ppDict = _PyObject_GetDictPtr(instance.ptr());

	if (!ppDict && !*ppDict) {
		return;
	}

	PyDict_DelItem(*ppDict, m_name.ptr());

	if (PyErr_Occurred()) {
		if (!PyErr_ExceptionMatches(PyExc_KeyError)) {
			throw_error_already_set();
		}

		PyErr_Clear();
	}
}


object CCachedProperty::bind(object self, object owner, str name)
{
	CCachedProperty &pSelf = extract<CCachedProperty &>(self);

	if (owner.is_none() && !name) {
		BOOST_RAISE_EXCEPTION(
			PyExc_ValueError,
			"Must provide a name and an owner."
		)
	}

	owner.attr(name) = self;
	pSelf.__set_name__(owner, name);
	return self;
}


void CCachedProperty::__set_name__(object owner, str name)
{
	if (m_name || !m_owner.is_none())
	{
		const char *szName = extract<const char *>(str(".").join(make_tuple(m_owner.attr("__qualname__"), m_name)));
		BOOST_RAISE_EXCEPTION(
			PyExc_RuntimeError,
			"This property was already bound as \"%s\".",
			szName
		)
	}

	m_name = name;
	m_owner = owner;
}

object CCachedProperty::__get__(object self, object instance, object owner=object())
{
	if (instance.is_none())
		return self;

	CCachedProperty &pSelf = extract<CCachedProperty &>(self);
	object value;

	try
	{
		value = pSelf.get_cached_value(instance);
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

		value = getter(
			*(make_tuple(handle<>(borrowed(instance.ptr()))) + pSelf.m_args),
			**pSelf.m_kwargs
		);

		pSelf.set_cached_value(instance, value);
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
		set_cached_value(instance, result);
	else
		delete_cached_value(instance);
}

void CCachedProperty::__delete__(object instance)
{
	if (!m_fdel.is_none())
		m_fdel(
			*(make_tuple(handle<>(borrowed(instance.ptr()))) + m_args),
			**m_kwargs
		);

	delete_cached_value(instance);
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
	boost::python::tuple args, object kwargs)
{
	CCachedProperty *pProperty = new CCachedProperty(
		descriptor.attr("__get__"), descriptor.attr("__set__"), descriptor.attr("__delete__"),
		descriptor.attr("__doc__"), args, kwargs
	);

	pProperty->__set_name__(owner, name);

	return pProperty;
}
