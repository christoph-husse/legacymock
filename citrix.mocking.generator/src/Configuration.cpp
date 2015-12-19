
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "Configuration.h"

namespace citrix { namespace mocking {

        std::string Configuration::pythonExePath = "";
        std::string Configuration::parserScriptPath = "";
        bool Configuration::enableCtorMocking = false;
        bool Configuration::enableDtorMocking = false;
        std::vector<std::string> Configuration::publicIncludes;
        std::vector<CppClass> Configuration::classes;
        std::vector<CppMethod> Configuration::methodsToMock;
        std::vector<CppMethod> Configuration::methodsAndVariablesToMock;
        std::vector<CppMethod> Configuration::variablesToMock;
        std::string Configuration::dstDir = "";
        std::string Configuration::headerPath = "";
        std::string Configuration::headerContent = "";
        std::string Configuration::classPrefix = "";
        std::string Configuration::className = "";
        bool Configuration::isFreeApi = false;
        bool Configuration::beSilent = true;
        bool Configuration::safeUpdateCheck = false;
        bool Configuration::enableTracing = false;
        bool Configuration::enableDefaultImpl = false;
        bool Configuration::pureStaticClass = false;
        bool Configuration::mockVariables = false;
        bool Configuration::disableGMock = false;
        bool Configuration::mockAllClasses = false;
        std::string Configuration::customProbingInstance = "";
        const CppClass* Configuration::classToMock = NULL;
        std::string Configuration::mockManagerName = "";
        std::string Configuration::mockBaseName = "";
        std::string Configuration::gmockName = "";
        std::string Configuration::hooksName = "";
        std::string Configuration::hooksInternalName = "";
        std::string Configuration::headerGuard = "";
        bool Configuration::enableIfdefMapping = false;
        bool Configuration::debugOmitImpl = false;
        std::vector<std::string> Configuration::typeDefs;
        std::vector<std::string> Configuration::publicMockInlines;
        CodeRegionMap<std::pair<PPDirective, PPDirective>> Configuration::ifdefMap;
        CodeRegionMap<RegexMatch> Configuration::templateMap;
        CodeRegionMap<RegexMatch> Configuration::typedefMap;



        void Configuration::throwError(std::string message) {
            std::cerr << "[ERROR]: " << message << std::endl;
            exit(-1);
        }

        void Configuration::logWarning(std::string message) {
            std::cout << "[WARNING]: " << message << std::endl;
        }
}}

