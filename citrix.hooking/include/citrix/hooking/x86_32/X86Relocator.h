
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_X86_32_X86RELOCATOR_H_
#define _CITRIX_HOOKING_X86_32_X86RELOCATOR_H_

#include "citrix/Platform.h"

#ifndef LEGACYMOCK_ARCH_X86_32
#   error "This class does not support the current processor."
#endif

namespace citrix { namespace hooking { namespace x86_32 {
        
    class X86Relocator {
    public:
        static int relocate(
            const void* const entryPoint,
            int const epSize,
            void* const relocatedCode,
            int const bufferSize);

    private:

        int process();
        bool processCallInstruction_E8(const unsigned char* const src);
        bool processJumpInstruction_E9(const unsigned char* const src);
        void rewriteJump(size_t relAddr);
        bool isInstructionSupported(const unsigned char* const src) const;
        void fail(std::string message) const;
        void writeInt32(int32_t imm32);
        void writeByte(unsigned char byte);
        void incrementDst(int byteDelta);
        void justCopyInstruction(const unsigned char* const src, int const opcodeLen);
        bool failIfNotEnoughSpaceLeft(int requiredBytes) const;

        int32_t readRelativeAddressAsAbsolute(const unsigned char* ptrToRelAddr, const unsigned char* relToWhat);
        void writeAbsoluteAddressAsRelative(
            int32_t absoluteAddress,
            const unsigned char* relativeToWhat);

        X86Relocator(const unsigned char* entryPoint, int entryPointLen);

        std::vector<std::string> failMessages;
        std::vector<unsigned char> dstBuffer;
        bool hasFailBit;
        unsigned char* dst;
        const unsigned char* const entryPoint;
        int const entryPointLen;
        const unsigned char* src;
        const unsigned char* realDst;
    };

}}}

#endif