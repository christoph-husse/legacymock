
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "HeaderEmitter.h"

namespace citrix { namespace mocking {

    void HeaderEmitter::emitMockBaseMethod(CppMethod m) {
        {
            EmitIfdefGuard ifdefGuard(*this, m);

            emit("    virtual ");
            emit(m.returnType);
            emit(" ");
            emit(m.name);
            emit("(");
            emitParameterDeclarations(m.parameters);
            emit(")");

            if(m.isConst) {
                emit(" const");
            }

            if(enableDefaultImpl) {
                emitln(" { ");
                BOOST_FOREACH(const CppParameter& p, m.parameters) {
                    emitln("        CITRIX_MOCK_UNREFPARAM(" + p.name + ");");
                }
                emitReturnStatement(m.returnType, "        ");
                emitln("    }");
            } else {
                emitln(" = 0;");
            }
        }

        emitln();
    }


    void HeaderEmitter::emitGoogleMockMethod(CppMethod m) {
        {
            EmitIfdefGuard ifdefGuard(*this, m);

            std::string constSpec = "";
            if(m.isConst)
                constSpec = "CONST_";

            emit("    MOCK_" + constSpec + "METHOD" + str(m.parameters.size()) + "(");

            emit(m.name);
            emit(", typename ");

            emit(m.returnType);
            emit("(");
            emitParameterDeclarations(m.parameters);
            emitln("));");
        }

        emitln();
    }

