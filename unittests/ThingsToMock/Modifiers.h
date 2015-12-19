
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_MODIFIERS_H_
#define _UNITTEST_MOCK_MODIFIERS_H_

#include "citrix/Config.h"
#include <exception>

namespace MockMe {
    class Modifiers {
    public:
        // test "throw()" more intensly since it is parsed by regular expression
        void const_throw1() const     throw   (    )   { }
        void const_throw2() const     throw   (    ){ }
        void const_throw3() const     throw   (){ }
        void const_throw4() const     throw(){ }
        void const_throw5() const
throw(){ }

        void _const_throw() const throw(){}
        void _throw() throw(){}
        void _const() const {}


        static void _static_throw() throw(){}
    };
}

#endif