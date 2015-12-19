import os, sys, shutil, subprocess, argparse, time

###################################################################################################
######################### PARSING PARAMETERS
###################################################################################################

parser = argparse.ArgumentParser(description='Builds LegacyMock.')

# required parameters
parser.add_argument('--build-directory', dest='buildDir', action='store', required=True, help='A directory where intermediate build artifacts are created (sort of a temporary directory).')
parser.add_argument('--build-artifacts', dest='buildArtifactsRoot', action='store', required=True, help='A directory where the built libraries and executables are stored (subdirectories are created automatically).')

# optional parameters
parser.add_argument('--cmake', dest='cmake', action='store', default="cmake", help='A valid path to your cmake executable.')
parser.add_argument('--python', dest='python', action='store', default=sys.executable, help='A valid path to your python 2.7 executable.')
parser.add_argument('--build-generator-only', dest='createGenerator', action='count', default=0, help='Only build the generator executable.')
parser.add_argument('--use-generator', dest='useGenerator', action='store', default="", help='Use an existing generator exe for mock generation instead of building it as a pre-build step.')
parser.add_argument('--boost', dest='boost', action='store', default="", help='Path to your Boost root directory.')
parser.add_argument('--boost-libs', dest='boostLibs', action='store', default="", help='Path to your compiled Boost library directory. This will use boost libraries instead of source code inlining.')
parser.add_argument('--gtest', dest='gtest', action='store', default="", help='Path to your GTest root directory.')
parser.add_argument('--gmock', dest='gmock', action='store', default="", help='Path to your GMock root directory.')
parser.add_argument('--clean', dest='clean', action='count', default=0, help='Delete build directory before building.')
parser.add_argument('--clean-output', dest='cleanOutput', action='count', default=0, help='Delete build artifact directory before building.')
parser.add_argument('--single-file-compile', dest='singleFileCompile', action='count', default=0, help='Build UnitTests and client package through single-file compilation.')
parser.add_argument('--cmake-generator', dest='cmakeGenerator', action='store', default="", help='A custom generator to use for CMake. By default, the cmake default for your platform is chosen.')

parser.add_argument('--7z', dest='zip7', action='store', default="7z", help='A valid path to your 7zip executable (reqired for deployment).')
parser.add_argument('--make-package', dest='makePackage', action='count', default=0, help='Should the output directory be populated into a deployable package including a 7zip compressed package of itself?.')

args = parser.parse_args()

###################################################################################################
######################### UTILITY FUNCTIONS
###################################################################################################

def logInfo(msg):
    sys.stdout.write(str(msg) + "\n")
    sys.stdout.flush()

def logError(msg):
    sys.stderr.write("[ERROR]: " + str(msg) + "\n")
    sys.stderr.flush()

def logFatal(msg):
    sys.stderr.write("[FATAL]: " + str(msg) + "\n")
    sys.stderr.flush()
    exit(-1)

def findExeOrNone(program):

    for ext in ["", ".exe"]:
        def is_exe(exePath):
            return os.path.isfile(exePath) and os.access(exePath, os.X_OK)

        fpath, fname = os.path.split(program + ext)
        if fpath:
            if is_exe(program + ext):
                return program + ext
        else:
            for path in os.environ["PATH"].split(os.pathsep):
                path = path.strip('"')
                exe_file = os.path.join(path, program + ext)
                if is_exe(exe_file):
                    return exe_file

    return None

def findExeOrFail(program):

    result = findExeOrNone(program)
    if(result):
        return result

    logFatal("Unable to find program '" + program + "'.")
    exit(-1)

def call(exe, args):
    logInfo("Current directory: '" + os.getcwd() + "'")
    logInfo("Running command '" + exe + " \"" + "\" \"".join(args) + "\"'")
    logInfo("")

    try:
        exitCode = subprocess.call([exe] + args)

        if(exitCode != 0):
            logFatal("Command '" + exe + " \"" + "\" \"".join(args) + "\"' failed with error code " + str(exitCode) + ".")
            exit(-1)

    except Exception as e:
        logFatal("Command '" + exe + " \"" + "\" \"".join(args) + "\"' failed with exception '" + str(e) + "'.")
        exit(-1)

