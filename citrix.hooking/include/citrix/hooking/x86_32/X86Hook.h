
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_X86_32_X86HOOK_H_
#define _CITRIX_HOOKING_X86_32_X86HOOK_H_

#include <boost/shared_ptr.hpp>

#include "citrix/Platform.h"

#ifndef LEGACYMOCK_ARCH_X86_32
#   error "This class does not support the current processor."
#endif

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86HookRecursionGuard.h"

namespace citrix { namespace hooking { namespace x86_32 {
        
    class X86Hook {
    private:
        friend class X86HookRecursionGuard;

        typedef boost::shared_ptr<struct X86HookPimpl> PX86HookPimpl;
        PX86HookPimpl pimpl;

        X86Hook(const X86Hook&);
        X86Hook& operator=(const X86Hook&);

        bool install(void* functionToHook, void* redirectTo);
        bool createRelocatedEntrypoint(PX86HookPimpl pimpl);

        static bool overwriteFunctionEntrypoint(PX86HookPimpl pimpl);
        static void writeJump(unsigned char* codePtr, void* jmpTarget);
        static int determineEntryPointSize(const void* const entryPoint);
        static int LEGACYMOCK_CALLER_CLEANS_STACK shouldCallHandler(X86HookPimpl* pimpl);
        static bool setupTrampoline(PX86HookPimpl pimpl);
    protected:
        void* getCleanEntryPoint();

    public:
        
        virtual ~X86Hook();
        X86Hook(void* functionToHook, void* redirectTo);

        template<class TSrc, class TDst>
        X86Hook(TSrc functionToHook, TDst redirectTo) {
            void* src = *((void**)&functionToHook);
            void* dst = *((void**)&redirectTo);

            install(src, dst);
        }

        void uninstall();
        bool isInstalled() const;
    };

}}}

#endif