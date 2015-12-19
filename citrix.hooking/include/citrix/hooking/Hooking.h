
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_H_
#define _CITRIX_HOOKING_H_

#include "citrix/type_traits/member_function_pointer_traits.hpp"
#include "citrix/Platform.h"
	
#ifdef LEGACYMOCK_ARCH_X86_32

	#include "x86_32/X86CtorAddressDetector.h"
	#include "x86_32/X86Hook.h"
	#include "x86_32/X86HookRecursionGuard.h"
	#include "x86_32/X86Utils.h"

    namespace citrix { namespace hooking {
	    typedef citrix::hooking::x86_32::X86CtorAddressDetector CtorAddressDetector;
	    typedef citrix::hooking::x86_32::X86Utils Utils;

        namespace internal {
            typedef citrix::hooking::x86_32::X86Hook UnsafeHook;
            typedef citrix::hooking::x86_32::X86HookRecursionGuard HookRecursionGuard;
        }
    }}

#else
#   error "Your processor is not supported!"
#endif

namespace citrix { namespace hooking {
    
    template<class TFunctionToHook, class TTargetMemberType = void>
    class TypedHook {
    private:
        friend class HookRecursionGuard;

        internal::UnsafeHook hook;

        TypedHook(const TypedHook&);
        TypedHook& operator=(const TypedHook&);
    public:
        TypedHook(
            TFunctionToHook functionToHook, 
            typename type_traits::member_function_pointer_traits<TFunctionToHook, TTargetMemberType>::type redirectTo) 
                : hook(functionToHook, redirectTo) { 

        }

        virtual void uninstall() { hook.uninstall(); }
        virtual bool isInstalled() const { return hook.isInstalled(); }
    };

    class HookRecursionGuard {
    private:
        internal::HookRecursionGuard guard;

        HookRecursionGuard(const HookRecursionGuard&);
        HookRecursionGuard& operator=(const HookRecursionGuard&);
    public:
        template<class TFunctionToHook, class TTargetMemberType>
        HookRecursionGuard(TypedHook<TFunctionToHook, TTargetMemberType>& hook) : guard(hook.hook) { }

		template<class TFunctionToHook>
        HookRecursionGuard(TypedHook<TFunctionToHook>& hook) : guard(hook.hook) { }
    };


}}

#endif