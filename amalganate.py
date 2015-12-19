'''
This script can be executed from any working directory without parameters.
It will scan the LegacyMock project structure for CPP files and emit amalganated
files:

/include/citrix/srclink/citrix.mocking-all.cpp
/include/citrix/srclink/citrix.unittests-all.cpp

They can then be used to single-file compile LegacyMock as well as it's UnitTests.
'''

import os, sys;

currDir = os.path.abspath(os.path.dirname(os.path.abspath(__file__)) + "/").replace("\\", "/")

def emitIncludes(dirs, excludes):
    result = ""
    for d in dirs:
        for root, dirs, files in os.walk(d):
            for file in files:
                excluded = False
                for ex in excludes:
                    if file.endswith(ex):
                        excluded = True

                if excluded:
                    continue

                if file.endswith(".cpp"):
                    fileName = os.path.abspath(os.path.join(root, file)).replace("\\", "/")
                    if(not fileName.startswith(currDir)):
                        print "[WARNING]: Ignoring file '" + fileName + "' because its not in a subdirectory relative to the current script."
                    else:
                        result += "#include \".." + fileName[len(currDir):] + "\"\n"
    return result

legacyMockAll = emitIncludes([currDir + "/citrix.binutils.udis86/src",
                    currDir + "/citrix.gmock/src",
                    currDir + "/citrix.hooking/src",
                    currDir + "/citrix.mocking/src",
                    currDir + "/citrix.tracing/src"], 
                    [])

unitTestsAll = "#include \".." + "/unittests/GeneratedMocks/NoGMockForThisOneMock.cpp\"\n"
unitTestsAll += legacyMockAll
unitTestsAll += emitIncludes([currDir + "/citrix.binutils.udis86/tests",
                    currDir + "/citrix.hooking/tests",
                    currDir + "/citrix.mocking/tests",
                    currDir + "/citrix.tracing/tests",
                    currDir + "/unittests"],
                    ["NoGMockForThisOneMock.cpp"])

f = open(currDir + "/include/citrix/srclink/citrix.mocking-all.cpp", 'w')
f.write(legacyMockAll)
f.close()

f = open(currDir + "/include/citrix/srclink/citrix.unittests-all.cpp", 'w')
f.write(unitTestsAll)
f.close()