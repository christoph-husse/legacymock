# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Citrix Online LLC nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: 
#      christoph.husse@citrix.com (Christoph Husse)
#
 

import sys, CppHeaderParser, EmitterBase
from EmitterBase import EmitterBase 
from EmitterConfig import EmitterConfig

class HooksInternalEmitter(EmitterBase):

    def getMethodAddress(self, m):
        if(self.config.isFreeApi):
            return "&" + self.nsPrefix + m["name"]
        else:
            return "&" + self.nsPrefix + self.config.className + "::" + m["name"]

    def _process(self, config):

        self.config = config
        emitln = self._emitln
        emit = self._emit
        warning = self._warning
        info = self._info
        getParamType = self._getParamType
        skipMethod = self._skipMethod
        getReturnType = self._getReturnType

####################################################################################################################
##    File header
####################################################################################################################

        emitln(r"// This file was automatically generated.")
        emitln()
        emitln(r'#include "' + config.mockBaseName + '.h"')
        emitln()
        emitln(r'#include "../citrix.hooking/include/citrix/hooking/Hooking.h"')
        emitln(r'#include "../citrix.mocking/include/citrix/mocking/MockManager.h"')
        emitln(r'#include "../citrix.mocking/include/citrix/mocking/MockWrapper.h"')
        emitln(r'#include "../citrix.mocking/include/citrix/mocking/VirtualCallDetector.h"')

        if(config.enableTracing):
            emitln(r'#if LEGACYMOCK_SUPPORTS_TRACING')
            emitln(r'#   include "../citrix.tracing/include/citrix/tracing/DefaultTranslators.h"')
            emitln(r'#endif')
            emitln()

        namespace = config.classToMock["namespace"]
        hasNamespace = namespace and len(namespace) > 0

        emitln()
        nsPrefix = ""

        if(hasNamespace):
            nsPrefix = namespace + "::"

            partialName = ""
            for name in namespace.split("::"):
                partialName += name
                emitln("using namespace " + partialName + ";");
                partialName += "::"
            emitln()
        else:
            nsPrefix = "::"

        emitln("using namespace citrix::type_traits;")
        self.nsPrefix = nsPrefix

####################################################################################################################
##    Class preamble
####################################################################################################################

        if(config.classPrefix and len(config.classPrefix) > 0):
            emitln(config.classPrefix)
            emitln()

        for typedef in config.typedefs:
            emitln(typedef);

        emitln()

        emitln(r"template<>")
        emitln(r"class CitrixMockingAccessor<" + config.className + "> : public citrix::mocking::internal::MockManagerAccessor<" + config.className + "> {")
        emitln()

        emitln(r"private:")
        emitln(r"    struct EmptyObject {};")

        for ln in config.publicMockInlines:
            emitln(r"    " + ln)
        emitln();

        methods = config.methodsToMock

        for x,m in enumerate(methods):

            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

####################################################################################################################
##    FunctionAddressDetector
####################################################################################################################

            if(not m["static"]):

                indexedName = m["name"] + str(x);

                emitln("    class FunctionAddressDetector_" + indexedName + " : citrix::mocking::internal::VirtualCallDetector {")
                emitln("    public:")
                emitln("        typedef " + self.getMethodSignature(m, "TSignature") + ";")
                emitln("    private:")
                emitln("        static size_t address;")
                emitln()

                retType = m["rtnType"]
                if(retType.startswith("static ")):
                    retType = retType[7:]
                emit("        ")
                emit(retType)
                emit(" patchedStub(")
                params = m["parameters"]
                for y,p in enumerate(params):
                    emit(getParamType(p) + " " + p["name"])
                    if(y + 1 < len(params)):
                        emit(", ")
                emitln(") { ")
                for p in params:
                    emitln("            CITRIX_MOCK_UNREFPARAM(" + p["name"] + ");")
                emitln("            address = getVirtualCallAddressFromTls();")
                self.emitReturnStatement(retType, "            ");
                emitln("        }")

                emitln()
                emitln("        FunctionAddressDetector_" + indexedName + "(" + config.className + "* instance) {")
                emitln("            void *localStart = NULL, *localEnd = NULL;")
                emitln("            const TSignature cppAddress = " + self.getMethodAddress(m) + ";")
                emitln()
                emitln("            address = *((size_t*)&cppAddress);")
                emitln()
                emitln("            for (int i = 0; i < 2; i++) {")
                emitln("                localStart = Utils::getInstructionPointer();")
                emitln("                if ((i == 1) && tryInitialize(localStart, localEnd, methodPtrToPVoid(")
                emitln("                        &FunctionAddressDetector_" + indexedName + "::patchedStub))) ")
                emitln("                {")

                for y,p in enumerate(params):
                    emitln("                    boost::shared_ptr<" + self.normalizeType(getParamType(p)) + "> param" + 
                            str(y) + " = ::citrix::mocking::internal::allocateDefaultInstance<" + self.normalizeType(getParamType(p)) + ">();")

                emit("                    instance->" + m["name"] + "(")
                for y,p in enumerate(params):
                    emit("*param" + str(y))
                    if(y + 1 < len(params)):
                        emit(", ")

                emitln(");")

                emitln("                    failFastRelativeCalls();")
                emitln("                    dummyCallForMoreBytes();")
                emitln("                }")
                emitln()
                emitln("                localEnd = Utils::getInstructionPointer();")
                emitln("            }")
                emitln("        }")
                emitln()
                emitln("    public:")
                emitln("        static TSignature getAddress(" + config.className + "* allocatedType) {")
                emitln("            static FunctionAddressDetector_" + indexedName + " instance(allocatedType);")
                emitln("            return *((TSignature*)&address);")
                emitln("        }")
                emitln("    };")
                emitln()

