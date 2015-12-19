# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.

import sys, CppHeaderParser, EmitterBase, os, copy, re
from MockHeaderEmitter import MockHeaderEmitter 
from HooksInternalEmitter import HooksInternalEmitter 
from EmitterBase import EmitterBase 
from EmitterConfig import EmitterConfig
from TypedefClosure import extractTypedefClosure, doesTemplateExtendToLine
import argparse

parser = argparse.ArgumentParser(description='Creates a mock framework for a given C++ class.')
parser.add_argument('--input', '-i', dest='input', action='store', default='', type=argparse.FileType('r'), help='A C++ header file to parse.')
parser.add_argument('--outdir', '-o', dest='outdir', action='store', help='All generated files will be stored in here. If the directory does not exist, it will be created.')
parser.add_argument('--safe-update-check', dest='safeUpdateCheck', action='count', default=0, help='Use file content comparison instead of file time comparison. This is recommended if there is a chance that the python generator itself changes. But it is also much slower.')
parser.add_argument('--verbose', dest='beVerbose', action='count', default=0, help='Do emit console output for non-error conditions.')
parser.add_argument('--class-name', '-c', dest='className', action='store', default='', help='Process the class with the given name (unqualified, no namespaces). By default, the first class encountered in the header file is processed.')
parser.add_argument('--mock-all-classes', dest='mockAllClasses', action='count', default=0, help='All classes in the file are processed and wrapped into individual mock stubs.')
parser.add_argument('--class-prefix', '-cp', dest='classPrefix', action='store', default='', help='Allows you to insert custom code into the public preamble of a class. This code is inserted into the public section before any generated code. It is also emitted into the global namespace within the mock implementation.')
parser.add_argument('--include', '-inc', dest='includes', action='append', default=[], help='Include this file in the generated public mock header. Use multiple arguments to add multiple files.')
parser.add_argument('--enable-tracing', dest='enableTracing', action='count', default=0, help="Emit mocks that are tracable. This will likely require you to implement translators for the various types that might need to be serialized.")
parser.add_argument('--enable-default-impl', dest='enableDefaultImpl', action='count', default=0, help="By default, an abstract mock interface is emitted. But if you know for sure that all return types have default constructors, you can use this option to generate default implementations instead.")
parser.add_argument('--mock-free-api', dest='isFreeApi', action='count', default=0, help='Interpret member functions as free functions (like Windows API). This will cause the free functions to be hooked but still grouped into mock classes with support for custom mocks, Google Mock & tracing as usual.')
parser.add_argument('--set-probing-instance', dest='probingInstance', action='store', default='', help='Sets the name of the probing class (fully qualified, make sure you also add it\'s header to the include list). You need to provide an explicit probing class if the mocked class is abstract. A probing class should derive from the abstract class you want to mock and provide a default implementation for all abstract functions.')
parser.add_argument('--pure-static-class', dest='pureStaticClass', action='count', default=0, help='Must be specified if your class is not constructible. This will prevent any non-static members from being hooked.')
parser.add_argument('--mock-protected', dest='mockProtected', action='count', default=0, help='Also mock protected member functions. This requires you to change the source code of the target class to include "template<typename> friend class ::CitrixMockingAccessor;" in the class definition.')
parser.add_argument('--mock-private', dest='mockPrivate', action='count', default=0, help='Also mock private member functions. This requires you to change the source code of the target class to include "template<typename> friend class ::CitrixMockingAccessor;" in the class definition.')
parser.add_argument('--variable-accessors', dest='mockVariables', action='count', default=0, help='Generates setters and getters for member variables, so you can assert on object state.')
parser.add_argument('--disable-gmock', dest='disableGMock', action='count', default=0, help='Do not generate any GMock code. This will significantly decrease compile times for generated headers and should be used when you don\'t intend to use GMock for the mocked class.')
parser.add_argument('--mock-lifetime', dest='mockLifetime', action='count', default=0, help='Mock constructors & destructor. This is only supported for classes who have user defined constructors & destructor and will raise an error otherwise.')

# parse command line and populate context
args = parser.parse_args()

if(not args.input):
    parser.print_help()
    sys.exit(-1)

# This is a bit ugly and doesn't always work (--mock-all-classes), as generated file names are based on class names.
# Since C++ parsing is costly, it makes no sense to do it first. We NEED to do without.
if((args.safeUpdateCheck == 0) and (len(args.className) > 0)):
    cppFile = os.path.abspath(args.outdir) + "/" + args.className + "Mock.cpp"
    hFile = os.path.abspath(args.outdir) + "/" + args.className + "Mock.h"
    srcTime = os.path.getmtime(args.input.name)

    if(os.path.isfile(cppFile) and os.path.isfile(hFile)):
        if((os.path.getmtime(cppFile) >= srcTime) and (os.path.getmtime(hFile) >= srcTime)):

            if(args.beVerbose > 0):
                print "File at '" + os.path.abspath(cppFile) + "' is up to date."
                print "File at '" + os.path.abspath(hFile) + "' is up to date."
            sys.exit(0)


