
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/FreeApiHookMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"

typedef citrix::mocking::GMock G;

using testing::Return;

int myFreeTestFunction(int a) { 
    return a * 7; 
}

namespace unittests {
    // TODO: tracing

    TEST(FreeApiHook, mocksOnlyInsideFixture) {
        ASSERT_EQ(123 * 7, myFreeTestFunction(123));
        ASSERT_EQ(123 * 3, FreeApiHook::myFreeTestFunction(123));

        {
            G::MockFixture fixture;
            EXPECT_CALL(G::StaticMock<FreeApiHook>(), myFreeTestFunction(123))
                .WillOnce(Return(-14));

            ASSERT_EQ(123 * 3, FreeApiHook::myFreeTestFunction(123));
            ASSERT_EQ(-14, myFreeTestFunction(123));
        }

        ASSERT_EQ(123 * 3, FreeApiHook::myFreeTestFunction(123));
        ASSERT_EQ(123 * 7, myFreeTestFunction(123));
    }

    TEST(FreeApiHook, allowsAccessorPassThrough) {
        G::MockFixture fixture;

        EXPECT_CALL(G::StaticMock<FreeApiHook>(), myFreeTestFunction(123))
            .Times(2).WillRepeatedly(Return(-14));

        ASSERT_EQ(-14, myFreeTestFunction(123));
        ASSERT_EQ(123 * 7, G::Accessor<FreeApiHook>::myFreeTestFunction(123));
        ASSERT_EQ(-14, myFreeTestFunction(123));
    }

    TEST(FreeApiHook, mocksOperatingSystemApi) {

        SECURITY_DESCRIPTOR secDesc;
        BOOL isPresent;

        ASSERT_EQ(0, ::GetSecurityDescriptorDacl(&secDesc, &isPresent, NULL, &isPresent));

        {
            G::MockFixture fixture;
            const PSECURITY_DESCRIPTOR pSecurityDescriptor = (PSECURITY_DESCRIPTOR)0x12;
            const LPBOOL lpbDaclPresent = (LPBOOL)0x77;
            PACL* const pDacl = (PACL*)0x15;
            const LPBOOL lpbDaclDefaulted = (LPBOOL)0x08;

            EXPECT_CALL(G::StaticMock<FreeApiHook>(), GetSecurityDescriptorDacl(
                    pSecurityDescriptor, lpbDaclPresent, pDacl, lpbDaclDefaulted))
                .WillOnce(Return(126));

            ASSERT_EQ(126, ::GetSecurityDescriptorDacl(pSecurityDescriptor, lpbDaclPresent, pDacl, lpbDaclDefaulted));
        }

        ASSERT_EQ(0, ::GetSecurityDescriptorDacl(&secDesc, &isPresent, NULL, &isPresent));
    }
}