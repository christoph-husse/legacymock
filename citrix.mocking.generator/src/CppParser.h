
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_GENERATOR_CPPPARSER_H_
#define _CITRIX_MOCKING_GENERATOR_CPPPARSER_H_

#include <string>
#include <vector>

#include "../citrix.mocking/include/citrix/mocking/CodeRegionMap.h"
#include "../citrix.mocking/include/citrix/mocking/RegexMapper.h"
#include "StringUtils.h"

namespace citrix { namespace mocking {

    class CppNestedType {
    };

    class CppParameter {
    public:
        std::string name;
        std::string type;
    };

    class Accessibility {
    public:
        enum {
            Public = 0,
            Protected = 1,
            Private = 2,
        };
    };

    class CppMethod {
    public:
        std::string name;
        int lineNumber;
        int accessibility;
        bool isDestructor;
        bool isConstructor;
        bool isVirtual;
        bool isAbstract;
        std::string returnType;
        std::string rawReturnType;
        bool isConst;
        bool isGetter;
        bool isStatic;
        std::string debug;
        std::vector<CppParameter> parameters;
        std::vector<std::pair<PPDirective, PPDirective>> ifdefs;
    };

    class CppClass {
    public:
        std::string name;
        int lineNumber;
        std::string nameSpace;
        std::vector<CppMethod> methods;
        std::vector<std::string> enums;
        CodeRegion region;
        std::vector<std::pair<PPDirective, PPDirective>> ifdefs;
    };

    class CppParser : StringUtils {
    public:
        static void parse(std::string pathToFile, std::vector<CppClass>& outClasses, std::string& outHeader);

    private:
        typedef std::vector<std::pair<std::string, std::string>> map_type;
        static int parseAccessibility(std::string str);
        static void invokeExternalParser(std::string pathToFile, map_type& outJson, std::string& outHeader);
        CppParser();
    };
}}

#endif