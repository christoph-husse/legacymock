#ifndef _CITRIX_MOCKING_GENERATOR_CODEREGIONMAP_H_
#define _CITRIX_MOCKING_GENERATOR_CODEREGIONMAP_H_

#include <vector>
#include <map>
#include <algorithm>
#include <boost/foreach.hpp>

#undef max
#undef min

namespace citrix { namespace mocking {

    class CodeRegion {
    private:
        int lower, upper;

    public:
        int from() const { return lower; }
        int to() const { return upper; }

        void extend(CodeRegion toBeEnclosed) {
            lower = std::min(toBeEnclosed.from(), from());
            upper = std::max(toBeEnclosed.to(), to());
        }

        CodeRegion(int line = 0) : lower(line), upper(line) { }
        CodeRegion(int lineFrom, int lineTo) : lower(lineFrom), upper(lineTo) { }

        bool operator !=(const CodeRegion& other) const {
            return !(*this == other);
        }

        bool operator ==(const CodeRegion& other) const {
            return (lower == other.lower) && (upper == other.upper);
        }

        bool operator <(const CodeRegion& other) const {
            return (lower < other.lower) || ((lower == other.lower) && (upper < other.upper));
        }

        bool encloses(CodeRegion region) const {
            return (from() <= region.from()) && (region.to() <= to());
        }

        bool enclosesLine(int line) const {
            return (from() <= line) && (line <= to());
        }

        bool intersects(CodeRegion region) const {
            return enclosesLine(region.from()) || enclosesLine(region.to()) || encloses(region) ||
                region.enclosesLine(from()) || region.enclosesLine(to()) || region.encloses(*this);
        }
    };

    template<class TValue>
    class CodeRegionMap {
        typedef std::pair<CodeRegion, TValue> entry_type;
        std::vector<entry_type> entries;

    public:

        void add(CodeRegion region, TValue value) {

            if(region.from() > region.to())
                return;

            entries.push_back(std::make_pair(region, value));
        }

        void removeIntersectedEntries(CodeRegion region) {
            std::vector<entry_type> result;
            
            if(region.from() > region.to())
                return;

            for(size_t i = 0; i < entries.size(); i++) {
                if(entries[i].first.intersects(region)) {
                    entries.erase(entries.begin() + i);
                    i--;
                }
            }
        }

        std::vector<entry_type> getIntersectedEntries(CodeRegion region) {
            std::vector<entry_type> result;
            
            if(region.from() > region.to())
                return result;

            BOOST_FOREACH(const entry_type& e, entries) {
                if(e.first.intersects(region)) {
                    result.push_back(e);
                }
            }
            return result;
        }

        std::vector<entry_type> getEnclosedEntries(CodeRegion region) {
            std::vector<entry_type> result;

            if(region.from() > region.to())
                return result;

            BOOST_FOREACH(const entry_type& e, entries) {
                if(region.encloses(e.first)) {
                    result.push_back(e);
                }
            }
            return result;
        }

        std::vector<entry_type> getEnclosingEntries(CodeRegion region) {
            std::vector<entry_type> result;

            if(region.from() > region.to())
                return result;

            BOOST_FOREACH(const entry_type& e, entries) {
                if(e.first.encloses(region)) {
                    result.push_back(e);
                }
            }
            return result;
        }
    };

}}

#endif