import sys, CppHeaderParser, os, hashlib, re
from EmitterConfig import EmitterConfig

class EmitterBase:
    _emittedLines = []
    config = None

    def _getParamType(self, param):
        t = param["type"].strip()
        if(not t.endswith("&") and param["reference"]):
            t += "&"
        return self.removeSpaces(t)

    def removeSpaces(self, t):
        prev = t
        t = t.replace(" &", "&").replace(" *", "*").strip()
        while(prev != t):
            prev = t
            t = t.replace(" &", "&").replace(" *", "*").strip()
        return t

    def _skipMethod(self, m):
        if(not self.config.enableCtorMocking and m["constructor"]):
            return 1

        if(not self.config.enableDtorMocking and m["destructor"]):
            return 1

        if(m["template"] or m["pure_virtual"]):
            return 1

        return 0

    def _getReturnType(self, retType_):
        retType = retType_
        if(retType.startswith("static ")):
            retType = retType[7:]
        return self.removeSpaces(retType)

    def _emitln(self, msg = ""):
        self._emit(msg)
        self._emittedLines += [""]

    def _emit(self, msg = ""):
        if(len(self._emittedLines) == 0):
            self._emittedLines = [""]
        self._emittedLines[len(self._emittedLines) - 1] += msg

    def _warning(self, msg = ""):
        print "[WARN]: " + msg

    def _info(self, msg = ""):
        print "[INFO]: " + msg
        sys.stdout.write("")

    def saveToFile(self, config):
        self._emittedLines = []
        self._process(config)

    def _saveEmittedLinesTo(self, fileName):
        needsUpdate = True
        newContent = ""
        for ln in self._emittedLines:
            newContent += ln + "\n"

        if(self.config.safeUpdateCheck):
            if(os.path.isfile(fileName)):
                existingContent = ""
                f = open(fileName,'r')
                existingContent = f.read()
                existingContent = existingContent.replace("\r\n", "\n")
                needsUpdate = newContent != existingContent
                f.close()

        if(needsUpdate):
            self.config.logInfo("Updating file at '" + os.path.abspath(fileName) + "'.")

            f = open(fileName, 'w')
            f.write(newContent)
            f.close()
        else:
            self.config.logInfo("File at '" + os.path.abspath(fileName) + "' is up to date.")


    def normalizeType(self, typeName):
        result = ""
        for word in re.split(r"([^a-zA-Z0-9_]+)", typeName):
            if(word.strip() == "const"):
                continue;

            result += word
        return result.replace("&", "").strip();

    def emitReturnStatement(self, retType, indent):
        normRet = self.normalizeType(retType)
        
        if("void" == normRet):
            self._emitln(indent + "return;")
        elif('*' in normRet):
            self._emitln(indent + "static boost::shared_ptr<" + normRet[0:-1] + "> __retDummy__ = ::citrix::mocking::internal::allocateDefaultInstance<" + normRet[0:-1] + ">();")
            self._emitln(indent + "return __retDummy__.get();")
        else:
            self._emitln(indent + "static boost::shared_ptr<" + normRet + "> __retDummy__ = ::citrix::mocking::internal::allocateDefaultInstance<" + normRet + ">();")
            self._emitln(indent + "return *__retDummy__;")

    def getMethodSignature(self, m, typedef):
        if(m["static"]):
            classPrefix = ""
            classPtr = ""
            mockPrefix = ""
        else:
            classPrefix = ", " + self.config.className 
            classPtr = "(" + self.config.className + "::*)"
            mockPrefix = "CitrixMockingAccessor<" + self.config.className + ">::"

        retType = self._getReturnType(m["rtnType"])
        params = m["parameters"]
        paramList = ""
        for i,p in enumerate(params):
            paramList += self._getParamType(p)
            if(len(params) > i + 1):
                paramList += ", "

        methodSuffix = ""
        if(m["const"]):
            methodSuffix = " const"

        return "member_function_pointer_traits<" + retType + classPtr + "(" + paramList + ")" + methodSuffix + classPrefix + ">::type " + typedef;


