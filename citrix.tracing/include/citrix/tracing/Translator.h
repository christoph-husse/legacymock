
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_TRACING_TRANSLATOR_H_
#define _CITRIX_MOCK_TRACING_TRANSLATOR_H_

#include "citrix/Platform.h"

#if !LEGACYMOCK_SUPPORTS_TRACING
#   error "Tracing is only supported since boost version 1.41!"
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace citrix { namespace tracing {
    
    namespace internal {
        template<class T, class K = void> struct TranslatorSFINAE { };
    }

    template<class T, class K = void>
    struct Translator { };

    struct TranslatorBase {
        typedef boost::property_tree::ptree ptree;

        template<class T>
        ptree translate(const T& something) {
            ptree json = Translator<T>().toJson(something);

            if(json.get_child_optional("value").is_initialized()) {
                return json.get_child_optional("value").get();
            } else {
                return json;
            }
        }
    };

#define MAKE_TRANSLATOR_FOR(forType) \
    namespace internal { template<> struct TranslatorSFINAE<forType> { typedef int SFINAE; typedef forType type; }; } \
    template<class T> \
    struct Translator< \
        T, \
        typename boost::enable_if<boost::is_same<typename boost::remove_const<T>::type, typename boost::remove_const<forType>::type> >::type> : public TranslatorBase

#define MAKE_TRANSLATOR_IF(type_trait) \
    namespace internal { template<class T> struct TranslatorSFINAE< \
        T, \
        typename boost::enable_if<type_trait<T> >::type> { typedef int SFINAE; typedef T type; }; } \
    template<class T> \
    struct Translator< \
        T, \
        typename boost::enable_if<type_trait<T> >::type>  : public TranslatorBase

}}

#endif