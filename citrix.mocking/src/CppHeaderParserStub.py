import sys, CppHeaderParser, os, argparse, json, re

def emit(key, value):
    if(not type(value) is str):
        value = str(value)
    print key + ":" + value.replace('\\n', '\\\\n').replace('\\r', '\\\\r').replace('\n', '\\n').replace('\r', '\\r')


def filterComments(content):
    result = ""
    i = 0
    contentLen = len(content)
    while(i + 1 < contentLen):
        c = content[i]
        nc = content[i+1]
    
        if((c == '/') and (nc == '/')):
            # consume single line comment
            while(i < contentLen):
                if(content[i] == '\n'):
                    break
                i += 1
            continue

        if((c == '/') and (nc == '*')):
            # consume multi-line comment
            hasLineBreak = False
            while(i + 1 < contentLen):
                c = content[i]
                if((c == '*') and (content[i+1] == '/')):
                    i += 1
                    break

                if(c == "\n"):
                    hasLineBreak = True
                i += 1

            if(hasLineBreak):
                result += "\n"
            i += 1
            continue

        i += 1
        result += c

    if(i < contentLen):
        result += content[contentLen - 1]

    return result

try:
    parser = argparse.ArgumentParser(description='Transforms a given C++ header to JSON.')
    parser.add_argument('-i', dest='input', action='store', default=None, type=argparse.FileType('r'), help='A C++ header file to parse.')

    args = parser.parse_args()
    if(not args.input):
        parser.print_help()
        sys.exit(-1)

    try:
        headerContent = filterComments(args.input.read().replace('\r', ''))
        headerContent = re.sub(r"[^a-zA-Z_0-9]{1}throw\s*\([^\)]*\)", "", headerContent)
        cppHeader = CppHeaderParser.CppHeader(headerContent, "string")
    except CppHeaderParser.CppParseError as e:
        print "{\"Exception\":\"" + str(e).replace('"', '\'') + "\"}"
        sys.exit(-1)

    staticPattern = re.compile(r"^static[^a-zA-Z_0-9]{1}|[^a-zA-Z_0-9]{1}static[^a-zA-Z_0-9]{1}|[^a-zA-Z_0-9]{1}static$|^static$")

    for className in cppHeader.classes:
        
        klass = cppHeader.classes[className]

        def extractMethod(m, protection):
            parameters = []

            m["rtnType"] = re.sub(staticPattern, "", m["rtnType"])

            def extractParameter(p):
                t = p["type"].strip()
                if(p["reference"] and not "&" in t):
                    t = "::boost::add_reference<" + t + ">::type"

                emit("Param-Name", p["name"])
                emit("Param-Type", t)

            emit("Method-Name", m["name"])
            emit("Method-Accessibility", protection)
            emit("Method-IsDestructor", m["destructor"])
            emit("Method-IsConstructor", m["constructor"])
            emit("Method-IsVirtual", m["virtual"])
            emit("Method-LineNumber", m["line_number"])
            emit("Method-IsAbstract", m["pure_virtual"])
            emit("Method-ReturnType", m["rtnType"])
            emit("Method-IsConst", m["const"])
            emit("Method-IsStatic", m["static"])
            if(("isGetter" in m) and m["isGetter"]):
                emit("Method-IsGetter", 1)
            else:
                emit("Method-IsGetter", 0)
            emit("Method-Debug", m["debug"])
            for p in m["parameters"]:
                extractParameter(p)

        def extractVariable(m, access):
            m["isGetter"] = 1
            m["destructor"] = 0
            m["constructor"] = 0
            m["virtual"] = 0
            m["template"] = 0
            m["const"] = 0
            m["reference"] = 0
            m["pure_virtual"] = 0
            m["debug"] = ""
            m["returns"] = m["rtnType"] = m["type"] 
            m["parameters"] = []

            extractMethod(m, access)

        emit("Class-Name", className)
        emit("Class-LineNumber", klass["line_number"])
        emit("Class-Namespace", klass["namespace"])

        enumDefs = []
        for e in klass["enums"]["public"] + klass["enums"]["protected"] + klass["enums"]["private"]:
            if("name" in e):
                enumDefs += [e["name"]]
        emit("Class-Enums", ','.join(enumDefs))

        for m in klass["methods"]["public"]:
            extractMethod(m, "Public")
        for m in klass["methods"]["protected"]:
            extractMethod(m, "Protected")
        for m in klass["methods"]["private"]:
            extractMethod(m, "Private")

        for m in klass["properties"]["public"]:
            extractVariable(m, "Public")
        for m in klass["properties"]["protected"]:
            extractVariable(m, "Protected")
        for m in klass["properties"]["private"]:
            extractVariable(m, "Private")

    print "Header:" 
    print headerContent

    sys.exit(0)
except Exception as e:
    print "{\"Exception\":\"" + str(e).replace('"', '\'') + "\"}"
    sys.exit(-1)
