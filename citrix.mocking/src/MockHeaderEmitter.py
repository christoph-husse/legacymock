# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
#
# Authors: 
#      * Christoph Husse

import sys, CppHeaderParser, EmitterBase, re
from EmitterBase import EmitterBase 
from EmitterConfig import EmitterConfig

class MockHeaderEmitter(EmitterBase):

    def _process(self, config):

        self.config = config
        emitln = self._emitln
        emit = self._emit
        warning = self._warning
        info = self._info
        getParamType = self._getParamType
        skipMethod = self._skipMethod
        getReturnType = self._getReturnType

        emitln(r"// This file was automatically generated.")
        emitln(r"")

        # emitHeaderGuardBegin();
        if(len(config.headerGuard) > 0):
            emitln(r"#ifndef __MOCK_" + config.mockBaseName + "_" + config.headerGuard)
            emitln(r"#define __MOCK_" + config.mockBaseName + "_" + config.headerGuard)
            emitln()

        # emitPublicIncludes();
        for inc in config.publicIncludes:
            if(not "<" in inc):
                emitln("#include \"" + inc + "\"")
            else:
                emitln("#include " + inc)

        emitln()

        emitln(r'#include <boost/type_traits/add_reference.hpp>')
        if(not config.disableGMock):
            emitln(r'#include <gmock/gmock.h>')
            emitln()

        emitln(r'#include "citrix/Platform.h"')
        emitln(r'#include "citrix/type_traits/member_function_pointer_traits.hpp"')
        emitln(r'#include "citrix/type_traits/return_type_of_function.hpp"')
        emitln(r'#include "../citrix.mocking/include/citrix/mocking/MockManager.h"')

        if(config.enableTracing):
            emitln(r'#if LEGACYMOCK_SUPPORTS_TRACING')
            emitln(r'#   include "../citrix.tracing/include/citrix/tracing/CallTrace.h"')
            emitln(r'#endif')


        emitln()
        emitln(r"namespace citrix { ")
        emitln(r"    namespace mocking { namespace internal {")
        emitln(r"            template<class T> struct MockTraits;")
        emitln(r"    }}")
        emitln()
        emitln(r"    namespace tracing {")
        emitln(r"        class TracerBase;")
        emitln(r"    }")
        emitln(r"}")
        emitln()

        namespace = config.classToMock["namespace"]
        hasNamespace = namespace and len(namespace) > 0

        if(hasNamespace):
            namespace = namespace.split("::")
            for ns in namespace:
                emit("namespace ")
                emit(ns)
                emit(" { ")
            emitln()
            emitln()

        #############################################################################################
        ###     Abstract instance mock interface


        emitln("class " + config.mockBaseName + "Prefix {")
        emitln("public:")
        emitln("    virtual ~" + config.mockBaseName + "Prefix() { }")
        emitln()

        if(config.classPrefix and len(config.classPrefix) > 0):
            emitln(r"    " + config.classPrefix)

        for typedef in config.typedefs:
            emitln(r"    " + typedef);

        emitln("};");
        emitln();

        emitln("class " + config.mockBaseName + "Instance : public virtual " + config.mockBaseName + "Prefix {")

        def emitMockBaseMethod(m):
            retType = m["rtnType"]
            if(retType.startswith("static ")):
                retType = retType[7:]

            emit("    virtual ")

            emit("citrix::type_traits::return_type_of_function<" + retType + "()>::type")
            emit(" ")
            emit(m["name"])
            emit("(")

            params = m["parameters"]
            if(len(params) == 1):
                p = params[0]
                emit(getParamType(p) + " " + p["name"])
            elif(len(params) == 0):
                emit()
            else:
                emitln()
                for i,p in enumerate(params):
                    emit(r"        " + getParamType(p) + " " + p["name"])

                    if(len(params) > i + 1):
                        emitln(",")


            emit(")");

            if(m["const"]):
                emit(" const")

            if(config.enableDefaultImpl):
                emitln(" { ")
                for p in params:
                    emitln("        CITRIX_MOCK_UNREFPARAM(" + p["name"] + ");")
                self.emitReturnStatement(retType, "        ");
                emitln("    }")
            else:
                emitln(" = 0;")

            emitln()
        # end of emitMockBaseMethod()

        if(not config.pureStaticClass):
            emitln("public:")

            emitln("    virtual void setRealThis(" + config.className + "* realThis) {")
            emitln("        CITRIX_MOCK_UNREFPARAM(realThis);")
            emitln("    }")
            emitln()

            for m in config.methodsToMock:

                if(skipMethod(m)):
                    continue

                if(m["rtnType"].startswith("static ")):
                    continue

                emitMockBaseMethod(m)

        emitln("};");
        emitln();


        #############################################################################################
        ###     Abstract static mock interface

        emitln("class " + config.mockBaseName + "Static : public virtual " + config.mockBaseName + "Prefix {")
        emitln("public:")

        for m in config.methodsToMock:

            if(skipMethod(m)):
                continue

            if(not m["rtnType"].startswith("static ")):
                continue

            emitMockBaseMethod(m)

        emitln("};");
        emitln();


        emitln("class " + config.mockBaseName + " : public virtual " + config.mockBaseName + "Static, public virtual " + config.mockBaseName + "Instance {")
        emitln("};");
        emitln();

        if(not config.disableGMock):
            #############################################################################################
            ###     Instance Google Mock

            emitln("#ifdef _MSC_VER")
            emitln("#   pragma warning (push)")
            emitln("#   pragma warning (disable: 4373) // previous versions of compiler did not override")
            emitln("#endif")
            emitln()

            emitln()
            emitln(r"class " + config.gmockName + r"Instance : public virtual " + config.mockBaseName + "Instance {")

            def emitGoogleMockMethod(m):
                params = m["parameters"]
                constSpec = ""    
                if(m["const"]):
                    constSpec = "CONST_"

                emit("    MOCK_" + constSpec + "METHOD" + str(len(params)) + "(")

                emit(m["name"])
                emit(", ")

                retType = m["rtnType"]
                if(retType.startswith("static ")):
                    retType = retType[7:]

                emit("typename citrix::type_traits::return_type_of_function<" + retType + "()>::type")
                emit("(")

                if(len(params) == 1):
                    p = params[0]
                    emit(getParamType(p) + " " + p["name"])
                elif(len(params) == 0):
                    emit()
                else:
                    emitln()
                    for i,p in enumerate(params):
                        emit(r"        " + getParamType(p) + " " + p["name"])

                        if(len(params) > i + 1):
                            emitln(",")


                emitln("));");
                emitln()

            if(not config.pureStaticClass):
                emitln(r"public:")

                for ln in config.publicMockInlines:
                    emitln(r"    " + ln)

                emitln();

                for m in config.methodsToMock:

                    if(skipMethod(m)):
                        continue

                    if(m["rtnType"].startswith("static ")):
                        continue
                    
                    emitGoogleMockMethod(m);

            emitln("};");
            emitln()


            #############################################################################################
            ###     Static Google Mock

            emitln(r"class " + config.gmockName + r"Static : public virtual " + config.mockBaseName + "Static {")
            emitln(r"public:")

            for ln in config.publicMockInlines:
                emitln(r"    " + ln)

            emitln();

            for m in config.methodsToMock:

                if(skipMethod(m)):
                    continue

                if(not m["rtnType"].startswith("static ")):
                    continue
                
                emitGoogleMockMethod(m)

            emitln("};");

            emitln("#ifdef _MSC_VER")
            emitln("#   pragma warning (pop)")
            emitln("#endif")

        nsPrefix = ""
        if(hasNamespace):
            nsPrefix = "::".join(namespace) + "::"


        #############################################################################################
        ###     Accessor

        emitln()
        emitln(r"class " + config.className + r"Accessor {")
        emitln(r"public:")
        emitln()
        if(config.classPrefix and len(config.classPrefix) > 0):
            emitln(r"    " + config.classPrefix)
        emitln()

        for typedef in config.typedefs:
            emitln(r"    " + typedef);
        emitln()

        for m in config.methodsToMock + config.variablesToMock:

            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

            retType = m["rtnType"]
            if(retType.startswith("static ")):
                retType = retType[7:]
            hasReturnType = getReturnType(retType).strip() != "void"

            emit("    static ")
            emit(retType)
            emit(" ")
            emit(m["name"])
            emit("(") 

            if(not m["static"]):
                emit(config.className + "* instance")

            params = m["parameters"]
            if(len(params) > 0):
                if(not m["static"]):
                    emit(", ")
                    
                for y,p in enumerate(params):
                    emit(getParamType(p) + " __" + p["name"])
                    if(y + 1 < len(params)):
                        emit(", ")

            emitln(");")
            emitln("");

        emitln("};")
        emitln()

        #############################################################################################
        ###     Tracing

        if(config.enableTracing):
            emitln(r'#if LEGACYMOCK_SUPPORTS_TRACING')
            emitln()
            emitln(r"class " + config.className + r"Tracer : public virtual " + config.mockBaseName + "Static, public virtual " + config.mockBaseName + "Instance {")
            emitln(r"    template<typename> friend class ::CitrixMockingAccessor;")
            emitln(r"public:")
            emitln()
            if(config.classPrefix and len(config.classPrefix) > 0):
                emitln(r"    " + config.classPrefix)
            emitln()
            
            for typedef in config.typedefs:
                emitln(r"    " + typedef);
            emitln()

            for m in config.methodsToMock:

                if(skipMethod(m)):
                    continue

                if(config.pureStaticClass and not m["static"]):
                    continue

                retType = m["rtnType"]
                if(retType.startswith("static ")):
                    retType = retType[7:]
                hasReturnType = getReturnType(retType).strip() != "void"

                emit("    ")
                emit("citrix::type_traits::return_type_of_function<" + retType + "()>::type")
                emit(" ")
                emit(m["name"])
                emit("(")

                params = m["parameters"]
                if(len(params) == 1):
                    p = params[0]
                    emit(getParamType(p) + " __" + p["name"])
                elif(len(params) == 0):
                    emit()
                else:
                    emitln()
                    for i,p in enumerate(params):
                        emit(r"            " + getParamType(p) + " __" + p["name"])

                        if(len(params) > i + 1):
                            emitln(",")


                emit(")");

                if(m["const"]):
                    emit(" const")

                emitln(";")
                emitln()

            emitln("private:")
            emitln("    " + config.className + "* threadLocalThis;")
            emitln()
            emitln("    virtual void setRealThis(" + config.className + "* realThis) { ")
            emitln("        threadLocalThis = realThis;")
            emitln("    }")
            emitln()
            emitln("protected:")
            emitln("    virtual void onTrace(::citrix::tracing::CallTrace& trace) = 0;")
            emitln("};")
            emitln()
            emitln(r'#endif')

        nsPrefix = ""
        if(hasNamespace):
            nsPrefix = "::".join(namespace) + "::"
            for ns in namespace:
                emit("}")
            emitln()

        #############################################################################################
        ###     Register types in metatemplate

        emitln()
        emitln("namespace citrix { namespace mocking { namespace internal {")
        emitln()
        emitln("    template<>")
        emitln("    struct MockTraits<" + nsPrefix + config.className + "> {")
        emitln("        BOOST_STATIC_CONSTANT(bool, isAvailable = true);")
        emitln("        typedef " + nsPrefix + config.className + " TOriginal;")
        emitln("        typedef " + nsPrefix + config.className + "Accessor TAccessor;")

        if(config.enableTracing):
            emitln("    #if LEGACYMOCK_SUPPORTS_TRACING")
            emitln("        typedef " + nsPrefix + config.className + "Tracer TTracer;")
            emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = true);")
            emitln("    #else")
            emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = false);")
            emitln("    #endif")
        else:
            emitln("        BOOST_STATIC_CONSTANT(bool, hasTracer = false);")

        emitln("        typedef " + nsPrefix + config.mockBaseName + "Static TStaticMock;")
        emitln("        typedef " + nsPrefix + config.mockBaseName + "Instance TMockInstance;")
        emitln("        typedef MockManager<" + nsPrefix + config.className + "> TManager;")

        if(not config.disableGMock):
            emitln("        BOOST_STATIC_CONSTANT(bool, hasGMock = true);")
            emitln("        typedef " + nsPrefix + config.gmockName + "Static TGoogleStaticMock;")
            emitln("        typedef " + nsPrefix + config.gmockName + "Instance TGoogleMockInstance;")
        else:
            emitln("        BOOST_STATIC_CONSTANT(bool, hasGMock = false);")

        if(len(config.customProbingInstance) > 0):
            emitln("        typedef " + config.customProbingInstance + " TProbingInstance;")
        else:
            emitln("        typedef TOriginal TProbingInstance;")

        emitln("};")
        emitln()
        emitln("}}}")
        emitln()
        emitln()

        if(len(config.headerGuard) > 0):
            emitln()
            emitln("#endif // __MOCK_" + config.mockBaseName + "_" + config.headerGuard)





        self._saveEmittedLinesTo(config.dstDir + r"/" + config.mockBaseName + ".h")