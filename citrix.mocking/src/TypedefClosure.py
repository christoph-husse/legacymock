# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
 

import re

def extractTypedefClosure(config):

    typedefPattern = re.compile(r"[^a-zA-Z_0-9]{1}(typedef\s+[^;]+;)")
    typedefs = []

    for m in typedefPattern.finditer(config.headerContent):
        line = m.group(1);
        lineNo = config.headerContent.count("\n", 0, m.start()) + 1

        if (config.classStart <= lineNo) and (config.classEnd >= lineNo):
            typedefs += [line]

    return typedefs;

def doesTemplateExtendToLine(config, lineNumber):

    templatePattern = re.compile(r"[^a-zA-Z_0-9]{1}(template[\s\n]*<[^;\{\}\(\)]+[;\{\}\(\)])")

    for m in templatePattern.finditer(config.headerContent):
        startLine = config.headerContent.count("\n", 0, m.start() + 1) + 1
        endLine = config.headerContent.count("\n", 0, m.end() - 1) + 1

        if (startLine <= lineNumber) and (endLine >= lineNumber):
            return m.group(1)

    return None;

