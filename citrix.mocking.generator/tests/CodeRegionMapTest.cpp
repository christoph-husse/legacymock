
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "../citrix.mocking/include/citrix/mocking/CodeRegionMap.h"
#include "gtest/gtest.h"
#include <boost/foreach.hpp>

using namespace citrix::mocking;

namespace unittests {

    TEST(CodeRegionMapTest, emptyQueryOnEmpty) {
        CodeRegionMap<int> testMap;
        ASSERT_TRUE(testMap.getEnclosedEntries(CodeRegion(12, 13)).empty());
    }

    TEST(CodeRegionMapTest, bruteforceTest) {
        typedef std::pair<CodeRegion, int> entry_type;
        std::vector<entry_type> shadow;
        for(int x = 0; x < 10; x++) {
            for(int y = 0; y < 20; y++) {
                shadow.push_back(std::make_pair(CodeRegion(x, y), x + 100 * y));
            }
        }

        CodeRegionMap<int> testMap;
        BOOST_FOREACH(const entry_type& e, shadow) {
            testMap.add(e.first, e.second);
        }

        for(int x = -1; x < 11; x++) {
            for(int y = -1; y < 21; y++) {
                std::vector<entry_type> query = testMap.getEnclosingEntries(CodeRegion(x, y));
                std::vector<entry_type> shadowQuery;

                // collect reference result
                if(x <= y) {
                    BOOST_FOREACH(const entry_type& e, shadow) {
                        if((e.first.from() <= x) && (e.first.to() >= y))
                            shadowQuery.push_back(e);
                    }
                }

                std::sort(query.begin(), query.end());
                std::sort(shadowQuery.begin(), shadowQuery.end());

                if(query != shadowQuery) {
                    ASSERT_TRUE(false);
                }
            }
        }
    }

    TEST(CodeRegionMapTest, bruteforceRangeTest) {
        typedef std::pair<CodeRegion, int> entry_type;
        std::vector<entry_type> shadow;
        for(int x = 0; x < 10; x++) {
            for(int y = 0; y < 20; y++) {
                shadow.push_back(std::make_pair(CodeRegion(x * 10, y * 10), x + 100 * y));
            }
        }

        CodeRegionMap<int> testMap;
        BOOST_FOREACH(const entry_type& e, shadow) {
            testMap.add(e.first, e.second);
        }

        for(int x = -1; x < 11; x++) {
            for(int y = -1; y < 21; y++) {
                std::vector<entry_type> query = testMap.getEnclosingEntries(CodeRegion(x * 10 + 5, y * 10 + 5));
                std::vector<entry_type> shadowQuery;

                // collect reference result
                if(x <= y) {
                    BOOST_FOREACH(const entry_type& e, shadow) {
                        if((e.first.from() <= x * 10 + 5) && (e.first.to() >= y * 10 + 5))
                            shadowQuery.push_back(e);
                    }
                }

                std::sort(query.begin(), query.end());
                std::sort(shadowQuery.begin(), shadowQuery.end());

                if(query != shadowQuery) {
                    ASSERT_TRUE(false);
                }
            }
        }
    }

    // TODO: add bruteforce random test
}