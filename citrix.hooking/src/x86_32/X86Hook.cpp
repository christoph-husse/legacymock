
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/Platform.h"

#ifdef LEGACYMOCK_ARCH_X86_32

#include <boost/array.hpp>
#include <boost/cstdint.hpp>
#include <string.h>

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Hook.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"

namespace citrix { namespace hooking { namespace x86_32 {

#define MINIMAL_EP_SIZE 5

struct X86HookPimpl {
    boost::array<unsigned char, 100> relocated;
    boost::array<unsigned char, 100> trampoline;
    int entrySize;
    boost::int64_t originalEntryPoint;
    unsigned char* entryPoint;
    unsigned char* hookHandler;

    // TODO: replace with  boost::thread_specific_ptr<> when proper testcases added
    boost::shared_ptr<int> recursionTracker;

    X86HookPimpl() :
        entrySize(0),
        originalEntryPoint(0),
        entryPoint(0),
        hookHandler(0)
    {
    }
};

void X86Hook::uninstall() {
    if(!pimpl)
        return;

    if(pimpl->entryPoint && pimpl->originalEntryPoint) {
        *((boost::int64_t*)pimpl->entryPoint) = pimpl->originalEntryPoint;
    }

    pimpl.reset();
}

void* X86Hook::getCleanEntryPoint() {
    if(!pimpl)
        return NULL;

    return &pimpl->relocated[0];
}

int X86Hook::determineEntryPointSize(const void* const entryPoint) {
    const unsigned char* ptr = (const unsigned char*)entryPoint;
    int entrySize = 0;

    do {
        int len = X86Utils::getInstructionLength(ptr);

        if(len <= 0)
            return false;

        entrySize += len;
        ptr += len;
    } while(entrySize < MINIMAL_EP_SIZE);

    return entrySize;
}

bool X86Hook::install(void* functionToHook, void* redirectTo) {

    if(this->pimpl)
        return true;

    boost::shared_ptr<X86HookPimpl> pimpl(new (std::nothrow) X86HookPimpl);
    pimpl->entryPoint = X86Utils::followJumps((unsigned char*)functionToHook);
    pimpl->hookHandler = X86Utils::followJumps((unsigned char*)redirectTo);
    pimpl->entrySize = determineEntryPointSize(pimpl->entryPoint);

    if(!createRelocatedEntrypoint(pimpl) || 
            !overwriteFunctionEntrypoint(pimpl) ||
            !setupTrampoline(pimpl))
        return false;

    this->pimpl = pimpl;
    return true;
}


bool X86Hook::createRelocatedEntrypoint(PX86HookPimpl pimpl) {

    int relocatedSize = X86Utils::relocate(
        pimpl->entryPoint, 
        pimpl->entrySize,
        &pimpl->relocated[0],
        pimpl->relocated.size());

    if(relocatedSize <= 0)
        return false;

    writeJump(
        &pimpl->relocated[0] + relocatedSize, 
        pimpl->entryPoint + pimpl->entrySize);
    
    relocatedSize += 5;

    return X86Utils::makeMemoryExecutable(&pimpl->relocated[0], relocatedSize);
}

bool X86Hook::setupTrampoline(PX86HookPimpl pimpl) {

    unsigned char trampolineSrc[31] = {
        0x90,               // nop (can be used for 0xCC, debug break)
        0x60,               // pushad  
        0x68, 0, 0, 0, 0,   // push [pimpl]
        0xE8, 0, 0, 0, 0,   // call [X86Hook::shouldCallHandler]
        0x83, 0xF8, 0,      // cmp eax,0  
        0x74, 0x07,         // je "invoke original"
    // invoke handler
        0x58,               // pop the argument, we pushed, back from stack (caller cleans)
        0x61,               // popad  
        0xE9, 0, 0, 0, 0,   // jmp [pimpl->hookHandler]
    // invoke original
        0x58,               // pop the argument, we pushed, back from stack (caller cleans)
        0x61,               // popad
        0xE9, 0, 0, 0, 0    // jmp [pimpl->relocated]
    };

    memcpy(&pimpl->trampoline[0], trampolineSrc, sizeof(trampolineSrc));

    *((uint32_t*)(&pimpl->trampoline[3])) = (uint32_t)pimpl.get();
    *((uint32_t*)(&pimpl->trampoline[8])) = (int32_t)((unsigned char*)X86Hook::shouldCallHandler - &pimpl->trampoline[12]);
    *((uint32_t*)(&pimpl->trampoline[20])) = (int32_t)(pimpl->hookHandler - &pimpl->trampoline[24]);
    *((uint32_t*)(&pimpl->trampoline[27])) = (int32_t)(pimpl->relocated.data() - &pimpl->trampoline[31]);

    return X86Utils::makeMemoryExecutable(&pimpl->trampoline[0], pimpl->trampoline.size());
}

int X86Hook::shouldCallHandler(X86HookPimpl* pimpl) {
    return (pimpl->recursionTracker == NULL) || (*pimpl->recursionTracker == 0);
}

bool X86Hook::overwriteFunctionEntrypoint(PX86HookPimpl pimpl) {
    pimpl->originalEntryPoint = *((boost::int64_t*)pimpl->entryPoint);

    if(!X86Utils::makeMemoryExecutable(pimpl->entryPoint, 8))
        return false;
   
    // This is the only external change. After this, there is no going back ;)
    writeJump(pimpl->entryPoint, &pimpl->trampoline[0]);
    
    return true;
}

bool X86Hook::isInstalled() const {
    return !!pimpl;
}

void X86Hook::writeJump(unsigned char* codePtr, void* jmpTarget) {
    codePtr[0] = 0xE9;
    
    int32_t relative = ((boost::int32_t)jmpTarget - ((boost::int32_t)codePtr + 5));
    
    *((boost::int32_t*)&codePtr[1]) = (boost::int32_t)relative;
}

X86Hook::X86Hook(void* functionToHook, void* redirectTo) {
    install(functionToHook, redirectTo);
}

X86Hook::~X86Hook() {
    uninstall();
}

X86HookRecursionGuard::X86HookRecursionGuard(X86Hook& hook) : hook(hook) {
    if(!hook.isInstalled())
        return;

    if(!hook.pimpl->recursionTracker.get()) {
        hook.pimpl->recursionTracker.reset(new int(0));
    } 

    (*hook.pimpl->recursionTracker)++;
}

X86HookRecursionGuard::~X86HookRecursionGuard() {
    if(!hook.isInstalled())
        return;

    (*hook.pimpl->recursionTracker)--;
}

}}}


#endif
