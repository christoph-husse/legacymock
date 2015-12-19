
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_META_MFP_TRAITS_H_
#define _CITRIX_MOCK_META_MFP_TRAITS_H_

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
#include "citrix/Platform.h"

namespace citrix { namespace type_traits {
        
    namespace internal {

        template <bool b, class T, class U>
        struct conditional : public boost::mpl::if_c<b, T, U>
        {
        };
    }

    /*
        API hooking requires us to re-route member functions to a different class.
        To make this operation type-safe, we need a way to replace the class within
        a member function pointer with a different class. And yes, this will make
        the "this" pointer anything but type-safe but that's just in the nature of
        things. At least the hook itself, with the calling convention and parameters
        is kept type-safe, which is really important in C++, since there are so many
        calling convention details and its easy to get it wrong when hooking something.
        Additionally, it allows us to automatically resolve overloaded functions :).

        This is pretty much what the following meta-template does. See Unittests
        for examples.
    */

    template<class TFunction, class TNewMemberType = void>
    struct member_function_pointer_traits { 
        BOOST_STATIC_ASSERT_MSG((boost::is_same<TFunction, void>::value), "This should not be instanciated. The given type is either not a function or something is broken.");
    };

    // this would have been ugly even with variadic templates, but this is just madness :D *claps excitedly*

    //======> Class::*()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TCurrentMemberType, class TNewMemberType     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (TCurrentMemberType::*)                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)), TNewMemberType> {                     \
            typedef typename internal::conditional<                                 \
                    boost::is_same<TNewMemberType, void>::value,                    \
                    TCurrentMemberType,                                             \
                    TNewMemberType                                                  \
                >::type class_type;                                                 \
            typedef TReturn (class_type::*type)(BOOST_PP_ENUM_PARAMS(N, A));        \
            typedef TReturn return_type;                                            \
            typedef type call_conv_erased_type;                                     \
            BOOST_STATIC_CONSTANT(bool, is_mfp = true);                             \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> Class::*() throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TCurrentMemberType, class TNewMemberType     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (TCurrentMemberType::*)                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)) throw(), TNewMemberType> {             \
            typedef typename internal::conditional<                                 \
                    boost::is_same<TNewMemberType, void>::value,                    \
                    TCurrentMemberType,                                             \
                    TNewMemberType                                                  \
                >::type class_type;                                                 \
            typedef TReturn (class_type::*type)(BOOST_PP_ENUM_PARAMS(N, A));        \
            typedef TReturn return_type;                                            \
            typedef type call_conv_erased_type;                                     \
            BOOST_STATIC_CONSTANT(bool, is_mfp = true);                             \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif

    //======> Class::*() const
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TCurrentMemberType, class TNewMemberType     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (TCurrentMemberType::*)                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)) const, TNewMemberType> {               \
            typedef typename internal::conditional<                                 \
                    boost::is_same<TNewMemberType, void>::value,                    \
                    TCurrentMemberType,                                             \
                    TNewMemberType                                                  \
                >::type class_type;                                                 \
            typedef TReturn (class_type::*type)(BOOST_PP_ENUM_PARAMS(N, A)) const;  \
            typedef TReturn return_type;                                            \
            typedef type call_conv_erased_type;                                     \
            BOOST_STATIC_CONSTANT(bool, is_mfp = true);                             \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> Class::*() const throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TCurrentMemberType, class TNewMemberType     \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (TCurrentMemberType::*)                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)) const throw(), TNewMemberType> {       \
            typedef typename internal::conditional<                                 \
                    boost::is_same<TNewMemberType, void>::value,                    \
                    TCurrentMemberType,                                             \
                    TNewMemberType                                                  \
                >::type class_type;                                                 \
            typedef TReturn (class_type::*type)(BOOST_PP_ENUM_PARAMS(N, A)) const;  \
            typedef TReturn return_type;                                            \
            typedef type call_conv_erased_type;                                     \
            BOOST_STATIC_CONSTANT(bool, is_mfp = true);                             \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif

    //======> (__stdcall* Function)()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (__stdcall*)                                                \
                (BOOST_PP_ENUM_PARAMS(N, A)), TNewMemberType> {                     \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__stdcall *type)(BOOST_PP_ENUM_PARAMS(N, A));          \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> (__stdcall* Function)() throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (__stdcall*)                                                \
                (BOOST_PP_ENUM_PARAMS(N, A)) throw(), TNewMemberType> {             \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__stdcall *type)(BOOST_PP_ENUM_PARAMS(N, A));          \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif

    //======> __stdcall Function()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn __stdcall                                                   \
                (BOOST_PP_ENUM_PARAMS(N, A)), TNewMemberType> {                     \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__stdcall *type)(BOOST_PP_ENUM_PARAMS(N, A));          \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> __stdcall Function() throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn __stdcall                                                   \
                (BOOST_PP_ENUM_PARAMS(N, A)) throw(), TNewMemberType> {             \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__stdcall *type)(BOOST_PP_ENUM_PARAMS(N, A));          \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif

    //======> (__cdecl* Function)()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (__cdecl*)                                                  \
                (BOOST_PP_ENUM_PARAMS(N, A)), TNewMemberType> {                     \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__cdecl *type)(BOOST_PP_ENUM_PARAMS(N, A));            \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()

#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> (__cdecl* Function)() throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn (__cdecl*)                                                  \
                (BOOST_PP_ENUM_PARAMS(N, A)) throw(), TNewMemberType> {             \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__cdecl *type)(BOOST_PP_ENUM_PARAMS(N, A));            \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif

    //======> __cdecl Function()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn __cdecl                                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)), TNewMemberType> {                     \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__cdecl* type)(BOOST_PP_ENUM_PARAMS(N, A));            \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
            
#if CITRIX_THROW_SPEC_OVERLOAD_SUPPORTED
    //======> __cdecl Function() throw()
    #define BOOST_PP_LOCAL_MACRO(N)                                                 \
        template <class TReturn, class TNewMemberType                               \
            BOOST_PP_COMMA_IF(N)                                                    \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                       \
        struct member_function_pointer_traits<                                      \
                TReturn __cdecl                                                     \
                (BOOST_PP_ENUM_PARAMS(N, A)) throw(), TNewMemberType> {             \
            typedef void class_type;                                                \
            typedef TReturn return_type;                                            \
            typedef TReturn (__cdecl* type)(BOOST_PP_ENUM_PARAMS(N, A));            \
            typedef TReturn (*call_conv_erased_type)(BOOST_PP_ENUM_PARAMS(N, A));   \
            BOOST_STATIC_CONSTANT(bool, is_mfp = false);                            \
        };
            
    #define BOOST_PP_LOCAL_LIMITS (0, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
    #include BOOST_PP_LOCAL_ITERATE()
#endif
        
}}

#endif