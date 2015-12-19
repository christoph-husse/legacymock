
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_TRACING_CALLSERIALIZER_H_
#define _CITRIX_TRACING_CALLSERIALIZER_H_

#include "citrix/Platform.h"

#if !LEGACYMOCK_SUPPORTS_TRACING
#   error "Tracing is only supported since boost version 1.41!"
#endif


#include "Translator.h"
#include "IndirectionResolver.hpp"

namespace citrix { namespace tracing {

    class CallTrace;

    class TracerBase {
    public:
        virtual ~TracerBase() { }
        virtual void onTrace(CallTrace& trace) = 0;
    };

    class CallTrace {
    private:
        typedef boost::property_tree::ptree ptree;

        ptree props;
        TracerBase* tracer;
    public:

        ptree getPtree() const { return props; }

        CallTrace(TracerBase* tracer = NULL) : tracer(tracer) { }

        CallTrace& beginInvoke(std::string methodName) {
            props.add("method", methodName);
            return *this;
        }

        template<class TResult>
        void endInvoke(std::string resultType, const TResult& result) {
            props.put_child("return", internal::IndirectionResolver<TResult>::toPropertyTree(result));
            props.put("return.type", resultType);

            if(tracer)
                tracer->onTrace(*this);
        }

        void endInvoke() {
            if(tracer)
                tracer->onTrace(*this);
        }

        void fail(std::exception& ex) {
            CITRIX_MOCK_UNREFPARAM(ex);

            if(tracer)
                tracer->onTrace(*this);
        }

        template<class TParam>
        CallTrace& addParameter(std::string paramType, std::string paramName, const TParam& param) {
            ptree json = internal::IndirectionResolver<TParam>::toPropertyTree(param);

            if(!json.empty())
                props.put_child("params." + paramName, json);

            props.put("params." + paramName + ".type", paramType);

            return *this;
        }


        template<class TParam>
        CallTrace& updateParameter(std::string paramName, const TParam& param) {
            ptree json = internal::IndirectionResolver<TParam>::toPropertyTree(param);

            std::string in = props.get<std::string>("params." + paramName + ".value");
            std::string out = json.get<std::string>("value");

            if(in.empty() && out.empty()) {
                ptree in = props.get_child("params." + paramName + ".value", ptree());
                ptree out = json.get_child("value", ptree());

                if(in != out) {
                    props.put_child("outParams." + paramName, out);
                }
            }
            else if(in != out) {
                props.put("outParams." + paramName, out);
            }

            return *this;
        }
    };
  
}}

#endif