
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
#include "Utils/AssemblerTestUtils.h"

// TODO: check "this" pointer in original function and hook handler


namespace unittests { namespace X86Hook_Simple {

    #define HOOK_SIGNATURE void (GuineaPig::*)()
    
    struct GuineaPig {
        int originalCount;
        int redirectionCount;
        int recallCount;
        TypedHook<HOOK_SIGNATURE>* hook;

        void functionToHook() {
            originalCount++;
        }

        void functionToRedirectTo() {
            redirectionCount++;
        }

        void functionForRecall() {
            HookRecursionGuard guard(*hook);
            recallCount++;

            functionToHook();
        }

        void setHook(TypedHook<HOOK_SIGNATURE>* hook) {
            this->hook = hook;
        }

        GuineaPig() :
            originalCount(0),
            redirectionCount(0),
            recallCount(0),
            hook(0) {
        }
    };

    #define SELF_TEST                           \
        GuineaPig pig;                          \
        ASSERT_EQ(0, pig.originalCount);        \
        ASSERT_EQ(0, pig.redirectionCount);     \
        ASSERT_EQ(0, pig.recallCount);          \
        pig.functionToHook();                   \
        ASSERT_EQ(1, pig.originalCount);        \
        ASSERT_EQ(0, pig.redirectionCount);     \
        ASSERT_EQ(0, pig.recallCount);          \
        pig.functionToRedirectTo();             \
        ASSERT_EQ(1, pig.originalCount);        \
        ASSERT_EQ(1, pig.redirectionCount);     \
        ASSERT_EQ(0, pig.recallCount);          



    TEST(X86Hook_Simple, SelfTest) {
        SELF_TEST;
    }
    
    struct HookTest {
        static int counter;
        
        static void staticTest() { counter++; }
        void run() { counter++; }
    };
    
    int HookTest::counter = 0;
    
    unsigned char* getCallTarget(unsigned char* startOfAsm, unsigned char* endOfAsm) {
        std::vector<unsigned char*> calls = X86Utils::collectFunctionCalls(startOfAsm, endOfAsm);
        
        if(!calls.empty())
            return calls.front();
        
        return NULL;
    }
    
    typedef void (*DAction)();
    typedef void (HookTest::*DMemberAction)();
    
    TEST(X86Hook_Simple, makeCodeWritable) {
        unsigned char* link = (unsigned char*)&HookTest::staticTest;
        unsigned char* ep = X86Utils::followJumps(link);
        ASSERT_TRUE(X86Utils::makeMemoryExecutable(ep, 8));
        *((int64_t*)ep) = *((int64_t*)ep);
    }
    
    TEST(X86Hook_Simple, hasUnambigousStaticMethods) {
        HookTest::counter = 0;
        unsigned char* startOfAsm = X86Utils::getInstructionPointer();
        HookTest::staticTest();
        unsigned char* endOfAsm = X86Utils::getInstructionPointer();
        unsigned char* target = getCallTarget(startOfAsm, endOfAsm);
        unsigned char* link = (unsigned char*)&HookTest::staticTest;
        
        ASSERT_EQ(1, HookTest::counter);
        ((DAction)&HookTest::staticTest)();
        ASSERT_EQ(2, HookTest::counter);
        (*((DAction*)&link))();
        ASSERT_EQ(3, HookTest::counter);
        (*((DAction*)&target))();
        ASSERT_EQ(4, HookTest::counter);
    
        
        ASSERT_EQ(X86Utils::followJumps(target), X86Utils::followJumps(link));
        
        ASSERT_EQ(
            AssemblerX86::toString(X86Utils::followJumps(target)),
            AssemblerX86::toString(X86Utils::followJumps(link)));
    }
    
    
    TEST(X86Hook_Simple, hasUnambigousMemberMethods) {
        HookTest ht;
        HookTest::counter = 0;
        unsigned char* startOfAsm = X86Utils::getInstructionPointer();
        ht.run();
        unsigned char* endOfAsm = X86Utils::getInstructionPointer();
        unsigned char* target = getCallTarget(startOfAsm, endOfAsm);
        unsigned char* link = forceCast<unsigned char*>(&HookTest::run);
        
        ASSERT_EQ(1, HookTest::counter);
        (ht.*forceCast<DMemberAction>(&HookTest::run))();
        ASSERT_EQ(2, HookTest::counter);
        (ht.*forceCast<DMemberAction>(link))();
        ASSERT_EQ(3, HookTest::counter);
        (ht.*forceCast<DMemberAction>(target))();
        ASSERT_EQ(4, HookTest::counter);
        
        ASSERT_EQ(X86Utils::followJumps(target), X86Utils::followJumps(link));
        
        ASSERT_EQ(
            AssemblerX86::toString(X86Utils::followJumps(target)),
            AssemblerX86::toString(X86Utils::followJumps(link)));
    }

    TEST(X86Hook_Simple, doesRedirect) {
        SELF_TEST;

        {
            std::string expected = AssemblerX86::toString(forceCast<unsigned char*>(&GuineaPig::functionToHook));
            
            TypedHook<HOOK_SIGNATURE> hook(&GuineaPig::functionToHook, &GuineaPig::functionToRedirectTo);
            if(!hook.isInstalled()) {
                ASSERT_EQ(expected, "Hooks could not be installed. Entry point bytes follow: ");
            }
            
            pig.functionToHook();

            ASSERT_EQ(1, pig.originalCount);
            ASSERT_EQ(2, pig.redirectionCount);
        }

        pig.functionToHook();
        ASSERT_EQ(2, pig.originalCount);
        ASSERT_EQ(2, pig.redirectionCount);
    }

    TEST(X86Hook_Simple, canCallOriginal) {
        SELF_TEST;

        TypedHook<HOOK_SIGNATURE> hook(&GuineaPig::functionToHook, &GuineaPig::functionForRecall);
        pig.setHook(&hook);
        ASSERT_TRUE(hook.isInstalled());

        pig.functionToHook();
        ASSERT_EQ(2, pig.originalCount);
        ASSERT_EQ(1, pig.recallCount);
    }


    TEST(X86Hook_Simple, canCallOriginalJmpBypass) {
        SELF_TEST;

        TypedHook<HOOK_SIGNATURE> hook(
            forceCast<HOOK_SIGNATURE>(X86Utils::followJumps(forceCast<unsigned char*>(&GuineaPig::functionToHook))),
            &GuineaPig::functionForRecall);
        pig.setHook(&hook);
        ASSERT_TRUE(hook.isInstalled());

        pig.functionToHook();
        ASSERT_EQ(2, pig.originalCount);
        ASSERT_EQ(1, pig.recallCount);
    }
}}
