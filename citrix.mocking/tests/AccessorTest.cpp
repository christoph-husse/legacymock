
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/PrivateAndProtectedMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"
#include "citrix\type_traits\member_function_pointer_traits.hpp"

typedef citrix::mocking::GMock G;

using namespace MockMe;

using testing::Return;

namespace unittests {


    TEST(AccessorTest, disablesMockForEachIndividualMember) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;

        ASSERT_EQ(2, G::Accessor<PrivateAndProtected>().myPrivateFunc(mock));
        ASSERT_EQ(1, G::Accessor<PrivateAndProtected>().myProtectedFunc(mock));

        EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-3));
        ASSERT_EQ(-3, G::Accessor<PrivateAndProtected>().disableMockTest_myPrivateFunc(mock));

        EXPECT_CALL(mock, myProtectedFunc()).WillOnce(Return(-4));
        ASSERT_EQ(-4, G::Accessor<PrivateAndProtected>().disableMockTest_myProtectedFunc(mock));
    }
}