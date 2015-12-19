
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include <gtest/gtest.h>

#include "citrix/Config.h"
#include "../citrix.hooking/include/citrix/hooking/Hooking.h"

using namespace citrix::hooking;


namespace unittests { namespace Hooking_CtorDtor {

    struct HasDtorOnly { ~HasDtorOnly() { } };
    struct HasCtorOnly { HasCtorOnly() { } };
    struct HasCtorAndDtor { HasCtorAndDtor() { } ~HasCtorAndDtor() { } };

    TEST(Hooking_CtorDtor, detect_DtorOnly) {
        ASSERT_FALSE(CtorAddressDetector::getCtorAndDtor<HasDtorOnly>().hasConstructor());
        ASSERT_TRUE(CtorAddressDetector::getCtorAndDtor<HasDtorOnly>().hasDestructor());

        ASSERT_EQ(CtorAddressDetector::getCtorAndDtor<HasDtorOnly>().dtor, CtorAddressDetector::getCtorAndDtor<HasDtorOnly>().dtor);
    }

    TEST(Hooking_CtorDtor, detect_CtorOnly) {
        ASSERT_TRUE(CtorAddressDetector::getCtorAndDtor<HasCtorOnly>().hasConstructor());

#if defined(LEGACYMOCK_COMP_GCC) & (BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(4,7,0))
        // before GCC 4.7, it will emit a destructor call anyway, no matter if user define dor not.
        ASSERT_TRUE(CtorAddress::getCtorAndDtor<HasCtorOnly>().hasDestructor());
#else
        ASSERT_FALSE(CtorAddressDetector::getCtorAndDtor<HasCtorOnly>().hasDestructor());
#endif
        ASSERT_EQ(CtorAddressDetector::getCtorAndDtor<HasCtorOnly>().ctor, CtorAddressDetector::getCtorAndDtor<HasCtorOnly>().ctor);
    }

    TEST(Hooking_CtorDtor, detect_CtorAndDtor) {
        ASSERT_TRUE(CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().hasConstructor());
        ASSERT_TRUE(CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().hasDestructor());

        ASSERT_EQ(CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().ctor, CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().ctor);
        ASSERT_EQ(CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().dtor, CtorAddressDetector::getCtorAndDtor<HasCtorAndDtor>().dtor);
    }

    struct RAIIMember {
        static int balance;

        RAIIMember() {
            balance++;
        }

        ~RAIIMember() {
            balance--;
        }
    };

    int RAIIMember::balance;

    struct TestClass {
        static int ctor_int_count;
        static int hooked_ctor_int_count;
        static int ctor_count;
        static int dtor_count;
        static int hooked_dtor_count;
        static TypedHook<void(TestClass::*)(int), TestClass>* ctor_hook;
        static TypedHook<void(TestClass::*)(), TestClass>* dtor_hook;

        static void reset() {
            RAIIMember::balance = 0;
            ctor_int_count = 0;
            hooked_ctor_int_count = 0;
            hooked_dtor_count = 0;
            ctor_count = 0;
            dtor_count = 0;
            ctor_hook = NULL;
            dtor_hook = NULL;
        }

        RAIIMember raii;

        void __constructor__(int i) {
            hooked_ctor_int_count++;
            CITRIX_MOCK_UNREFPARAM(i);
        }

        void __destructor__() {
            hooked_dtor_count++;
        }

        void __constructor_redirect__(int i) {
            HookRecursionGuard guard(*ctor_hook);

            hooked_ctor_int_count++;
            CITRIX_MOCK_UNREFPARAM(i);
            
            new (this) TestClass(i);
        }

        void __destructor_redirect__() {
            HookRecursionGuard guard(*dtor_hook);

            hooked_dtor_count++;

            this->~TestClass();
        }

        TestClass(int i) {
            CITRIX_MOCK_UNREFPARAM(i);
            ctor_int_count++;
        }

        TestClass() {
            ctor_count++;
        }

        ~TestClass() {
            dtor_count++;
        }
    };

    int TestClass::ctor_int_count;
    int TestClass::hooked_ctor_int_count;
    int TestClass::ctor_count;
    int TestClass::dtor_count;
    int TestClass::hooked_dtor_count;
    TypedHook<void(TestClass::*)(int), TestClass>* TestClass::ctor_hook;
    TypedHook<void(TestClass::*)(), TestClass>* TestClass::dtor_hook;

