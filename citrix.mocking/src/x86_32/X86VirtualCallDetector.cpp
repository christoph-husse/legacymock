
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/SystemPreincludeFixes.h"
#include "citrix/Config.h"

#include "../citrix.mocking/include/citrix/mocking/VirtualCallDetector.h"

namespace citrix { namespace mocking { namespace internal {


boost::shared_ptr<size_t> VirtualCallDetector::tlsVirtualCallAddress;

VirtualCallDetector::VirtualCallDetector() :
        virtualCallAddr(NULL),
        dummyAddr(NULL),
        callRegister(0)
{
    reset();
}

bool VirtualCallDetector::tryInitialize(void* startAddr_, void* endAddr_, void* stubEaxReceiver) {
    if(startAddr_ > endAddr_) {
        std::swap(startAddr_, endAddr_);
    }

    if(!detectVirtualCallAndDummy(startAddr_, endAddr_))
        return false;

    shiftSandwichCodeIntoDummyCall();
    rerouteVirtualCallToPatchedStub(stubEaxReceiver);

    return true;
}

void VirtualCallDetector::dummyCallForMoreBytes() { 
    // this will never actually be called...
}

void VirtualCallDetector::failFastRelativeCalls() {
    // this is needed to make sure that the relocator can handle relative calls
    // that the compiler may generator sometimes under the hood...
}
    
bool VirtualCallDetector::findVirtualAddress(uint16_t* startAddr, uint16_t* endAddr) {
    virtualCallAddr = NULL;

    while(startAddr < endAddr) {
        if((*startAddr & 0xF8FF) == 0xD0FF /* call %register% */) {
            if(virtualCallAddr) {
                virtualCallAddr = NULL;
                return false;
            }

            callRegister = (*startAddr & 0x0700) >> 8;
            virtualCallAddr = startAddr;
        }

        startAddr = (uint16_t*)(((unsigned char*)startAddr) + Utils::getInstructionLength(startAddr));
    }

    return virtualCallAddr != NULL;
}

bool VirtualCallDetector::findDummyAddress(uint16_t* startAddr, uint16_t* endAddr) {
    dummyAddr = NULL;

    unsigned char* ptr = (unsigned char*)startAddr;

    for(; ptr < (unsigned char*)endAddr; ptr += Utils::getInstructionLength(ptr)) {
        if(*ptr != 0xE8)
            continue;

        void* addr = (ptr + 5) + *((boost::int32_t*)(ptr + 1));

        if(addr == methodPtrToPVoid(&VirtualCallDetector::dummyCallForMoreBytes)) {
            dummyAddr = ptr;
            return true;
        }
    }
   
    return false;
}

bool VirtualCallDetector::detectVirtualCallAndDummy(void* startAddr_, void* endAddr_) {
    uint16_t* startAddr = (uint16_t*)startAddr_;
    uint16_t* endAddr = (uint16_t*)endAddr_;

    do {
        if(!findVirtualAddress(startAddr, endAddr))
            break;

        if(!findDummyAddress(startAddr, endAddr))
            break;

        if(dummyAddr < virtualCallAddr)
            break;

        if(!Utils::makeMemoryExecutable(&patchStubBuffer[0], patchStubBuffer.size()))
            break;

        if(!Utils::makeMemoryExecutable(virtualCallAddr, (((unsigned char*)dummyAddr) + 5) - (unsigned char*)virtualCallAddr))
            break;

        return true;
    }
    // error handling goes here...        
    while(false);

    reset();

    return false;
}

void VirtualCallDetector::reset() {
    virtualCallAddr = NULL;
    dummyAddr = NULL;
    callRegister = 0;

    patchStubBuffer.resize(128);
    memset(&patchStubBuffer[0], 0x90, patchStubBuffer.size());
}

void VirtualCallDetector::shiftSandwichCodeIntoDummyCall() {
    unsigned char* vptr = (unsigned char*)virtualCallAddr;
    unsigned char* dptr = (unsigned char*)dummyAddr;
    const int byteCount = dptr - vptr;

    // shift the code between virtual call and dummy 5 bytes forwards,
    // which will effectively overwrite the "call dummy" (which is 5 bytes)
    // with the preceding code. 
    int relocatedSize = Relocator::relocate(
        vptr,
        byteCount,
        vptr + 5,
        byteCount);

    if(relocatedSize <= 0) 
        throw "Could not relocate sandwich code.";

    // we bought 7 bytes (2 bytes of virtual call + 5 bytes of dummy call)

    // the NOPs will leave the code in a well-behaved state when this method returns...
    memset(vptr, 0x90 /* NOP */, 5);
}

void VirtualCallDetector::rerouteVirtualCallToPatchedStub(void* stubAddr) {

    unsigned char trampolineSrc[31] = {
        0x90,                           // nop (can be used for 0xCC, debug break)
        0x60,                           // pushad  
        0x50 + (callRegister & 0x7),    // push %register%
        0xE8, 0, 0, 0, 0,               // call [VirtualCallDetector::writeVirtualCallAddressToTls]

        0x58,                           // pop the argument, we pushed, back from stack (caller cleans)
        0x61,                           // popad  
        0xE8, 0, 0, 0, 0,               // call patchedStub
        0xE9, 0, 0, 0, 0,               // jmp back into sandwich code
    };

    unsigned char* const vptr = (unsigned char*)virtualCallAddr;
    unsigned char* const trampoline = &patchStubBuffer[0];
    memcpy(trampoline, trampolineSrc, sizeof(trampolineSrc));

    *((uint32_t*)(&trampoline[4])) = (int32_t)((unsigned char*)VirtualCallDetector::writeVirtualCallAddressToTls - &trampoline[8]);
    *((uint32_t*)(&trampoline[11])) = ((uint32_t)stubAddr - (uint32_t)&trampoline[15]);
    *((uint32_t*)(&trampoline[16])) = ((uint32_t)&vptr[5] - (uint32_t)&trampoline[20]);


    vptr[0] = 0xE9; // jmp trampoline
    *((uint32_t*)(vptr + 1)) = ((uint32_t)trampoline - (uint32_t)(vptr + 5));
    vptr[5] = 0x90; // NOP
    vptr[6] = 0x90; // NOP
}

void LEGACYMOCK_CALLER_CLEANS_STACK VirtualCallDetector::writeVirtualCallAddressToTls(void* address) {
    if(!tlsVirtualCallAddress.get()) {
        tlsVirtualCallAddress.reset(new size_t());
    }

    *tlsVirtualCallAddress = (size_t)address;
}

size_t VirtualCallDetector::getVirtualCallAddressFromTls() {
    if(!tlsVirtualCallAddress.get()) {
        return 0;
    }

    return *tlsVirtualCallAddress;
}

}}}

