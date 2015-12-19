
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "GeneratedMocks/AllInOneMock.h"
#include "../citrix.mocking/include/citrix/mocking/Mocking.h"

typedef citrix::mocking::GMock G;

namespace unittests {

    TEST(Mocking, StaticGoogleMock_ExpectMethod1) {
        G::MockFixture fixture;

        EXPECT_CALL(G::StaticMock<AllInOne>(), S_void__void()).Times(1);

        AllInOne::S_void__void();
    }

    TEST(Mocking, StaticGoogleMock_ExpectMethod2) {
        G::MockFixture fixture;

        EXPECT_CALL(G::StaticMock<AllInOne>(), S_void__void()).Times(2);

        AllInOne::S_void__void();
        AllInOne::S_void__void();
    }

    TEST(Mocking, InstanceGoogleMock_ExpectCall) {
        G::MockFixture fixture;

        G::LazyMock<AllInOne> mock;
        EXPECT_CALL(mock, void__void()).Times(1);

        AllInOne instance;
        instance.void__void();
    }

    TEST(Mocking, InstanceGoogleMock_ExpectCall_TwoInstances) {
        G::MockFixture fixture;

        G::LazyMock<AllInOne> mock1;
        EXPECT_CALL(mock1, void__void()).Times(1);

        G::LazyMock<AllInOne> mock2;
        EXPECT_CALL(mock2, void__void()).Times(2);

        AllInOne instance1;
        instance1.void__void();

        AllInOne instance2;
        instance2.void__void();
        instance2.void__void();
    }


    TEST(Mocking, InstanceGoogleMock_wereInstanceMocksConsumed) {
        G::MockFixture fixture;

        ASSERT_TRUE(fixture.wereLazyMocksConsumed());

        G::LazyMock<AllInOne> mock1;

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        G::LazyMock<AllInOne> mock2;

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        AllInOne instance1;
        instance1.void__void();

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        AllInOne instance2;
        instance2.void__void();

        ASSERT_TRUE(fixture.wereLazyMocksConsumed());
    }

    TEST(Mocking, DefaultGoogleMock_DifferentInstancesAreCaught) {
        G::MockFixture fixture;

        EXPECT_CALL(G::DefaultMock<AllInOne>(), void__void()).Times(2);

        AllInOne instanceA;
        AllInOne instanceB;

        instanceA.void__void();
        instanceB.void__void();
    }

    TEST(Mocking, DefaultGoogleMock_DoesAllowExplicitMock) {
        G::MockFixture fixture;
        G::Mock<AllInOne> mock;

        EXPECT_CALL(G::DefaultMock<AllInOne>(), void__void()).Times(2);
        EXPECT_CALL(mock, void__void()).Times(1);

        AllInOne instanceA;
        AllInOne instanceB;

        instanceA.void__void();
        mock->void__void();
        instanceB.void__void();
    }

    TEST(Mocking, DefaultGoogleMock_DoesAllowLazyMock) {
        G::MockFixture fixture;
        G::LazyMock<AllInOne> mock;

        EXPECT_CALL(G::DefaultMock<AllInOne>(), void__void()).Times(2);
        EXPECT_CALL(mock, void__void()).Times(1);

        AllInOne instanceA, instanceB, instanceC;

        instanceA.void__void();
        instanceC.void__void();
        instanceB.void__void();
    }
}
