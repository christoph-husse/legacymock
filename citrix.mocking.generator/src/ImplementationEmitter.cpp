
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "ImplementationEmitter.h"

namespace citrix { namespace mocking {

    void ImplementationEmitter::emitMockBaseMethod(CppMethod m) {
    }

    void ImplementationEmitter::emitGoogleMockMethod(CppMethod m) {
    }

    std::string ImplementationEmitter::getMethodSignature(CppMethod m, std::string typeDef, std::string returnType) {
        std::string classPrefix, classPtr, mockPrefix;

        if(m.isStatic) {
            classPrefix = "";
            classPtr = "";
            mockPrefix = "";
        } else {
            classPrefix = ", " + className;
            classPtr = "(" + className + "::*)";
            mockPrefix = "CitrixMockingAccessor<" + className + ">::";
        }

        std::string paramList = "";
        for(size_t i = 0; i < m.parameters.size(); i++) {
            paramList += m.parameters[i].type;
            if(m.parameters.size() > i + 1)
                paramList += ", ";
        }

        std::string methodSuffix = "";
        if(m.isConst)
            methodSuffix = " const";

        if(returnType.empty())
            returnType = m.returnType;

        return "member_function_pointer_traits<" + returnType + classPtr + "(" + paramList + ")" + methodSuffix + classPrefix + ">::type " + typeDef;
    }

    std::string ImplementationEmitter::getMethodAddress(CppMethod m) {
        if(isFreeApi)
            return "&" + classToMock->nameSpace + "::" + m.name;
        else
            return "&" + classToMock->nameSpace + "::" + className + "::" + m.name;
    }

    void ImplementationEmitter::traceUpdatedParams(CppMethod m) {
        emit("            trace");
        BOOST_FOREACH(const CppParameter& p, m.parameters) {
            emitln("");
            emit("                .updateParameter(\"" + p.name + "\", ___" + p.name + ")");
        }
        if(!m.parameters.empty()) {
            emitln("");
            emit("                ");
        }
    }

