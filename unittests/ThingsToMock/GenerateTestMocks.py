import os, sys, shutil, subprocess, argparse

parser = argparse.ArgumentParser(description='Creates mocks for UnitTests.')
parser.add_argument('-gen', dest='generatorExe', action='store', required=True, type=argparse.FileType('r'), help='Path to compiled citrix.mocking.generator!')
args = parser.parse_args()

generatorExe = args.generatorExe.name
currDir = os.path.dirname(os.path.abspath(__file__)) + "/"

if not os.path.exists(currDir + "../GeneratedMocks/"):
    os.makedirs(currDir + "../GeneratedMocks/")
    
# Make sure that CMake invocation will pickup newly generated mocks
os.utime(currDir + "../CMakeLists.txt", None)

def call(args):
	args = [generatorExe,
		"--python", sys.executable,
		"--parser-script", currDir + "../../citrix.mocking/src/CppHeaderParserStub.py",
	] + args;

	if(subprocess.call(args) != 0):
		print "[FATAL]: Failed to generate mock for " + str(args)
		sys.exit(-1)

call([
	"--input", currDir + "AllInOne.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "AllInOne",
	"--include", "ThingsToMock/AllInOne.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--safe-update-check",
]) 

call([
	"--input", currDir + "TestObject.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "TestObject",
	"--include", "ThingsToMock/TestObject.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--safe-update-check",
])

call([
	"--input", currDir + "BaseABC.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "BaseABC",
	"--include", "ThingsToMock/BaseABC.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--safe-update-check",
])

call([
	"--input", currDir + "BaseABC.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "BaseC",
	"--include", "ThingsToMock/BaseABC.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--safe-update-check",
])

call([
	"--input", currDir + "BaseABC.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "BaseA",
	"--include", "ThingsToMock/BaseABC.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--set-probing-instance", "::MockMe::internal::BaseA_ProbingInstance",
	"--safe-update-check",
])

call([
	"--input", currDir + "BaseABC.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "BaseB",
	"--include", "ThingsToMock/BaseABC.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--set-probing-instance", "::MockMe::internal::BaseB_ProbingInstance",
	"--safe-update-check",
])

call([
	"--input", currDir + "NonCopyableNonDefaultCtor.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "NonCopyableNonDefaultCtor",
	"--include", "ThingsToMock/NonCopyableNonDefaultCtor.h",
	"--safe-update-check",
])

call([
	"--input", currDir + "NotConstructibleStaticOnly.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "NotConstructibleStaticOnly",
	"--include", "ThingsToMock/NotConstructibleStaticOnly.h",
	"--pure-static-class",
	"--enable-tracing",
	"--enable-default-impl",
	"--safe-update-check",
])

call([
	"--input", currDir + "PrivateAndProtected.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "PrivateAndProtected",
	"--include", "ThingsToMock/PrivateAndProtected.h",
	"--enable-tracing",
	"--enable-default-impl",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--safe-update-check",
])

call([
	"--input", currDir + "NoGMockForThisOne.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "NoGMockForThisOne",
	"--include", "ThingsToMock/NoGMockForThisOne.h",
	"--class-prefix", "#ifdef GMOCK_INCLUDE_GMOCK_GMOCK_H_\n#error \"GMock should not be active here!\"\n#endif",
	"--disable-gmock",
	"--safe-update-check",
])

call([
	"--input", currDir + "VariableAccessibility.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "VariableAccessibility",
	"--include", "ThingsToMock/VariableAccessibility.h",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--safe-update-check",
])

call([
	"--input", currDir + "FreeApiHook.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "FreeApiHook",
	"--include", "ThingsToMock/FreeApiHook.h",
	"--mock-free-api",
#	"--enable-tracing",
	"--safe-update-check",
])

#call([
#	"--input", currDir + "ObjectLifetime.h",
#	"--outdir", currDir + "../GeneratedMocks/",
#	"--include", "ThingsToMock/ObjectLifetime.h",
#	"--mock-protected",
#	"--mock-private",
#	"--mock-all-classes",
#	"--mock-lifetime",
#	"--safe-update-check",
#])

call([
	"--input", currDir + "Modifiers.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "Modifiers",
	"--include", "ThingsToMock/Modifiers.h",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--safe-update-check",
])

call([
	"--input", currDir + "NotSupportedAndIgnored.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--include", "ThingsToMock/NotSupportedAndIgnored.h",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--mock-all-classes",
	"--safe-update-check",
])

call([
	"--input", currDir + "TemplateClass.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--include", "ThingsToMock/TemplateClass.h",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--mock-all-classes",
	"--safe-update-check",
])

call([
	"--input", currDir + "NestedTypes.h",
	"--outdir", currDir + "../GeneratedMocks/",
	"--class-name", "NestedTypes",
	"--include", "ThingsToMock/NestedTypes.h",
	"--mock-protected",
	"--mock-private",
	"--variable-accessors",
	"--safe-update-check",
])

call([
 	"--input", currDir + "ConditionalCompilation.h",
 	"--outdir", currDir + "../GeneratedMocks/",
 	"--mock-all-classes",
 	"--include", "ThingsToMock/ConditionalCompilation.h",
 	"--mock-protected",
 	"--mock-private",
 	"--variable-accessors",
 	"--safe-update-check",
 	"--enable-ifdef-mapping",
])

