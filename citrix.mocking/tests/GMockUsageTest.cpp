
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


    void testFunc(AllInOne value, AllInOne* ptr, const AllInOne& constValue, const AllInOne* constPtr) {
        ptr->void__str("Hello World!");
        const_cast<AllInOne&>(constValue).void__str("Hello World!");
        const_cast<AllInOne*>(constPtr)->void__str("Hello World!");

        value.void__str("Oh my god, a world!");
        value.void__str("Oh my god, a world!");
    }

    TEST(Mocking, GMockUsage_Basic) {
        {
            G::MockFixture fixture; 
            G::Mock<AllInOne> myMock; 
            G::LazyMock<AllInOne> myLazyMock;

            EXPECT_CALL(myMock, void__str("Hello World!")).Times(3);
            EXPECT_CALL(myLazyMock, void__str("Oh my god, a world!")).Times(2);

            testFunc(*myMock, myMock, *myMock, myMock);
        }

        AllInOne dummy;
        testFunc(AllInOne(), &dummy, AllInOne(), &dummy);

        {
            G::MockFixture fixture; 
            G::Mock<AllInOne> myMock; 
            G::LazyMock<AllInOne> myLazyMock;

            EXPECT_CALL(myMock, void__str("Hello World!")).Times(3);
            EXPECT_CALL(myLazyMock, void__str("Oh my god, a world!")).Times(2);

            testFunc(*myMock, myMock, *myMock, myMock);
        }
    }


}
