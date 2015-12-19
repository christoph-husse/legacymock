
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/Platform.h"

#ifdef LEGACYMOCK_ARCH_X86_32

#include <boost/cstdint.hpp>
#include <vector>
#include <string.h>

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Relocator.h"

namespace citrix { namespace hooking { namespace x86_32 {


bool X86Relocator::isInstructionSupported(const unsigned char* const src) const {

    unsigned char b1 = *src;
    unsigned char b2 = *(src + 1);

    switch(b1) {
        /*
	        The problem with (conditional) jumps is that there will be no return into the relocated entry point.
	        So the execution will be proceeded in the original method and this will cause the whole
	        application to remain in an unstable state. Only near jumps with 32-bit offset are allowed as
	        first instruction...
	    */
	    case 0xEB: // jmp imm8
	    case 0xE3: // jcxz imm8
	    {
            fail("Hooking near (conditional) jumps is not supported.");
		    return false;
	    }break;
	    case 0x0F:
	    {
		    if ((b2 & 0xF0) == 0x80) { // jcc imm16/imm32
                fail("Hooking far conditional jumps is not supported.");
			    return false;
            }
	    }break;
    }

    if ((b1 & 0xF0) == 0x70) { // jcc imm8
        fail("Hooking near conditional jumps is not supported.");
        return false;
    }

    return true;
}

int32_t X86Relocator::readRelativeAddressAsAbsolute(const unsigned char* ptrToRelAddr, const unsigned char* relativeToWhat) {
    return (int32_t)(relativeToWhat + *((boost::int32_t*)ptrToRelAddr));
}

void X86Relocator::writeAbsoluteAddressAsRelative(
            int32_t absoluteAddress,
            const unsigned char* relativeToWhat) {

    if (((size_t)absoluteAddress >= (size_t)entryPoint) && ((size_t)absoluteAddress < (size_t)entryPoint + entryPointLen)) {
		fail("Jumps back into entry point from within entry point are not supported.");
        return;
    }

    writeInt32(absoluteAddress - (int32_t)relativeToWhat);
}

bool X86Relocator::processJumpInstruction_E9(const unsigned char* const src) {
    if(*src != 0xE9) {
        return false;
    }

    writeByte(0xE9); 
    writeAbsoluteAddressAsRelative(
        readRelativeAddressAsAbsolute(src + 1, src + 5), 
        realDst + 4);

    return true;
}

void X86Relocator::writeByte(unsigned char byte) {
    if(!failIfNotEnoughSpaceLeft(1))
        return;

    *dst = byte;
    incrementDst(1);
}

void X86Relocator::writeInt32(int32_t imm32) {
    if(!failIfNotEnoughSpaceLeft(4))
        return;

    *((int32_t*)dst) = imm32;
    incrementDst(4);
}

bool X86Relocator::processCallInstruction_E8(const unsigned char* const src) {
    if(*src != 0xE8) {
        return false;
    }

    const int absAddr = readRelativeAddressAsAbsolute(src + 1, src + 5);

    if(absAddr == (int32_t)(src + 5)) {
        // this is likely an EIP extraction. We emulate with original EIP, instead
        // of relocated one (best guess, might crash, but will surely crash otherwise)
        writeByte(0x68); // push imm32
        writeInt32(absAddr);

        // we pushed the original return address so now we just need a jump
        writeByte(0xE9); 
    } else {
        writeByte(0xE8); 
    }

    writeAbsoluteAddressAsRelative(
        absAddr, 
        realDst + 4);

    return true;
}

void X86Relocator::incrementDst(int byteDelta) {
    dst += byteDelta;
    realDst += byteDelta;
}

void X86Relocator::justCopyInstruction(const unsigned char* const src, int const opcodeLen) {
    if(!failIfNotEnoughSpaceLeft(opcodeLen))
        return;

    memcpy(dst, src, opcodeLen);
    incrementDst(opcodeLen);
}

bool X86Relocator::failIfNotEnoughSpaceLeft(int requiredBytes) const {
    if(dst + requiredBytes > &dstBuffer[0] + dstBuffer.size()) {
        fail("There was not enough space to relocate all instructions.");
        return false;
    }

    return true;
}

int X86Relocator::process() {
    int opcodeLen = 0;
    const unsigned char* src = entryPoint;

    std::fill(dstBuffer.begin(), dstBuffer.end(), 0x90 /* NOP */);

    while((src + opcodeLen) < (unsigned char*)entryPoint + entryPointLen)
	{
        // always move one instruction forward in each iteration
        src += opcodeLen;
        opcodeLen = X86Utils::getInstructionLength(src);

        if(!isInstructionSupported(src))
            return 0;

        if(processCallInstruction_E8(src))
           continue; 

        if(processJumpInstruction_E9(src))
           continue; 
        
        justCopyInstruction(src, opcodeLen);
	}

    if(hasFailBit) {
        return 0;
    } else {
        return (int)(dst - &dstBuffer[0]);
    }
}

void X86Relocator::fail(std::string message) const {
    // we want fail to be available from "const" methods, but still record the message and fail later...
    X86Relocator* errThis = const_cast<X86Relocator*>(this);

    errThis->hasFailBit = true;
    errThis->failMessages.push_back(message);
}

X86Relocator::X86Relocator(const unsigned char* entryPoint, int entryPointLen) :
            entryPoint(entryPoint),
            entryPointLen(entryPointLen) {
}

int X86Relocator::relocate(
	const void* const entryPoint,
    int const entryPointLen,
	void* const relocatedCode,
    int const bufferSize)
{
    // TODO: really cover all relative instructions and write proper UnitTests...
    X86Relocator instance((const unsigned char*)entryPoint, entryPointLen);

    instance.dstBuffer.resize(bufferSize);
    instance.hasFailBit = false;
    instance.dst = &instance.dstBuffer[0];
    instance.realDst = (const unsigned char*)relocatedCode;

    int relocatedSize = instance.process();
    if(relocatedSize <= 0)
        return 0;

    memmove(relocatedCode, &instance.dstBuffer[0], relocatedSize);

    return relocatedSize;
}

}}}

#endif
