# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.

import sys, os, re, CppHeaderParser, subprocess, json, argparse

parser = argparse.ArgumentParser(description='Scans a directory for C++ header files and invoked mock framework creation on each of them.')
parser.add_argument('--generator', dest='generatorExe', action='store', required=True, type=argparse.FileType('r'), help='Path to compiled citrix.mocking.generator!')
parser.add_argument('--bulk-input', dest='bulkInput', action='store', required=True, help='A directory to scan for C++ header files.')
parser.add_argument('--bulk-output', dest='bulkOutput', action='store', required=True, help='A directory where all generated mocks will be stored.')
parser.add_argument('--bulk-config', dest='bulkConfig', action='store', type=argparse.FileType('r'), help='A JSON map from class-name to array of command line arguments that should be passed in addition to the bulk arguments when generating that particular class.')
parser.add_argument('--bulk-extensions', dest='bulkExtensions', action='store', default='h,hpp', help='A comma separated list of C++ header file extensions (defaults to "h,hpp"). You can use Python regular expressions for each entry. An empty list will match all extensions.')
parser.add_argument('--bulk-file-pattern', dest='bulkFilePattern', action='store', default='.*', help='A Python regular expression to include filenames.')
parser.add_argument('--bulk-file-anti-pattern', dest='bulkFileAntiPattern', action='store', default='', help='A Python regular expression to exclude filenames.')
parser.add_argument('--bulk-dir-pattern', dest='bulkDirPattern', action='store', default='.*', help='A Python regular expression to include directories.')
parser.add_argument('--bulk-dir-anti-pattern', dest='bulkDirAntiPattern', action='store', default='', help='A Python regular expression to exclude directories.')
parser.add_argument('--bulk-path-pattern', dest='bulkPathPattern', action='store', default='.*', help='A Python regular expression to include paths.')
parser.add_argument('--bulk-path-anti-pattern', dest='bulkPathAntiPattern', action='store', default='', help='A Python regular expression to exclude paths.')
parser.add_argument('--bulk-class-pattern', dest='bulkClassPattern', action='store', default='.*', help='A Python regular expression to include C++ class names.')
parser.add_argument('--bulk-class-anti-pattern', dest='bulkClassAntiPattern', action='store', default='', help='A Python regular expression to exclude C++ class names.')
parser.add_argument('--safe-update-check', dest='safeUpdateCheck', action='count', default=0, help="Use file content comparison instead of file time comparison. This is recommended if there is a chance that the generator itself changes. But it is also much slower.")

parser.add_argument('--class-prefix', '-cp', dest='classPrefix', action='store', default='', help='Allows you to insert custom code into the public preamble of a class. This code is inserted into the public section before any generated code. It is also emitted into the global namespace within the mock implementation.')
parser.add_argument('--include', '-inc', dest='includes', action='append', default=[], help='Include this file in the generated public mock header. Use multiple arguments to add multiple files.')
parser.add_argument('--set-probing-instance', dest='probingInstance', action='store', default='', help='Sets the name of the probing class (fully qualified, make sure you also add it\'s header to the include list). You need to provide an explicit probing class if the mocked class is abstract. A probing class should derive from the abstract class you want to mock and provide a default implementation for all abstract functions.')

parser.add_argument('--enable-tracing', dest='enableTracing', action='count', default=0, help="Emit mocks that are tracable. This will likely require you to implement translators for the various types that might need to be serialized.")
parser.add_argument('--enable-default-impl', dest='enableDefaultImpl', action='count', default=0, help="By default, an abstract mock interface is emitted. But if you know for sure that all return types have default constructors, you can use this option to generate default implementations instead.")
parser.add_argument('--mock-free-api', dest='isFreeApi', action='count', default=0, help='Interpret member functions as free functions (like Windows API). This will cause the free functions to be hooked but still grouped into mock classes with support for custom mocks, Google Mock & tracing as usual.')
parser.add_argument('--pure-static-class', dest='pureStaticClass', action='count', default=0, help='Must be specified if your class is not constructible. This will prevent any non-static members from being hooked.')
parser.add_argument('--mock-protected', dest='mockProtected', action='count', default=0, help='Also mock protected member functions. This requires you to change the source code of the target class to include "template<typename> friend class ::CitrixMockingAccessor;" in the class definition.')
parser.add_argument('--mock-private', dest='mockPrivate', action='count', default=0, help='Also mock private member functions. This requires you to change the source code of the target class to include "template<typename> friend class ::CitrixMockingAccessor;" in the class definition.')
parser.add_argument('--variable-accessors', dest='mockVariables', action='count', default=0, help='Generates setters and getters for member variables, so you can assert on object state.')
parser.add_argument('--disable-gmock', dest='disableGMock', action='count', default=0, help='Do not generate any GMock code. This will significantly decrease compile times for generated headers and should be used when you don\'t intend to use GMock for the mocked class.')
parser.add_argument('--mock-lifetime', dest='mockLifetime', action='count', default=0, help='Mock constructors & destructor. This is only supported for classes who have user defined constructors & destructor and will raise an error otherwise.')
parser.add_argument('--enable-ifdef-mapping', dest='enableIfdefMapping', action='count', default=0, help='Try to replicate conditional compilation layout in mocked files. This is a very fragile feature.')
parser.add_argument('--debug-omit-impl', dest='debugOmitImpl', action='count', default=0, help="Do not implement the internal mock code. This is useful for easier debugging when mocks can't be compiled and can limit the error amount to something more manageable.")


