# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
#
import re, os

class TestGenerator:
    emittedLines = []
    expectedStr = "";
    methodNamePrefix = ""

    initialValuesJson = {
        "char*": "\"Hello World!\"",
        "bool": "\"true\"",
        "int8_t": "\"124\"",
        "int16_t": "\"1236\"",
        "int32_t": "\"-1423\"",
        "int64_t": "\"-1235345345345345343\"",
        "uint8_t": "\"143\"",
        "uint16_t": "\"1239\"",
        "uint32_t": "\"1883345323\"",
        "uint64_t": "\"1237429349238472938\"",
        "double": "' + integralToString(-13345323.122345) + '",
        "float": "' + integralToString(+2.3e-8) + '",
        "std::string": "\"I am a String in Theory!\"",
        "MyOwnStruct": "{\"number\":123, \"str\":\"Hello MyStruct!\", \"sub\": {\"number\":-178, \"str\": \"Hello SubStruct!\"}}",
    }

    initialValuesCpp = {
        "char*": "\"Hello World!\"",
        "bool": "true",
        "int8_t": "124",
        "int16_t": "1236",
        "int32_t": "-1423",
        "int64_t": "-1235345345345345343LL",
        "uint8_t": "143",
        "uint16_t": "1239",
        "uint32_t": "1883345323",
        "uint64_t": "1237429349238472938ULL",
        "double": "-13345323.122345",
        "float": "+2.3e-8f",
        "std::string": "\"I am a String in Theory!\"",
        "MyOwnStruct": "MyOwnStruct()",
    }

    updateValuesCpp = {
        "char*": "\"There is no World :(\"",
        "bool": "false",
        "int8_t": "-23",
        "int16_t": "-3445",
        "int32_t": "1422356763",
        "int64_t": "3297109850394757343LL",
        "uint8_t": "233",
        "uint16_t": "24455",
        "uint32_t": "3564677783",
        "uint64_t": "2534545768788782938ULL",
        "double": "+13345323.122345",
        "float": "-2.3e-8f",
        "std::string": "\"I was a String in Theory!\"",
        "MyOwnStruct": "MyOwnStruct(true)",
    }

    updatedValuesJson = {
        "char*": "\"There is no World :(\"",
        "bool": "\"false\"",
        "int8_t": "\"-23\"",
        "int16_t": "\"-3445\"",
        "int32_t": "\"1422356763\"",
        "int64_t": "\"3297109850394757343\"",
        "uint8_t": "\"233\"",
        "uint16_t": "\"24455\"",
        "uint32_t": "\"3564677783\"",
        "uint64_t": "\"2534545768788782938\"",
        "double": "' + integralToString(+13345323.122345) + '",
        "float": "' + integralToString(-2.3e-8) + '",
        "std::string": "\"I was a String in Theory!\"",
        "MyOwnStruct": "{\"number\":123, \"str\":\"Hello MyStruct!\", \"sub\": {\"number\":-178, \"str\": \"Hello SubStruct Updated!\"}}",
    }

    def makeExpected(self, signature, updatedParams):
        self.expectedStr = ""

        self.writeMethodName(signature)
        self.writeMethodParameters(signature)
        self.writeUpdatedMethodParameters(signature, updatedParams)
        self.writeReturnValue(signature)

        return self.expectedStr

    def writeMethodName(self, signature):
        self.expectedStr += "\"method\":\"" + self.methodNamePrefix +  signature["name"] + "\""

    def formatType(self, type):
        return type.replace(" &", "&").replace("&", " &").replace(" *", "*").replace("*", " *");

    def writeMethodParameters(self, signature):
        if(len(signature["params"]) == 0): 
            return

        self.expectedStr += ", \"params\":{"
        for i,param in enumerate(signature["params"]):
            self.expectedStr += "\"" + param["name"] + "\":{\"value\":" + self.getInitialValueJson(param["type"])
            if((param["type"].count("*") > 0) and not self.normalizeType(param["type"]) in self.initialValuesJson):
                self.expectedStr += ", \"indirection\": " + str(param["type"].count("*"))

            self.expectedStr += ", \"type\":\"" + self.formatType(param["type"]) + "\"}"

            if(len(signature["params"]) > i + 1):
                self.expectedStr += ", "

        self.expectedStr += "}"

    def writeUpdatedMethodParameters(self, signature, updatedParams):
        if(len(updatedParams) == 0):
            return

        self.expectedStr += ", \"outParams\":{"
        for i,p in enumerate(updatedParams):
            param = signature["params"][p["index"]]
            self.expectedStr += "\"" + param["name"] + "\":"
            self.expectedStr += p["value"]

            if(i + 1 < len(updatedParams)):
                self.expectedStr += ", "

        self.expectedStr += "}"

    def writeReturnValue(self, signature):
        if(signature["return"] == "void"):
            return;

        self.expectedStr += ", \"return\":{ \"value\":" + self.getInitialValueJson(signature["return"])
        if((signature["return"].count("*") > 0) and not self.normalizeType(signature["return"]) in self.initialValuesJson):
                self.expectedStr += ", \"indirection\": " + str(signature["return"].count("*"))
        self.expectedStr += ", \"type\":\"" + self.formatType(signature["return"]) + "\"}"

    def emitln(self, msg = ""):
        self.emit(msg)
        self.emittedLines += [""]

    def emit(self, msg = ""):
        if(len(self.emittedLines) == 0):
            self.emittedLines = [""]
        self.emittedLines[len(self.emittedLines) - 1] += msg

    def normalizeType(self, typeName):
        result = ""
        for word in re.split(r"([^a-zA-Z0-9_]+)", typeName):
            if(word.strip() == "const"):
                continue;

            result += word
        return result.replace("&", "").strip();

    def stripReference(self, typeName):
        return typeName.replace("&", "").strip();

    def emitReturnStatement(self, retType):
        normRet = self.normalizeType(retType)
        if('*' in normRet):
            self.emitln("        static " + normRet[0:-1] + " __retDummy__;")
            self.emitln("        return &__retDummy__;")
        elif('&' in retType):
            self.emitln("        static " + normRet + " __retDummy__;")
            self.emitln("        return __retDummy__;")
        else:
            self.emitln("        return " + normRet + "();")

    def getBareType(self, typeName):
        typeName = self.normalizeType(typeName)
        while(typeName[len(typeName)-1] == '*'):
            typeName = typeName[0:-1].strip()
        return typeName

    def getBareKnownType(self, typeName):
        typeName = self.normalizeType(typeName)
        while((not typeName in self.initialValuesJson) and (typeName[len(typeName)-1] == '*')):
            typeName = typeName[0:-1].strip()
        return typeName

    def isParamAssignable(self, typeName):
        return (not (self.getBareKnownType(typeName) == "char*")) and (('&' in typeName) or ('*' in typeName))

    def getInitialValueCpp(self, typeName):

        isRef = '&' in typeName
        typeName = self.normalizeType(typeName)
        if((not typeName in self.initialValuesCpp) and (typeName[len(typeName)-1] == '*') or isRef):
            if isRef:
                return "storageForInitial_" + self.makeIdentifier(typeName)
            else:
                return "&storageForInitial_" + self.makeIdentifier(typeName[0:-1])
        else:
            return self.initialValuesCpp[typeName]

    def getInitialValueJson(self, typeName):
        return self.initialValuesJson[self.getBareKnownType(typeName)]

    def getUpdatedValueCpp(self, typeName):

        isRef = '&' in typeName
        typeName = self.normalizeType(typeName)
        if((not typeName in self.updateValuesCpp) and (typeName[len(typeName)-1] == '*') or isRef):
            if isRef:
                return "storageForUpdated_" + self.makeIdentifier(typeName)
            else:
                return "&storageForUpdated_" + self.makeIdentifier(typeName[0:-1])
        else:
            return self.updateValuesCpp[typeName]

    def getUpdatedValueJson(self, typeName):
        
        return self.updatedValuesJson[self.getBareKnownType(typeName)]

    def makeSignature(self, retType, paramTypes):
        params = []
        name = retType + "___"

        for i,p in enumerate(paramTypes):
            params += [{"name": "param" + str(i), "type":p}]
            name += p
            if(len(paramTypes) > i + 1):
                name += "__"

        return {
            "name":self.makeIdentifier(name),
            "return":retType, 
            "params": params
        }

    def makeIdentifier(self, text):
        result = ""
        validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
        repChars = {"*":"Ptr", "&":"Ref"}
        for c in text:
            if(c in validChars):
                result += c
            elif(c in repChars):
                result += repChars[c]
        return result

    def run(self):

        self.knownTypes = ["char*", "int8_t", "int16_t", "bool", "int32_t", "int64_t", "uint8_t", "uint16_t"
            , "uint32_t", "uint64_t", "double", "float", "std::string", "MyOwnStruct"]

        self.callSignatures = [
            self.makeSignature("void", ["char*"]),
            self.makeSignature("void", ["bool"]),
            self.makeSignature("void", ["int8_t"]),
            self.makeSignature("void", ["int16_t"]),
            self.makeSignature("void", ["int32_t"]),
            self.makeSignature("void", ["int64_t"]),
            self.makeSignature("void", ["uint8_t"]),
            self.makeSignature("void", ["std::string"]),
            self.makeSignature("void", ["uint32_t"]),
            self.makeSignature("void", ["uint64_t"]),
            self.makeSignature("void", ["float"]),
            self.makeSignature("void", ["const MyOwnStruct&"]),

            self.makeSignature("char*", []),
            self.makeSignature("bool", []),
            self.makeSignature("int8_t", []),
            self.makeSignature("int16_t", []),
            self.makeSignature("int32_t", []),
            self.makeSignature("int64_t", []),
            self.makeSignature("uint8_t", []),
            self.makeSignature("std::string", []),
            self.makeSignature("uint32_t", []),
            self.makeSignature("uint64_t", []),
            self.makeSignature("float", []),
            self.makeSignature("MyOwnStruct&", []),
            self.makeSignature("const MyOwnStruct&", []),
            self.makeSignature("const MyOwnStruct*", []),
            self.makeSignature("MyOwnStruct*", []),

            self.makeSignature("bool", ["const MyOwnStruct&", "std::string&"]),
            self.makeSignature("int8_t", ["MyOwnStruct&", "const std::string&"]),
            self.makeSignature("int16_t", ["const MyOwnStruct*", "std::string*"]),
            self.makeSignature("int32_t", ["MyOwnStruct*", "const std::string*"]),
            self.makeSignature("const char*", ["MyOwnStruct* const"]),
            self.makeSignature("char*", ["const MyOwnStruct* const"]),
            self.makeSignature("std::string", ["const uint32_t*"]),
            self.makeSignature("uint32_t", ["const uint64_t&"]),
            self.makeSignature("uint64_t", ["bool*"]),
            self.makeSignature("float", ["const float&"]),
        ]

        self.setup()
        self.emitTestCase()

        currDir = os.path.dirname(os.path.abspath(__file__)) + "/"
        d = os.path.dirname(currDir + self.getFileName())
        if not os.path.exists(d):
            os.makedirs(d)

        existingContent = ""
        newContent = ""
        fileName = currDir + self.getFileName()
        if(os.path.isfile(fileName)):
            f = open(fileName,'r')
            existingContent = f.read()
            existingContent = existingContent.replace("\r\n", "\n")

        for ln in self.emittedLines:
            newContent += ln + "\n"

        if(newContent != existingContent):
            print "Updating file at '" + os.path.abspath(fileName) + "'."

            f = open(fileName, 'w')
            f.write(newContent)
            f.close()
        else:
            print "File at '" + os.path.abspath(fileName) + "' is up to date."
