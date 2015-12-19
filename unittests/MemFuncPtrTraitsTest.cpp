
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "citrix/type_traits/member_function_pointer_traits.hpp"

using namespace citrix::type_traits;

namespace UnitTests {

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*), std::string>::type,
        int (std::string::*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly convert member function pointers without const modifiers.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*)>::type,
        int (std::wstring::*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly work on member function pointers without class change.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) const, std::string>::type,
        int (std::string::*)(char*) const
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly convert member function pointers with const modifiers.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*)>::class_type,
        std::wstring
    >::value), 
    "member_function_pointer_traits<> is broken! Member type is not preserved if no replacement given.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*), std::string>::class_type,
        std::string
    >::value), 
    "member_function_pointer_traits<> is broken! Member type is not replaced.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*), std::string>::class_type,
        void
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not indicated as void.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*)>::class_type,
        void
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not indicated as void.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*), std::string>::type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*)>::type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*), std::string>::type,
        int (__stdcall *)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*)>::type,
        int (__stdcall *)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*), std::string>::call_conv_erased_type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not erased.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*)>::call_conv_erased_type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not erased.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*), std::string>::return_type,
        int
    >::value), 
    "member_function_pointer_traits<> is broken! Wrong return type.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*)>::return_type,
        int
    >::value), 
    "member_function_pointer_traits<> is broken! Wrong return type.");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) throw(), std::string>::type,
        int (std::string::*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly convert member function pointers without const modifiers.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) throw()>::type,
        int (std::wstring::*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly work on member function pointers without class change.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) const throw(), std::string>::type,
        int (std::string::*)(char*) const
    >::value), 
    "member_function_pointer_traits<> is broken! Does not properly convert member function pointers with const modifiers.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) throw()>::class_type,
        std::wstring
    >::value), 
    "member_function_pointer_traits<> is broken! Member type is not preserved if no replacement given.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (std::wstring::*)(char*) throw(), std::string>::class_type,
        std::string
    >::value), 
    "member_function_pointer_traits<> is broken! Member type is not replaced.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*) throw(), std::string>::class_type,
        void
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not indicated as void.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*) throw()>::class_type,
        void
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not indicated as void.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*) throw(), std::string>::type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (*)(char*) throw()>::type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Non-member-function is not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw(), std::string>::type,
        int (__stdcall *)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw()>::type,
        int (__stdcall *)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not preserved.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw(), std::string>::call_conv_erased_type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not erased.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw()>::call_conv_erased_type,
        int (*)(char*)
    >::value), 
    "member_function_pointer_traits<> is broken! Calling conventions are not erased.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw(), std::string>::return_type,
        int
    >::value), 
    "member_function_pointer_traits<> is broken! Wrong return type.");

BOOST_STATIC_ASSERT_MSG((boost::is_same<
        member_function_pointer_traits<int (__stdcall *)(char*) throw()>::return_type,
        int
    >::value), 
    "member_function_pointer_traits<> is broken! Wrong return type.");

}