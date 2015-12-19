from TestGenerator import TestGenerator


class CallTraceTestGenerator(TestGenerator):

    def setup(self):

	    self.testCases = [
	        {"name":"AddOnly", "updatedParams": lambda i : 0, "updatedValueCpp": self.getInitialValueCpp, "updatedValueJson": self.getInitialValueJson},
	        {"name":"AddAndNoopUpdate", "updatedParams": lambda i : 1, "updatedValueCpp": self.getInitialValueCpp, "updatedValueJson": self.getInitialValueJson},
	        {"name":"AddAndUpdateAll", "updatedParams": lambda i : 1, "updatedValueCpp": self.getUpdatedValueCpp, "updatedValueJson": self.getUpdatedValueJson},
	        {"name":"AddAndUpdateOdd", "updatedParams": lambda i : (i % 2) == 1, "updatedValueCpp": self.getUpdatedValueCpp, "updatedValueJson": self.getUpdatedValueJson},
	        {"name":"AddAndUpdateEven", "updatedParams": lambda i : (i % 2) == 0, "updatedValueCpp": self.getUpdatedValueCpp, "updatedValueJson": self.getUpdatedValueJson},
	    ]

    def getFileName(self):
    	return "../../citrix.tracing/tests/Generated/CallTraceTest.cpp"

    def emitTestCase(self):
    	emitln = self.emitln;
        emit = self.emit;

    	emitln("// This file was automatically generated! ")
        emitln()
        emitln("#include \"citrix/Platform.h\"")
        emitln("")
        emitln("#if LEGACYMOCK_SUPPORTS_TRACING")
        emitln("")
        emitln("")
        emitln("#include \"../citrix.tracing/include/citrix/tracing/CallTrace.h\"")
        emitln("#include \"Utils/StringUtils.h\"")
        emitln("#include \"../citrix.tracing/include/citrix/tracing/DefaultTranslators.h\"")
        emitln("#include \"CustomTypesForTesting.h\"")
        emitln("")
        emitln("#include <gtest/gtest.h>")
        emitln()
        emitln("namespace unittests { namespace CallTraceTest {")

        emitln()
        for t in self.knownTypes:
            if t == "MyOwnStruct":
                emitln("static " + t + " storageForInitial_" + self.makeIdentifier(t) + ";")
            else:
                emitln("static " + t + " storageForInitial_" + self.makeIdentifier(t) + " = " + self.getInitialValueCpp(t) + ";")
        emitln()

        emitln()
        for t in self.knownTypes:
            if t == "MyOwnStruct":
                emitln("static " + t + " storageForUpdated_" + self.makeIdentifier(t) + "(true);")
            else:
                emitln("static " + t + " storageForUpdated_" + self.makeIdentifier(t) + " = " + self.getUpdatedValueCpp(t) + ";")
        emitln()

        for case in self.testCases:
            emitln()
            for sig in self.callSignatures:
                emitln("TEST(CallTrace, canSerialize_" + case["name"] + "_" + sig["name"] + ") {")
                emitln("    citrix::tracing::CallTrace serializer;")
                emitln()

                emitln("    serializer.beginInvoke(\"" + sig["name"] + "\");")
                for i,p in enumerate(sig["params"]):
                    emitln("    serializer.addParameter<" + self.stripReference(p["type"]) + ">(\"" + self.formatType(p["type"]) + "\", \"" + p["name"] + "\", " + self.getInitialValueCpp(p["type"]) + ");")

                updated = []
                for i,p in enumerate(sig["params"]):
                    if(case["updatedParams"](i)):
                        if(case["updatedValueJson"](p["type"]) != self.getInitialValueJson(p["type"])):
                            updated += [{"index":i, "value":case["updatedValueJson"](p["type"])}]
                            emitln("    serializer.updateParameter<" + self.stripReference(p["type"]) + ">(\"" + p["name"] + "\", " + case["updatedValueCpp"](p["type"]) + ");")

                if(sig["return"] == "void"):
                    emitln("    serializer.endInvoke();")
                else:
                    emitln("    serializer.endInvoke<" + self.stripReference(sig["return"]) + ">(\"" + self.formatType(sig["return"]) + "\", " + self.getInitialValueCpp(sig["return"]) + ");")

                emitln()

                expected = self.makeExpected(sig, updated)
                emitln("    std::string expected = \"{" + expected.replace("\"", "\\\"").replace("'", "\"")  + "}\";")
                emitln("    ASSERT_EQ(boostJson(expected), boostJson(serializer.getPtree()));")
                emitln("}")
                emitln()

        emitln()
        emitln("}}")
        emitln()
        emitln("#endif")
        emitln()