def callCMake(args):
    call(currDir + "/build-tools/CMakeRunner.exe", [cmake] + args)

###################################################################################################
######################### VALIDATING CONFIGURATION
###################################################################################################

currDir = os.path.abspath(os.path.dirname(os.path.abspath(__file__)) + "/").replace("\\", "/")
buildDir = os.path.abspath(args.buildDir)
buildArtifactsRoot = os.path.abspath(args.buildArtifactsRoot)
cmake = findExeOrFail(args.cmake)
python = findExeOrFail(args.python)
zip7 = findExeOrNone(args.zip7)
cmakeGenerator = args.cmakeGenerator
fsRootBuilds = args.fsRootBuilds
boost = args.boost
boostLibs = args.boostLibs
makePackage = args.makePackage > 0
gtest = args.gtest
useGenerator = args.useGenerator
gmock = args.gmock
createGenerator = args.createGenerator > 0
singleFileCompile = args.singleFileCompile > 0

logInfo("")
logInfo("Running buildscript with following configuration:")
logInfo("    Project-Directory = '" + currDir + "'")
logInfo("    Build-Directory = '" + buildDir + "'")
logInfo("    Build-Artifacts = '" + buildArtifactsRoot + "'")
logInfo("    CMake = '" + cmake + "'")
logInfo("    Python 2.7 = '" + python + "'")
logInfo("    Citrix Root = '" + fsRootBuilds + "'")
logInfo("    Make Package = '" + str(makePackage) + "'")
logInfo("    Single-File-Compile = " + str(singleFileCompile))

def cmakeDefaultIfEmpty(name, path):
    if(path and len(path) > 0):
        path = os.path.abspath(path)
        logInfo("    " + name + " = '" + path + "'")
    else:
        logInfo("    " + name + " = [CMAKE-DEFAULT]")
        path = None

    return path

if(len(cmakeGenerator) > 0):
    logInfo("    CMake-Generator = '" + cmakeGenerator + "'")
else:
    logInfo("    CMake-Generator = [CMAKE-DEFAULT]")
    cmakeGenerator = None

boost = cmakeDefaultIfEmpty("Boost-Root", boost)
boostLibs = cmakeDefaultIfEmpty("Boost-Libraries", boostLibs)
gtest = cmakeDefaultIfEmpty("GTest-Root", gtest)
gmock = cmakeDefaultIfEmpty("GMock-Root", gmock)
useGenerator = cmakeDefaultIfEmpty("Use Generator", args.useGenerator)

if(zip7 and len(zip7) > 0):
    logInfo("    7Zip = '" + zip7 + "'")
else:
    logInfo("    7Zip = [DISABLED]")
    zip7 = None

logInfo("")

if(args.clean > 0) and os.path.exists(buildDir):
    logInfo("Deleting build directory...")
    shutil.rmtree(buildDir)

if(args.cleanOutput > 0) and os.path.exists(buildArtifactsRoot):
    logInfo("Deleting build artifact directory...")
    shutil.rmtree(buildArtifactsRoot, ignore_errors = True)

if not os.path.exists(buildDir):
    logInfo("Creating build directory (which did not exist).")
    time.sleep(1)
    os.makedirs(buildDir)
else:
    logInfo("Build directory already exists.")

if not os.path.exists(buildArtifactsRoot):
    logInfo("Creating build artifact directory (which did not exist).")
    time.sleep(1)
    os.makedirs(buildArtifactsRoot)
else:
    logInfo("Build artifact directory already exists.")


def genWithCMake(srcDir):
    cmakeArgs = [
            "-DPYTHON_BINARY=" + python,
            "-DARTIFACTS_ROOT=" + buildArtifactsRoot
    ]

    if(cmakeGenerator):
        cmakeArgs += ["-G", cmakeGenerator]

    if(boost):
        cmakeArgs += ["-DBOOST_ROOT=" + boost]

    if(useGenerator):
        cmakeArgs += ["-DUSE_GENERATOR=" + useGenerator]

    if(boostLibs):
        cmakeArgs += ["-DBOOST_USE_LIBRARIES=" + boostLibs]

    if(gtest):
        cmakeArgs += ["-DGTEST_ROOT=" + gtest]

    if(gmock):
        cmakeArgs += ["-DGMOCK_ROOT=" + gmock]

    callCMake(cmakeArgs + [srcDir])