####################################################################################################################
##    Hooked function implementation
####################################################################################################################

            emit("    ")

            if(m["constructor"]):
                m["name"] = "constructor__"
            elif(m["destructor"]):
                m["name"] = "destructor__"

            retType = m["rtnType"]
            emit(retType)
            emit(" __")
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

            emitln(" {")

            invokePrefix = ""
            if(m["static"]):
                if(not config.isFreeApi):
                    invokePrefix = config.className + "::"
                else:
                    invokePrefix = nsPrefix 
            else:
                invokePrefix += "((" + config.className + "*)this)->"
            
            invokeParams = "("
            for i,p in enumerate(params):
                invokeParams += "__" + p["name"]

                if(len(params) > i + 1):
                    invokeParams += ", "
            invokeParams += ")"
            invokeMethod = m["name"] + invokeParams

            emitln("        ::citrix::hooking::HookRecursionGuard guard(get().mock__" + m["name"] + str(x) + ");")            
            emitln("        ::boost::shared_ptr<::citrix::mocking::internal::MockManager<" + config.className + "> > mgr = ::citrix::mocking::internal::MockManager<" + config.className + ">::get();")
            emitln()
            emitln("        if(!mgr->isActive<" + self.getMethodSignature(m, "") + ">(" + self.getMethodAddress(m) + ")) {")
            if(not m["static"]):
                emitln("            FunctionAddressDetector_" + indexedName + "::TSignature callAddr = FunctionAddressDetector_" + indexedName + "::getAddress(NULL);")
                emitln("            // *Facepalm* This will treat normal calls, virtual calls, etc. all like normal calls")
                emitln("            // This is necessary because the vtable pointer has been resolved by the time we get here (no it's not confusing).")
                emitln("            // The current this pointer will already be shifted, and a normal virtual call would crash here...")
                emitln("            return (")
                emitln("                ((EmptyObject*)this)->*")
                emitln("                *(")
                emitln("                    (::citrix::type_traits::member_function_pointer_traits<FunctionAddressDetector_" + indexedName + "::TSignature, EmptyObject>::type*)")
                emitln("                    &callAddr")
                emitln("                )")
                emitln("            )" + invokeParams + ";")
            else:
                emitln("            return " + invokePrefix + invokeMethod + ";")
            emitln("        }")
            emitln()
            if(not m["static"]):
                emitln("        TMockInstance* mock = resolveMockForPointer((void*)this);")
                emitln("        mock->setRealThis((" + config.className + "*)this);")
                emit(  "        return mock")
            else:
                emit("        return resolveStaticMock()")
            emitln("->" + invokeMethod + ";")
            emitln("    }")
            emitln()

        emitln()
        
####################################################################################################################
##    get()
####################################################################################################################

        emitln()
        emitln("    bool areHooksInstalled;") 
        emitln("    boost::shared_ptr<" + config.className + "> probingInstance;") 
        emitln()    
        emitln("    static CitrixMockingAccessor<" + config.className + ">& get() {") 
        emitln("        static CitrixMockingAccessor<" + config.className + "> hooks;") 
        emitln("        return hooks;") 
        emitln("    }") 
        emitln()
        emitln("    static boost::shared_ptr<" + config.className + "> createProbingInstance() {") 

        if(config.pureStaticClass):
            emitln("        return boost::shared_ptr<" + config.className + ">();")
        else:
            if(config.customProbingInstance):
                emitln("        return ::citrix::mocking::internal::allocateDefaultInstance<" + config.customProbingInstance + ">();")
            else:
                ctorParams = sorted(
                    [m["parameters"] for m in config.methodsToMock if m["constructor"]],
                    key = lambda x: len(x))

                if(len(ctorParams) > 0):
                    ctorParams = ctorParams[0]

                for y,p in enumerate(ctorParams):
                    emitln("        boost::shared_ptr<" + self.normalizeType(getParamType(p)) + "> param" + 
                        str(y) + " = ::citrix::mocking::internal::allocateDefaultInstance<" + self.normalizeType(getParamType(p)) + ">();")

                emit("        return ::citrix::mocking::internal::allocateDefaultInstance<" + config.className + ">(")
                for y,p in enumerate(ctorParams):
                    emit("*param" + str(y))
                    if(y + 1 < len(ctorParams)):
                        emit(", ")
                emitln(");") 

        emitln("    }") 
        emitln()

