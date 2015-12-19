
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_TYPETRAITS_MAKEREFERENCE_HPP_
#define _CITRIX_TYPETRAITS_MAKEREFERENCE_HPP_

#include "citrix/SystemPreincludeFixes.h"
#include "citrix/Config.h"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_extent.hpp>

namespace citrix { namespace type_traits {

    /*
        Make a reference to any type where it is possible.
        For arrays, this means a reference to the first element,
        while allows traversal. The C++ generator is not able to deal
        with arrays properly, so this is a workaround to still make
        variable access possible.
    */
    template<class T>
    static typename boost::enable_if<
            boost::is_array<T>, 
            typename boost::remove_extent<T>::type&
    >::type make_reference(T* value) {
        return (*value)[0];
    }

    template<class T>
    static typename boost::disable_if<
            boost::is_array<T>, 
            T&
    >::type make_reference(T* value) {
        return *value;
    }
}}

#endif