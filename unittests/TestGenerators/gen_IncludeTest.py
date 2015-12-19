import os
from os import walk
from os.path import join, abspath, dirname, splitext, split

f = []
includeDir = abspath("../../../../include/LegacyMocks")
targetDir = abspath("../../../../src/IncludeTests")
for (dirpath, dirnames, filenames) in walk(includeDir):
    f.extend([abspath(join(dirpath, x)) for x in filenames])

hdrFiles = [x for x in f if splitext(x)[1] in [".h", ".hpp"]]

if not os.path.exists(targetDir):
	os.makedirs(targetDir)

for hdr in hdrFiles:
	target = hdr
	relDir = abspath(dirname(target))[len(includeDir):]
	cppDir = abspath(targetDir + relDir)
	cppName = split(splitext(target)[0])[1] + ".cpp"
	cppPath = join(cppDir, cppName)

	if not os.path.exists(join(targetDir, cppDir)):
		os.makedirs(join(targetDir, cppDir))

	if not os.path.isfile(cppPath):
		f = open(cppPath,'w')
		f.write("// This file was automatically generated!\n\n")
		f.write("#include \"LegacyMocks/" + relDir + "/" + split(target)[1] + "\"\n\n")
		f.close()
