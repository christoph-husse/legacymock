
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "Emitter.h"

namespace citrix { namespace mocking {

    using namespace boost::xpressive;

    Emitter::Emitter() : ifdefLevel(0) { }

    void Emitter::emit(std::string str) {
        stream << str;
    }

    void Emitter::emitln(std::string line) {
        stream << line << std::endl;
    }

    std::string Emitter::process() {
        processInternal();
        return stream.str();
    }

    std::string Emitter::getParameterInvocation(const std::vector<CppParameter>& params, std::string prefix) {
        std::string result;

        for(size_t i = 0; i < params.size(); i++) {
            const CppParameter& p = params[i];
            result += prefix + p.name;
            if(i + 1 < params.size())
                result += ", ";
        }

        return result;
    }

    void Emitter::emitParameterInvocation(const std::vector<CppParameter>& params, std::string prefix) {
        emit(getParameterInvocation(params, prefix));
    }

    void Emitter::emitParameterDeclarations(const std::vector<CppParameter>& params, std::string prefix) {
        for(size_t i = 0; i < params.size(); i++) {
            const CppParameter& p = params[i];
            emitln();
            emit("                " + p.type + " " + prefix + p.name);
            if(i + 1 < params.size())
                emit(",");
        }
    }

    void Emitter::emitReturnStatement(std::string retType, std::string indent) {
        emitln(indent + "return ::citrix::mocking::internal::allocateNullInstance<" + retType + ">();");
    }

    void Emitter::enterIfdefLevel(int depthDelta) {
        ifdefLevel += depthDelta;
    }

    void Emitter::leaveIfdefLevel(int depthDelta) {
        ifdefLevel -= depthDelta;

        assert(ifdefLevel >= 0 && "The current ifdef level is smaller than zero. This shouldn't happen.");
    }

    int Emitter::getCurrentIfdefLevel() {
        return ifdefLevel;
    }

    EmitIfdefGuard::EmitIfdefGuard(Emitter& emitter, const CppMethod& method) : emitter(emitter), depthDelta(0) {
        init(method.ifdefs);
    }

    EmitIfdefGuard::EmitIfdefGuard(Emitter& emitter, const CppClass& klass) : emitter(emitter), depthDelta(0) {
        init(klass.ifdefs);
    }

    void EmitIfdefGuard::init(const std::vector<entry_type>& ifdefs) {

        std::string intro;
        int depth = 0;

        for(size_t i = emitter.getCurrentIfdefLevel(); i < ifdefs.size(); i++) {
            intro += ifdefs[i].first.combinedExpression + "\n";
            depth++;
        }

        for(size_t i = 0; i < depth; i++) {
            outro += "#endif\n";
        }

        depthDelta = ifdefs.size() - emitter.getCurrentIfdefLevel();
        emitter.enterIfdefLevel(depthDelta);

        if(depthDelta > 0)
            emitter.emit(intro);
    }

    EmitIfdefGuard::~EmitIfdefGuard() {
        if(depthDelta > 0)
            emitter.emit(outro);

        emitter.leaveIfdefLevel(depthDelta);
    }
}}