headerContent = args.input.read().replace('\r', '')
headerContent = re.sub(r"[^a-zA-Z_0-9]{1}throw\s*\(\s*\)", "", headerContent)
operatorPattern = re.compile(r"^operator[^a-zA-Z_0-9]{1}")

try:
    cppHeader = CppHeaderParser.CppHeader(headerContent, "string")
except CppHeaderParser.CppParseError as e:
    print(e)
    sys.exit(1)

for iClass,className in enumerate(cppHeader.classes):

    if(args.mockAllClasses == 0):
        if(args.className and len(args.className) > 0):
            if(className != args.className):
                continue

    config = EmitterConfig()
    config.headerContent = headerContent
    config.classToMock = cppHeader.classes[className]
    config.className = className
    config.mockManagerName = className + "MockManager"
    config.mockBaseName = className + "Mock"
    config.gmockName = className + "GMock"
    config.hooksName = className + "Hooks"
    config.hooksInternalName = className + "HooksInternal"
    config.enableCtorMocking = 0
    config.enableDtorMocking = 0
    config.headerGuard = "_CITRIX_MOCKING_AUTOGEN_" + className + "_H_"
    config.publicIncludes = args.includes
    config.dstDir = os.path.abspath(args.outdir)
    config.classPrefix = args.classPrefix
    config.isFreeApi = args.isFreeApi > 0
    config.beSilent = not (args.beVerbose > 0)
    config.safeUpdateCheck = args.safeUpdateCheck > 0
    config.enableTracing = args.enableTracing > 0
    config.enableDefaultImpl = args.enableDefaultImpl > 0
    config.pureStaticClass = args.pureStaticClass > 0
    config.mockVariables = args.mockVariables > 0
    config.disableGMock = args.disableGMock > 0
    config.customProbingInstance = args.probingInstance.strip()
    config.classStart = int(config.classToMock["line_number"])
    config.classEnd = config.classStart
    for m in config.classToMock["methods"]["public"] + config.classToMock["methods"]["protected"] + config.classToMock["methods"]["private"] + config.classToMock["properties"]["public"] + config.classToMock["properties"]["protected"] + config.classToMock["properties"]["private"]:
        config.classEnd = max(config.classEnd, int(m["line_number"]))

    if(config.isFreeApi):
        config.pureStaticClass = 1

    methodsToMock = config.classToMock["methods"]["public"] 
    config.variablesToMock = config.classToMock["properties"]["public"] 

    if(args.mockProtected > 0):
        methodsToMock += config.classToMock["methods"]["protected"]
        config.variablesToMock += config.classToMock["properties"]["protected"] 

    if(args.mockPrivate > 0):
        methodsToMock += config.classToMock["methods"]["private"]
        config.variablesToMock += config.classToMock["properties"]["private"] 

    config.methodsToMock = []
    for m in methodsToMock:
        if(operatorPattern.match(m["name"]) != None):
            continue

        if(doesTemplateExtendToLine(config, int(m["line_number"]))):
            config.logInfo("Ignoring template method \"" + className + "::" + m["name"] + "\"")
            continue

        config.methodsToMock += [m]

    if not os.path.exists(config.dstDir):
        os.makedirs(config.dstDir)

    variablesToMock = []
    if(config.mockVariables and not config.isFreeApi):
        for var in config.variablesToMock:

            var["isVariable"] = 1
            var["destructor"] = 0
            var["constructor"] = 0
            var["virtual"] = 0
            var["template"] = 0
            var["const"] = 0
            var["reference"] = 0
            var["pure_virtual"] = 0

            getter = copy.deepcopy(var)
            getter["returns"] = getter["rtnType"] = "::boost::add_reference<" + var["type"] + ">::type"
            getter["parameters"] = []
            variablesToMock += [getter]

    config.variablesToMock = variablesToMock

    config.typedefs = extractTypedefClosure(config);

    if(doesTemplateExtendToLine(config, config.classStart)):
        config.logInfo("Ignoring template class \"" + className + "\".")
        continue

    HooksInternalEmitter().saveToFile(config)
    MockHeaderEmitter().saveToFile(config)

    if(args.mockAllClasses == 0):
        break