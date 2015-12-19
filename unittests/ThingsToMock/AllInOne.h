
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_ALLINONE_H_
#define _UNITTEST_MOCK_ALLINONE_H_

#include "citrix/Config.h"
#include <string>

// namespace MockMe {

    class AllInOne {
    public:
        void void__void() const { }
        void overload(int a) const { }
        void overload(std::string value) { }
        void void__str(const std::string& str1) throw   (      ) { 
            CITRIX_MOCK_UNREFPARAM(str1);
        } 

        std::string str__str_rint(const std::string& str, int& i) const { 
            CITRIX_MOCK_UNREFPARAM(str);
            CITRIX_MOCK_UNREFPARAM(i);
            i = str.size();
            return str;
        }

        static void S_void__void() { }
        static void S_void__str(const std::string& str) { 
            CITRIX_MOCK_UNREFPARAM(str);
        }

        static std::string S_str__str_rint(const std::string& str, int& i) { 
            CITRIX_MOCK_UNREFPARAM(str);
            CITRIX_MOCK_UNREFPARAM(i);
            i = str.size();
            return str;
        }


    protected:
    private:
    };

// }


#endif