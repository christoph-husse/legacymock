
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "../citrix.mocking/include/citrix/mocking/RegexMapper.h"
#include "gtest/gtest.h"

using namespace citrix::mocking;

namespace unittests {

    class PPDirectiveScannerTest : public testing::Test {
    public:
        CodeRegionMap<std::pair<PPDirective, PPDirective>> map;
        std::string level1, level2, level3;
        std::string level1Accu, level2Accu, level3Accu;

        bool hasLevelsAt(int line, bool state1, bool state2, bool state3) {
            typedef std::pair<CodeRegion, std::pair<PPDirective, PPDirective>> query_type;
            std::vector<query_type> res = map.getEnclosingEntries(CodeRegion(line, line));
            std::vector<std::string> expected, actual, expectedAccu, actualAccu;
            std::vector<int> expectedAccuCount, actualAccuCount;

            if(!state1) {
                return res.empty();
            } else {
                expected.push_back(level1);
                expectedAccu.push_back(level1Accu);
                expectedAccuCount.push_back(1);
            }

            if(!state2) {
                if(res.size() > 1)
                    return false;
            } else {
                expected.push_back(level2);
                expectedAccu.push_back(level2Accu);
                expectedAccuCount.push_back(2);
            }

            if(!state3) {
                if(res.size() > 2)
                    return false;
            } else {
                expected.push_back(level3);
                expectedAccu.push_back(level3Accu);
                expectedAccuCount.push_back(3);
            }

            std::string accuExpr;
            BOOST_FOREACH(const query_type& e, res) {
                PPDirective ifdef = e.second.first;

                actual.push_back(ifdef.expression);
                actualAccuCount.push_back(ifdef.depth);

                if(!accuExpr.empty())
                    accuExpr += "\n";

                accuExpr += ifdef.combinedExpression;
                actualAccu.push_back(accuExpr);
            }

            if(expected != actual) 
                return false;

            if(expectedAccu != actualAccu) 
                return false;

            if(expectedAccuCount != actualAccuCount) 
                return false;

            return true;
        }
    };

    TEST_F(PPDirectiveScannerTest, emptyScanningScenario) {
        ASSERT_EQ(0, PPDirectiveScanner().scan("").size());
    }

    TEST_F(PPDirectiveScannerTest, complexScanningScenario) {
        std::string headerContent =
"\n\
UNCOVERED2\n\
#if !BOOST_ENDIAN_BIG_BYTE && !BOOST_ENDIAN_BIG_WORD && \\\r\n\
    !BOOST_ENDIAN_LITTLE_BYTE && !BOOST_ENDIAN_LITTLE_WORD\n\
UNCOVERED5\n\
\t #  \t     ifdef      (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) || \\ \\\sdlfk \ \3\4 \\\n\
            defined(__ARMEB__) || \\\\\n\
            defined(__MIPSEB) \ \\\ \\\sdlfk \ \3\4 \  \\ \\ \ \\\ \\ \\\\ \\ \\\ \\\\ \\|| \\\\\r\n\
            defined( endif __MIPSEB__)\\ \n\
UNCOVERED10\n\
#   else    \r\n\
# endif";
        PPDirectiveScanner scanner;
        const std::vector<PPDirective> d = scanner.scan(headerContent);

        ASSERT_EQ(4, d.size());

        ASSERT_EQ("if", d[0].name);
        ASSERT_EQ("ifdef", d[1].name);
        ASSERT_EQ("else", d[2].name);
        ASSERT_EQ("endif", d[3].name);

        ASSERT_TRUE(d[0].isOpening() && !d[0].isClosing() && !d[0].isUnconditional());
        ASSERT_TRUE(d[1].isOpening() && !d[1].isClosing() && !d[1].isUnconditional());
        ASSERT_TRUE(d[2].isOpening() && d[2].isClosing() && !d[2].isUnconditional());
        ASSERT_TRUE(!d[3].isOpening() && d[3].isClosing() && !d[3].isUnconditional());

        ASSERT_EQ("#if !BOOST_ENDIAN_BIG_BYTE && !BOOST_ENDIAN_BIG_WORD && \\\r\n\
    !BOOST_ENDIAN_LITTLE_BYTE && !BOOST_ENDIAN_LITTLE_WORD", d[0].expression);
        ASSERT_EQ("#  \t     ifdef      (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__)) || \\ \\\sdlfk \ \3\4 \\\n\
            defined(__ARMEB__) || \\\\\n\
            defined(__MIPSEB) \ \\\ \\\sdlfk \ \3\4 \  \\ \\ \ \\\ \\ \\\\ \\ \\\ \\\\ \\|| \\\\\r\n\
            defined( endif __MIPSEB__)\\", d[1].expression);
        ASSERT_EQ("#   else", d[2].expression);
        ASSERT_EQ("# endif", d[3].expression);

        ASSERT_EQ(12, d[0].startPos); ASSERT_EQ(129, d[0].endPos);
        ASSERT_EQ(141, d[1].startPos); ASSERT_EQ(388, d[1].endPos);
        ASSERT_EQ(401, d[2].startPos); ASSERT_EQ(414, d[2].endPos);
        ASSERT_EQ(415, d[3].startPos); ASSERT_EQ(421, d[3].endPos);

        ASSERT_EQ(3, d[0].startLine); ASSERT_EQ(4, d[0].endLine);
        ASSERT_EQ(6, d[1].startLine); ASSERT_EQ(9, d[1].endLine);
        ASSERT_EQ(11, d[2].startLine); ASSERT_EQ(11, d[2].endLine);
        ASSERT_EQ(12, d[3].startLine); ASSERT_EQ(12, d[3].endLine);
    }

