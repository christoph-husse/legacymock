
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


namespace unittests { namespace Hooking_Recursive {

    struct GuineaPig {
        int aCount, aHandlerCount;
        int bCount, bHandlerCount;
        TypedHook<void (GuineaPig::*)()>* hookA;
        TypedHook<void (GuineaPig::*)()>* hookB;

        void aHandler() {
            HookRecursionGuard detector(*hookA);

            aHandlerCount++;    
            b();
        }

        void bHandler() {
            HookRecursionGuard detector(*hookB);

            bHandlerCount++;               
            a();
        }

        void a() {
            aCount++;
        }

        void b() {
            bCount++;
        }

        void setHook(
                TypedHook<void (GuineaPig::*)()>* hookA,
                TypedHook<void (GuineaPig::*)()>* hookB) {
            this->hookA = hookA;
            this->hookB = hookB;
        }

        GuineaPig() :
            aCount(0),
            bCount(0),
            aHandlerCount(0),
            bHandlerCount(0),
            hookA(0),
            hookB(0) {
        }
    };

    // self-test before each test is important, because hooking changes static state.
    #define SELF_TEST                       \
        GuineaPig pig;                      \
                                            \
        ASSERT_EQ(0, pig.aCount);           \
        ASSERT_EQ(0, pig.bCount);           \
        ASSERT_EQ(0, pig.aHandlerCount);    \
        ASSERT_EQ(0, pig.bHandlerCount);    \
                                            \
        pig.a();                            \
                                            \
        ASSERT_EQ(1, pig.aCount);           \
        ASSERT_EQ(0, pig.bCount);           \
        ASSERT_EQ(0, pig.aHandlerCount);    \
        ASSERT_EQ(0, pig.bHandlerCount);    \
                                            \
        pig.b();                            \
                                            \
        ASSERT_EQ(1, pig.aCount);           \
        ASSERT_EQ(1, pig.bCount);           \
        ASSERT_EQ(0, pig.aHandlerCount);    \
        ASSERT_EQ(0, pig.bHandlerCount);

    // does self-test and makes sure hooks for A & B are installed
    #define FIXTURE                                                                 \
        SELF_TEST;                                                                  \
        TypedHook<void (GuineaPig::*)()> hookA(&GuineaPig::a, &GuineaPig::aHandler);       \
        TypedHook<void (GuineaPig::*)()> hookB(&GuineaPig::b, &GuineaPig::bHandler);       \
        pig.setHook(&hookA, &hookB);                                                \
        ASSERT_TRUE(hookA.isInstalled() && hookB.isInstalled());


    TEST(Hooking_Recursive, SelfTest) { 
        SELF_TEST;
    }


    TEST(Hooking_Recursive, recursiveOnA) {
        FIXTURE;

        pig.a();

        ASSERT_EQ(2, pig.aCount);
        ASSERT_EQ(1, pig.bCount);
        ASSERT_EQ(1, pig.aHandlerCount);
        ASSERT_EQ(1, pig.bHandlerCount);
    }

    TEST(Hooking_Recursive, recursiveOnB) {
        FIXTURE;

        pig.b();

        ASSERT_EQ(1, pig.aCount);
        ASSERT_EQ(2, pig.bCount);
        ASSERT_EQ(1, pig.aHandlerCount);
        ASSERT_EQ(1, pig.bHandlerCount);
    }

    TEST(Hooking_Recursive, uninstallA) {
        FIXTURE;

        hookA.uninstall();
        pig.b();

        ASSERT_EQ(2, pig.aCount);
        ASSERT_EQ(1, pig.bCount);
        ASSERT_EQ(0, pig.aHandlerCount);
        ASSERT_EQ(1, pig.bHandlerCount);
    }

    TEST(Hooking_Recursive, uninstallB) {
        FIXTURE;

        hookB.uninstall();
        pig.a();

        ASSERT_EQ(1, pig.aCount);
        ASSERT_EQ(2, pig.bCount);
        ASSERT_EQ(1, pig.aHandlerCount);
        ASSERT_EQ(0, pig.bHandlerCount);
    }
}}