    void HeaderEmitter::processInternal() {
        emitln("// This file was automatically generated.");
        emitln();

        emitln("#ifndef __MOCK_" + mockBaseName + "_" + headerGuard);
        emitln("#define __MOCK_" + mockBaseName + "_" + headerGuard);
        emitln();

        BOOST_FOREACH(const std::string& inc, publicIncludes) {
            if(!contains(inc, "<")) {
                emitln("#include \"" + inc + "\"");
            } else {
                emitln("#include " + inc);
            }
        }

        emitln();

        {
            EmitIfdefGuard ifdefGuard(*this, *Configuration::classToMock);

            emitln("#include <boost/type_traits/add_reference.hpp>");
            if(!disableGMock) {
                emitln("#include <gmock/gmock.h>");
                emitln();
            }

            emitln("#include \"citrix/Platform.h\"");
            emitln("#include \"citrix/type_traits/member_function_pointer_traits.hpp\"");
            emitln("#include \"citrix/type_traits/return_type_of_function.hpp\"");
            emitln("#include \"../citrix.mocking/include/citrix/mocking/MockManager.h\"");

            if(enableTracing) {
                emitln("#if LEGACYMOCK_SUPPORTS_TRACING");
                emitln("#   include \"../citrix.tracing/include/citrix/tracing/CallTrace.h\"");
                emitln("#endif");
            }

            emitln();
            emitln("namespace citrix { ");
            emitln("    namespace mocking { namespace internal {");
            emitln("            template<class T> struct MockTraits;");
            emitln("    }}");
            emitln();
            emitln("    namespace tracing {");
            emitln("        class TracerBase;");
            emitln("    }");
            emitln("}");
            emitln();

            std::string nameSpace = classToMock->nameSpace;
            bool hasNamespace = !nameSpace.empty();

            if(hasNamespace) {
                BOOST_FOREACH(const std::string& ns, split(nameSpace, "::")) {
                    emit("namespace ");
                    emit(ns);
                    emit(" { ");
                }
                emitln();
                emitln();
            }

           //  #############################################################################################
            //###     Abstract instance mock interface


            emitln("class " + mockBaseName + "Prefix {");
            emitln("public:");
            emitln("    virtual ~" + mockBaseName + "Prefix() { }");
            emitln();

            if(!classPrefix.empty()) {
                emitln("    " + classPrefix);
            }

            BOOST_FOREACH(const std::string& typeDef, typeDefs) {
                emitln("    " + typeDef);
            }

            emitln("};");
            emitln();

            emitln("class " + mockBaseName + "Instance : public virtual " + mockBaseName + "Prefix {");
        
            if(!pureStaticClass) {
                emitln("public:");

                emitln("    virtual void setRealThis(" + className + "* realThis) {");
                emitln("        CITRIX_MOCK_UNREFPARAM(realThis);");
                emitln("    }");
                emitln();

                BOOST_FOREACH(const CppMethod& m, methodsToMock) {
                    if(m.isStatic)
                        continue;

                    emitMockBaseMethod(m);
                }
            }

            emitln("};");
            emitln();

        
            //#############################################################################################
            //###     Abstract static mock interface

            emitln("class " + mockBaseName + "Static : public virtual " + mockBaseName + "Prefix {");
            emitln("public:");

            BOOST_FOREACH(const CppMethod& m, methodsToMock) {
                if(!m.isStatic) 
                    continue;

                emitMockBaseMethod(m);
            }

            emitln("};");
            emitln();


            emitln("class " + mockBaseName + " : public virtual " + mockBaseName + "Static, public virtual " + mockBaseName + "Instance {");
            emitln("};");
            emitln();

            if(!disableGMock) {
                //#############################################################################################
                //###     Instance Google Mock

                emitln("#ifdef _MSC_VER");
                emitln("#   pragma warning (push)");
                emitln("#   pragma warning (disable: 4373) // previous versions of compiler did not override");
                emitln("#endif");
                emitln();

                emitln();
                emitln("class " + gmockName + "Instance : public virtual " + mockBaseName + "Instance {");

                if(!pureStaticClass) {
                    emitln("public:");

                    BOOST_FOREACH(const std::string& ln, publicMockInlines) {
                        emitln("    " + ln);
                    }

                    emitln();

                    BOOST_FOREACH(const CppMethod& m, methodsToMock) {
                        if(m.isStatic)
                            continue;
                    
                        emitGoogleMockMethod(m);
                    }
                }

                emitln("};");
                emitln();


                //#############################################################################################
                //###     Static Google Mock

                emitln("class " + gmockName + "Static : public virtual " + mockBaseName + "Static {");
                emitln("public:");

                BOOST_FOREACH(const std::string& ln, publicMockInlines) {
                    emitln("    " + ln);
                }

                emitln();

                BOOST_FOREACH(const CppMethod& m, methodsToMock) {
                    if(!m.isStatic)
                        continue;
                
                    emitGoogleMockMethod(m);
                }

                emitln("};");

                emitln("#ifdef _MSC_VER");
                emitln("#   pragma warning (pop)");
                emitln("#endif");
            }

        
            std::string nsPrefix = "";
            if(hasNamespace)
                nsPrefix = nameSpace + "::";


            //#############################################################################################
            //###     Accessor

            emitln();
            emitln("class " + className + "Accessor {");
            emitln("public:");

            if(!classPrefix.empty()) {
                emitln("    " + classPrefix);
                emitln();
            }

            if(!typeDefs.empty()) {
                BOOST_FOREACH(const std::string& typeDef, typeDefs) {
                    emitln("    " + typeDef);
                }
                emitln();
            }

            BOOST_FOREACH(const CppMethod& m, methodsAndVariablesToMock) {

                if(pureStaticClass && !m.isStatic)
                    continue;

                {
                    EmitIfdefGuard ifdefGuard(*this, m);

                    emit("    static ");
                    emit(m.returnType);
                    emit(" ");
                    emit(m.name);
                    emit("(");

                    if(!m.isStatic) {
                        emit(className + "* instance");

                        if(!m.parameters.empty()) 
                            emit(",");
                    }

                    emitParameterDeclarations(m.parameters, "___");

                    emitln(");");
                }
                emitln();
            }

            emitln("};");
            emitln();

            //#############################################################################################
            //###     Tracing

            if(enableTracing) {
                emitln("#if LEGACYMOCK_SUPPORTS_TRACING");
                emitln();
                emitln("class " + className + "Tracer : public virtual " + mockBaseName + "Static, public virtual " + mockBaseName + "Instance, public ::citrix::tracing::TracerBase {");
                emitln("    template<typename> friend class ::CitrixMockingAccessor;");
                emitln("public:");

                if(!classPrefix.empty()) {
                    emitln("    " + classPrefix);
                    emitln();
                }
            
                if(!typeDefs.empty()) {
                    BOOST_FOREACH(const std::string& typeDef, typeDefs) {
                        emitln("    " + typeDef);
                    }
                    emitln();
                }

                BOOST_FOREACH(const CppMethod& m, methodsAndVariablesToMock) {
                
                    if(pureStaticClass && !m.isStatic)
                        continue;

                    {
                        EmitIfdefGuard ifdefGuard(*this, m);

                        emit("    ");
                        emit(m.returnType);
                        emit(" ");
                        emit(m.name);
                        emit("(");
                        emitParameterDeclarations(m.parameters, "___");
                        emit(")");

                        if(m.isConst)
                            emit(" const");

                        emitln(";");
                    }
                    emitln();
                }

                emitln("private:");
                emitln("    " + className + "* threadLocalThis;");
                emitln();
                emitln("    virtual void setRealThis(" + className + "* realThis) { ");
                emitln("        threadLocalThis = realThis;");
                emitln("    }");
                emitln();
                emitln("protected:");
                emitln("    virtual void onTrace(::citrix::tracing::CallTrace& trace) = 0;");
                emitln("};");
                emitln();
                emitln("#endif");
            }

            nsPrefix = "";
            if(hasNamespace) {
                nsPrefix = nameSpace + "::";
                BOOST_FOREACH(const std::string& ns, split(nameSpace, "::")) {
                    emit("}");
                }
                emitln();
            }

            //#############################################################################################
            //###     Register types in metatemplate

            emitln();
            emitln("namespace citrix { namespace mocking { namespace internal {");
            emitln();
            emitln("    template<>");
            emitln("    struct MockTraits<" + nsPrefix + className + "> {");
            emitln("        BOOST_STATIC_CONSTANT(bool, isAvailable = true);");
            emitln("        typedef " + nsPrefix + className + " TOriginal;");
            emitln("        typedef " + nsPrefix + className + "Accessor TAccessor;");

            if(enableTracing) {
                emitln("    #if LEGACYMOCK_SUPPORTS_TRACING");
                emitln("        typedef " + nsPrefix + className + "Tracer TTracer;");
                emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = true);");
                emitln("    #else");
                emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = false);");
                emitln("    #endif");
            } else {
                emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = false);");
            }

            emitln("        typedef " + nsPrefix + mockBaseName + "Static TStaticMock;");
            emitln("        typedef " + nsPrefix + mockBaseName + "Instance TMockInstance;");
            emitln("        typedef MockManager<" + nsPrefix + className + "> TManager;");

            if(!disableGMock) {
                emitln("        BOOST_STATIC_CONSTANT(bool, hasGMock = true);");
                emitln("        typedef " + nsPrefix + gmockName + "Static TGoogleStaticMock;");
                emitln("        typedef " + nsPrefix + gmockName + "Instance TGoogleMockInstance;");
            } else {
                emitln("        BOOST_STATIC_CONSTANT(bool, hasGMock = false);");
            }

            if(!customProbingInstance.empty()) {
                emitln("        typedef " + customProbingInstance + " TProbingInstance;");
            } else {
                emitln("        typedef TOriginal TProbingInstance;");
            }

            emitln("};");
            emitln();
            emitln("}}}");
            emitln();
            emitln();
        }

        emitln();
        emitln("#endif // __MOCK_" + mockBaseName + "_" + headerGuard);
    }
}}