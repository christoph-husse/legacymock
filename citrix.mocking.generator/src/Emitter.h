
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_GENERATOR_EMITTER_H_
#define _CITRIX_MOCKING_GENERATOR_EMITTER_H_

#include <string>
#include <sstream>
#include <vector>

#include "Configuration.h"

namespace citrix { namespace mocking {

    class EmitIfdefGuard;

    class Emitter : protected Configuration, protected StringUtils {
        friend class EmitIfdefGuard;
    public:

        virtual ~Emitter() { }
        Emitter();

        void emit(std::string str = "");
        void emitln(std::string line = "");

        void emitParameterDeclarations(const std::vector<CppParameter>& params, std::string prefix = "");
        void emitParameterInvocation(const std::vector<CppParameter>& params, std::string prefix = "");
        std::string getParameterInvocation(const std::vector<CppParameter>& params, std::string prefix = "");
        void emitReturnStatement(std::string retType, std::string indent);

        std::string process();
    protected:
        virtual void processInternal() = 0;
    private:
        std::stringstream stream;
        int ifdefLevel;

        void enterIfdefLevel(int depthDelta);
        void leaveIfdefLevel(int depthDelta);
        int getCurrentIfdefLevel();
    };

    class EmitIfdefGuard {
    private:
        typedef std::pair<PPDirective, PPDirective> entry_type;

        void init(const std::vector<entry_type>& ifdefs);

        std::string outro;
        Emitter& emitter;
        int depthDelta;
    public:
        ~EmitIfdefGuard();
        EmitIfdefGuard(Emitter& emitter, const CppMethod& method);
        EmitIfdefGuard(Emitter& emitter, const CppClass& klass);
    };
}}

#endif