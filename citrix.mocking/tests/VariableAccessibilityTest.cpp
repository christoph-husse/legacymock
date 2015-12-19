
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/VariableAccessibilityMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"

typedef citrix::mocking::GMock G;

using namespace MockMe;

int VariableAccessibility::S_publicInt;
const MyOwnStruct VariableAccessibility::S_publicOwnStruct__const;

std::string VariableAccessibility::S_protectedString;
MyOwnStruct S_protectedOwnStruct__Ref_BackingValue;
MyOwnStruct& VariableAccessibility::S_protectedOwnStruct__Ref = S_protectedOwnStruct__Ref_BackingValue;
MyOwnStruct S_protectedOwnStruct__const_Ref_BackingValue;
const MyOwnStruct& VariableAccessibility::S_protectedOwnStruct__const_Ref = S_protectedOwnStruct__const_Ref_BackingValue;

int VariableAccessibility::S_privateInt;
MyOwnStruct VariableAccessibility::S_privateOwnStruct;
MyOwnStruct* VariableAccessibility::S_privateOwnStruct__Ptr = NULL;
const MyOwnStruct* VariableAccessibility::S_privateOwnStruct__const_Ptr = NULL;
const MyOwnStruct* const VariableAccessibility::S_privateOwnStruct__const_Ptr_const = NULL;

int VariableAccessibility::S_arrayTest1[8] = {0,1,2,3,4,5,6,7};
int VariableAccessibility::S_arrayTest2[] = {0,1,2,3,4,5,6,7};
int VariableAccessibility::S_privArrayTest1[8] = {0,1,2,3,4,5,6,7};
int VariableAccessibility::S_privArrayTest2[] = {0,1,2,3,4,5,6,7};

namespace unittests { namespace MockVariableTest {

    class VariableAccessibilityTest : public ::testing::Test {
    public:
        int& S_publicInt() { return VariableAccessibility::S_publicInt; }
        const MyOwnStruct& S_publicOwnStruct__const() { return VariableAccessibility::S_publicOwnStruct__const; }
        std::string& S_protectedString() { return VariableAccessibility::S_protectedString; }
        MyOwnStruct& S_protectedOwnStruct__Ref() { return VariableAccessibility::S_protectedOwnStruct__Ref; }
        const MyOwnStruct& S_protectedOwnStruct__const_Ref() { return VariableAccessibility::S_protectedOwnStruct__const_Ref; }
        int& S_privateInt() { return VariableAccessibility::S_privateInt; }
        MyOwnStruct& S_privateOwnStruct() { return VariableAccessibility::S_privateOwnStruct; }
        MyOwnStruct*& S_privateOwnStruct__Ptr() { return VariableAccessibility::S_privateOwnStruct__Ptr; }
        const MyOwnStruct*& S_privateOwnStruct__const_Ptr() { return VariableAccessibility::S_privateOwnStruct__const_Ptr; }
        const MyOwnStruct* const& S_privateOwnStruct__const_Ptr_const () { return VariableAccessibility::S_privateOwnStruct__const_Ptr_const; }

        int& publicInt(VariableAccessibility* instance) { return instance->publicInt; }
        const MyOwnStruct& publicOwnStruct__const(VariableAccessibility* instance) { return instance->publicOwnStruct__const; }
        std::string& protectedString(VariableAccessibility* instance) { return instance->protectedString; }
        MyOwnStruct& protectedOwnStruct__Ref(VariableAccessibility* instance) { return instance->protectedOwnStruct__Ref; }
        const MyOwnStruct& protectedOwnStruct__const_Ref(VariableAccessibility* instance) { return instance->protectedOwnStruct__const_Ref; }
        int& privateInt(VariableAccessibility* instance) { return instance->privateInt; }
        MyOwnStruct& privateOwnStruct(VariableAccessibility* instance) { return instance->privateOwnStruct; }
        MyOwnStruct*& privateOwnStruct__Ptr(VariableAccessibility* instance) { return instance->privateOwnStruct__Ptr; }
        const MyOwnStruct*& privateOwnStruct__const_Ptr(VariableAccessibility* instance) { return instance->privateOwnStruct__const_Ptr; }
        const MyOwnStruct* const& privateOwnStruct__const_Ptr_const (VariableAccessibility* instance) { return instance->privateOwnStruct__const_Ptr_const; }
    };

