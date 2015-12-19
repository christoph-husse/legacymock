
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/Platform.h"

#ifdef LEGACYMOCK_ARCH_X86_32

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Decoder.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Relocator.h"

#include "../citrix.binutils.udis86/include/citrix/binutils/udis86/InstructionLengthDecoder.h"

#ifdef LEGACYMOCK_COMP_MSVC
#   include <intrin.h>
#   pragma intrinsic(_ReturnAddress)
#endif

#if defined(LEGACYMOCK_OS_WINDOWS)
#   include <windows.h>
#elif defined(LEGACYMOCK_OS_MACOS)
#   include <sys/mman.h>
#   include <mach-o/dyld.h>
#   include <mach/mach_host.h>
#   include <mach/mach_init.h>
#   include <mach/vm_map.h>
#elif defined(LEGACYMOCK_OS_LINUX)
#   include <sys/mman.h>
#endif

namespace citrix { namespace hooking { namespace x86_32 {

int X86Utils::getInstructionLength(const void* instrPtr) {
    return citrix::binutils::udis86::InstructionLengthDecoder::decode(instrPtr);

    // TODO: this should somehow supports plugins?
    //return X86Decoder::getInstructionLength(instrPtr);
}

int X86Utils::relocate(const void* entryPoint, int epSize, void* relocatedCode, int bufferSize) {
    return X86Relocator::relocate(entryPoint, epSize, relocatedCode, bufferSize);
}

std::vector<unsigned char*> X86Utils::collectFunctionCalls(void* start, void* end) {
    unsigned char* ptr = (unsigned char*)start;
    std::vector<unsigned char*> calls;

    for(; ptr < (unsigned char*)end; ptr += getInstructionLength(ptr)) {
        if(*ptr != 0xE8)
            continue;

        calls.push_back((ptr + 5) + *((boost::int32_t*)(ptr + 1)));
    }
    
    return calls;
}

unsigned char* X86Utils::followJumps(unsigned char *codePtr){
    unsigned char* nextPtr = codePtr;

    if((codePtr[0] == 0xFF) && (codePtr[1] == 0x25)) {
        // "jmp [0]" followed by absolute address to a memory location where the absolute code location is stored
        nextPtr = (unsigned char*)**((int32_t**)(codePtr + 2));
    }
    
    if(codePtr[0] == 0xE9) {
        nextPtr += 5 + *((int32_t*)(codePtr + 1));
    }
    
    if(nextPtr != codePtr)
        return followJumps(nextPtr);
    else
        return codePtr;
}


unsigned char* X86Utils::getInstructionPointer() {
    unsigned char* retAddr =
#ifdef LEGACYMOCK_COMP_MSVC
        (unsigned char*)_ReturnAddress();
#elif defined(LEGACYMOCK_COMP_CLANG) | defined(LEGACYMOCK_COMP_GCC)
        (unsigned char*)__builtin_return_address(0);
#else
#   error "Your compiler is not supported."
#endif
    return retAddr;
}

#if defined(LEGACYMOCK_OS_WINDOWS)

#   include <windows.h>

    bool X86Utils::makeMemoryExecutable(void* ptr, size_t len) {
        DWORD prevProtect;
        return VirtualProtect(ptr, len, PAGE_EXECUTE_READWRITE, &prevProtect) != 0;
    }

#elif defined(LEGACYMOCK_OS_MACOS)

#   include <sys/mman.h>
#   include <mach-o/dyld.h>
#   include <mach/mach_host.h>
#   include <mach/mach_init.h>
#   include <mach/vm_map.h>

    bool X86Utils::makeMemoryExecutable(void* ptr, size_t len) {

        if(vm_protect(mach_task_self(),
				    (vm_address_t) ptr,
				    len, false, (VM_PROT_ALL | VM_PROT_COPY))) {
            return false;
        }

        return true;
    }

#elif defined(LEGACYMOCK_OS_LINUX)

#   include <sys/mman.h>

    bool X86Utils::makeMemoryExecutable(void* ptr, size_t len) {

        unsigned char* aligned = (unsigned char*)ptr;

        int pageSize = sysconf(_SC_PAGE_SIZE);
        if (pageSize <= 0)
            return false;
    
        aligned = (unsigned char*) ((size_t) aligned & ~(pageSize - 1));
        while(aligned < (unsigned char*)ptr + len) {
            if(mprotect(aligned, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
                return false;
         
            aligned += pageSize;
        }

        return true;
    }

#else

#   error "Your operating system is not supported."

#endif

}}}

#endif
