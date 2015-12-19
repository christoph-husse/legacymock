
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_TRYPETRAITS_RETURNTYPEOFFUNCTIONH_
#define _CITRIX_TRYPETRAITS_RETURNTYPEOFFUNCTIONH_

#include "citrix/SystemPreincludeFixes.h"

#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <string>

#include "citrix/Config.h"

namespace citrix { namespace type_traits {
        
    // only supports true functions, not function pointers and no member function pointers
    // (use member_function_pointer_traits for that)
    template<class TFunction>
    struct return_type_of_function { 
        BOOST_STATIC_ASSERT_MSG((boost::is_same<TFunction, void>::value), "This should not be instanciated. The given type is either not a function or something is broken.");
    };

    // non member stdcall
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn                                                     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct return_type_of_function<                                             \
                TReturn __stdcall                                                   \
                (BOOST_PP_ENUM_PARAMS(N, A))> {                                     \
            typedef TReturn type;                                                   \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

    // non member __cdecl
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn                                                     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct return_type_of_function<                                             \
                TReturn __cdecl                                                     \
                (BOOST_PP_ENUM_PARAMS(N, A))> {                                     \
            typedef TReturn type;                                                   \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
            


}}

#endif