    void ImplementationEmitter::processInternal() {
        
    // ####################################################################################################################
    // ##    File header
    // ####################################################################################################################

        emitln("// This file was automatically generated.");
        emitln();
        emitln("#include \"" + mockBaseName + ".h\"");
        emitln();
        emitln("#include \"../citrix.hooking/include/citrix/hooking/Hooking.h\"");
        emitln("#include \"../citrix.mocking/include/citrix/mocking/MockManager.h\"");
        emitln("#include \"../citrix.mocking/include/citrix/mocking/MockWrapper.h\"");
        emitln("#include \"../citrix.mocking/include/citrix/mocking/VirtualCallDetector.h\"");
        emitln("#include \"citrix/type_traits/make_reference.hpp\"");
		emitln("#include \"citrix/type_traits/normalize_type.hpp\"");

        if(enableTracing) {
            emitln("#if LEGACYMOCK_SUPPORTS_TRACING");
            emitln("#   include \"../citrix.tracing/include/citrix/tracing/DefaultTranslators.h\"");
            emitln("#endif");
        }

        emitln();

        EmitIfdefGuard ifdefGuard(*this, *classToMock);

        std::string nameSpace = classToMock->nameSpace;
        bool hasNamespace = !nameSpace.empty();

        if(hasNamespace) {
            std::string components;
            BOOST_FOREACH(const std::string& ns, split(nameSpace, "::")) {
                if(!components.empty())
                    components += "::";
                components = ns;
                emitln("using namespace " + components + ";");
            }
            emitln();
        }

        emitln("using namespace citrix::type_traits;");
        emitln();

// ####################################################################################################################
// ##    Class preamble
// ####################################################################################################################

        if(!classPrefix.empty()) {
            emitln(classPrefix);
            emitln();
        }

        BOOST_FOREACH(const std::string& typeDef, typeDefs) {
            emitln(typeDef);
        }

        emitln();

        if(Configuration::debugOmitImpl)
            return;

        emitln("template<>");
        emitln("class CitrixMockingAccessor<" + className + "> : public citrix::mocking::internal::MockManagerAccessor<" + className + "> {");
        emitln();

        emitln("private:");
        emitln("    struct EmptyObject {};");

        BOOST_FOREACH(const std::string& ln, publicMockInlines) {
            emitln("    " + ln);
        }

        emitln();

        for(size_t x = 0; x < methodsToMock.size(); x++) {
            const CppMethod& m = methodsToMock[x];
            const std::vector<CppParameter>& params = m.parameters;

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

// ####################################################################################################################
// ##    FunctionAddressDetector
// ####################################################################################################################

            if(!m.isStatic) {

                std::string indexedName = m.name + str(x);

                emitln("    class FunctionAddressDetector_" + indexedName + " : citrix::mocking::internal::VirtualCallDetector {");
                emitln("    public:");
                emitln("        typedef " + getMethodSignature(m, "TSignature") + ";");
                emitln("    private:");
                emitln("        static size_t address;");
                emitln();
                emit("        ");
                emit(m.returnType);
                emit(" patchedStub(");
                emitParameterDeclarations(m.parameters);
                emitln(") { ");
                BOOST_FOREACH(const CppParameter& p, params) {
                    emitln("            CITRIX_MOCK_UNREFPARAM(" + p.name + ");");
                }
                emitln("            address = getVirtualCallAddressFromTls();");
                emitReturnStatement(m.returnType, "            ");
                emitln("        }");

                emitln();
                emitln("        FunctionAddressDetector_" + indexedName + "(" + className + "* instance) {");
                emitln("            void *localStart = NULL, *localEnd = NULL;");
                emitln("            const TSignature cppAddress = " + getMethodAddress(m) + ";");
                emitln();
                emitln("            address = *((size_t*)&cppAddress);");
                emitln();
                emitln("            for (int i = 0; i < 2; i++) {");
                emitln("                localStart = Utils::getInstructionPointer();");
                emitln("                if ((i == 1) && tryInitialize(localStart, localEnd, methodPtrToPVoid(");
                emitln("                        &FunctionAddressDetector_" + indexedName + "::patchedStub))) ");
                emitln("                {");

                for(size_t y = 0; y < params.size(); y++) {
                    const CppParameter& p = params[y];
                    emitln("                    boost::shared_ptr<::citrix::type_traits::normalize_type<" + p.type + ">::type> param" + 
                            str(y) + " = ::citrix::mocking::internal::allocateDefaultInstance<::citrix::type_traits::normalize_type<" + p.type + ">::type>();");
                }

                emit("                    instance->" + m.name + "(");
                for(size_t y = 0; y < params.size(); y++) {
                    const CppParameter& p = params[y];
                    emit("*param" + str(y));
                    if(y + 1 < params.size())
                        emit(", ");
                }

                emitln(");");

                emitln("                    failFastRelativeCalls();");
                emitln("                    dummyCallForMoreBytes();");
                emitln("                }");
                emitln();
                emitln("                localEnd = Utils::getInstructionPointer();");
                emitln("            }");
                emitln("        }");
                emitln();
                emitln("    public:");
                emitln("        static TSignature getAddress(" + className + "* allocatedType) {");
                emitln("            static FunctionAddressDetector_" + indexedName + " instance(allocatedType);");
                emitln("            return *((TSignature*)&address);");
                emitln("        }");
                emitln("    };");
                emitln();
            }
// ####################################################################################################################
// ##    Hooked function implementation
// ####################################################################################################################

            emit("    ");
            if(m.isStatic)
                emit("static ");
            emit(m.rawReturnType);
            emit(" ___");
            emit(m.name);
            emit("(");
            emitParameterDeclarations(m.parameters, "___");
            emit(")");

            if(m.isConst)
                emit(" const");

            emitln(" {");

            std::string invokePrefix = "";
            if(m.isStatic) {
                if(!isFreeApi)
                    invokePrefix = className + "::";
                else
                    invokePrefix = nameSpace + "::";
            } else {
                invokePrefix += "((" + className + "*)this)->";
            }

            std::string invokeParams = "(" + getParameterInvocation(m.parameters, "___") + ")";
            std::string invokeMethod = m.name + invokeParams;
            std::string indexedName = m.name + str(x);

            emitln("        ::citrix::hooking::HookRecursionGuard guard(get().mock__" + m.name + str(x) + ");");         
            emitln("        ::boost::shared_ptr<::citrix::mocking::internal::MockManager<" + className + "> > mgr = ::citrix::mocking::internal::MockManager<" + className + ">::get();");
            emitln();
            emitln("        if(!mgr->isActive<" + getMethodSignature(m, "", m.rawReturnType) + ">(" + getMethodAddress(m) + ")) {");
            if(!m.isStatic) {
                emitln("            FunctionAddressDetector_" + indexedName + "::TSignature callAddr = FunctionAddressDetector_" + indexedName + "::getAddress(NULL);");
                emitln("            // *Facepalm* This will treat normal calls, virtual calls, etc. all like normal calls");
                emitln("            // This is necessary because the vtable pointer has been resolved by the time we get here (no it's not confusing).");
                emitln("            // The current this pointer will already be shifted, and a normal virtual call would crash here...");
                emitln("            return (");
                emitln("                ((EmptyObject*)this)->*");
                emitln("                *(");
                emitln("                    (::citrix::type_traits::member_function_pointer_traits<FunctionAddressDetector_" + indexedName + "::TSignature, EmptyObject>::type*)");
                emitln("                    &callAddr");
                emitln("                )");
                emitln("            )" + invokeParams + ";");
            } else {
                emitln("            return " + invokePrefix + invokeMethod + ";");
            }

            emitln("        }");
            emitln();
            if(!m.isStatic) {
                emitln("        TMockInstance* mock = resolveMockForPointer((void*)this);");
                emitln("        mock->setRealThis((" + className + "*)this);");
                emit(  "        return mock");
            } else {
                emit("        return resolveStaticMock()");
            }

            emitln("->" + invokeMethod + ";");
            emitln("    }");
            emitln();
        }
        emitln();
        
// ####################################################################################################################
// ##    get();
// ####################################################################################################################

        emitln();
        emitln("    bool areHooksInstalled;");
        emitln("    boost::shared_ptr<" + className + "> probingInstance;");
        emitln();
        emitln("    static CitrixMockingAccessor<" + className + ">& get() {");
        emitln("        static CitrixMockingAccessor<" + className + "> hooks;");
        emitln("        return hooks;");
        emitln("    }");
        emitln();
        emitln("    static boost::shared_ptr<" + className + "> createProbingInstance() {");

        if(pureStaticClass) {
            emitln("        return boost::shared_ptr<" + className + ">();");
        } else {
            if(!customProbingInstance.empty()) {
                emitln("        return ::citrix::mocking::internal::allocateDefaultInstance<" + customProbingInstance + ">();");
            } else {
                std::map<int, CppMethod> ctorCandidates;

                BOOST_FOREACH(const CppMethod& m, classToMock->methods) {
                    if(m.isConstructor && (m.accessibility == Accessibility::Public))
                        ctorCandidates[m.parameters.size()] = m;
                }

                CppMethod ctor;
                if(ctorCandidates.size() > 0) {
                    ctor = ctorCandidates.begin()->second;
                }

                emit("        return ::citrix::mocking::internal::allocateDefaultInstance<" + className + ">(");

                if(ctorCandidates.size() > 0) {
                    for(size_t y = 0; y < ctor.parameters.size(); y++) {
                        const CppParameter& p = ctor.parameters[y];
                        emitln();
                        emit("                ::citrix::mocking::internal::allocateNullInstance<" + p.type + ">()");
                        if(y + 1 < ctor.parameters.size())
                            emit(", ");
                    }
                }

                emitln(");");
            }
        }

        emitln("    }");
        emitln();

// ####################################################################################################################
// ##    TypedHook member variables
// ####################################################################################################################

        for(size_t x = 0; x < methodsToMock.size(); x++) {
            const CppMethod& m = methodsToMock[x];

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

            emit("    ::citrix::hooking::TypedHook<");
            emit(getMethodSignature(m, "", m.rawReturnType));
            emit(", CitrixMockingAccessor<" + className + ">");
            emit(">");
            emitln(" mock__" + m.name + str(x) + ";");
        }

// ####################################################################################################################
// ##    Constructor
// ####################################################################################################################

        emitln();
        emitln("public:");
        emitln("    CitrixMockingAccessor() :");

        for(size_t x = 0; x < methodsToMock.size(); x++) {
            const CppMethod& m = methodsToMock[x];
            std::vector<CppParameter> params = m.parameters;
            std::string paramList = "";

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

            std::string targetName = m.name;

            if(m.isConstructor)
                targetName = className;
            if(m.isDestructor)
                targetName = "~" + className;

            emit("        mock__" + m.name + str(x) + "(");

            if(m.isStatic)
                emit(getMethodAddress(m));
            else
                emit("FunctionAddressDetector_" + targetName + str(x) + "::getAddress(probingInstance.get())");

            emitln(", &CitrixMockingAccessor<" + className + ">::___" + m.name + "), ");
        }

        emit("        probingInstance(createProbingInstance())");

        emitln();
        emitln("    {");
        emitln("        areHooksInstalled = true");
        for(size_t x = 0; x < methodsToMock.size(); x++) {
            const CppMethod& m = methodsToMock[x];

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);
            emitln("            && mock__" + m.name + str(x) + ".isInstalled()");
        }

