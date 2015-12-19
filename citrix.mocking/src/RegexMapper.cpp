#include "../citrix.mocking/include/citrix/mocking/RegexMapper.h"
#include <boost/algorithm/string.hpp>

namespace citrix { namespace mocking {

    using namespace boost::xpressive;

    bool PPDirective::isUnconditional() const {
        return !isOpening() && !isClosing();
    }

    bool PPDirective::isOpening() const {
        return (name == "if") || (name == "ifdef") || (name == "ifndef") || (name == "elif") || (name == "else");
    }

    bool PPDirective::isClosing() const {
        return (name == "endif") || (name == "elif") || (name == "else");
    }

    bool PPDirectiveScanner::map(const std::vector<PPDirective>& directives, CodeRegionMap<std::pair<PPDirective, PPDirective>>& outMap) {

        // match #if with #else or #endif
        std::stack<PPDirective> ops;
        std::map<int, std::pair<PPDirective, PPDirective>> ifClosure;

        // go through all directives in file order
        BOOST_FOREACH(PPDirective d, directives) {

            if(d.isClosing()) {
                // there needs to be a pending #if
                if(ops.empty())
                    return false;

                PPDirective start = ops.top();
                d.combinedExpression = start.expression + "\n" + d.expression;
                d.depth = start.depth;
                ifClosure[start.startPos] = std::make_pair(start, d);
                ops.pop();

            } else {
                d.combinedExpression = d.expression;
            }

            if(d.isOpening()) {
                // only #if's are pushed on the stack
                d.depth = ops.size() + 1;
                ops.push(d);
            } 
        }

         if(!ops.empty()) {
             return false;
         }

        typedef std::pair<int, std::pair<PPDirective, PPDirective>> range_type;
        BOOST_FOREACH(const range_type& e, ifClosure) {
            PPDirective start = e.second.first, end = e.second.second;
            outMap.add(CodeRegion(start.endLine, end.startLine - 1), std::make_pair(start, end));
        }

        return true;
    }

    void PPDirectiveScanner::filter(const std::vector<std::string>& allowedDirectives, std::vector<PPDirective>& inOutDirectives) {
        for(size_t i = 0; i < inOutDirectives.size(); i++) {
            const std::string& name = inOutDirectives[i].name;
            bool exists = false;

            BOOST_FOREACH(const std::string& allowed, allowedDirectives) {
                if(allowed == name) {
                    exists = true;
                    break;
                }
            }

            if(!exists) {
                inOutDirectives.erase(inOutDirectives.begin() + i);
                i--;
            }
        }
    }

    int advanceLineCounter(const std::string& headerFile, int advanceToPos, int& currentLine, int& currentPos) {
        while(currentPos < advanceToPos) {
            if(headerFile[currentPos++] == '\n') {
                currentLine++;
            }
        }

        return currentLine;
    }

    std::vector<PPDirective> PPDirectiveScanner::scan(const std::string& headerFile) {
        CodeRegionMap<PPDirective> mapIgnore;
        std::vector<PPDirective> directives = RegexMapper::map(
            headerFile,
            "[^\\n]*#[ \\t]*([a-zA-Z0-9_]+)([^\\r\\n]*\\\\\\r?\\n)*([^\\r\\n]*(?<!\\\\)\\r?\\n)",
            mapIgnore);

        BOOST_FOREACH(PPDirective& directive, directives) {
            boost::trim(directive.expression);
        }

        return directives;
    }

    CodeRegionMap<RegexMatch> TypedefScanner::scan(const std::string& headerFile) {
        CodeRegionMap<RegexMatch> map;
        RegexMapper::map(
            headerFile,
            "(?<![a-zA-Z_0-9]{1})(typedef\\s+[^;]+;)",
            map);

        return map;
    }

    CodeRegionMap<RegexMatch> TemplateScanner::scan(const std::string& headerFile) {
        CodeRegionMap<RegexMatch> map;
        RegexMapper::map(
            headerFile,
            "(?<![a-zA-Z_0-9]{1})(template[\\s\\n]*<[^;\\{\\}\\(\\)]+[;\\{\\}\\(\\)])",
            map);

        return map;
    }
}}