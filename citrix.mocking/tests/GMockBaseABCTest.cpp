
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/BaseABCMock.h"
#include "GeneratedMocks/BaseAMock.h"
#include "GeneratedMocks/BaseBMock.h"
#include "GeneratedMocks/BaseCMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"

typedef citrix::mocking::GMock G;

using namespace MockMe;

using testing::Return;

namespace unittests {

    TEST(BaseA, DoFundamentalsWork) {
        // this should work like normal c++
        {
			MockMe::internal::BaseA_ProbingInstance probing;

            ASSERT_EQ(BaseA::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * BaseA::member_MAGIC * BaseA::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * BaseA::member_MAGIC * BaseA::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
        }

        // this will be hooked and rerouted into GMock
        {
            G::MockFixture fixture;
            G::Mock<BaseA> mock;

            EXPECT_CALL(mock, inAOverriddenByB(BaseA::inAOverriddenByB_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-1));

            EXPECT_CALL(mock, inAOverriddenByBABC(BaseA::inAOverriddenByBABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-2));

            EXPECT_CALL(mock, getMember())
                .Times(1)
                .WillRepeatedly(Return(-13));

            BaseA& instance = mock;

            ASSERT_EQ(-13, instance.getMember());
            ASSERT_EQ(-1, instance.inAOverriddenByB(BaseA::inAOverriddenByB_MAGIC));
            ASSERT_EQ(-2, instance.inAOverriddenByBABC(BaseA::inAOverriddenByBABC_MAGIC));
        }

        // this should work like normal c++ (hooks are still installed, but will just pass-through)
        {
            MockMe::internal::BaseA_ProbingInstance probing;

            ASSERT_EQ(BaseA::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * BaseA::member_MAGIC * BaseA::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * BaseA::member_MAGIC * BaseA::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
        }
    }

    TEST(BaseB, DoFundamentalsWork) {
        // this should work like normal c++
        {
            MockMe::internal::BaseB_ProbingInstance probing;

            ASSERT_EQ(BaseB::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * probing.member * BaseB::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * probing.member * BaseB::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
        }

        // this will be hooked and rerouted into GMock
        {
            G::MockFixture fixture;
            G::Mock<BaseB> mock;

            EXPECT_CALL(mock, inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-3));

            EXPECT_CALL(mock, inAOverriddenByBABC(BaseB::inAOverriddenByBABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-4));

            EXPECT_CALL(mock, getMember())
                .Times(1)
                .WillRepeatedly(Return(-13));

            BaseB& instance = mock;

            ASSERT_EQ(-13, instance.getMember());
            ASSERT_EQ(-3, instance.inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC));
            ASSERT_EQ(-4, instance.inAOverriddenByBABC(BaseB::inAOverriddenByBABC_MAGIC));
        }

        // this should work like normal c++ (hooks are still installed, but will just pass-through)
        {
            MockMe::internal::BaseB_ProbingInstance probing;

            ASSERT_EQ(BaseB::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * probing.member * BaseB::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * probing.member * BaseB::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
        }
    }

    TEST(BaseC, DoFundamentalsWork) {
        // this should work like normal c++
        {
            BaseC probing;

            ASSERT_EQ(BaseC::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * ((BaseA&)probing).member * BaseC::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * ((BaseA&)probing).member * BaseC::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
            ASSERT_EQ(5 * BaseC::member_MAGIC * BaseC::inCImplementsAOverriddenByABC_MAGIC, probing.inCImplementsAOverriddenByABC(5));
        }

        // this will be hooked and rerouted into GMock
        {
            G::MockFixture fixture;
            G::Mock<BaseC> mockC;
            G::LazyMock<BaseA> lazyA; // we need a lazy mock here, because BaseC does not overwrite the methods

            EXPECT_CALL(lazyA, inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-5));

            EXPECT_CALL(lazyA, inAOverriddenByBABC(BaseC::inAOverriddenByBABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-6));

            EXPECT_CALL(mockC, inCImplementsAOverriddenByABC(BaseC::inCImplementsAOverriddenByABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-7));

            EXPECT_CALL(mockC, getMember())
                .Times(1)
                .WillRepeatedly(Return(-21));

            BaseC& instance = mockC;

            ASSERT_EQ(-21, instance.getMember());
            ASSERT_EQ(-5, instance.inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC));
            ASSERT_EQ(-6, instance.inAOverriddenByBABC(BaseC::inAOverriddenByBABC_MAGIC));
            ASSERT_EQ(-7, instance.inCImplementsAOverriddenByABC(BaseC::inCImplementsAOverriddenByABC_MAGIC));
        }

        // this should work like normal c++ (hooks are still installed, but will just pass-through)
        {
            BaseC probing;

            ASSERT_EQ(BaseC::member_MAGIC, probing.getMember());
            ASSERT_EQ(3 * ((BaseA&)probing).member * BaseC::inAOverriddenByB_MAGIC, probing.inAOverriddenByB(3));
            ASSERT_EQ(4 * ((BaseA&)probing).member * BaseC::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
            ASSERT_EQ(5 * BaseC::member_MAGIC * BaseC::inCImplementsAOverriddenByABC_MAGIC, probing.inCImplementsAOverriddenByABC(5));
        }
    }

    TEST(BaseABC, DoFundamentalsWork) {
        // this should work like normal c++
        {
            BaseABC probing;

            ASSERT_EQ(BaseABC::member_MAGIC, probing.getMember());

            // this will test if "this" pointer's are shifted correctly is mock-disabled code paths
            ASSERT_EQ(3 * ((BaseA&)(BaseC&)probing).member * BaseA::inAOverriddenByB_MAGIC, ((BaseC&)probing).inAOverriddenByB(3));
            ASSERT_EQ(3 * ((BaseB&)probing).member * BaseB::inAOverriddenByB_MAGIC, ((BaseB&)probing).inAOverriddenByB(3));

            ASSERT_EQ(4 * BaseABC::member_MAGIC * BaseABC::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
            ASSERT_EQ(5 * BaseABC::member_MAGIC * BaseABC::inCImplementsAOverriddenByABC_MAGIC, probing.inCImplementsAOverriddenByABC(5));
        }

        // this will be hooked and rerouted into GMock
        {
            G::MockFixture fixture;
            G::Mock<BaseABC> mockC;
            G::LazyMock<BaseA> lazyA; // we need a lazy mock here, because BaseC does not overwrite the methods
            G::LazyMock<BaseB> lazyB;

            EXPECT_CALL(lazyA, inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-5));

            EXPECT_CALL(lazyB, inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-99));

            EXPECT_CALL(mockC, inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-6));

            EXPECT_CALL(mockC, inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC))
                .Times(1)
                .WillRepeatedly(Return(-7));

            EXPECT_CALL(mockC, getMember())
                .Times(1)
                .WillRepeatedly(Return(-21));

            BaseABC& instance = mockC;

            ASSERT_EQ(-21, instance.getMember());
            ASSERT_EQ(-5, ((BaseC&)instance).inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC));
            ASSERT_EQ(-99, ((BaseB&)instance).inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC));
            ASSERT_EQ(-6, instance.inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC));
            ASSERT_EQ(-7, instance.inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC));
        }

        // this should work like normal c++ (hooks are still installed, but will just pass-through)
        {
            BaseABC probing;

            ASSERT_EQ(BaseABC::member_MAGIC, probing.getMember());

            // this will test if "this" pointer's are shifted correctly is mock-disabled code paths
            ASSERT_EQ(3 * ((BaseA&)(BaseC&)probing).member * BaseA::inAOverriddenByB_MAGIC, ((BaseC&)probing).inAOverriddenByB(3));
            ASSERT_EQ(3 * ((BaseB&)probing).member * BaseB::inAOverriddenByB_MAGIC, ((BaseB&)probing).inAOverriddenByB(3));

            ASSERT_EQ(4 * BaseABC::member_MAGIC * BaseABC::inAOverriddenByBABC_MAGIC, probing.inAOverriddenByBABC(4));
            ASSERT_EQ(5 * BaseABC::member_MAGIC * BaseABC::inCImplementsAOverriddenByABC_MAGIC, probing.inCImplementsAOverriddenByABC(5));
        }
    }

    TEST(BaseB, DoesHookThrough_BaseA) {
        G::MockFixture fixture;
        G::Mock<BaseB> mock;

        EXPECT_CALL(mock, inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-3));

        EXPECT_CALL(mock, inAOverriddenByBABC(BaseB::inAOverriddenByBABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-4));

        EXPECT_CALL(mock, getMember())
            .Times(1)
            .WillRepeatedly(Return(-19));

        BaseA& instance = mock;

        ASSERT_EQ(-19, instance.getMember());
        ASSERT_EQ(-3, instance.inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC));
        ASSERT_EQ(-4, instance.inAOverriddenByBABC(BaseB::inAOverriddenByBABC_MAGIC));
    }

    TEST(BaseC, DoesHookThrough_BaseA) {
        G::MockFixture fixture;
        G::Mock<BaseC> mockC;
        G::LazyMock<BaseA> lazyA;

        EXPECT_CALL(lazyA, inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-5));

        EXPECT_CALL(lazyA, inAOverriddenByBABC(BaseC::inAOverriddenByBABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-6));

        EXPECT_CALL(mockC, inCImplementsAOverriddenByABC(BaseC::inCImplementsAOverriddenByABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-7));

        EXPECT_CALL(mockC, getMember())
            .Times(1)
            .WillRepeatedly(Return(-17));

        BaseA& instance = mockC;

        ASSERT_EQ(-17, instance.getMember());
        ASSERT_EQ(-5, instance.inAOverriddenByB(BaseC::inAOverriddenByB_MAGIC));
        ASSERT_EQ(-6, instance.inAOverriddenByBABC(BaseC::inAOverriddenByBABC_MAGIC));
        ASSERT_EQ(-7, instance.inCImplementsAOverriddenByABC(BaseC::inCImplementsAOverriddenByABC_MAGIC));
    }

    TEST(BaseABC, DoesHookThrough_BaseA_of_BaseB) {
        G::MockFixture fixture;
        G::Mock<BaseABC> mock;
        G::LazyMock<BaseB> lazyB;

        EXPECT_CALL(lazyB, inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-99));

        EXPECT_CALL(mock, inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-6));

        EXPECT_CALL(mock, inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-7));

        EXPECT_CALL(mock, getMember())
            .Times(1)
            .WillRepeatedly(Return(-21));

        BaseA& instance = (BaseB&)mock;

        ASSERT_EQ(-21, instance.getMember());
        ASSERT_EQ(-99, instance.inAOverriddenByB(BaseB::inAOverriddenByB_MAGIC));
        ASSERT_EQ(-6, instance.inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC));
        ASSERT_EQ(-7, instance.inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC));
    }

    TEST(BaseABC, DoesHookThrough_BaseA_of_BaseC) {
        G::MockFixture fixture;
        G::Mock<BaseABC> mock;
        G::LazyMock<BaseA> lazyA;

        EXPECT_CALL(lazyA, inAOverriddenByB(BaseA::inAOverriddenByB_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-23));

        EXPECT_CALL(mock, inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-6));

        EXPECT_CALL(mock, inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC))
            .Times(1)
            .WillRepeatedly(Return(-7));

        EXPECT_CALL(mock, getMember())
            .Times(1)
            .WillRepeatedly(Return(-21));

        BaseA& instance = (BaseC&)mock;

        ASSERT_EQ(-21, instance.getMember());
        ASSERT_EQ(-23, instance.inAOverriddenByB(BaseA::inAOverriddenByB_MAGIC));
        ASSERT_EQ(-6, instance.inAOverriddenByBABC(BaseABC::inAOverriddenByBABC_MAGIC));
        ASSERT_EQ(-7, instance.inCImplementsAOverriddenByABC(BaseABC::inCImplementsAOverriddenByABC_MAGIC));
    }

    bool BaseABC_NeedsLazyMockForEachInheritanceBranch(bool orderReversed) {
        G::MockFixture fixture;
        G::LazyMock<BaseA> lazyA1;
        G::LazyMock<BaseA> lazyA2;

        // obtain a pair of shifted this pointers
        BaseABC instanceABC;
        BaseC* instanceC = &instanceABC;
        BaseA* instanceAofC = instanceC;
        BaseB* instanceB = &instanceABC;
        BaseA* instanceAofB = instanceB;

        if (instanceAofC == instanceAofB) {
            return false;
        }

        EXPECT_CALL(lazyA1, inANotOverridden(133)).Times(1).WillRepeatedly(Return(-887));
        EXPECT_CALL(lazyA2, inANotOverridden(132)).Times(1).WillRepeatedly(Return(-886));

        if (orderReversed) {
            return (-887 == instanceAofB->inANotOverridden(133)) &&
                (-886 == instanceAofC->inANotOverridden(132));
        } else {
            return (-887 == instanceAofC->inANotOverridden(133)) &&
                (-886 == instanceAofB->inANotOverridden(132));
        }
    }

    TEST(BaseABC, NeedsLazyMockForEachInheritanceBranch_Order1) {
        ASSERT_TRUE(BaseABC_NeedsLazyMockForEachInheritanceBranch(false));
    }

    TEST(BaseABC, NeedsLazyMockForEachInheritanceBranch_Order2) {
        ASSERT_TRUE(BaseABC_NeedsLazyMockForEachInheritanceBranch(true));
    }

    TEST(BaseABC, DoesDetectMockAfterThisShiftingCovariantCast) {
        G::MockFixture fixture;
        G::Mock<BaseABC> mock;

        // obtain a pair of shifted this pointers
        BaseABC* instanceABC = mock;
        BaseC* instanceC = instanceABC;
        ASSERT_NE((void*)instanceABC, (void*)instanceC);

        EXPECT_CALL(mock, inCOverriddenByABC(133)).WillOnce(Return(-887));

        // Call method that is implemented in more derived mock, but invoked
        // through a this pointer shifted base class
        ASSERT_EQ(-887, instanceC->inCOverriddenByABC(133));

    }

    TEST(BaseABC, IsLazyMockThisShiftingCastOrderInvariant) {
        // TODO: Associate lazy mocks in a way that this pointers get unshifted during registration.
        // Extending backwards does not work, because its ambigous. You need the "unshifted" this
        // and then extend forward to be sure that you stay inside the mocked object.
        // This is a more involved task! Dummy call every hooked method and memorize this shifting.
        // Use that shift by passing it to mock resolver.


        //G::MockFixture fixture;
        //G::LazyMock<BaseABC> lazy;

        //// obtain a pair of shifted this pointers
        //BaseABC instanceABCMem;
        //BaseABC* instanceABC = &instanceABCMem;
        //BaseC* instanceC = instanceABC;
        //ASSERT_NE((void*)instanceABC, (void*)instanceC);

        //EXPECT_CALL(lazy, inCOverriddenByABC(133)).Times(2).WillRepeatedly(Return(-887));

        ///*
        //    This is a complicated scenario. We want to register the lazy mock under
        //    a shifted this pointer. For this to happen we need to call a method first
        //    that was first defined in a base class (BaseC) but overridden by BaseABC.
        //    Further, that base class must require a shifted this pointer (BaseC, not BaseB).

        //    This will force the lazy mock to bind to a shifted this pointer, an address
        //    to which it doesn't really belong, because it's of type BaseABC but is registered
        //    at an address that belongs to BaseC.

        //    Second, we need to call a method that was first defined in BaseABC, since this
        //    will force the this pointer of the call to be unshifted. In the end, the second
        //    call is still expected to bind to the existing lazy mock.
        //*/
        //ASSERT_EQ(-887, instanceC->inCOverriddenByABC(133));
        //ASSERT_EQ(-887, instanceABC->inABCNotOverridden(133));
    }

    TEST(BaseABC, IsShiftedThisUnshiftedWhenStoredAsRealThisInMock) {
        // TODO: IsShiftedThisUnshiftedWhenStoredAsRealThisInMock
    }
}