        emitln("        ;");
        emitln("    }");
        emitln();
        emitln("    static bool tryToInstall() {");
        emitln("        return get().areHooksInstalled;");
        emitln("    }");
        emitln();

// ####################################################################################################################
// ##    Accessors
// ####################################################################################################################
        emitln();

        BOOST_FOREACH(const CppMethod& m, methodsAndVariablesToMock) {

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

            emit("    static ");
            emit(m.returnType);
            emit(" ");
            emit(className + "Accessor__" + m.name);
            emit("(");

            if(!m.isStatic)
                emit(className + "* instance");

            if(!m.parameters.empty()) {
                if(!m.isStatic)
                    emit(", ");
            }

            emitParameterDeclarations(m.parameters, "___");
            emitln(") {");

            if(!m.isGetter) {
                emitln("        ::citrix::mocking::internal::DisableMemberMockGuard<" + getMethodSignature(m, "", m.rawReturnType) + "> guard((" + getMethodAddress(m) + "));");

                if(m.isStatic) {
                    if(!isFreeApi)
                        emit("        return " + className + "::" + m.name + "(");
                    else
                        emit("        return " + nameSpace + "::" + m.name + "(");
                } else {
                    emit("        return instance->" + m.name + "(");
                }

                emitParameterInvocation(m.parameters, "___");
                emitln(");");
            } else {
                if(m.isStatic)
                    emit("        return ::citrix::type_traits::make_reference(&" + className + "::");
                else
                    emit("        return ::citrix::type_traits::make_reference(&instance->");

                emitln(m.name + ");");
            }

            emitln("    }");
            emitln();
        }

// ####################################################################################################################
// ##    Tacer implementations with class accessors
// ####################################################################################################################
        if(enableTracing) {
            emitln();
            emitln("#if LEGACYMOCK_SUPPORTS_TRACING");

            for(size_t x = 0; x < methodsToMock.size(); x++) {
                const CppMethod& m = methodsToMock[x];

                if(pureStaticClass && !m.isStatic)
                    continue;

                EmitIfdefGuard ifdefGuard(*this, m);

                std::string traceMethodName = className + "Tracer__" + m.name;
                std::string invokePrefix, thisParameter;
                if(m.isStatic) {
                    invokePrefix += className;
                    invokePrefix += "::";
                } else {
                    invokePrefix += "realThis->";
                    thisParameter = ", " + className + "* realThis";
                }

                std::string  invokeMethod = m.name + "(";
                for(size_t i = 0; i < m.parameters.size(); i++) {
                    const CppParameter& p = m.parameters[i];
                    invokeMethod += "___" + p.name;

                    if(m.parameters.size() > i + 1)
                        invokeMethod += ", ";
                }
                invokeMethod += ")";

                emitln("    template<class T>");
                emitln("    struct Returns__" + traceMethodName + str(x) + " {");
                emit("        " + m.returnType + " operator()(::citrix::tracing::CallTrace& trace" + thisParameter);
                if(!m.parameters.empty()) {
                    emit(", ");
                    emitParameterDeclarations(m.parameters, "___");
                }
                emitln(") {");
                
                emitln("            " + m.returnType);
                emitln("                result = " + invokePrefix + invokeMethod + ";");

                traceUpdatedParams(m);
                emitln(".endInvoke(\"" + m.rawReturnType + "\", result);");
                emitln("            return result;");
                emitln("        }");
                emitln("    };");
                emitln();
                emitln("    template<>");
                emitln("    struct Returns__" + traceMethodName + str(x) + "<void> {");
                emit("        void operator()(::citrix::tracing::CallTrace& trace" + thisParameter);
                if(!m.parameters.empty()) {
                    emit(", ");
                    emitParameterDeclarations(m.parameters, "___");
                }
                emitln(") {");
                    emitln("            " + invokePrefix + invokeMethod + ";");
                    traceUpdatedParams(m);
                    emitln(".endInvoke();");
                    emitln("        }");
                emitln("    };");
                emitln();

                emit("    static ");
                emit(m.returnType);
                emit(" ");
                emit(traceMethodName + "(" + className + "Tracer* tracer");
                if(!m.parameters.empty())
                    emit(",");

                emitParameterDeclarations(m.parameters, "___");
                emitln(") { ");
                emitln("        ::citrix::tracing::CallTrace trace(tracer);");
                emitln();

                emit("        trace.beginInvoke(\"" + className + "::" + m.name + "\")");
                BOOST_FOREACH(const CppParameter& p, m.parameters) {
                    emitln();
                    emit("           .addParameter(\"" + p.type + "\", \"" + p.name + "\", ___" + p.name + ")");
                }

                emitln(";");

                emitln();
                emitln("        try {");

                emit("            return Returns__" + traceMethodName + str(x) + "<" + m.returnType + ">()(trace");
                emit(m.isStatic ? "" : ", tracer->threadLocalThis");
                if(!m.parameters.empty())
                    emitln(", ");
                emitParameterInvocation(m.parameters, "___");
                emitln(");");
                emitln();

                emitln("        } catch(::std::exception& ex) {");
                emitln("            trace.fail(ex);");
                emitln("            throw;");
                emitln("        }");
                emitln("    }");
                emitln();
            }

            emitln("#endif");
        }

