
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "CppParser.h"
#include "Configuration.h"

#include <fstream>

#ifdef _MSC_VER
#   define popen _popen
#   undef min
#   undef max
#endif

namespace citrix { namespace mocking {

    using namespace boost::xpressive;

    int CppParser::parseAccessibility(std::string str) {
        if(str == "Public") {
            return Accessibility::Public;
        } else if(str == "Protected") {
            return Accessibility::Protected;
        } else {
            return Accessibility::Private;
        }
    }

    template<class T> 
    T get(std::vector<std::pair<std::string, std::string>>& json, size_t& index, std::string expectedKey) {
        std::pair<std::string, std::string> entry = json[index++];
        if(entry.first != expectedKey)
            throw std::bad_exception();
        return entry.second;
    }

    template<> 
    bool get<bool>(std::vector<std::pair<std::string, std::string>>& json, size_t& index, std::string expectedKey) {
        std::string value = get<std::string>(json, index, expectedKey);
        if((value == "False") || (value == "0"))
            return false;
        else if((value == "True") || (value == "1"))
            return true;
        else
            throw std::bad_exception();
    }

    template<> 
    int get<int>(std::vector<std::pair<std::string, std::string>>& json, size_t& index, std::string expectedKey) {
        return boost::lexical_cast<int>(get<std::string>(json, index, expectedKey));
    }

    void CppParser::parse(std::string pathToFile, std::vector<CppClass>& outClasses, std::string& outHeader) {
        map_type json;
        sregex operatorPattern = sregex::compile("(?<![a-zA-Z_0-9])operator[^a-zA-Z_0-9]{1}");

        invokeExternalParser(pathToFile, json, outHeader);

        if(Configuration::enableIfdefMapping) {
            PPDirectiveScanner scanner;
            std::vector<PPDirective> directives = scanner.scan(Configuration::headerContent);
            std::vector<std::string> allowedDirectives;
            CodeRegionMap<std::pair<PPDirective, PPDirective>> mappedDirectives;
            if(!scanner.map(directives, Configuration::ifdefMap)) {
                Configuration::throwError("At least one #if #endif scope was not closed properly.");
            }

            // filter header guard
            std::string content = Configuration::headerContent;
            int maxLine = 1;
            for(size_t i = 0; i + 1 < content.size(); i++) {
                char c = content[i];
                char nc = content[i+1];
        
                if((c == '/') && (nc == '/')) {
                    // consume single line comment
                    for(; i < content.size(); i++) {
                        if(content[i] == '\n')
                            break;
                    }

                    maxLine++;
                    continue;
                }

                if((c == '/') && (nc == '*')) {
                    // consume multi-line comment
                    for(; i + 1 < content.size(); i++) {
                        if((content[i] == '*') && (content[i+1] == '/')) {
                            i++;
                            break;
                        }

                        if(content[i] == '\n')
                            maxLine++;
                    }
                    continue;
                }

                if(c == '\n')
                    maxLine++;
                else {
                    if(!isspace(c)) 
                        break;
                }
            }

            Configuration::ifdefMap.removeIntersectedEntries(CodeRegion(maxLine, maxLine));
        }

        Configuration::templateMap = TemplateScanner::scan(Configuration::headerContent);
        Configuration::typedefMap = TypedefScanner::scan(Configuration::headerContent);


        try {
            for(size_t i = 0; i < json.size(); ) {
                CppClass klass;

                klass.name = get<std::string>(json, i, "Class-Name");
                klass.lineNumber = get<int>(json, i, "Class-LineNumber");
                klass.nameSpace = get<std::string>(json, i, "Class-Namespace");
                klass.enums = StringUtils::split(get<std::string>(json, i, "Class-Enums"), ",");

                int maxLineExtend = klass.lineNumber;
                for(; (i < json.size()) && (json[i].first == "Method-Name"); ) {
                    CppMethod method;

                    method.name = get<std::string>(json, i, "Method-Name");
                    method.accessibility = parseAccessibility(get<std::string>(json, i, "Method-Accessibility"));
                    method.isDestructor = get<bool>(json, i, "Method-IsDestructor");
                    method.isConstructor = get<bool>(json, i, "Method-IsConstructor");
                    method.isVirtual = get<bool>(json, i, "Method-IsVirtual");
                    method.lineNumber = get<int>(json, i, "Method-LineNumber");
                    method.isAbstract = get<bool>(json, i, "Method-IsAbstract");
                    method.rawReturnType = get<std::string>(json, i, "Method-ReturnType");
                    method.isConst = get<bool>(json, i, "Method-IsConst");
                    method.isStatic = get<bool>(json, i, "Method-IsStatic");
                    method.isGetter = get<bool>(json, i, "Method-IsGetter");
                    method.debug = get<std::string>(json, i, "Method-Debug");

                    // fix malformatted but acceptable C++ member declarations which include class name
                    int pos = method.rawReturnType.find(klass.name + "::");
                    if(pos != std::string::npos) {
                        method.rawReturnType = method.rawReturnType.substr(0, pos);
                    }

                    method.returnType = "::citrix::type_traits::return_type_of_function<" + method.rawReturnType + "()>::type";
                    if(method.isGetter) {
                        method.returnType = "::boost::add_reference<::citrix::type_traits::return_type_of_function<" + method.rawReturnType + "()>::type>::type";
                        method.rawReturnType = "::boost::add_reference<" + method.rawReturnType + ">::type";
                    }

                    if(startsWith(method.returnType, "static ")) {
                        method.returnType = method.returnType.substr(7);
                        method.isStatic = true;
                    }

                    for(; (i < json.size()) && (json[i].first == "Param-Name"); ) {
                        CppParameter param;

                        param.name = get<std::string>(json, i, "Param-Name");
                        param.type = get<std::string>(json, i, "Param-Type");

                        method.parameters.push_back(param);
                    }

                    smatch opMatch;
                    if(regex_search(method.name, opMatch, operatorPattern))
                        continue;

                    if(!Configuration::templateMap.getIntersectedEntries(CodeRegion(method.lineNumber)).empty())
                        continue;

                    typedef std::pair<CodeRegion, std::pair<PPDirective, PPDirective>> ifdef_type;
                    BOOST_FOREACH(const ifdef_type& ifdef, Configuration::ifdefMap.getIntersectedEntries(CodeRegion(method.lineNumber))) {
                        method.ifdefs.push_back(ifdef.second);
                    }

                    maxLineExtend = std::max(maxLineExtend, method.lineNumber);
                    klass.methods.push_back(method);
                }

                if(!Configuration::templateMap.getIntersectedEntries(CodeRegion(klass.lineNumber)).empty())
                    continue;

                klass.region = CodeRegion(klass.lineNumber, maxLineExtend);

                typedef std::pair<CodeRegion, std::pair<PPDirective, PPDirective>> ifdef_type;
                BOOST_FOREACH(const ifdef_type& ifdef, Configuration::ifdefMap.getEnclosingEntries(klass.region)) {
                    klass.ifdefs.push_back(ifdef.second);
                }

                typedef std::pair<CodeRegion, RegexMatch> typedef_type;
                BOOST_FOREACH(typedef_type typeDef, Configuration::typedefMap.getIntersectedEntries(klass.region)) {
                    Configuration::typeDefs.push_back(typeDef.second.expression);
                }

                outClasses.push_back(klass);
            }
        } catch(const std::exception& e) {
            Configuration::throwError("Unexpected exception while parsing C++ header.");
        }
    }