    #define SELF_TEST \
        TestClass::reset();\
        ASSERT_EQ(0, TestClass::ctor_count);\
        ASSERT_EQ(0, TestClass::ctor_int_count);\
        ASSERT_EQ(0, TestClass::dtor_count);\
        ASSERT_EQ(0, TestClass::hooked_ctor_int_count);\
        ASSERT_EQ(0, TestClass::hooked_dtor_count);\
        {\
            TestClass __init;\
            ASSERT_EQ(1, TestClass::ctor_count);\
            ASSERT_EQ(0, TestClass::ctor_int_count);\
            ASSERT_EQ(0, TestClass::dtor_count);\
            ASSERT_EQ(0, TestClass::hooked_ctor_int_count);\
            ASSERT_EQ(0, TestClass::hooked_dtor_count);\
        }\
        ASSERT_EQ(1, TestClass::ctor_count);\
        ASSERT_EQ(0, TestClass::ctor_int_count);\
        ASSERT_EQ(1, TestClass::dtor_count);\
        ASSERT_EQ(0, TestClass::hooked_ctor_int_count);\
        ASSERT_EQ(0, TestClass::hooked_dtor_count);\
        {\
            TestClass __init(123);\
            ASSERT_EQ(1, TestClass::ctor_count);\
            ASSERT_EQ(1, TestClass::ctor_int_count);\
            ASSERT_EQ(1, TestClass::dtor_count);\
            ASSERT_EQ(0, TestClass::hooked_ctor_int_count);\
            ASSERT_EQ(0, TestClass::hooked_dtor_count);\
            __init.__constructor__(123);\
            ASSERT_EQ(1, TestClass::ctor_count);\
            ASSERT_EQ(1, TestClass::ctor_int_count);\
            ASSERT_EQ(1, TestClass::dtor_count);\
            ASSERT_EQ(1, TestClass::hooked_ctor_int_count);\
            ASSERT_EQ(0, TestClass::hooked_dtor_count);\
        }\
        ASSERT_EQ(1, TestClass::ctor_count);\
        ASSERT_EQ(1, TestClass::ctor_int_count);\
        ASSERT_EQ(2, TestClass::dtor_count);\
        ASSERT_EQ(1, TestClass::hooked_ctor_int_count);\
        ASSERT_EQ(0, TestClass::hooked_dtor_count);\
        TestClass::reset();

    TEST(Hooking_CtorDtor, simpleHook) {
        SELF_TEST;

        TypedCtorAndDtor<void(TestClass::*)(int)> pair = CtorAddressDetector::getCtorAndDtor<TestClass>(123);
        {
            TypedHook<void(TestClass::*)(int), TestClass> hook_ctor(pair.ctor, &TestClass::__constructor__);
            TypedHook<void(TestClass::*)(), TestClass> hook_dtor(pair.dtor, &TestClass::__destructor__);

            ASSERT_TRUE(!TestClass::ctor_count && !TestClass::ctor_int_count && !TestClass::dtor_count &&
                !TestClass::hooked_ctor_int_count && !TestClass::hooked_dtor_count && !RAIIMember::balance);

            {
                TestClass test(123);

                ASSERT_TRUE(!TestClass::ctor_count && !TestClass::ctor_int_count && !TestClass::dtor_count && !RAIIMember::balance);
                ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
                ASSERT_EQ(0, TestClass::hooked_dtor_count);
            }

            ASSERT_TRUE(!TestClass::ctor_count && !TestClass::ctor_int_count && !TestClass::dtor_count && !RAIIMember::balance);
            ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
            ASSERT_EQ(1, TestClass::hooked_dtor_count);
        }

        {
            TestClass test(123);

            ASSERT_EQ(0, TestClass::ctor_count);
            ASSERT_EQ(1, TestClass::ctor_int_count);
            ASSERT_EQ(0, TestClass::dtor_count);
            ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
            ASSERT_EQ(1, TestClass::hooked_dtor_count);
            ASSERT_EQ(1, RAIIMember::balance);
        }

        ASSERT_EQ(0, TestClass::ctor_count);
        ASSERT_EQ(1, TestClass::ctor_int_count);
        ASSERT_EQ(1, TestClass::dtor_count);
        ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
        ASSERT_EQ(1, TestClass::hooked_dtor_count);
        ASSERT_EQ(0, RAIIMember::balance);
    }

    TEST(Hooking_CtorDtor, hookAndCallOriginal) {
        SELF_TEST;

        TypedCtorAndDtor<void(TestClass::*)(int)> pair = CtorAddressDetector::getCtorAndDtor<TestClass>(123);
        {
            TypedHook<void(TestClass::*)(int), TestClass> hook_ctor(pair.ctor, &TestClass::__constructor_redirect__);
            TypedHook<void(TestClass::*)(), TestClass> hook_dtor(pair.dtor, &TestClass::__destructor_redirect__);

            TestClass::ctor_hook = &hook_ctor;
            TestClass::dtor_hook = &hook_dtor;

            ASSERT_TRUE(!TestClass::ctor_count && !TestClass::ctor_int_count && !TestClass::dtor_count &&
                !TestClass::hooked_ctor_int_count && !TestClass::hooked_dtor_count);

            {
                TestClass test(123);

                ASSERT_TRUE(!TestClass::ctor_count && !TestClass::dtor_count && !TestClass::hooked_dtor_count);
                ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
                ASSERT_EQ(1, TestClass::ctor_int_count);
                ASSERT_EQ(1, RAIIMember::balance);
            }

            ASSERT_TRUE(!TestClass::ctor_count);
            ASSERT_EQ(1, TestClass::hooked_ctor_int_count);
            ASSERT_EQ(1, TestClass::ctor_int_count);
            ASSERT_EQ(1, TestClass::hooked_dtor_count);
            ASSERT_EQ(1, TestClass::dtor_count);
            ASSERT_EQ(0, RAIIMember::balance);
        }
    }
}}
 
 
