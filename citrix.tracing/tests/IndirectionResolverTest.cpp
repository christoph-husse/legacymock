
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse
#include "stdafx.h"

#if LEGACYMOCK_SUPPORTS_TRACING

#include "../citrix.tracing/include/citrix/tracing/IndirectionResolver.hpp"
#include "Utils/StringUtils.h"

#include <boost/format.hpp>

using namespace citrix::tracing::internal;

namespace unittests {
    // TODO: Much more tests needed here

    TEST(IndirectionResolverTest, void_) {
        int intArray4[4] = { 1,2,3,4 };

        IndirectionResolver<void*>::toPropertyTree((void*)NULL);
        IndirectionResolver<int[4]>::toPropertyTree(intArray4);
        IndirectionResolver<int(*)[4]>::toPropertyTree(&intArray4);
        IndirectionResolver<std::string>::toPropertyTree("Hello World!");

        // TODO: support unicode
        IndirectionResolver<wchar_t*>::toPropertyTree(L"ب ت ث ج ح خ د ذ ر ز س ش ص ض ط ظ ع غ ف ق ك ل م ن ه و ي");
    }

    int myLoggedFunction() {
        return 123;
    }

    TEST(IndirectionResolverTest, bareFunction) {
        typedef int (Func)();
       
        std::string actual = boostJson(IndirectionResolver<Func>::toPropertyTree(myLoggedFunction));
        std::string expected = (boost::format("{\"pointer\": \"%p\"}") % (&myLoggedFunction)).str();

        ASSERT_EQ(boostJson(expected), actual);
    }

    TEST(IndirectionResolverTest, functionPointer) {
        typedef int (Func)();
       
        std::string actual = boostJson(IndirectionResolver<Func*>::toPropertyTree(&myLoggedFunction));
        std::string expected = (boost::format("{\"pointer\": \"%p\"}") % (&myLoggedFunction)).str();

        ASSERT_EQ(boostJson(expected), actual);
    }

    TEST(IndirectionResolverTest, functionPointerPtr) {
        typedef int (Func)();
       
        Func* value = myLoggedFunction;
        std::string actual = boostJson(IndirectionResolver<Func**>::toPropertyTree(&value));
        std::string expected = (boost::format("{\"pointer\": \"%p\", \"indirection\" : 1}") % (&myLoggedFunction)).str();

        ASSERT_EQ(boostJson(expected), actual);
    }
     
    TEST(IndirectionResolverTest, voidPtr) {
        std::string actual = boostJson(IndirectionResolver<void*>::toPropertyTree((void*)0x123));
        ASSERT_EQ(boostJson("{\"value\": \"00000123\"}"), actual);
    }

    TEST(IndirectionResolverTest, voidPtrPtr) {
        std::string actual = boostJson(IndirectionResolver<void**>::toPropertyTree((void**)0x123));
        ASSERT_EQ(boostJson("{\"pointer\": \"00000123\" }"), actual);
    }

    TEST(IndirectionResolverTest, voidPtrPtr_Indirection1) {
        void* value = (void*)0x123;
        std::string actual = boostJson(IndirectionResolver<void**>::toPropertyTree(&value));
        ASSERT_EQ(boostJson("{\"value\": \"00000123\", \"indirection\" : 1}"), actual);
    }

    TEST(IndirectionResolverTest, voidPtrPtr_Indirection2) {
        void* value = (void*)0x123;
        void** valuePtr = &value;
        std::string actual = boostJson(IndirectionResolver<void***>::toPropertyTree(&valuePtr));
        ASSERT_EQ(boostJson("{\"value\": \"00000123\", \"indirection\" : 2}"), actual);
    }

    TEST(IndirectionResolverTest, voidPtrPtr_Indirection2_Faulty) {
        void** valuePtr = (void**)0x123;
        std::string actual = boostJson(IndirectionResolver<void***>::toPropertyTree(&valuePtr));
        ASSERT_EQ(boostJson("{\"pointer\": \"00000123\", \"indirection\" : 1}"), actual);
    }
}

#endif