####################################################################################################################
##    TypedHook member variables
####################################################################################################################

        for x,m in enumerate(methods):
            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

            emit("    ::citrix::hooking::TypedHook<")
            emit(self.getMethodSignature(m, ""))
            emit(", CitrixMockingAccessor<" + config.className + ">")
            emit(">")
            emitln(" mock__" + m["name"] + str(x) + ";")    


####################################################################################################################
##    Constructor
####################################################################################################################

        emitln()
        emitln("public:")
        emitln("    CitrixMockingAccessor() :")

        emit("        probingInstance(createProbingInstance())")

        for x,m in enumerate(methods):
            params = m["parameters"]
            paramList = ""

            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

            emitln(",")
            targetName = m["name"]

            if(m["constructor"]):
                targetName = config.className
            if(m["destructor"]):
                targetName = "~" + config.className

            retType = getReturnType(m["rtnType"])

            emit("        mock__" + m["name"] + str(x) + "(")

            if(m["static"]):
                emit(self.getMethodAddress(m))
            else:
                emit("FunctionAddressDetector_" + targetName + str(x) + "::getAddress(probingInstance.get())")

            emit(", &CitrixMockingAccessor<" + config.className + ">::__" + m["name"] + ")")

        emitln()
        emitln("    {")
        emitln("        areHooksInstalled = true")
        for x,m in enumerate(methods):
            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

            emitln("            && mock__" + m["name"] + str(x) + ".isInstalled()")
        emitln("        ;")
        emitln("    }")
        emitln()       
        emitln("    static bool tryToInstall() {")
        emitln("        return get().areHooksInstalled;")
        emitln("    }")
        emitln()

####################################################################################################################
##    Accessors
####################################################################################################################
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
            emit(config.className + "Accessor__" + m["name"])
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
                        emit(",")


            emitln(") {")

            if(not "isVariable" in m):
                emitln("        ::citrix::mocking::internal::DisableMemberMockGuard<" + self.getMethodSignature(m, "") + "> guard((" + self.getMethodAddress(m) + "));")

                if(m["static"]):
                    if(not config.isFreeApi):
                        emit("        return " + config.className + "::" + m["name"] + "(")
                    else:
                        emit("        return " + nsPrefix + m["name"] + "(")
                else:
                    emit("        return instance->" + m["name"] + "(")

                for y,p in enumerate(params):
                    emit("__" + p["name"])
                    if(y + 1 < len(params)):
                        emit(", ")

                emitln(");")
            else:
                retStmt = ""
                if(m["rtnType"] != "void"):
                    retStmt = "return "

                if(m["static"]):
                    emit("        " + retStmt + config.className + "::")
                else:
                    emit("        " + retStmt + "instance->")

                if(m["rtnType"] == "void"):
                    emitln(m["name"] + " = __citrix_mocking__value__;")
                else:
                    emitln(m["name"] + ";")

            emitln("    }")
            emitln()

