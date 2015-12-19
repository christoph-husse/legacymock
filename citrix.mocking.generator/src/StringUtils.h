
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_GENERATOR_STRINGUTILS_H_
#define _CITRIX_MOCKING_GENERATOR_STRINGUTILS_H_

#include <string>
#include <sstream>
#include <vector>

namespace citrix { namespace mocking {

    class StringUtils {
    public:

        virtual ~StringUtils() { }

        static std::vector<std::string> split(std::string string, std::string regexDelimiter);
        static bool contains(std::string string, std::string subString);
        static int count(std::string string, std::string subString);
        static bool startsWith(std::string string, std::string subString);
        static std::string fromFile(std::string filePath);
        static bool toFile(std::string filePath, const std::string& content);

        template<class T>
        static std::string str(T value) {
            std::stringstream sout;
            sout << value;
            return sout.str();
        }
    };
}}

#endif