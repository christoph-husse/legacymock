
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/PrivateAndProtectedMock.h"
#include "GeneratedMocks/TemplateClassProbingMock.h"
#include "GeneratedMocks/OperatorsMock.h"
#include "GeneratedMocks/TemplateMethodMock.h"
#include "GeneratedMocks/ConditionalCompilationMock.h"
#include "GeneratedMocks/ConditionalCompilation2Mock.h"
#include "GeneratedMocks/MyOtherChoiceMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"

typedef citrix::mocking::GMock G;

using namespace MockMe;

using testing::Return;

namespace unittests {

    TEST(FileProbing, hasCompiledFiles) {
        G::MockFixture fixture;
        G::Mock<TemplateClassProbing> templateClass;
        G::Mock<Operators> opClass;
        G::Mock<TemplateMethod> tempMethClass;
        G::Mock<ConditionalCompilation> conditionalClass;

        G::Accessor<ConditionalCompilation>().method0(conditionalClass);
        G::Accessor<ConditionalCompilation>().method1(conditionalClass);
        G::Accessor<ConditionalCompilation>().method2(conditionalClass);
        G::Accessor<ConditionalCompilation>().method5(conditionalClass);
        G::Accessor<ConditionalCompilation>().method6(conditionalClass);

    }
     
    TEST(PrivateAndProtected, DoesMockProtected) {

        PrivateAndProtected instance;

        ASSERT_EQ(1, PrivateAndProtectedForwarder::forward_myProtectedFunc(instance));

        G::MockFixture fixture; 
        G::LazyMock<PrivateAndProtected> lazy;

        EXPECT_CALL(lazy, myProtectedFunc()).WillOnce(Return(-1));

        ASSERT_EQ(-1, PrivateAndProtectedForwarder::forward_myProtectedFunc(instance));
    }

    TEST(PrivateAndProtected, DoesMockPrivate) {
        PrivateAndProtected instance;

        ASSERT_EQ(2, PrivateAndProtectedForwarder::forward_myPrivateFunc(instance));

        G::MockFixture fixture;
        G::LazyMock<PrivateAndProtected> lazy;

        EXPECT_CALL(lazy, myPrivateFunc()).WillOnce(Return(-2));

        ASSERT_EQ(-2, PrivateAndProtectedForwarder::forward_myPrivateFunc(instance));
    }

    TEST(DisableMemberMock, DoesPassThroughDisabledMember) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-2));

        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_EQ(-2, instance.disableMockTest_myPrivateFunc());
    }

    TEST(DisableMemberMock, DoesPassThroughDoublyDisabledMember) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-2));

        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_EQ(-2, instance.disableMockTest_myPrivateFunc());
    }
    
    TEST(DisableMemberMock, DoesNotPassThroughByDefault) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, disableMockTest_myPrivateFunc()).WillOnce(Return(-7));

        ASSERT_EQ(-7, instance.disableMockTest_myPrivateFunc());
    }

    TEST(DisableMemberMock, DoesNotPassThroughByAfterEnabled) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, disableMockTest_myPrivateFunc()).WillOnce(Return(-7));

        fixture.enableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_EQ(-7, instance.disableMockTest_myPrivateFunc());
    }


    TEST(DisableMemberMock, DoesNotPassThroughDisabledEnabledMember) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, disableMockTest_myPrivateFunc()).WillOnce(Return(-7));

        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        fixture.enableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_EQ(-7, instance.disableMockTest_myPrivateFunc());
    }

    TEST(DisableMemberMock, DoesPassThroughDisabledEnabledDisabledMember) {
        G::MockFixture fixture;
        G::Mock<PrivateAndProtected> mock;
        PrivateAndProtected& instance = mock;

        EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-2));

        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        fixture.enableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_EQ(-2, instance.disableMockTest_myPrivateFunc());
    }

    TEST(DisableMemberMock, isMemberMockEnabled) {
        G::MockFixture fixture;

        ASSERT_TRUE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_FALSE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
        fixture.enableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_TRUE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
        fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_FALSE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
        fixture.enableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
        ASSERT_TRUE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
    }

    TEST(DisableMemberMock, isResetByFixture) {
        {
            G::MockFixture fixture;
            G::Mock<PrivateAndProtected> mock;
            PrivateAndProtected& instance = mock;

            EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-2));
            EXPECT_CALL(mock, disableMockTest_myPrivateFunc()).WillOnce(Return(-4));

            ASSERT_EQ(-4, instance.disableMockTest_myPrivateFunc());

            ASSERT_TRUE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
            fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
            ASSERT_FALSE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));

            ASSERT_EQ(-2, instance.disableMockTest_myPrivateFunc());
        }

        {
            G::MockFixture fixture;
            G::Mock<PrivateAndProtected> mock;
            PrivateAndProtected& instance = mock;

            EXPECT_CALL(mock, myPrivateFunc()).WillOnce(Return(-2));
            EXPECT_CALL(mock, disableMockTest_myPrivateFunc()).WillOnce(Return(-4));

            ASSERT_EQ(-4, instance.disableMockTest_myPrivateFunc());

            ASSERT_TRUE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));
            fixture.disableMemberMock(&PrivateAndProtected::disableMockTest_myPrivateFunc);
            ASSERT_FALSE(fixture.isMemberMockEnabled(&PrivateAndProtected::disableMockTest_myPrivateFunc));

            ASSERT_EQ(-2, instance.disableMockTest_myPrivateFunc());
        }
    }
}