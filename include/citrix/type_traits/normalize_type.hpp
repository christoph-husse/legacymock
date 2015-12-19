
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_TYPETRAITS_NORMALIZETYPE_HPP_
#define _CITRIX_TYPETRAITS_NORMALIZETYPE_HPP_

#include "citrix/SystemPreincludeFixes.h"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/static_assert.hpp>

#include "citrix/Config.h"

namespace citrix { namespace type_traits {
    /*
        Recursively strips a type of all "const", "volatile" and reference
        stuff so that only the original bare type and all its pointers remain.
        (see bottom for static_assert's as example)

        This feature is essential for matching user defined types to parameters
        and return types, because we want the user to only define their stuff for
        bare types and not for every single qualifier/reference/pointer variation too.
    */
    template<class T, class Condition = void>
    struct normalize_type { };

    template<class T>
    struct normalize_type<T,
        typename boost::disable_if<boost::is_pointer<typename boost::remove_reference<T>::type>, void>::type
    > {
        typedef typename boost::remove_cv<
            typename boost::remove_reference<
                typename boost::remove_cv<T>::type
            >::type
        >::type type;
    };

    template<class T>
    struct normalize_type<T,
        typename boost::enable_if<boost::is_pointer<typename boost::remove_reference<T>::type>, void>::type
    > {
        typedef typename boost::add_pointer<
            typename normalize_type<
                typename boost::remove_pointer<
                    typename boost::remove_reference<T>::type
                >::type
            >::type
        >::type type;
    };

    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<double&>::type, double>::value),                                         "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<int>::type, int>::value),                                                "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<int const>::type, int>::value),                                          "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<int const volatile>::type, int>::value),                                 "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<int const volatile &>::type, int>::value),                               "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const int*>::type, int*>::value),                                        "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const volatile int*>::type, int*>::value),                               "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const volatile int**>::type, int**>::value),                             "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const volatile int* const*>::type, int**>::value),                       "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const volatile int* const* volatile * const>::type, int***>::value),     "Gee, you screwed up... Too bad!");
    BOOST_STATIC_ASSERT_MSG((boost::is_same<normalize_type<const volatile int* const* volatile * const &>::type, int***>::value),   "Gee, you screwed up... Too bad!");
}}


#endif