####################################################################################################################
##    Tacer implementations with class accessors
####################################################################################################################
        if(config.enableTracing):
            emitln()
            emitln(r'#if LEGACYMOCK_SUPPORTS_TRACING')
            for m in config.methodsToMock:

                if(skipMethod(m)):
                    continue

                if(config.pureStaticClass and not m["static"]):
                    continue

                retType = m["rtnType"]
                isStatic = 0
                if(retType.startswith("static ")):
                    retType = retType[7:]
                    isStatic = 1

                hasReturnType = getReturnType(retType).strip() != "void"

                emit("    static ")
                emit(retType)
                emit(" ")
                emit(config.className + "Tracer__" + m["name"] + "(" + config.className + "Tracer* tracer")

                params = m["parameters"]
                if(len(params) > 0):
                    emitln(",")
                    for i,p in enumerate(params):
                        emit(r"            " + getParamType(p) + " __" + p["name"])

                        if(len(params) > i + 1):
                            emitln(",")

                emitln(") { ")

                if(not isStatic):
                    emit("        ")
                    if(m["const"]):
                        emit("const ") 
                    emitln(config.className + "* realThis = tracer->threadLocalThis;")
                    
                emitln("        ::citrix::tracing::CallTrace trace;")
                emitln()

                emit("        trace.beginInvoke(\"" + config.className + "::" + m["name"] + "\")")
                for p in params:
                    emitln()
                    emit("           .addParameter(\"" + getParamType(p) + "\", \"" + p["name"] + "\", __" + p["name"] + ")")
                emitln(";")

                emitln()    
                emitln("        try {")    

                def traceUpdatedParams():
                    emit("            trace")
                    for p in params:
                        emitln("")
                        emit("                .updateParameter(\"" + p["name"] + "\", __" + p["name"] + ")")
                    if(len(params) > 0):
                        emitln("")
                        emit("                ")

                invokePrefix = ""
                if(m["static"]):
                    invokePrefix += config.className
                    invokePrefix += "::"
                else:
                    invokePrefix += "realThis->"
                
                invokeMethod = m["name"] + "("
                for i,p in enumerate(params):
                    invokeMethod += "__" + p["name"]

                    if(len(params) > i + 1):
                        invokeMethod += ", "
                invokeMethod += ")"

                if(hasReturnType):
                    rt = getReturnType(retType)
                    if('&' in rt):
                        rt = rt.replace('&', "*")
                        prefix = "&"
                        deref = "*"
                    else:
                        prefix = ""
                        deref = ""

                    emit("            citrix::type_traits::return_type_of_function<" + rt + "()>::type")
                    emitln(" result = " + prefix + invokePrefix + invokeMethod + ";")

                    traceUpdatedParams()
                    emitln(".endInvoke(\"" + getReturnType(retType) + "\", " + deref + "result);")
                    emitln("            tracer->onTrace(trace);")
                    emitln("            return " + deref + "result;")
                else:
                    emitln("            " + invokePrefix + invokeMethod + ";")
                    traceUpdatedParams()
                    emitln(".endInvoke();")
                    emitln("            tracer->onTrace(trace);")


                emitln("        } catch(::std::exception& ex) {")
                emitln("            trace.fail(ex);")
                emitln("            tracer->onTrace(trace);")
                emitln("            throw;")
                emitln("        }")
                emitln("    }")
                emitln()
            emitln(r'#endif')

        emitln("};")
        emitln()

####################################################################################################################
##    Adress variable definitions
####################################################################################################################
        for x,m in enumerate(methods):

            if(skipMethod(m)):
                continue

            if(config.pureStaticClass and not m["static"]):
                continue

            if(m["static"]):
                continue;

            indexedName = m["name"] + str(x);

            emitln("size_t CitrixMockingAccessor<" + config.className + ">::FunctionAddressDetector_" + m["name"] + str(x) + "::address;")

####################################################################################################################
##    Accessors
####################################################################################################################
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

            emit(retType)
            emit(" ")
            emit(config.className + "Accessor::" + m["name"])
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


            emitln(") {")
            emit("    return CitrixMockingAccessor<" + config.className + ">::" + config.className + "Accessor__" + m["name"] + "(")

            if(not m["static"]):
                emit("instance")

            if(len(params) > 0):
                if(not m["static"]):
                    emit(", ")

                for y,p in enumerate(params):
                    emit("__" + p["name"])
                    if(y + 1 < len(params)):
                        emit(", ")

            emitln(");")
            emitln("}")
            emitln()

####################################################################################################################
##    Tacers
####################################################################################################################
        if(config.enableTracing):
            emitln()
            emitln(r'#if LEGACYMOCK_SUPPORTS_TRACING')
            for m in config.methodsToMock:

                if(skipMethod(m)):
                    continue

                if(config.pureStaticClass and not m["static"]):
                    continue

                retType = m["rtnType"]
                isStatic = 0
                if(retType.startswith("static ")):
                    retType = retType[7:]
                    isStatic = 1

                hasReturnType = getReturnType(retType).strip() != "void"

                emit(retType)
                emit(" ")
                emit(config.className + r"Tracer::" + m["name"])
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

                emitln(" { ")

                emitln("    return CitrixMockingAccessor<" + config.className + ">::" + config.className + "Tracer__" + m["name"] + "(")
                emit("        const_cast<" + config.className + r"Tracer*>(this)")
                for p in params:
                    emit(", __" + p["name"])

                emitln(");")
                emitln("}")

            emitln(r'#endif')

####################################################################################################################
##    File footer
####################################################################################################################

        nsPrefix = ""
        if(hasNamespace):
            nsPrefix = namespace + "::"

        emitln()
        emitln("namespace citrix { namespace mocking { namespace internal {")
        emitln("    template<>")
        emitln("    bool tryToInstallHooks<" + nsPrefix + config.className + ">() {")
        emitln("        return CitrixMockingAccessor<" + config.className + ">::tryToInstall();")
        emitln("    }")
        emitln("}}}")
        emitln()

        self._saveEmittedLinesTo(config.dstDir + r"/" + config.mockBaseName + ".cpp")