logInfo("")
logInfo("###############################################################")
logInfo("### Generate CMake project")
logInfo("###############################################################")
logInfo("")

os.chdir(buildDir)
genWithCMake(currDir)

if(not useGenerator):
    logInfo("")
    logInfo("###############################################################")
    logInfo("### Build Code Generator")
    logInfo("###############################################################")
    logInfo("")

    buildMode = "Debug"
    if(createGenerator):
        buildMode = "Release"

    callCMake([
        "--build", "./",
        "--target", "citrix.mocking.generator",
        "--config", buildMode
    ])

logInfo("")
logInfo("###############################################################")
logInfo("### Generate mocks")
logInfo("###############################################################")
logInfo("")

call(python, ["../Unittests/TestGenerators/GenerateTests.py"])

generatorPath = ""
if(not useGenerator):
    generatorPath = buildArtifactsRoot + "/bin/citrix.mocking.generator.exe"
else:
    generatorPath = useGenerator

call(python, ["../Unittests/ThingsToMock/GenerateTestMocks.py", "-gen", generatorPath])
call(python, ["../citrix.tracing/src/BoostJsonParserHotfix.py", "-i", boost, "-o", currDir + "/unittests/boost_json_parser_hotfix.hpp"])
call(python, ["../amalganate.py"])

if(not createGenerator):

    logInfo("")
    logInfo("###############################################################")
    logInfo("### Build full project")
    logInfo("###############################################################")
    logInfo("")

    if(singleFileCompile):
        sfcDir = buildDir + "/single-file-compile/"
        if not os.path.exists(sfcDir):
            os.makedirs(sfcDir)

        os.chdir(sfcDir)
        genWithCMake(currDir + "/single-file-compile/cmake/")

        callCMake(["--build", "./", "--config", "Debug"])
        os.chdir(buildDir)
    else:
        genWithCMake(currDir)
        callCMake(["--build", "./", "--config", "Debug"])

    logInfo("")
    logInfo("###############################################################")
    logInfo("### Run UnitTests")
    logInfo("###############################################################")
    logInfo("")

    os.chdir(buildArtifactsRoot + "/bin/")
    call("./unittests.exe", ["--gtest_output=xml:unittests.xml"])
    os.chdir(buildDir)

    if(makePackage):
        logInfo("")
        logInfo("###############################################################")
        logInfo("### Create Deployment package")
        logInfo("###############################################################")
        logInfo("")

        def deepCopy(fromDir, toDir):
            logInfo('Copying directory from "' + fromDir + '" to "' + toDir + '"...')
            if os.path.exists(toDir):
                shutil.rmtree(toDir)
            shutil.copytree(fromDir, toDir)

        deepCopy(currDir + "/citrix.hooking/include", buildArtifactsRoot + "/citrix.hooking/include")
        deepCopy(currDir + "/citrix.mocking/include", buildArtifactsRoot + "/citrix.mocking/include")
        deepCopy(currDir + "/citrix.gmock/include", buildArtifactsRoot + "/citrix.gmock/include")
        deepCopy(currDir + "/citrix.binutils.udis86/include", buildArtifactsRoot + "/citrix.binutils.udis86/include")
        deepCopy(currDir + "/citrix.tracing/include", buildArtifactsRoot + "/citrix.tracing/include")
        deepCopy(currDir + "/include", buildArtifactsRoot + "/include")
        deepCopy(currDir + "/docs", buildArtifactsRoot + "/docs")

        logInfo("Generating 7zip package...")

        if os.path.exists(buildArtifactsRoot + "/package.7z"):
            os.remove(buildArtifactsRoot + "/package.7z")

        call(zip7, ["a", "-t7z", buildArtifactsRoot + "/package.7z", buildArtifactsRoot + "/*"])

logInfo("")
logInfo("###############################################################")
logInfo("### BUILD COMPLETED SUCCESSFULLY")
logInfo("###############################################################")
logInfo("")

exit(0)
