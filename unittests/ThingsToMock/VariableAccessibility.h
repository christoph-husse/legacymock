
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_VARIABLEACCESSIBILITY_H_
#define _UNITTEST_MOCK_VARIABLEACCESSIBILITY_H_

#include "citrix/Config.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "CustomTypesForTesting.h"

template<typename> class CitrixMockingAccessor;

namespace unittests { namespace MockVariableTest {
    class VariableAccessibilityTest;
}}

 namespace MockMe {

    class VariableAccessibility {
        template<typename> friend class ::CitrixMockingAccessor;
        friend class ::unittests::MockVariableTest::VariableAccessibilityTest;
    public:
        typedef int (*MyHandler)();
        typedef int (MyHandlerValue)();
        typedef struct _UNKNOWN_STRUCT_* PUnknownStruct;

        // TODO: function returning function not supported but there should be a way to disable this member
        // MyHandlerValue funcTest2;

        MyHandler funcTest1;
        PUnknownStruct unknownTest;
        int arrayTest1[8];
        int publicInt;
        static int S_arrayTest1[8];
        static int S_arrayTest2[];

        const MyOwnStruct publicOwnStruct__const;

        static int S_publicInt;
        static const MyOwnStruct S_publicOwnStruct__const;

        VariableAccessibility() :
            protectedOwnStruct__Ref(privateOwnStruct),
            protectedOwnStruct__const_Ref(privateOwnStruct),
            privateOwnStruct__const_Ptr_const(NULL),
            privateOwnStruct__const_Ptr(NULL),
            privateOwnStruct__Ptr(NULL)
        {
        }
    protected:
        std::string protectedString;
        MyOwnStruct& protectedOwnStruct__Ref;
        const MyOwnStruct& protectedOwnStruct__const_Ref;

        static std::string S_protectedString;
        static MyOwnStruct& S_protectedOwnStruct__Ref;
        static const MyOwnStruct& S_protectedOwnStruct__const_Ref;
    private:
        int privateInt;
        MyOwnStruct privateOwnStruct;
        MyOwnStruct* privateOwnStruct__Ptr;
        const MyOwnStruct* privateOwnStruct__const_Ptr;
        const MyOwnStruct* const privateOwnStruct__const_Ptr_const;

        static int S_privateInt;
        static MyOwnStruct S_privateOwnStruct;
        static MyOwnStruct* S_privateOwnStruct__Ptr;
        static const MyOwnStruct* S_privateOwnStruct__const_Ptr;
        static const MyOwnStruct* const S_privateOwnStruct__const_Ptr_const;

        static int S_privArrayTest1[8];
        static int S_privArrayTest2[];
    };
}

 namespace citrix { namespace mocking { namespace internal {
     template<class T> boost::shared_ptr<T> allocateDefaultInstance();

     template<> inline boost::shared_ptr<::MockMe::VariableAccessibility> allocateDefaultInstance() { 
         return boost::shared_ptr<::MockMe::VariableAccessibility>(new ::MockMe::VariableAccessibility()); 
     }
 }}}

#endif