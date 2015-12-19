from TestGenerator import TestGenerator


class TestObjectGenerator(TestGenerator):

    def setup(self):
	    return;

    def getFileName(self):
    	return "../ThingsToMock/TestObject.h"

    def emitReturnStatement(self, retType):
        if(retType != "void"):
            self.emitln("        return " + self.getInitialValueCpp(retType) + ";")

    def emitReturnStatementUnsafe(self, retType):
        if(retType != "void"):
            self.emitln("        return ::citrix::type_traits::default_constructor<" + retType + ">::get();")

    def emitTestCase(self):
    	emitln = self.emitln;
        emit = self.emit;

    	emitln("// This file was automatically generated! ")
        emitln()
        
        emitln("#ifndef _UNITTEST_MOCK_GENERATED_TESTOBJECT_H_")
        emitln("#define _UNITTEST_MOCK_GENERATED_TESTOBJECT_H_")
        
        emitln()
        emitln("#include \"citrix/Config.h\"")
        emitln("#include \"citrix/Platform.h\"")
        emitln("#include \"citrix/type_traits/default_constructor.hpp\"")
        emitln("#include \"CustomTypesForTesting.h\"")
        emitln()
        emitln()
        emitln("template<class T> class CitrixMockingAccessor;")
        emitln()

        emitln("struct TestObject {")
        emitln("template<class T> friend class ::CitrixMockingAccessor;")

        def emitSignatures(static, prefix):
            for sig in self.callSignatures:
                emit("    " + static + sig["return"] + " " + prefix + sig["name"] + " (")

                for i,p in enumerate(sig["params"]):
                    emit(p["type"] + " " + p["name"])
                    if(len(sig["params"]) > i + 1):
                        emit(", ")

                emitln(") { ")
                for p in sig["params"]:
                    emitln("        CITRIX_MOCK_UNREFPARAM(" + p["name"] + ");")

                if((static == "") and (prefix == "")):
                    for i,p in enumerate(sig["params"]):
                        if (self.getBareKnownType(p["type"]) == "char*"):
                            continue
                        emitln("        forceAssign(" + p["name"] + ", " + self.getUpdatedValueCpp(p["type"]) + ");")

                    emitln("        " + sig["name"] + "__COUNTER__++;")
                    self.emitReturnStatement(sig["return"])
                else:
                    self.emitReturnStatementUnsafe(sig["return"])

                emitln("    }")
                emitln()

        emitln("public:")
        emitSignatures("", "")
        emitSignatures("static ", "S_")
        emitln("protected:")
        emitSignatures("", "protected_")
        emitSignatures("static ", "S_protected_")
        emitln("private:")
        emitSignatures("", "private_")
        emitSignatures("static ", "S_private_")
        emitln("public:")

        for sig in self.callSignatures:
            if(sig["return"] != "void"):
                continue;

            emit("     TestObject(")
            for i,p in enumerate(sig["params"]):
                emit(p["type"] + " " + p["name"])
                if(len(sig["params"]) > i + 1):
                    emit(", ")

            emitln(") : __constructor_Name__(\"" + sig["name"] + "\"), storageForUpdated_MyOwnStruct(true) {")
            for p in sig["params"]:
                emitln("        CITRIX_MOCK_UNREFPARAM(" + p["name"] + ");")
            emitln("        __init__internal__();")
            emitln("    }")
            emitln()

        emitln("    std::string __constructor_Name__;")
        for sig in self.callSignatures:
            emitln("    int32_t " + sig["name"] + "__COUNTER__;")
            emitln("    int32_t " + sig["name"] + "__COUNTER__const;")

        emitln()
        for t in self.knownTypes:
            emitln("    " + t + " storageForInitial_" + self.makeIdentifier(t) + ";")
            emitln("    " + t + " storageForUpdated_" + self.makeIdentifier(t) + ";")
        emitln()

        emitln()
        emitln("    void __init__internal__() {")
        for sig in self.callSignatures:
            emitln("        " + sig["name"] + "__COUNTER__ = 0;")
            emitln("        " + sig["name"] + "__COUNTER__const = 0;")
        for t in self.knownTypes:
            if(t == "MyOwnStruct"):
                continue;

            emitln("        storageForInitial_" + self.makeIdentifier(t) + " = " + self.getInitialValueCpp(t) + ";")
            emitln("        storageForUpdated_" + self.makeIdentifier(t) + " = " + self.getUpdatedValueCpp(t) + ";")
        emitln("    }")
        emitln()
        emitln("};")
        emitln()
        emitln("#endif")