    TEST_F(PPDirectiveScannerTest, complexMappingScenario) {

        std::string headerContent =
"\n\
#ifdef LEVEL_1\n\
#   ifdef LEVEL_1___LEVEL_2\n\
#       ifdef LEVEL_1___LEVEL_2___LEVEL_3\n\
            LEVEL_1___LEVEL_2___LEVEL_3();\n\
#       else // LEVEL_1___LEVEL_2___LEVEL_3\n\
            LEVEL_1___LEVEL_2___NOT_LEVEL_3();\n\
#       endif\n\
#   else // LEVEL_1___LEVEL_2\n\
#       ifdef LEVEL_1___NOT_LEVEL_2___LEVEL_3\n\
            LEVEL_1___NOT_LEVEL_2___LEVEL_3();\n\
#       else // LEVEL_1___NOT_LEVEL_2___LEVEL_3\n\
            LEVEL_1___NOT_LEVEL_2___NOT_LEVEL_3();\n\
#       endif\n\
#   endif\n\
#else //LEVEL_1\n\
#   ifdef NOT_LEVEL_1___LEVEL_2\n\
#       if NOT_LEVEL_1___LEVEL_2___LEVEL_3\n\
            NOT_LEVEL_1___LEVEL_2___LEVEL_3();\n\
#       elif NOT_LEVEL_1___LEVEL_2___LEVEL_3\n\
            NOT_LEVEL_1___LEVEL_2___NOT_LEVEL_3();\n\
#       endif\n\
#   else //NOT_LEVEL_1___LEVEL_2\n\
#       if NOT_LEVEL_1___NOT_LEVEL_2___LEVEL_3\n\
            NOT_LEVEL_1___NOT_LEVEL_2___LEVEL_3();\n\
#       elif NOT_LEVEL_1___LEVEL_2\n\
            NOT_LEVEL_1___NOT_LEVEL_2___NOT_LEVEL_3();\n\
#       endif\n\
#   endif\n\
#endif";

        PPDirectiveScanner scanner;
        const std::vector<PPDirective> d = scanner.scan(headerContent);

        ASSERT_EQ(21, d.size());

        scanner.map(d, map);

        ASSERT_TRUE(hasLevelsAt(0, false, false, false));
        ASSERT_TRUE(hasLevelsAt(1, false, false, false));

        level1 = "#ifdef LEVEL_1";
        level1Accu = "#ifdef LEVEL_1";
        ASSERT_TRUE(hasLevelsAt(2, true, false, false));
            level2 = "#   ifdef LEVEL_1___LEVEL_2";
            level2Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2";
            ASSERT_TRUE(hasLevelsAt(3, true, true, false));
                level3 = "#       ifdef LEVEL_1___LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2\n#       ifdef LEVEL_1___LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(4, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(5, true, true, true));
                level3 = "#       else // LEVEL_1___LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2\n#       ifdef LEVEL_1___LEVEL_2___LEVEL_3\n#       else // LEVEL_1___LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(6, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(7, true, true, true));
                ASSERT_TRUE(hasLevelsAt(8, true, true, false));

            level2 = "#   else // LEVEL_1___LEVEL_2";
            level2Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2\n#   else // LEVEL_1___LEVEL_2";
            ASSERT_TRUE(hasLevelsAt(9, true, true, false));
                level3 = "#       ifdef LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2\n#   else // LEVEL_1___LEVEL_2\n#       ifdef LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(10, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(11, true, true, true));
                level3 = "#       else // LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#   ifdef LEVEL_1___LEVEL_2\n#   else // LEVEL_1___LEVEL_2\n#       ifdef LEVEL_1___NOT_LEVEL_2___LEVEL_3\n#       else // LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(12, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(13, true, true, true));
                ASSERT_TRUE(hasLevelsAt(14, true, true, false));
            ASSERT_TRUE(hasLevelsAt(15, true, false, false));

        level1 = "#else //LEVEL_1";
        level1Accu = "#ifdef LEVEL_1\n#else //LEVEL_1";
        ASSERT_TRUE(hasLevelsAt(16, true, false, false));
            level2 = "#   ifdef NOT_LEVEL_1___LEVEL_2";
            level2Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2";
            ASSERT_TRUE(hasLevelsAt(17, true, true, false));
                level3 = "#       if NOT_LEVEL_1___LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2\n#       if NOT_LEVEL_1___LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(18, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(19, true, true, true));
                level3 = "#       elif NOT_LEVEL_1___LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2\n#       if NOT_LEVEL_1___LEVEL_2___LEVEL_3\n#       elif NOT_LEVEL_1___LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(20, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(21, true, true, true));
                ASSERT_TRUE(hasLevelsAt(22, true, true, false));

            level2 = "#   else //NOT_LEVEL_1___LEVEL_2";
            level2Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2\n#   else //NOT_LEVEL_1___LEVEL_2";
            ASSERT_TRUE(hasLevelsAt(23, true, true, false));
                level3 = "#       if NOT_LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                level3Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2\n#   else //NOT_LEVEL_1___LEVEL_2\n#       if NOT_LEVEL_1___NOT_LEVEL_2___LEVEL_3";
                ASSERT_TRUE(hasLevelsAt(24, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(25, true, true, true));
                level3 = "#       elif NOT_LEVEL_1___LEVEL_2";
                level3Accu = "#ifdef LEVEL_1\n#else //LEVEL_1\n#   ifdef NOT_LEVEL_1___LEVEL_2\n#   else //NOT_LEVEL_1___LEVEL_2\n#       if NOT_LEVEL_1___NOT_LEVEL_2___LEVEL_3\n#       elif NOT_LEVEL_1___LEVEL_2";
                ASSERT_TRUE(hasLevelsAt(26, true, true, true));
                    ASSERT_TRUE(hasLevelsAt(27, true, true, true));
                ASSERT_TRUE(hasLevelsAt(28, true, true, false));
            ASSERT_TRUE(hasLevelsAt(29, true, false, false));
        ASSERT_TRUE(hasLevelsAt(30, false, false, false));

        ASSERT_TRUE(hasLevelsAt(31, false, false, false));
        ASSERT_TRUE(hasLevelsAt(130, false, false, false));
    }
}
