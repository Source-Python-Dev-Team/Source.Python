/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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

#ifndef _ARRAY_H
#define _ARRAY_H

// ============================================================================
// >> INCLUDES
// ============================================================================
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_utilities.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>


// ============================================================================
// >> Array
// ============================================================================
// Implementation is based on this:
// https://stackoverflow.com/a/27560620
template<class T>
class Array
{
public:
	typedef T* pointer;
	typedef T value_type;
	typedef std::size_t size_type;
	typedef T* iterator;
	typedef T const* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	Array()
		:	arr_(NULL),
			length_(-1)
	{}

	Array(pointer arr)
		:	arr_(arr),
			length_(-1)
	{}

	Array(void* arr)
		:	arr_(reinterpret_cast<pointer>(arr)),
			length_(-1)
	{}

	Array(void* arr, size_type length)
		:	arr_(reinterpret_cast<pointer>(arr)),
			length_(length)
	{}

	Array(pointer arr, size_type length)
		:	arr_(arr),
			length_(length)
	{}

	Array( pointer first, pointer last )
		:	arr_(first),
			length_(static_cast<size_type>(std::distance(first, last)))
	{}

	T& operator[](size_type index)
	{
		return arr_[index];
	}
	
	iterator begin()
	{
		return arr_;
	}  

	const_iterator cbegin() const
	{
		return arr_;
	}

	iterator end()
	{
		return begin() + size();
	}

	const_iterator cend() const
	{
		return cbegin() + size();
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator( end() );
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator( cend() );
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator( cend() );
	}

	reverse_iterator rend()
	{
		return reverse_iterator( begin() );
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( cbegin() );
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator( cbegin() );
	}

	size_type size() const
	{
		validate_length();
		return length_;
	}

	void check_index(size_type index) const
	{
		size_type length = size();
		if (!(0 <= index < length))
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index (%d) out of range (%d)", index, length)
	}

	void validate_length() const 
	{
		if (!has_length())
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Operation requires length of the array.")
	}

	bool has_length() const
	{
		return length_ != -1;
	}

	static void set_length(Array& arr, size_type size)
	{
		arr.length_ = size;
	}

protected:
	iterator arr_;
	size_type length_;
};


// ============================================================================
// >> array_indexing_suite
// ============================================================================
// Forward declaration
template<
  typename Array,
  bool NoProxy,
  typename DerivedPolicies>
class array_indexing_suite;


namespace detail {

template<typename Array, bool NoProxy>
struct final_array_derived_policies
: array_indexing_suite<Array, NoProxy, final_array_derived_policies<Array, NoProxy>>
{};

}   /* namespace detail */


template<
	typename Array,
	bool NoProxy = std::is_arithmetic<typename Array::value_type>::value,
	typename DerivedPolicies = ::detail::final_array_derived_policies<Array, NoProxy>
>
class array_indexing_suite
	:	public boost::python::indexing_suite<Array, DerivedPolicies, NoProxy>
{
public:
	typedef typename Array::value_type data_type;
	typedef typename Array::value_type key_type;
	typedef typename Array::size_type index_type;
	typedef typename Array::size_type size_type;

	template <class Class>
	static void extension_def(Class& cl)
	{
		cl
			.def(init<void*>())
			.def(init<void*, size_type>())
			.def("set_length", Array::set_length)

			.def(GET_PTR_NAME, &get_ptr, manage_new_object_policy())
			.def(GET_OBJ_NAME, &make_obj, manage_new_object_policy()).staticmethod(GET_OBJ_NAME)
			.add_property(GET_SIZE_NAME, &get_size)
		;

		extern dict g_oExposedClasses;
		g_oExposedClasses[cl.attr("__name__")] = cl;
	}

	static CPointer* get_ptr(Array& arr)
	{
		return new CPointer((unsigned long) arr.begin());
	}

	static Array* make_obj(CPointer* pPtr)
	{
		return new Array((void *) pPtr->m_ulAddr);
	}

	static size_type get_size(Array& arr)
	{
		return arr.size() * sizeof(data_type);
	}

	static data_type & get_item(Array & arr, index_type i) {
		return arr[i];
	}

	static void set_item(Array & arr, index_type i, data_type const & v) {
		arr[i] = v;
	}

	static void delete_item(Array & arr, index_type i) {
		BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "delete_item")
	}

	static size_type size(Array & arr) {
		return arr.size();
	}

	static bool contains(Array & arr, key_type const & key) {
		return std::find(arr.cbegin(), arr.cend(), key) != arr.cend();
	}

	static index_type get_min_index(Array & arr) {
		return 0;
	}

	static index_type get_max_index(Array & arr) {
		return arr.size();
	}

	static bool compare_index(Array & , index_type a, index_type b) {
		return a < b;
	}

	static index_type convert_index(Array & arr, PyObject * i_) {
		boost::python::extract < long > i(i_);
		if (i.check()) {
			long index = i();

			if (index < 0) {
				index += static_cast < decltype(index) > (DerivedPolicies::size(arr));
			}

			// This allows wrapping raw pointers and treat them like an array.
			// The programmer has to take care of not exceeding the array size.
			if (arr.has_length())
				arr.validate_length();

			return index;
		}

		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Invalid index type");

		return index_type();
	}

	static boost::python::object get_slice(Array & arr, index_type from, index_type to) {
		if (from > to) {
			return boost::python::object(Array());
		}
		return boost::python::object(Array(arr.begin() + from, arr.begin() + to));
	}

	static void set_slice(Array & arr, index_type from, index_type to, data_type
		const & v) {
		if (from > to) {
			return;

		} else if (to > arr.size()) {
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range");

		} else {
			std::fill(arr.begin() + from, arr.begin() + to, v);

		}
	}

	template < typename Iter >
	static void set_slice(Array & arr, index_type from, index_type to, Iter first, Iter last) {
		auto num_items = std::distance(first, last);

		if ((from + num_items) > arr.size()) {
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range");
		}

		if (from > to) {
			std::copy(first, last, arr.begin() + from);

		} else {
			if (static_cast < decltype(num_items) > (to - from) != num_items) {
				BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Array length is immutable");
			}

			std::copy(first, last, arr.begin() + from);
		}
	}

	static void delete_slice(Array & arr, index_type from , index_type to) {
		BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "delete_slice")
	}
};

#endif // _ARRAY_H