// boost lockfree
//
// Copyright (C) 2011 Tim Blechmann
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_LOCKFREE_POLICIES_HPP_INCLUDED
#define BOOST_LOCKFREE_POLICIES_HPP_INCLUDED

#include <boost/parameter/template_keyword.hpp>

#include <type_traits>

namespace boost { namespace lockfree {

#ifndef BOOST_DOXYGEN_INVOKED
namespace tag {
struct allocator;
struct fixed_sized;
struct capacity;
} // namespace tag

/** Configures a data structure as \b fixed-sized.
 *
 *  The internal nodes are stored inside an array and they are addressed by array indexing. This limits the possible
 * size of the queue to the number of elements that can be addressed by the index type (usually 2**16-2), but on
 * platforms that lack double-width compare-and-exchange instructions, this is the best way to achieve lock-freedom.
 * This implies that a data structure is bounded.
 * */
template < bool IsFixedSized >
struct fixed_sized : boost::parameter::template_keyword< tag::fixed_sized, std::integral_constant< bool, IsFixedSized > >
{};

/** Sets the \b capacity of a data structure at compile-time.
 *
 * This implies that a data structure is bounded and fixed-sized.
 * */
template < size_t Size >
struct capacity : boost::parameter::template_keyword< tag::capacity, std::integral_constant< size_t, Size > >
{};

/** Defines the \b allocator type of a data structure.
 * */
template < class Alloc >
struct allocator : boost::parameter::template_keyword< tag::allocator, Alloc >
{};

#else

/** Configures a data structure as \b fixed-sized.
 *
 *  The internal nodes are stored inside an array and they are addressed by array indexing. This limits the possible
 * size of the queue to the number of elements that can be addressed by the index type (usually 2**16-2), but on
 * platforms that lack double-width compare-and-exchange instructions, this is the best way to achieve lock-freedom.
 * This implies that a data structure is bounded.
 * */
template < bool IsFixedSized >
struct fixed_sized;

/** Sets the \b capacity of a data structure at compile-time.
 *
 * This implies that a data structure is bounded and fixed-sized.
 * */
template < size_t Size >
struct capacity;

/** Defines the \b allocator type of a data structure.
 * */
template < class Alloc >
struct allocator;

#endif

}}     // namespace boost::lockfree

#endif /* BOOST_LOCKFREE_POLICIES_HPP_INCLUDED */
