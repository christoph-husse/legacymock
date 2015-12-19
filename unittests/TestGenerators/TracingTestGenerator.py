from TestGenerator import TestGenerator


class TracingTestGenerator(TestGenerator):

    def setup(self):
        self.methodNamePrefix = "TestObject::"
        return

    def getFileName(self):
    	return "../../citrix.tracing/tests/Generated/TracingTest.cpp"

    def emitTestCase(self):
    	emitln = self.emitln;
        emit = self.emit;

        emitln("// This file was automatically generated! ")
        emitln()
        emitln("#include \"citrix/Platform.h\"")
        emitln()
        emitln("#if LEGACYMOCK_SUPPORTS_TRACING")
        emitln()
        emitln("#include \"GeneratedMocks/TestObjectMock.h\"")
        emitln("#include \"../citrix.mocking/include/citrix/mocking/Mocking.h\"")
        emitln("#include \"Utils/StringUtils.h\"")
        emitln()
        emitln("using namespace citrix::mocking;")
        emitln("using namespace citrix::tracing;")
        emitln("")
        emitln("#pragma warning (disable:4189) // local variable is initialized but not referenced")
        emitln("")
        emitln("namespace unittests { namespace TracingTest {")
        emitln("")
        emitln("    class TestTracer : public TestObjectTracer {")
        emitln("    public:")
        emitln("        std::string last;")
        emitln("")
        emitln("        virtual void onTrace(::citrix::tracing::CallTrace& trace) {")
        emitln("            last = boostJson(trace.getPtree());")
        emitln("        }")
        emitln("    };")
        emitln("")
        emitln("    template<class T>")
        emitln("    struct TestTracerContext {")
        emitln("        typedef TestTracer TStatic;")
        emitln("        typedef TestTracer TInstance;")
        emitln("    };")
        emitln("")
        emitln("    typedef citrix::mocking::MockWrapper<TestTracerContext> Trace;")
        emitln("    ")
        emitln("#   define FIXTURE\\")
        for t in self.knownTypes:
            if t == "MyOwnStruct":
                emitln("        " + t + " storageForInitial_" + self.makeIdentifier(t) + ";\\")
            else:
                emitln("        " + t + " storageForInitial_" + self.makeIdentifier(t) + " = " + self.getInitialValueCpp(t) + ";\\")
        for t in self.knownTypes:
            if t == "MyOwnStruct":
                emitln("        " + t + " storageForUpdated_" + self.makeIdentifier(t) + "(true);\\")
            else:
                emitln("        " + t + " storageForUpdated_" + self.makeIdentifier(t) + " = " + self.getUpdatedValueCpp(t) + ";\\")
        emitln("        Trace::MockFixture fixture;\\")
        emitln("        Trace::Mock<TestObject> obj(true);")
        emitln()

        for sig in self.callSignatures:
            emitln("TEST(TracingTest, canTrace_" + sig["name"] + ") {")
            emitln("    FIXTURE;")
            emitln()
            emitln("    ASSERT_EQ(0, obj->" + sig["name"] + "__COUNTER__);")

            emit("    obj->" + sig["name"] + "(")
            for i,p in enumerate(sig["params"]):
                emit(self.getInitialValueCpp(p["type"]))
                if(i + 1 < len(sig["params"])):
                    emitln(",")
                    emit("         ")
            emitln(");")

            emitln("    ASSERT_EQ(1, obj->" + sig["name"] + "__COUNTER__);")
            emitln()

            updated = []
            for i,p in enumerate(sig["params"]):
                if(not self.isParamAssignable(p["type"])):
                    continue
                updated += [{"index":i, "value":self.getUpdatedValueJson(p["type"])}]

            expected = self.makeExpected(sig, updated)
            emitln("    std::string expected = \"{" + expected.replace("\"", "\\\"").replace("'", "\"")  + "}\";")
            emitln("    ASSERT_EQ(boostJson(expected), obj.last);")

            emitln("}")
            emitln()
        emitln()
        emitln("}}")
        emitln()
        emitln("#endif")
        emitln()
