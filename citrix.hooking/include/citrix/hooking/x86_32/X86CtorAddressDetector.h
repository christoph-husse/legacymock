
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_X86_32_X86CTORADDRESSDETECTOR_H_
#define _CITRIX_HOOKING_X86_32_X86CTORADDRESSDETECTOR_H_

#include <boost/cstdint.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include "citrix/Platform.h"
#include "X86Utils.h"
#include "../CtorAndDtor.h"

#ifndef LEGACYMOCK_ARCH_X86_32
#   error "This class does not support the current processor."
#endif

namespace citrix { namespace hooking { namespace x86_32 {
        
    /*
        This class will extrapolate constructor & destructor addresses of arbitrary
        classes. 

        Beware of the following:

            * Is highly platform & compiler dependent
            * Will likely not work with optimizations enabled (unless your compiler is really bad at it)
            * Can only detect user defined constructors & destructors
    */
    class X86CtorAddressDetector {
    public:

        template <class T>
        static TypedCtorAndDtor<void(T::*)()> getCtorAndDtor() { 
            static boost::int64_t alwaysFalse = 1; 
            unsigned char* start = X86Utils::getInstructionPointer();
            if(alwaysFalse++ == 0) { 
                T t; 
                t.~T();
            }
            unsigned char* end = X86Utils::getInstructionPointer();
            return scanForCtorAndDtor((boost::int64_t)start, (boost::int64_t)end - 5).asTyped<void(T::*)()>();
        }

        #define BOOST_PP_LOCAL_MACRO(N)                                                 \
            template <class T                                                           \
            BOOST_PP_COMMA_IF(N)                                                        \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                           \
            static TypedCtorAndDtor<void(T::*)(BOOST_PP_ENUM_PARAMS(N, A))>             \
                    getCtorAndDtor(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) {              \
                static boost::int64_t alwaysFalse = 1;                                  \
                unsigned char* start = X86Utils::getInstructionPointer();               \
                if(alwaysFalse++ == 0) {                                                \
                    T t(BOOST_PP_ENUM_PARAMS(N, a));                                    \
                    t.~T();                                                             \
                }                                                                       \
                unsigned char* end = X86Utils::getInstructionPointer();                 \
                return scanForCtorAndDtor((boost::int64_t)start, (boost::int64_t)end - 5).asTyped<void(T::*)(BOOST_PP_ENUM_PARAMS(N, A))>();\
            }
            
        #define BOOST_PP_LOCAL_LIMITS (1, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
        #include BOOST_PP_LOCAL_ITERATE()

    private:
        static bool firstFunctionCallsSecondFunction(void* first, void* second);
        static internal::CtorAndDtor scanForCtorAndDtor(boost::int64_t start, boost::int64_t end);
    };
    
}}}

#endif