    void CppParser::invokeExternalParser(std::string pathToFile, map_type& outJson, std::string& outHeader) {
        try {
            std::string cmdLine;

            cmdLine += Configuration::pythonExePath;
            cmdLine += " ";
            cmdLine += "\"";
            cmdLine += Configuration::parserScriptPath;
            cmdLine += "\"";
            cmdLine += " -i \"";
            cmdLine += pathToFile;
            cmdLine += "\"";

            FILE* parser = popen(cmdLine.c_str(), "r");

            char buffer[1024];
            std::stringstream sout;
            std::string content;
            bool hasHeader = false;

            while(!feof(parser) && fgets(buffer, sizeof(buffer) - 1, parser)) {
                if(!hasHeader && (strncmp(buffer, "Header:", 7) == 0)) {
                    hasHeader = true;
                    content = sout.str();

                    sout.str(std::string());
                    sout.clear();
                } else {
                    sout << buffer;
                }
            }
            fclose(parser);

            outHeader = sout.str();

            for(size_t i = 0, lineStart = 0, delimPos = 0; i < content.size(); i++) {
                char c = content[i];
                if(c == ':') {
                    if(delimPos == 0)
                        delimPos = i;
                } else if(c == '\n') {
                    int skipper = 1;
                    if(content[i - 1] == '\r')
                        skipper = 2;

                    outJson.push_back(std::make_pair(
                        content.substr(lineStart, delimPos - lineStart), 
                        content.substr(delimPos + 1, i - delimPos - skipper)));
                    lineStart = i + 1;
                    delimPos = 0;
                }
            }

            std::ofstream dump;
            dump.open("./dump.h", std::ios::out | std::ios::trunc );
            dump << outHeader;
            dump.close();

            if(!outJson.empty() && (outJson[0].first == "Exception")) {
                Configuration::throwError("While parsing C++ header: " + outJson[0].second);
            }

        } catch(const std::exception& e) {
            Configuration::throwError("Unexpected exception while parsing C++ header: \"" + (std::string)e.what() + "\"");
        }
    }
}}