    TEST_F(VariableAccessibilityTest, S_privateOwnStruct) {

        ASSERT_EQ(MyOwnStruct(), S_privateOwnStruct());
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::S_privateOwnStruct());

        MyOwnStruct::internal_copy_accessor(
            &G::Accessor<VariableAccessibility>::S_privateOwnStruct(), &MyOwnStruct(true));

        ASSERT_EQ(MyOwnStruct(true), S_privateOwnStruct());
        ASSERT_EQ(MyOwnStruct(true), G::Accessor<VariableAccessibility>::S_privateOwnStruct());
    }

    TEST_F(VariableAccessibilityTest, privateOwnStruct) {
        VariableAccessibility test;

        ASSERT_EQ(MyOwnStruct(), privateOwnStruct(&test));
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::privateOwnStruct(&test));

        MyOwnStruct::internal_copy_accessor(
            &G::Accessor<VariableAccessibility>::privateOwnStruct(&test), &MyOwnStruct(true));

        ASSERT_EQ(MyOwnStruct(true), privateOwnStruct(&test));
        ASSERT_EQ(MyOwnStruct(true), G::Accessor<VariableAccessibility>::privateOwnStruct(&test));
    }

    TEST_F(VariableAccessibilityTest, S_protectedOwnStruct__Ref) {
        ASSERT_EQ(MyOwnStruct(), S_protectedOwnStruct__Ref());
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::S_protectedOwnStruct__Ref());

        MyOwnStruct::internal_copy_accessor(&G::Accessor<VariableAccessibility>::S_protectedOwnStruct__Ref(), &MyOwnStruct(true));
        MyOwnStruct::internal_copy_accessor(&S_protectedOwnStruct__Ref(), &MyOwnStruct(true));

        ASSERT_EQ(MyOwnStruct(true), S_protectedOwnStruct__Ref());
        ASSERT_EQ(MyOwnStruct(true), G::Accessor<VariableAccessibility>::S_protectedOwnStruct__Ref());
    }

    TEST_F(VariableAccessibilityTest, protectedOwnStruct__Ref) {
        VariableAccessibility test;

        ASSERT_NE(MyOwnStruct(true), protectedOwnStruct__Ref(&test));
        ASSERT_NE(MyOwnStruct(true), G::Accessor<VariableAccessibility>::protectedOwnStruct__Ref(&test));

        MyOwnStruct::internal_copy_accessor(
            &G::Accessor<VariableAccessibility>::protectedOwnStruct__Ref(&test), &MyOwnStruct(true));

        ASSERT_EQ(MyOwnStruct(true), protectedOwnStruct__Ref(&test));
        ASSERT_EQ(MyOwnStruct(true), G::Accessor<VariableAccessibility>::protectedOwnStruct__Ref(&test));
    }

    TEST_F(VariableAccessibilityTest, S_protectedOwnStruct__const_Ref) {

        ASSERT_EQ(MyOwnStruct(), S_protectedOwnStruct__const_Ref());
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::S_protectedOwnStruct__const_Ref());
    }

    TEST_F(VariableAccessibilityTest, protectedOwnStruct__const_Ref) {
        VariableAccessibility test;

        ASSERT_EQ(MyOwnStruct(), protectedOwnStruct__const_Ref(&test));
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::protectedOwnStruct__const_Ref(&test));
    }

    TEST_F(VariableAccessibilityTest, S_privateOwnStruct__Ptr) {
        MyOwnStruct updated;

        ASSERT_NE(&updated, S_privateOwnStruct__Ptr());
        ASSERT_NE(&updated, G::Accessor<VariableAccessibility>::S_privateOwnStruct__Ptr());

        G::Accessor<VariableAccessibility>::S_privateOwnStruct__Ptr() = &updated;

        ASSERT_EQ(&updated, S_privateOwnStruct__Ptr());
        ASSERT_EQ(&updated, G::Accessor<VariableAccessibility>::S_privateOwnStruct__Ptr());
    }

    TEST_F(VariableAccessibilityTest, privateOwnStruct__Ptr) {
        VariableAccessibility test;
        MyOwnStruct updated;

        ASSERT_NE(&updated, privateOwnStruct__Ptr(&test));
        ASSERT_NE(&updated, G::Accessor<VariableAccessibility>::privateOwnStruct__Ptr(&test));

        G::Accessor<VariableAccessibility>::privateOwnStruct__Ptr(&test) = &updated;

        ASSERT_EQ(&updated, privateOwnStruct__Ptr(&test));
        ASSERT_EQ(&updated, G::Accessor<VariableAccessibility>::privateOwnStruct__Ptr(&test));
    }

    TEST_F(VariableAccessibilityTest, S_privateOwnStruct__const_Ptr) {
        MyOwnStruct updated;

        ASSERT_NE(&updated, S_privateOwnStruct__const_Ptr());
        ASSERT_NE(&updated, G::Accessor<VariableAccessibility>::S_privateOwnStruct__const_Ptr());

        G::Accessor<VariableAccessibility>::S_privateOwnStruct__const_Ptr() = &updated;

        ASSERT_EQ(&updated, S_privateOwnStruct__const_Ptr());
        ASSERT_EQ(&updated, G::Accessor<VariableAccessibility>::S_privateOwnStruct__const_Ptr());
    }

    TEST_F(VariableAccessibilityTest, privateOwnStruct__const_Ptr) {
        VariableAccessibility test;
        MyOwnStruct updated;

        ASSERT_NE(&updated, privateOwnStruct__const_Ptr(&test));
        ASSERT_NE(&updated, G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr(&test));

        G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr(&test) = &updated;

        ASSERT_EQ(&updated, privateOwnStruct__const_Ptr(&test));
        ASSERT_EQ(&updated, G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr(&test));
    }

    TEST_F(VariableAccessibilityTest, S_protectedString) {
        ASSERT_NE("Hello World!", S_protectedString());
        ASSERT_NE("Hello World!", G::Accessor<VariableAccessibility>::S_protectedString());

        G::Accessor<VariableAccessibility>::S_protectedString() = "Hello World!";

        ASSERT_EQ("Hello World!", S_protectedString());
        ASSERT_EQ("Hello World!", G::Accessor<VariableAccessibility>::S_protectedString());
    }

    TEST_F(VariableAccessibilityTest, protectedString) {
        VariableAccessibility test;

        ASSERT_NE("Hello World!", protectedString(&test));
        ASSERT_NE("Hello World!", G::Accessor<VariableAccessibility>::protectedString(&test));

        G::Accessor<VariableAccessibility>::protectedString(&test) = "Hello World!";

        ASSERT_EQ("Hello World!", protectedString(&test));
        ASSERT_EQ("Hello World!", G::Accessor<VariableAccessibility>::protectedString(&test));
    }

    TEST_F(VariableAccessibilityTest, privateOwnStruct__const_Ptr_const) {
        VariableAccessibility test;
        MyOwnStruct update;

        ASSERT_NE(&update, privateOwnStruct__const_Ptr_const(&test));
        ASSERT_NE(&update, G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr_const(&test));

        const_cast<MyOwnStruct*&>(G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr_const(&test)) = &update;
         
        ASSERT_EQ(&update, privateOwnStruct__const_Ptr_const(&test));
        ASSERT_EQ(&update, G::Accessor<VariableAccessibility>::privateOwnStruct__const_Ptr_const(&test));
    }

    TEST_F(VariableAccessibilityTest, S_publicOwnStruct__const) {

        ASSERT_EQ(MyOwnStruct(), S_publicOwnStruct__const());
        ASSERT_EQ(MyOwnStruct(), G::Accessor<VariableAccessibility>::S_publicOwnStruct__const());
    }

    TEST_F(VariableAccessibilityTest, publicOwnStruct__const) {
        VariableAccessibility test;

        ASSERT_NE(MyOwnStruct(true), publicOwnStruct__const(&test));
        ASSERT_NE(MyOwnStruct(true), G::Accessor<VariableAccessibility>::publicOwnStruct__const(&test));

        MyOwnStruct::internal_copy_accessor(
            const_cast<MyOwnStruct*>(&G::Accessor<VariableAccessibility>::publicOwnStruct__const(&test)),
            &MyOwnStruct(true));

        ASSERT_EQ(MyOwnStruct(true), publicOwnStruct__const(&test));
        ASSERT_EQ(MyOwnStruct(true), G::Accessor<VariableAccessibility>::publicOwnStruct__const(&test));
    }

    TEST_F(VariableAccessibilityTest, S_publicInt) {
        ASSERT_NE(123, S_publicInt());
        ASSERT_NE(123, G::Accessor<VariableAccessibility>::S_publicInt());

        G::Accessor<VariableAccessibility>::S_publicInt() = 123;

        ASSERT_EQ(123, S_publicInt());
        ASSERT_EQ(123, G::Accessor<VariableAccessibility>::S_publicInt());
    }

    TEST_F(VariableAccessibilityTest, publicInt) {
        VariableAccessibility test;

        ASSERT_NE(123, publicInt(&test));
        ASSERT_NE(123, G::Accessor<VariableAccessibility>::publicInt(&test));

        G::Accessor<VariableAccessibility>::publicInt(&test) = 123;

        ASSERT_EQ(123, publicInt(&test));
        ASSERT_EQ(123, G::Accessor<VariableAccessibility>::publicInt(&test));
    }

    TEST_F(VariableAccessibilityTest, S_privateInt) {
        ASSERT_NE(123, S_privateInt());
        ASSERT_NE(123, G::Accessor<VariableAccessibility>::S_privateInt());

        G::Accessor<VariableAccessibility>::S_privateInt() = 123;

        ASSERT_EQ(123, S_privateInt());
        ASSERT_EQ(123, G::Accessor<VariableAccessibility>::S_privateInt());
    }

    TEST_F(VariableAccessibilityTest, privateInt) {
        VariableAccessibility test;

        ASSERT_NE(123, privateInt(&test));
        ASSERT_NE(123, G::Accessor<VariableAccessibility>::privateInt(&test));

        G::Accessor<VariableAccessibility>::privateInt(&test) = 123;

        ASSERT_EQ(123, privateInt(&test));
        ASSERT_EQ(123, G::Accessor<VariableAccessibility>::privateInt(&test));
    }

    TEST_F(VariableAccessibilityTest, unknownTest) {
        VariableAccessibility test;

        ASSERT_NE((_UNKNOWN_STRUCT_*)123, test.unknownTest);
        ASSERT_NE((_UNKNOWN_STRUCT_*)123, G::Accessor<VariableAccessibility>::unknownTest(&test));

        G::Accessor<VariableAccessibility>::unknownTest(&test) = (_UNKNOWN_STRUCT_*)123;

        ASSERT_EQ((_UNKNOWN_STRUCT_*)123, test.unknownTest);
        ASSERT_EQ((_UNKNOWN_STRUCT_*)123, G::Accessor<VariableAccessibility>::unknownTest(&test));
    }

    TEST_F(VariableAccessibilityTest, arrayTest) {
        VariableAccessibility test;

        for(int i = 0; i < 8; i++) {
            ASSERT_NE(133 + i, test.arrayTest1[i]);
            ASSERT_NE(233 + i, test.S_arrayTest1[i]);
            ASSERT_NE(333 + i, test.S_arrayTest2[i]);

            ASSERT_NE(133 + i, (&G::Accessor<VariableAccessibility>::arrayTest1(&test))[i]);
            ASSERT_NE(233 + i, (&G::Accessor<VariableAccessibility>::S_arrayTest1())[i]);
            ASSERT_NE(333 + i, (&G::Accessor<VariableAccessibility>::S_arrayTest2())[i]);

            (&G::Accessor<VariableAccessibility>::arrayTest1(&test))[i] = 133 + i;
            (&G::Accessor<VariableAccessibility>::S_arrayTest1())[i] = 233 + i;
            (&G::Accessor<VariableAccessibility>::S_arrayTest2())[i] = 333 + i;

            ASSERT_EQ(133 + i, test.arrayTest1[i]);
            ASSERT_EQ(233 + i, test.S_arrayTest1[i]);
            ASSERT_EQ(333 + i, test.S_arrayTest2[i]);

            ASSERT_EQ(133 + i, (&G::Accessor<VariableAccessibility>::arrayTest1(&test))[i]);
            ASSERT_EQ(233 + i, (&G::Accessor<VariableAccessibility>::S_arrayTest1())[i]);
            ASSERT_EQ(333 + i, (&G::Accessor<VariableAccessibility>::S_arrayTest2())[i]);
        }

        for(int i = 0; i < 8; i++) {
            ASSERT_EQ(133 + i, test.arrayTest1[i]);
            ASSERT_EQ(233 + i, test.S_arrayTest1[i]);
            ASSERT_EQ(333 + i, test.S_arrayTest2[i]);
        }
    }
}}