#ifndef _CITRIX_MOCKING_GENERATOR_REGEXMAPPER_H_
#define _CITRIX_MOCKING_GENERATOR_REGEXMAPPER_H_

#include "citrix/Config.h"
#include "../citrix.mocking/include/citrix/mocking/CodeRegionMap.h"
#include <boost/xpressive/xpressive.hpp>
#include <string>

namespace citrix { namespace mocking {

    class RegexMatch {
    public:
        std::string expression;
        std::string combinedExpression;
        int depth;
        int startPos, endPos;
        int startLine, endLine;

        RegexMatch() { }
        RegexMatch(const boost::xpressive::match_results<boost::xpressive::sregex::iterator_type>& match) :
              depth(0), startPos(0), endPos(0), startLine(0), endLine(0)
        { 
            CITRIX_MOCK_UNREFPARAM(match);
        }
    };

    class RegexMapper {
    public:
        template<class TRegexMatch>
        static std::vector<TRegexMatch> map(
                    const std::string& stringToScan,
                    const std::string& regex,
                    CodeRegionMap<TRegexMatch>& outMap) {

            using namespace boost::xpressive;

            typedef match_results<sregex::iterator_type>::iterator::value_type match_type;

            std::vector<TRegexMatch> result;
            match_results<sregex::iterator_type> matches;
            std::string content = stringToScan + "\n";
            regex_iterator<sregex::iterator_type> begin(content.begin(), content.end(), 
                sregex::compile(regex)), end;
            int currentLine = 1, currentPos = 0;

            for(; begin != end; begin++) {
                match_results<sregex::iterator_type> match = *begin;
                TRegexMatch entry(match);

                entry.startPos = match.position();
                entry.endPos = std::min((int)stringToScan.size() - 1, match.length() + entry.startPos - 1);
                entry.expression = content.substr(entry.startPos, entry.endPos - entry.startPos + 1);
                entry.startLine = advanceLineCounter(content, entry.startPos, currentLine, currentPos);
                entry.endLine = advanceLineCounter(content, entry.endPos, currentLine, currentPos);

                outMap.add(CodeRegion(entry.startLine, entry.endLine), entry);
                result.push_back(entry);
            }

            return result;
        }
    };

    class PPDirective : public RegexMatch {
    public:
        std::string name;

        bool isUnconditional() const;
        bool isOpening() const;
        bool isClosing() const;

        PPDirective() { }
        PPDirective(const boost::xpressive::match_results<boost::xpressive::sregex::iterator_type>& match) : name(match[1]) { }
    };

    class PPDirectiveScanner {
    public:
        static std::vector<PPDirective> scan(const std::string& headerFile);
        static void filter(const std::vector<std::string>& allowedDirectives, std::vector<PPDirective>& inOutDirectives);
        static bool map(const std::vector<PPDirective>& directives, CodeRegionMap<std::pair<PPDirective, PPDirective>>& outMap);
    };

    class TypedefScanner {
    public:
        static CodeRegionMap<RegexMatch> scan(const std::string& headerFile);
    };

    class TemplateScanner {
    public:
        static CodeRegionMap<RegexMatch> scan(const std::string& headerFile);
    };
}}

#endif