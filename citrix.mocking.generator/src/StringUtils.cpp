
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "StringUtils.h"

namespace citrix { namespace mocking {

    using namespace boost::xpressive;

    std::vector<std::string> StringUtils::split(std::string string, std::string regexDelimiter) {
        sregex re = sregex::compile(regexDelimiter);
        std::vector<std::string> splitted;

        sregex_token_iterator it(string.begin(), string.end(), re, -1), end;
        for(; it != end; it++) { splitted.push_back(*it); }

        return splitted;
    }

    int StringUtils::count(std::string string, std::string subString) {
        int pos = -1, count = 0;
        while(true) {
            pos = string.find(subString, pos + 1);
            if(pos == std::string::npos)
                break;
            count++;
        }
        return count;
    }

    bool StringUtils::contains(std::string string, std::string subString) {
        return string.find(subString) != std::string::npos;
    }

    bool StringUtils::startsWith(std::string string, std::string subString) {
        return string.find(subString) == 0;
    }

    std::string StringUtils::fromFile(std::string filePath) {
        FILE* file = fopen(filePath.c_str(), "r");

        if(!file) {
            return "";
        }

        char buffer[1024];
        std::stringstream sout;
        while(!feof(file) && fgets(buffer, sizeof(buffer) - 1, file)) {
            sout << buffer;
        }
        fclose(file);

        return sout.str();
    }

    bool StringUtils::toFile(std::string filePath, const std::string& content) {
        FILE* file = fopen(filePath.c_str(), "w");
        if(!file) {
            return false;
        }

        bool res = fwrite(content.c_str(), 1, content.size(), file) == content.size();
        fclose(file);
        return res;
    }
}}