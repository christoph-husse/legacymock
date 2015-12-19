
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_LEGACYMOCKS_PLATFORM_H_
#define _CITRIX_LEGACYMOCKS_PLATFORM_H_

#include "citrix/Config.h"

#include <boost/version.hpp>

#include "citrix/predef/predef.h" // bundled, because only available in VERY recent boost versions.

#if BOOST_ARCH_X86_32
#   define LEGACYMOCK_ARCH_X86_32
#else
#   error "Your processor architecture is not supported!"
#endif

#if BOOST_COMP_MSVC
#   define LEGACYMOCK_COMP_MSVC
#elif BOOST_COMP_CLANG || BOOST_COMP_LLVM
#   define LEGACYMOCK_COMP_CLANG
#elif BOOST_COMP_GCC || BOOST_COMP_GCC_EMULATED || BOOST_COMP_GNUC
#   define LEGACYMOCK_COMP_GCC
#else
#   error "Your compiler is not supported!"
#endif

#if BOOST_OS_WINDOWS
#   define LEGACYMOCK_OS_WINDOWS
#elif BOOST_OS_MACOS
#   define LEGACYMOCK_OS_MACOS
#elif BOOST_OS_LINUX
#   define LEGACYMOCK_OS_LINUX
#else
#   error "Your operating system is not supported!"
#endif

#ifdef LEGACYMOCK_OS_WINDOWS
#	define LEGACYMOCK_CALLER_CLEANS_STACK __cdecl
#else
#	define LEGACYMOCK_CALLER_CLEANS_STACK __attribute__((cdecl))
#endif

#include <boost/cstdint.hpp>

#if !defined(INT8_MAX) && !defined(_STDINT) 
    using boost::int8_t;
    using boost::int16_t;
    using boost::int32_t;
    using boost::int64_t;
    using boost::uint8_t;
    using boost::uint16_t;
    using boost::uint32_t;
    using boost::uint64_t;

	#define INT8_MAX
    #define _STDINT
#endif

#if BOOST_VERSION < 104100 // version 1.41 (introduction of property_tree)
#   define LEGACYMOCK_SUPPORTS_TRACING 0
#else
#   define LEGACYMOCK_SUPPORTS_TRACING 1
#endif

#define CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED 0

#ifdef _MSC_VER
#   if _MSC_VER < 1700
#       undef CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
#       define CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED 1
#   endif
#endif

#endif
