
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_TRACING_DEFAULTTRANSLATORS_H_
#define _CITRIX_MOCK_TRACING_DEFAULTTRANSLATORS_H_

#include <clocale>
#include <locale>
#include <vector>

#include "citrix/Platform.h"

#if !LEGACYMOCK_SUPPORTS_TRACING
#   error "Tracing is only supported since boost version 1.41!"
#endif


#include "Translator.h"

namespace citrix { namespace tracing {

    namespace internal {
        struct UnicodeSupport {
            static bool fromWide(const wchar_t* str, std::string& outUtf8);
            static bool fromWide(const std::wstring& str, std::string& outUtf8);
        };
    }
	 
#	define IMPLEMENT_DEFAULT_TRANSLATOR \
        T fromJson(const ptree& json) {\
            return T();\
        }\
        ptree toJson(const T& value) {\
            ptree result;\
            result.put("value", value);\
            return result;\
        }

    MAKE_TRANSLATOR_IF(boost::is_integral) { IMPLEMENT_DEFAULT_TRANSLATOR };
    MAKE_TRANSLATOR_IF(boost::is_float) { IMPLEMENT_DEFAULT_TRANSLATOR };
    MAKE_TRANSLATOR_IF(boost::is_enum) { IMPLEMENT_DEFAULT_TRANSLATOR };
    MAKE_TRANSLATOR_FOR(char*) { IMPLEMENT_DEFAULT_TRANSLATOR };
    MAKE_TRANSLATOR_FOR(void*) { IMPLEMENT_DEFAULT_TRANSLATOR };
    MAKE_TRANSLATOR_FOR(std::string) { IMPLEMENT_DEFAULT_TRANSLATOR };

    MAKE_TRANSLATOR_FOR(wchar_t*) {

        const wchar_t* fromJson(const ptree& json) {
            CITRIX_MOCK_UNREFPARAM(json);
            return NULL;
        }

        ptree toJson(const wchar_t* value) {
            ptree result;
            std::string utf8;

            if(!internal::UnicodeSupport::fromWide(value, utf8))
                utf8 = "{{{Invalid Unicode String}}}";

            result.put("value", utf8);
            return result;
        }
    };

    MAKE_TRANSLATOR_FOR(std::wstring) {

        const wchar_t* fromJson(const ptree& json) {
            CITRIX_MOCK_UNREFPARAM(json);
            return NULL;
        }

        ptree toJson(const std::wstring& value) {
            ptree result;
            result.put("value", internal::UnicodeSupport::fromWide(value));
            return result;
        }
    };

#	undef IMPLEMENT_DEFAULT_TRANSLATOR

}}

#endif