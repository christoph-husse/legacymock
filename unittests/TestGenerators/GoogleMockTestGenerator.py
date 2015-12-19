# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
#
# Authors: 
#      * Christoph Husse

import os
from TestGenerator import TestGenerator


class GoogleMockTestGenerator(TestGenerator):

    def setup(self):
        self.methodNamePrefix = "TestObject::"
        return

    def getFileName(self):
    	return "../../citrix.mocking/tests/Generated/GoogleMockTest.cpp"

    def isMutable(self, typeName):
        return (("&" in typeName) or ("*" in typeName)) and not (typeName.startswith("const ") or ("char*" in typeName) or ("MyOwnStruct" in typeName))

    def emitTestCase(self):
    	emitln = self.emitln;
        emit = self.emit;

        emitln("// This file was automatically generated! ")
        emitln()
        emitln("#include \"GeneratedMocks/TestObjectMock.h\"")
        emitln("#include \"../citrix.mocking/include/citrix/mocking/Mocking.h\"")
        emitln()
        emitln("using namespace citrix::mocking;")
        emitln("using testing::Return;")
        emitln("using testing::ReturnRef;")
        emitln("using testing::DoAll;")
        emitln("using testing::SetArgPointee;")
        emitln("using testing::SetArgReferee;")
        emitln("using testing::StrEq;")
        emitln()
        emitln("namespace unittests { namespace GoogleMockTest {")
        emitln()
        emitln("    typedef citrix::mocking::GMock G;")
        emitln()
        emitln("#   define FIXTURE\\")
        for t in self.knownTypes:
            ident = "storageForInitial_" + self.makeIdentifier(t)
            if t == "MyOwnStruct":
                emitln("        " + t + " " + ident + ";\\")
            else:
                emitln("        " + t + " " + ident + " = " + self.getInitialValueCpp(t) + ";\\")
            emitln("        CITRIX_MOCK_UNREFPARAM(" + ident + ");\\")

        for t in self.knownTypes:
            ident = "storageForUpdated_" + self.makeIdentifier(t)
            if t == "MyOwnStruct":
                emitln("        " + t + " " + ident + "(true);\\")
            else:
                emitln("        " + t + " " + ident + " = " + self.getUpdatedValueCpp(t) + ";\\")
            emitln("        CITRIX_MOCK_UNREFPARAM(" + ident + ");\\")

        emitln("        G::MockFixture fixture;")
        emitln()

        for sig in self.callSignatures:
            hasParams = len(sig["params"]) > 0
            hasMutableParams = 0

            emitln("TEST(Mocking, GoogleMockTest_" + sig["name"] + ") {")
            emitln("    FIXTURE;")
            emitln()
            emitln("    G::LazyMock<TestObject> mock;")

            emitln("    EXPECT_CALL(mock, __init__internal__());")
            emit("    EXPECT_CALL(mock, " + sig["name"] + "(")

            if(hasParams):
                emitln()
                for i,p in enumerate(sig["params"]):
                    if(self.isMutable(p["type"])):
                        hasMutableParams = 1

                    emit("            ")

                    if("char*" in p["type"]):
                        emit("StrEq(" + self.getInitialValueCpp(p["type"]) + ")")
                    else:
                        emit(self.getInitialValueCpp(p["type"]))

                    if(i + 1 < len(sig["params"])):
                        emitln(",")
                emitln()
                emit("        )).WillOnce(")
            else:
                emit(")).WillOnce(")

            if(hasMutableParams):
                emit("DoAll(")
            emitln()

            for i,p in enumerate(sig["params"]):
                t = p["type"]
                ident = self.makeIdentifier(self.getBareKnownType(t))

                if(not self.isMutable(t)):
                    continue

                if(("*" in t) and (not "char*" in t)):
                    emitln("            SetArgPointee<" + str(i) + ">(storageForUpdated_" + ident + "),")
                else:
                    emitln("            SetArgReferee<" + str(i) + ">(storageForUpdated_" + ident + "),")

            # expect Return()
            if (sig["return"] != "void"):
                ident = self.makeIdentifier(self.getBareKnownType(sig["return"]))
                if(('*' in sig["return"]) and (not "char*" in sig["return"])):
                    emit("            Return(&storageForInitial_" + ident + ")")
                elif("MyOwnStruct" in sig["return"]):
                    emit("            ReturnRef(storageForInitial_" + ident + ")")
                else:
                    emit("            Return(storageForInitial_" + ident + ")")
            else:
                emit("            Return()")

            emitln()
            if(hasMutableParams):
                emitln("        ));")
            else:
                emitln("        );")

            # emit actual method invocation
            emitln()
            emitln("    TestObject obj(true);")
            emit("    obj." + sig["name"] + "(")
            
            for i,p in enumerate(sig["params"]):
                emit(self.getInitialValueCpp(p["type"]))
                if(i + 1 < len(sig["params"])):
                    emitln(",")
                    emit("         ")

            emitln(");")
            emitln("}")
            emitln()
        emitln()
        emitln("}}")

GoogleMockTestGenerator().run()