        emitln("};");
        emitln();

// ####################################################################################################################
// ##    Adress variable definitions
// ####################################################################################################################
        for(size_t x = 0; x < methodsToMock.size(); x++) {
            const CppMethod& m = methodsToMock[x];

            if(pureStaticClass && !m.isStatic)
                continue;

            if(m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

            std::string indexedName = m.name + str(x);

            emitln("size_t CitrixMockingAccessor<" + className + ">::FunctionAddressDetector_" + m.name + str(x) + "::address;");
        }

// ####################################################################################################################
// ##    Accessors
// ####################################################################################################################
        emitln();

        BOOST_FOREACH(const CppMethod& m, methodsAndVariablesToMock) {

            if(pureStaticClass && !m.isStatic)
                continue;

            EmitIfdefGuard ifdefGuard(*this, m);

            emit(m.returnType);
            emit(" ");
            emit(className + "Accessor::" + m.name);
            emit("(");

            if(!m.isStatic)
                emit(className + "* instance");

            if(!m.parameters.empty()) {
                if(!m.isStatic)
                    emit(", ");
            }
            emitParameterDeclarations(m.parameters, "___");
            emitln(") {");
            emit("    return CitrixMockingAccessor<" + className + ">::" + className + "Accessor__" + m.name + "(");

            if(!m.isStatic)
                emit("instance");

            if(!m.parameters.empty()) {
                if(!m.isStatic)
                    emit(", ");
            }

            emitParameterInvocation(m.parameters, "___");

            emitln(");");
            emitln("}");
            emitln();
        }
// ####################################################################################################################
// ##    Tacers
// ####################################################################################################################
        if(enableTracing) {
            emitln();
            emitln("#if LEGACYMOCK_SUPPORTS_TRACING");
            BOOST_FOREACH(const CppMethod& m, methodsToMock) {

                if(pureStaticClass && !m.isStatic)
                    continue;

                EmitIfdefGuard ifdefGuard(*this, m);

                emit(m.returnType);
                emit(" ");
                emit(className + "Tracer::" + m.name);
                emit("(");
                emitParameterDeclarations(m.parameters, "___");
                emit(")");

                if(m.isConst)
                    emit(" const");

                emitln(" { ");

                emitln("    return CitrixMockingAccessor<" + className + ">::" + className + "Tracer__" + m.name + "(");
                emit("        const_cast<" + className + "Tracer*>(this)");
                if(!m.parameters.empty())
                    emit(", ");
                emitParameterInvocation(m.parameters, "___");
                emitln(");");
                emitln("}");
                emitln();
            }
            emitln("#endif");
        }

// ####################################################################################################################
// ##    File footer
// ####################################################################################################################

        std::string nsPrefix = "";
        if(hasNamespace)
            nsPrefix = nameSpace + "::";

        emitln();
        emitln("namespace citrix { namespace mocking { namespace internal {");
        emitln("    template<>");
        emitln("    bool tryToInstallHooks<" + nsPrefix + className + ">() {");
        emitln("        return CitrixMockingAccessor<" + className + ">::tryToInstall();");
        emitln("    }");
        emitln("}}}");
        emitln();
        

    }
}}