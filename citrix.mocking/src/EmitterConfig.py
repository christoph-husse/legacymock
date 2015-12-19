# Copyright (C) Citrix Online LLC   
# All Rights Reserved Worldwide.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Citrix Online LLC nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: 
#      christoph.husse@citrix.com (Christoph Husse)
#

import sys, CppHeaderParser 

class EmitterConfig:

    dstDir = r"../UnitTests/GeneratedMocks"
    classToMock = []
    className = "ECFile"
    headerGuard = "_TDD_INTRUSIVEMOCKS_" + className + "_H_"
    publicIncludes = ["\"" + className + ".h\""]
    internalIncludes = []
    publicMockInlines = []
    privateMockInlines = []
    enableCtorMocking = 0
    enableDtorMocking = 0
    mockManagerName = className + "MockManager"
    mockName = className + "Mock"
    hooksName = className + "Hooks"
    hooksInternalName = className + "HooksInternal"
    beSilent = False

    def logInfo(self, message): 
        if(not self.beSilent):
            print message