# parse command line and populate context
args = parser.parse_args()

filePattern = re.compile(args.bulkFilePattern)
dirPattern = re.compile(args.bulkDirPattern)
pathPattern = re.compile(args.bulkPathPattern)
classPattern = re.compile(args.bulkClassPattern)
outputDirectory = args.bulkOutput;
currDir = os.path.dirname(os.path.abspath(__file__)) + "/"
bulkConfig = {}

if(not args.bulkInput):
    parser.print_help()
    sys.exit(-1)

if(args.bulkConfig):
    bulkConfig = json.loads(args.bulkConfig.read())

def logError(msg): 
    sys.stderr.write("[ERROR]: " + str(msg) + "\n")
    sys.stderr.flush()

def logInfo(msg): 
    sys.stdout.write("[INFO]: " + str(msg) + "\n")
    sys.stdout.flush()

def logWarning(msg): 
    sys.stdout.write("[WARNING]: " + str(msg) + "\n")
    sys.stdout.flush()

def call(path, className):
    childArgs = [args.generatorExe.name,
        "--python", sys.executable,
        "--parser-script", currDir + "/CppHeaderParserStub.py",
        "--input", path,
        "--outdir", outputDirectory,
        "--class-name", className,
        "--include", path,
        ]

    if(args.enableTracing > 0):
        childArgs += ["--enable-tracing"]
    if(args.enableDefaultImpl > 0):
        childArgs += ["--enable-default-impl"]
    if(args.isFreeApi > 0):
        childArgs += ["--mock-free-api"]
    if(args.pureStaticClass > 0):
        childArgs += ["--pure-static-class"]
    if(args.mockProtected > 0):
        childArgs += ["--mock-protected"]
    if(args.mockPrivate > 0):
        childArgs += ["--mock-private"]
    if(args.mockVariables > 0):
        childArgs += ["--variable-accessors"]
    if(args.disableGMock > 0):
        childArgs += ["--disable-gmock"]
    if(args.mockLifetime > 0):
        childArgs += ["--mock-lifetime"]
    if(args.probingInstance):
        childArgs += ["--set-probing-instance", args.probingInstance]
    for inc in args.includes:
        childArgs += ["--include", inc]
    if(args.classPrefix):
        childArgs += ["--class-prefix", args.classPrefix]
    if(args.enableIfdefMapping > 0):
        childArgs += ["--enable-ifdef-mapping"]
    if(args.debugOmitImpl > 0):
        childArgs += ["--debug-omit-impl"]
    if(args.safeUpdateCheck > 0):
        childArgs += ["--safe-update-check"]

    if(className in bulkConfig):
        childArgs += bulkConfig[className]

    if(subprocess.call(childArgs) != 0):
        logError(str(childArgs))
        logError("Failed to generate mock for \"" + className + "\".")

def emitEntry(path):
    logInfo("Parsing \"" + path + "\"")

    try:
        cppHeader = CppHeaderParser.CppHeader(path)
    except Exception as e:
        logError("    > Failure while parsing. Will be ignored!")
        logError(e)
        return False

    for className in cppHeader.classes:

        if("<" in className):
            logInfo("    > Skipping class \"" + className + "\" (unsupported template specialization)")
            continue

        if("::" in className):
            logInfo("    > Skipping class \"" + className + "\" (unsupported nested class)")
            continue

        if(classPattern.search(className) == None):
            logInfo("    > Skipping class \"" + className + "\"")
            continue

        logInfo("    > Found class \"" + className + "\"") 

        if(args.bulkOutput):
            call(path, className);

    return True

def traverseDirectory(dir):
    hasAny = False

    dir = os.path.abspath(dir);

    logInfo("Processing files in directory \"" + dir + "\"...");

    for dirName, subdirList, fileList in os.walk(dir):
        for fileName in fileList:
            dirName = os.path.abspath(dirName)
            path = os.path.abspath(dirName + "/" + fileName)

            if((pathPattern.search(path) != None) and (filePattern.search(fileName) != None) and (dirPattern.search(dirName) != None)):
                if(emitEntry(path)):
                    hasAny = True

        for subDir in subdirList:
            if(traverseDirectory(subDir)):
                hasAny = True

    return hasAny

if not traverseDirectory(args.bulkInput):
    logError("No mocks have been processed. Maybe your search pattern is too narrow.")
    exit(-1)

exit(0);
