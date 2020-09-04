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
#include "export_main.h"
#include "sp_main.h"
#include "core_cache.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_cached_property(scope);


//-----------------------------------------------------------------------------
// Declare the _core._cache module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_core, _cache)
{
	export_cached_property(_cache);
}


//-----------------------------------------------------------------------------
// Exports CachedProperty.
//-----------------------------------------------------------------------------
void export_cached_property(scope _cache)
{
	class_<CCachedProperty, CCachedProperty *> CachedProperty(
		"CachedProperty",
		init<object, object, object, object, bool, boost::python::tuple, object>(
			(
				arg("self"), arg("fget")=object(), arg("fset")=object(), arg("fdel")=object(), arg("doc")=object(),
				arg("unbound")=false, arg("args")=boost::python::tuple(), arg("kwargs")=object()
			),
			"Represents a property attribute that is only"
			" computed once and cached.\n"
			"\n"
			":param function fget:\n"
			"	Optional getter function.\n"
			"	Once the value has been computed, the result is cached and"
			" returned if this property is requested again.\n"
			"\n"
			"	Getter signature: self, *args, **kwargs\n"
			":param function fset:\n"
			"	Optional setter function.\n"
			"	When a new value is assigned to this property, that"
			" function is called and the cache is invalidated if nothing"
			" was returned otherwise the cached value is updated accordingly.\n"
			"\n"
			"	Setter signature: self, value, *args, **kwargs\n"
			":param function fdel:\n"
			"	Optional deleter function.\n"
			"	When this property is deleted, that function is called and the"
			" cached value (if any) is invalidated.\n"
			"\n"
			"	Deleter signature: self, *args, **kwargs\n"
			":param str doc:\n"
			"	Documentation string for this property.\n"
			":param bool unbound:\n"
			"	Whether the cached objects should be independently maintained rather than bound to"
			" the instance they belong to. The cache will be slightly slower to lookup, but this can"
			" be required for instances that do not have a `__dict__` attribute.\n"
			":param tuple args:\n"
			"	Extra arguments passed to the getter, setter and deleter functions.\n"
			":param dict kwargs:\n"
			"	Extra keyword arguments passed to the getter, setter and deleter functions.\n"
			"\n"
			":raises TypeError:\n"
			"	If the given getter, setter or deleter is not callable.\n"
			"\n"
			".. warning ::\n"
			"	If a cached object hold a strong reference of the instance it belongs to,"
			"	this will result in a circular reference preventing their garbage collection."
			"\n"
			"Example:\n"
			"\n"
			".. code:: python\n"
			"\n"
			"	from random import randint\n"
			"	from core.cache import cached_property\n"
			"\n"
			"	class Test:\n"
			"	    @cached_property(kwargs=dict(range=(0, 1000)))\n"
			"	    def test(self, range):\n"
			"	        return randint(*range)\n"
			"\n"
			"	    @test.setter\n"
			"	    def set_test(self, value, range):\n"
			"	        return int(value / 2)\n"
			"\n"
			"	test = Test()\n"
			"\n"
			"	# Compute and cache the value for the first time\n"
			"	i = test.test\n"
			"\n"
			"	# The first computed value was cached, so it should always be the same\n"
			"	assert i is test.test\n"
			"	assert i is test.test\n"
			"	assert i is test.test\n"
			"	assert i is test.test\n"
			"\n"
			"	# Deleting the property is invalidating the cache\n"
			"	del test.test\n"
			"	assert i is not test.test\n"
			"\n"
			"	# The cache will be updated to 5, because our setter computes value / 2\n"
			"	test.test = 10\n"
			"	assert test.test is 5\n"
			"\n"
			"	# The new value should be 1, because we updated our userdata\n"
			"	Test.test['range'] = (1, 1)\n"
			"	del test.test\n"
			"	assert test.test is 1\n"
		)
	);


	CachedProperty.def(
		"getter",
		&CCachedProperty::set_getter,
		"Decorator used to register the getter function for this property.\n"
		"\n"
		":param function fget:\n"
		"	The function to register as getter function.\n"
		"\n"
		":rtype:\n"
		"	function",
		args("self", "fget")
	);

	CachedProperty.add_property(
		"fget",
		&CCachedProperty::get_getter,
		&CCachedProperty::set_getter,
		"The getter function used to compute the value of this property.\n"
		"\n"
		":rtype:\n"
		"	function"
	);


	CachedProperty.def(
		"setter",
		&CCachedProperty::set_setter,
		"Decorator used to register the setter function for this property.\n"
		"\n"
		":param function fset:\n"
		"	The function to register as setter function.\n"
		"\n"
		":rtype:\n"
		"	function",
		args("self", "fset")
	);

	CachedProperty.add_property(
		"fset",
		&CCachedProperty::get_setter,
		&CCachedProperty::set_setter,
		"The setter function used to assign the value of this property.\n"
		"\n"
		":rtype:\n"
		"	function"
	);


	CachedProperty.def(
		"deleter",
		&CCachedProperty::set_deleter,
		"Decorator used to register the deleter function for this property.\n"
		"\n"
		":param function fdel:\n"
		"	The function to register as deleter function.\n"
		"\n"
		":rtype:\n"
		"	function",
		args("self", "fdel")
	);

	CachedProperty.add_property(
		"fdel",
		&CCachedProperty::get_deleter,
		&CCachedProperty::set_deleter,
		"The deleter function used to delete this property.\n"
		"\n"
		":rtype:\n"
		"	function"
	);


	CachedProperty.def_readwrite(
		"__doc__",
		&CCachedProperty::m_doc,
		"Documentation string for this property.\n"
		"\n"
		":rtype:\n"
		"	str"
	);


	CachedProperty.add_property(
		"name",
		&CCachedProperty::get_name,
		"The name this property is registered as.\n"
		"\n"
		":rtype:\n"
		"	str"
	);

	CachedProperty.add_property(
		"owner",
		&CCachedProperty::get_owner,
		"The owner class this property attribute was bound to.\n"
		"\n"
		":rtype:\n"
		"	type"
	);


	CachedProperty.def_readwrite(
		"args",
		&CCachedProperty::m_args,
		"The extra arguments passed to the getter, setter and deleter functions.\n"
		"\n"
		":rtype:\n"
		"	tuple"
	);

	CachedProperty.def_readwrite(
		"kwargs",
		&CCachedProperty::m_kwargs,
		"The extra keyword arguments passed to the getter, setter and deleter functions.\n"
		"\n"
		":rtype:\n"
		"	dict"
	);

	CachedProperty.def(
		"__set_name__",
		&CCachedProperty::__set_name__,
		"Called when this property is being bound to a class.\n"
		"\n"
		":param class owner:\n"
		"	The class this property is being bound to.\n"
		":param str name:\n"
		"	The name this property is being bound as."
		"\n"
		":raises RuntimeError:\n"
		"	If this property was already bound to a class.",
		args("self", "owner", "name")
	);

	CachedProperty.def(
		"__get__",
		&CCachedProperty::__get__,
		"Retrieves the value of this property.\n"
		"\n"
		":param object instance:\n"
		"	The instance for which this property is retrieved.\n"
		":param class owner:\n"
		"	The class for which this property is retrieved.\n"
		"\n"
		":rtype:\n"
		"	object",
		("self", "instance", arg("owner")=object())
	);

	CachedProperty.def(
		"__set__",
		&CCachedProperty::__set__,
		"Assigns the value of this property.\n"
		"\n"
		":param object instance:\n"
		"	The instance this property is being assigned to.\n"
		":param object value:\n"
		"	The value assigned to this property.\n"
		":rtype:\n"
		"	object",
		args("self", "instance", "value")
	);

	CachedProperty.def(
		"__delete__",
		&CCachedProperty::__delete__,
		"Deletes this property and invalidates its cached value.\n"
		"\n"
		":param object instance:\n"
		"	The instance for which this property is being deleted.",
		args("self", "instance")
	);

	CachedProperty.def(
		"__call__",
		&CCachedProperty::__call__,
		"Decorator used to register the getter function for this property.\n"
		"\n"
		":param function fget:\n"
		"	The function to register as getter function.\n"
		"\n"
		":rtype:\n"
		"	function",
		args("self", "fget")
	);

	CachedProperty.def(
		"__getitem__",
		&CCachedProperty::__getitem__,
		"Returns the value of a keyword argument.\n"
		"\n"
		":param str item:\n"
		"	The name of the keyword.\n"
		"\n"
		":rtype:"
		"	object",
		args("self", "item")
	);

	CachedProperty.def(
		"__setitem__",
		&CCachedProperty::__setitem__,
		"Sets the value of a keyword argument.\n"
		"\n"
		":param str item:\n"
		"	The name of the keyword.\n"
		":param object value:\n"
		"	The value to assign to the given keyword.",
		args("self", "item", "value")
	);

	CachedProperty.def(
		"get_cached_value",
		&CCachedProperty::get_cached_value,
		"Returns the cached value for the given instance.\n"
		"\n"
		":param object instance:\n"
		"	The instance to get the cached value for.\n"
		"\n"
		":raises KeyError:\n"
		"	If the given instance didn't have a cached value.\n"
		"\n"
		":rtype:"
		"	object",
		args("self", "instance")
	);

	CachedProperty.def(
		"set_cached_value",
		&CCachedProperty::set_cached_value,
		"Sets the cached value for the given instance.\n"
		"\n"
		":param object instance:\n"
		"	The instance to set the cached value for.\n"
		":param object value:\n"
		"	The value to set as cached value.\n",
		args("self", "instance", "value")
	);

	CachedProperty.def(
		"bind",
		&CCachedProperty::bind,
		"Binds this property to the given class as the given name.\n"
		"\n"
		":param class owner:\n"
		"	The class the property should be bound to.\n"
		":param str name:\n"
		"	The name of this property.\n"
		"\n"
		":rtype:\n"
		"	CachedProperty",
		args("self", "owner", "name")
	);

	CachedProperty.def(
		"wrap_descriptor",
		&CCachedProperty::wrap_descriptor,
		manage_new_object_policy(),
		"Wraps a descriptor as a cached property.\n"
		"\n"
		":param property descriptor:\n"
		"	Property descriptor to wrap.\n"
		"	Must have a __get__, __set__ and a __delete__ methods bound to it, either"
		"	callable or set to None.\n"
		":param class owner:\n"
		"	The class the wrapped property should be bound to.\n"
		":param str name:\n"
		"	The name of this property.\n"
		":param tuple args:\n"
		"	Extra arguments passed to the getter, setter and deleter functions.\n"
		":param dict kwargs:\n"
		"	Extra keyword arguments passed to the getter, setter and deleter functions.\n"
		"\n"
		":raises AttributeError:\n"
		"	If the given descriptor doesn't have the required methods.\n"
		":raises TypeError:\n"
		"	If the getter, setter or deleter are not callable.",
		(
			"descriptor", arg("owner")=object(), arg("name")=str(),
			arg("unbound")=false, arg("args")=boost::python::tuple(), arg("kwargs")=dict()
		)
	)
	.staticmethod("wrap_descriptor");

	scope().attr("cached_property") = scope().attr("CachedProperty");
}
