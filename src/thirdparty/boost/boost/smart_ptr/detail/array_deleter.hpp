/*
 * Copyright (c) 2012 Glen Joseph Fernandes 
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License, 
 * Version 1.0. (See accompanying file LICENSE_1_0.txt 
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_SMART_PTR_DETAIL_ARRAY_DELETER_HPP
#define BOOST_SMART_PTR_DETAIL_ARRAY_DELETER_HPP

#include <boost/config.hpp>
#include <cstddef>

namespace boost {
    namespace detail {
        template<typename T>
        class array_deleter {
        public:
            array_deleter() 
                : size(0) {
            }
            ~array_deleter() {
                destroy();
            }
            void construct(T* memory, std::size_t count) {
                for (object = memory; size < count; size++) {
                    void* p1 = object + size;
                    ::new(p1) T();
                }
            }
#if defined(BOOST_HAS_VARIADIC_TMPL) && defined(BOOST_HAS_RVALUE_REFS)
            template<typename... Args>
            void construct(T* memory, std::size_t count, Args&&... args) {
                for (object = memory; size < count; size++) {
                    void* p1 = object + size;
                    ::new(p1) T(args...);
                }
            }
#endif
#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
            void construct_list(T* memory, std::size_t count, const T* list) {
                for (object = memory; size < count; size++) {
                    void* p1 = object + size;
                    ::new(p1) T(list[size]);
                }
            }
            void construct_list(T* memory, std::size_t count, const T* list, std::size_t n) {
                for (object = memory; size < count; size++) {
                    void* p1 = object + size;
                    ::new(p1) T(list[size % n]);
                }
            }
#endif
            void construct_noinit(T* memory, std::size_t count) {
                for (object = memory; size < count; size++) {
                    void* p1 = object + size;
                    ::new(p1) T;
                }
            }
            void operator()(const void*) {
                destroy();
            }
        private:
            void destroy() {
                while (size > 0) {
                    object[--size].~T();
                }
            }
            std::size_t size;
            T* object;
        };
    }
}

#endif
