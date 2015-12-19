
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

namespace unittests {

    #define FIXTURE \
        CustomMock::static_counter = 0; \
        C::MockFixture fixture; \
        C::Mock<CustomMock> instance_mock1; \
        C::Mock<CustomMock> instance_mock2; 

    struct CustomMock : public AllInOneMock {
        static int static_counter;
        int counter;

        CustomMock() : counter(0) { }

        virtual std::string str__str_rint(const std::string & str, int & i) const { 
            std::ostringstream sout;    
            sout << str << " " << i;
            i *= 3;
            const_cast<CustomMock*>(this)->counter++;
            return sout.str();
        }

        virtual std::string S_str__str_rint(const std::string & str, int & i) { 
            std::ostringstream sout;
            sout << str << " [static] " << i;
            i *= 2;
            static_counter++;
            return sout.str();
        }
    };

    template<class T>
    struct CustomMockContext {
        typedef CustomMock TStatic;
        typedef CustomMock TInstance;
    };

    typedef citrix::mocking::MockWrapper<CustomMockContext> C;

    int CustomMock::static_counter = 0;

    TEST(Mocking, StaticCustomMock_ExpectMethod2) {
        CustomMock::static_counter = 0;
        C::MockFixture fixture;

        C::StaticMock<AllInOne>();

        int i = 12;

        ASSERT_EQ(0, CustomMock::static_counter);
        ASSERT_EQ("Hello! [static] 12", AllInOne::S_str__str_rint("Hello!", i));
        ASSERT_EQ(12 * 2, i);
        ASSERT_EQ(1, CustomMock::static_counter);
    }


    TEST(Mocking, InstanceCustomMock_ExpectCall) {
        C::MockFixture fixture;
        C::Mock<AllInOne> instance_mock1;

        int i = 12;

        ASSERT_EQ(0, instance_mock1.counter);
        ASSERT_EQ("Hello! 12", instance_mock1->str__str_rint("Hello!", i));
        ASSERT_EQ(12 * 3, i);
        ASSERT_EQ(1, instance_mock1.counter);
    }

    TEST(Mocking, InstanceCustomMock_ExpectCall_TwoInstances) {
        CustomMock::static_counter = 0;
        C::MockFixture fixture;
        C::Mock<AllInOne> instance_mock1;
        C::Mock<AllInOne> instance_mock2; 

        int i = 12;

        ASSERT_EQ(0, instance_mock1.counter);
        ASSERT_EQ("Hello 1! 12", instance_mock1->str__str_rint("Hello 1!", i));
        ASSERT_EQ(12 * 3, i);
        ASSERT_EQ(1, instance_mock1.counter);

        ASSERT_EQ(0, instance_mock2.counter);
        ASSERT_EQ("Hello 2! 36", instance_mock2->str__str_rint("Hello 2!", i));
        ASSERT_EQ(12 * 3 * 3, i);
        ASSERT_EQ(1, instance_mock2.counter);

        ASSERT_EQ("Hello 2#! 108", instance_mock2->str__str_rint("Hello 2#!", i));
        ASSERT_EQ(12 * 3 * 3 * 3, i);
        ASSERT_EQ(2, instance_mock2.counter);
    }


    TEST(Mocking, InstanceCustomMock_wereInstanceMocksConsumed) {
        C::MockFixture fixture;

        ASSERT_TRUE(fixture.wereLazyMocksConsumed());

        C::LazyMock<AllInOne> lazy1;

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        C::LazyMock<AllInOne> lazy2;

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        AllInOne instance1;
        instance1.void__void();

        ASSERT_FALSE(fixture.wereLazyMocksConsumed());

        AllInOne instance2;
        instance2.void__void();

        ASSERT_TRUE(fixture.wereLazyMocksConsumed());
    }

}
