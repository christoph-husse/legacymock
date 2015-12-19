
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/Platform.h"

#if LEGACYMOCK_SUPPORTS_TRACING

#include "../citrix.tracing/include/citrix/tracing/DefaultTranslators.h"

#ifdef _WIN32
#   include <Windows.h>
#else
#   include <boost/locale.hpp>
#endif

namespace citrix { namespace tracing { namespace internal {

#ifdef _WIN32
    bool UnicodeSupport::fromWide(const wchar_t* str, std::string& outUtf8) {
        std::string result;

        int requiredBytes = WideCharToMultiByte(
            CP_UTF8,
            0,
            str,
            -1,
            &result[0],
            0,
            NULL,
            NULL);

        if(requiredBytes == 0)
            return false;

        result.resize(requiredBytes);

        WideCharToMultiByte(
            CP_UTF8,
            0,
            str,
            -1,
            &result[0],
            result.size(),
            NULL,
            NULL);

        return true;
    }
#else
    bool UnicodeSupport::fromWide(const wchar_t* str, std::string& outUtf8) {
        static boost::locale::generator gen;
        static std::locale loc(gen(""));

        outUtf8 = boost::locale::conv::from_utf(str, loc);
        return true;
    }
#endif

    bool UnicodeSupport::fromWide(const std::wstring& str, std::string& outUtf8) {
        return fromWide(str.c_str(), outUtf8);
    }

}}}

#endif
