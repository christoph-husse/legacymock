
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_LEGACYMOCKS_CONFIG_H_
#define _CITRIX_LEGACYMOCKS_CONFIG_H_

#define LEGACYMOCK_MAX_VARIADIC_ARG_COUNT 10

#ifndef CITRIX_MOCK_UNREFPARAM
#	define CITRIX_MOCK_UNREFPARAM(param) ((void)&param)
#endif

#include <boost/static_assert.hpp>
#ifndef BOOST_STATIC_ASSERT_MSG
#   define BOOST_STATIC_ASSERT_MSG(cond, msg) BOOST_STATIC_ASSERT(cond)
#endif 

#ifndef LEGACYMOCK_BUILD_BOOST
#   define LEGACYMOCK_BUILD_BOOST 0
#endif

#endif
