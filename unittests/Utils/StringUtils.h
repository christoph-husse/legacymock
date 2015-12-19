
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_UNITTESTS_STRINGUTILS_H_
#define _CITRIX_MOCK_UNITTESTS_STRINGUTILS_H_

#include <string>
#include <sstream>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>

namespace unittests {
    template<class TTo, class TFrom>
    TTo forceCast(const TFrom& ptr) {
        return *((TTo*)&ptr);
    }

    static inline std::string stringConcat() { return ""; }
    
    #define BOOST_PP_LOCAL_MACRO(N)                                                     \
        template <class T                                                               \
            BOOST_PP_COMMA_IF(N)                                                        \
            BOOST_PP_ENUM_PARAMS(N, class A)>                                           \
        static std::string stringConcat(T head BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) { \
            std::ostringstream sout;                                                    \
            sout << head << stringConcat(BOOST_PP_ENUM_PARAMS(N, a));                   \
            return sout.str();                                                          \
        }
            
    #define BOOST_PP_LOCAL_LIMITS (0, 10)
    #include BOOST_PP_LOCAL_ITERATE()
}

#if LEGACYMOCK_SUPPORTS_TRACING

#include "boost_json_parser_hotfix.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace unittests {

    static inline std::string boostJson(boost::property_tree::ptree input) {
        std::ostringstream sout;
        boost::property_tree::json_parser::write_json(sout, input);
        return sout.str();
    } 

    static inline std::string boostJson(std::string json) {
        std::stringstream sin(json);
        std::ostringstream sout;
        boost::property_tree::ptree ptree;

        boost::property_tree::json_parser::read_json(sin, ptree);
        boost::property_tree::json_parser::write_json(sout, ptree);

        return sout.str();
    } 

    template<class T>
    static std::string integralToString(T value) {
        boost::property_tree::ptree tree;
        tree.put("value", value);

        std::ostringstream sout;
        boost::property_tree::json_parser::write_json(sout, tree);
        std::string tmp = sout.str();

        int right = tmp.rfind('"');
        int left = tmp.rfind('"', right - 1);
        tmp = tmp.substr(left + 1, right - left - 1);

        return tmp;
    }
}

#endif

#endif