/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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

// The following code is taken from here. Thank you!
// http://members.gamedev.net/sicrane/articles/metaprogramming.html
// It has been altered, so that it correctly detects calling
// conventions and works on Linux as well.

#if !BOOST_PP_IS_ITERATING
  #ifndef _MEMORY_CALLING_CONVENTION_H
  #define _MEMORY_CALLING_CONVENTION_H

  #include "modules/memory/memory_function.h"
  #include <boost/type_traits.hpp>

  #include <boost/preprocessor/iteration/iterate.hpp>

  #include <boost/preprocessor/repetition/enum_params.hpp>
  #include <boost/preprocessor/repetition/enum_trailing_params.hpp>

  #ifdef _WIN32
  #define CC_CDECL __cdecl
  #elif defined(__linux__)
  #define CC_CDECL
  #else
  #error "Unsupported platform."
  #endif

  class Undefined;
  void TypeDoesntSeemToBeAFunction(Undefined &);

  template <typename T>
  struct CallingConvention {
    CallingConvention() {
      TypeDoesntSeemToBeAFunction(T());
    }
  };

  // Call this function to get the calling convention of the given function
  template <typename Function>
  Convention_t GetCallingConvention(Function func) {
    return CallingConvention<Function>();
  }

  #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PYTHON_MAX_ARITY, "modules/memory/memory_calling_convention.h"))
  #include BOOST_PP_ITERATE()

  #endif // _MEMORY_CALLING_CONVENTION_H
#elif BOOST_PP_ITERATION_DEPTH() == 1
  #define NUM_ARGS BOOST_PP_FRAME_ITERATION(1)

  // Handle function of the global scope
  template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL *)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A))>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  // Handle variadic functions
  template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL *)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A) ...)>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  #ifdef _WIN32
  // We only need to care about __stdcall on Windows
  template<typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (__stdcall *)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A))>
    : boost::integral_constant<Convention_t, CONV_STDCALL> {};
  #endif // _WIN32

  // Handle class methods
  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A))>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A)) const>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  // Handle variadic class methods
  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A) ...)>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (CC_CDECL T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A) ...) const>
    : boost::integral_constant<Convention_t, CONV_CDECL> {};

  #ifdef _WIN32
  // We only need to care about __thiscall and __stdcall on Windows
  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (__thiscall T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A))>
    : boost::integral_constant<Convention_t, CONV_THISCALL> {};

  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (__thiscall T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A)) const>
    : boost::integral_constant<Convention_t, CONV_THISCALL> {};

  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (__stdcall T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A))>
    : boost::integral_constant<Convention_t, CONV_STDCALL> {};

  template<typename T, typename R BOOST_PP_ENUM_TRAILING_PARAMS(NUM_ARGS, typename A)>
  struct CallingConvention<R (__stdcall T::*)(BOOST_PP_ENUM_PARAMS(NUM_ARGS, A)) const>
    : boost::integral_constant<Convention_t, CONV_STDCALL> {};
  #endif // _WIN32
#endif // BOOST_PP_IS_ITERATING