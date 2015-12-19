import sys, os


import argparse

parser = argparse.ArgumentParser(description='Creates a mock framework for a given C++ class.')
parser.add_argument('--input', '-i', dest='boostRoot', action='store', required=True, help='Path to boost root directory.')
parser.add_argument('--output', '-o', dest='targetFile', action='store', required=True, help='Where to write the patched boost header to.')
args = parser.parse_args()

boostRoot = os.path.abspath(args.boostRoot)
targetFile = args.targetFile

boostFile = boostRoot + "/boost/shared_ptr.hpp"

if(not os.path.isfile(boostFile)):
	print "Boost root '" + boostRoot + "' does not seem to be valid!"
	sys.exit(-1)

boostFile = boostRoot + "/boost/property_tree/detail/json_parser_read.hpp"
patched = ""

if(os.path.isfile(boostFile)):
	print "Creating hotfix file for '" + boostFile + "'..."
	f = open(boostFile,'r')
	patched = f.read()
	f.close()

else:
	print "Boost file '" + boostFile + "' does not exist. Creating empty patch file..."

newContent = patched.replace("c.stack.back()->push_back(std::make_pair(c.name, Str(b, e)));", "c.stack.back()->push_back(std::make_pair(c.name, Ptree(Str(b, e))));")
existingContent = "  "

if(os.path.isfile(targetFile)):
    f = open(targetFile,'r')
    existingContent = f.read()

if(newContent != existingContent):
    print "Updating file at '" + os.path.abspath(targetFile) + "'."

    f = open(targetFile, 'w')
    f.write(newContent)
    f.close()
else:
    print "File at '" + os.path.abspath(targetFile) + "' is up to date."

sys.exit(0)