import os, sys, subprocess

currDir = os.path.dirname(os.path.abspath(__file__)) + "/"
generatorExe = currDir + "../../bin/citrix.mocking.generator.exe"

if not os.path.exists(currDir + "GeneratedMocks/"):
    os.makedirs(currDir + "GeneratedMocks/")

def call(args):
    args = [generatorExe,
        "--python", sys.executable,
        "--parser-script", currDir + "../../citrix.mocking/src/CppHeaderParserStub.py",
    ] + args;

    if(subprocess.call(args) != 0):
        print "[FATAL]: Failed to generate mock for " + str(args)
        sys.exit(-1)

# this is the important part
call([
    "--input", currDir + "MyClass.h",
    "--outdir", currDir + "GeneratedMocks/",
    "--class-name", "MyClass",
    "--include", "MyClass.h",
    "--enable-default-impl",
    "--safe-update-check",
]) 
