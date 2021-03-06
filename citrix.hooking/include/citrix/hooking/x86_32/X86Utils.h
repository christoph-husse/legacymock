
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_X86_32_X86UTILS_H_
#define _CITRIX_HOOKING_X86_32_X86UTILS_H_

#include <vector>

#include "citrix/Platform.h"

#ifndef LEGACYMOCK_ARCH_X86_32
#   error "This class does not support the current processor."
#endif

namespace citrix { namespace hooking { namespace x86_32 {
        
    class X86Utils {
    public:
        static unsigned char* followJumps(unsigned char* codePtr);
        static unsigned char* getInstructionPointer();
        static bool makeMemoryExecutable(void* ptr, size_t len);
        static std::vector<unsigned char*> collectFunctionCalls(void* start, void* end);
        static int getInstructionLength(const void* instrPtr);
        static int relocate(const void* entryPoint, int epSize, void* relocatedCode, int bufferSize);
    };

}}}

#endif