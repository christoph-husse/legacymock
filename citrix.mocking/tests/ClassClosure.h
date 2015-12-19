
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_GENERATOR_CLASSCLOSURE_H_
#define _CITRIX_MOCKING_GENERATOR_CLASSCLOSURE_H_

#include <vector>

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

        CodeRegion(int line) : lower(line), upper(line) { }
        CodeRegion(int lineFrom, int lineTo) : lower(lineFrom), upper(lineTo) { }
    };

    class IHasCodeRegion {
    public:
        virtual ~IHasCodeRegion() { }
        virtual CodeRegion codeRegion() const = 0;
        virtual void codeRegion(CodeRegion newRegion) = 0;
    };

    class HasCodeRegion : public IHasCodeRegion {
    private:
        CodeRegion region;
    public:
        CodeRegion codeRegion() const { return region; }
        void codeRegion(CodeRegion newRegion)  { region = newRegion; }
    };

}}

#endif