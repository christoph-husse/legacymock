
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_GENERATOR_CONFIGURATION_H_
#define _CITRIX_MOCKING_GENERATOR_CONFIGURATION_H_

#include <string>
#include <vector>

#include "CppParser.h"
#include "../citrix.mocking/include/citrix/mocking/RegexMapper.h"
#include "../citrix.mocking/include/citrix/mocking/CodeRegionMap.h"

namespace citrix { namespace mocking {

    class Configuration {
    public:
        static std::string pythonExePath;
        static std::string parserScriptPath;
        static bool enableCtorMocking;
        static bool enableDtorMocking;
        static std::vector<std::string> publicIncludes;
        static std::vector<std::string> typeDefs;
        static std::vector<CppClass> classes;
        static std::vector<CppMethod> methodsToMock, methodsAndVariablesToMock, variablesToMock;
        static std::string dstDir;
        static std::string headerPath;
        static std::string headerContent;
        static std::string classPrefix;
        static std::string className;
        static bool isFreeApi;
        static bool beSilent;
        static bool safeUpdateCheck;
        static bool enableTracing;
        static bool enableDefaultImpl;
        static bool pureStaticClass;
        static bool mockVariables;
        static bool disableGMock;
        static bool mockAllClasses;
        static bool debugOmitImpl;
        static std::string customProbingInstance;
        static const CppClass* classToMock;
        static std::string mockManagerName;
        static std::string mockBaseName;
        static std::string gmockName;
        static std::string hooksName;
        static std::string hooksInternalName;
        static std::string headerGuard;
        static bool enableIfdefMapping;
        static std::vector<std::string> publicMockInlines;
        static CodeRegionMap<std::pair<PPDirective, PPDirective>> ifdefMap;
        static CodeRegionMap<RegexMatch> templateMap;
        static CodeRegionMap<RegexMatch> typedefMap;

        static void throwError(std::string message);
        static void logWarning(std::string message);
    };
}}

#endif