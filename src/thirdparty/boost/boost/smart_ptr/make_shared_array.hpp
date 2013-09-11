/*
 * Copyright (c) 2012 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP
#define BOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/detail/array_deleter.hpp>
#include <boost/smart_ptr/detail/array_traits.hpp>
#include <boost/smart_ptr/detail/make_array_helper.hpp>
#include <boost/smart_ptr/detail/sp_if_array.hpp>
#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#include <initializer_list>
#endif

namespace boost {
    template<typename T>
    inline typename detail::sp_if_array<T>::type
    make_shared(std::size_t size) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = size * detail::array_total<T1>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;
        p1 = reinterpret_cast<T1*>(p2);        
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct(p2, n1);
        return shared_ptr<T>(s1, p1);
    }
#if defined(BOOST_HAS_VARIADIC_TMPL) && defined(BOOST_HAS_RVALUE_REFS)
    template<typename T, typename... Args>
    inline typename detail::sp_if_array<T>::type
    make_shared(std::size_t size, Args&&... args) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = size * detail::array_total<T1>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;
        p1 = reinterpret_cast<T1*>(p2);        
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct(p2, n1, std::forward<Args>(args)...);
        return shared_ptr<T>(s1, p1);
    }
    template<typename T, typename... Args>
    inline typename detail::sp_if_size_array<T>::type
    make_shared(Args&&... args) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = detail::array_total<T>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;
        p1 = reinterpret_cast<T1*>(p2);        
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct(p2, n1, std::forward<Args>(args)...);
        return shared_ptr<T>(s1, p1);
    }
#endif
#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
    template<typename T>
    inline typename detail::sp_if_array<T>::type
    make_shared(std::initializer_list<typename detail::array_inner<T>::type> list) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        typedef const T2 T3;
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = 0;
        std::size_t n1 = list.size() * detail::array_total<T1>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;        
        p3 = reinterpret_cast<T3*>(list.begin());
        p1 = reinterpret_cast<T1*>(p2);
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct_list(p2, n1, p3);
        return shared_ptr<T>(s1, p1);
    }
    template<typename T>
    inline typename detail::sp_if_size_array<T>::type
    make_shared(std::initializer_list<typename detail::array_inner<T>::type> list) {
        BOOST_ASSERT(list.size() == detail::array_size<T>::size);
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        typedef const T2 T3;
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = 0;
        std::size_t n1 = detail::array_total<T>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;        
        p3 = reinterpret_cast<T3*>(list.begin());
        p1 = reinterpret_cast<T1*>(p2);
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct_list(p2, n1, p3);
        return shared_ptr<T>(s1, p1);
    }
    template<typename T>
    inline typename detail::sp_if_array<T>::type
    make_shared(std::size_t size, 
        std::initializer_list<typename detail::arrays_inner<T>::type> list) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        typedef const T2 T3;
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = 0;
        std::size_t n0 = detail::array_total<T1>::size;
        std::size_t n1 = n0 * size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;        
        p3 = reinterpret_cast<T3*>(list.begin());
        p1 = reinterpret_cast<T1*>(p2);
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct_list(p2, n1, p3, n0);
        return shared_ptr<T>(s1, p1);
    }
#endif
    template<typename T>
    inline typename detail::sp_if_array<T>::type
    make_shared_noinit(std::size_t size) {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = size * detail::array_total<T1>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;
        p1 = reinterpret_cast<T1*>(p2);        
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct_noinit(p2, n1);
        return shared_ptr<T>(s1, p1);
    }
    template<typename T>
    inline typename detail::sp_if_size_array<T>::type
    make_shared_noinit() {
        typedef typename detail::array_inner<T>::type T1;
        typedef typename detail::array_base<T1>::type T2;
        T1* p1 = 0;
        T2* p2 = 0;
        std::size_t n1 = detail::array_total<T>::size;
        detail::make_array_helper<T2> a1(n1, &p2);
        detail::array_deleter<T2> d1;
        shared_ptr<T> s1(p1, d1, a1);
        detail::array_deleter<T2>* d2;
        p1 = reinterpret_cast<T1*>(p2);        
        d2 = get_deleter<detail::array_deleter<T2> >(s1);
        d2->construct_noinit(p2, n1);
        return shared_ptr<T>(s1, p1);
    }
}

#endif
