
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "citrix/Platform.h"

#ifdef LEGACYMOCK_ARCH_X86_32

#include <algorithm>
#include <iostream>

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86CtorAddressDetector.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"

namespace citrix { namespace hooking { namespace x86_32 {

bool X86CtorAddressDetector::firstFunctionCallsSecondFunction(void* first, void* second) {

    // TODO: safety switch to make sure we have made the right choice.

    CITRIX_MOCK_UNREFPARAM(first);
    CITRIX_MOCK_UNREFPARAM(second);
    return true;
}

internal::CtorAndDtor X86CtorAddressDetector::scanForCtorAndDtor(boost::int64_t start, boost::int64_t end) {
    const internal::CtorAndDtor not_found;

    if(std::abs((long)(end - start)) > 1000)
        return not_found;

    if(end < start)
        std::swap(start, end);

    std::vector<unsigned char*> calls = X86Utils::collectFunctionCalls((void*)start, (void*)end);
    

#if defined(LEGACYMOCK_COMP_MSVC)

    switch(calls.size()) {
    case 1: {
        // must be a constructor, and we have no destructor
        return internal::CtorAndDtor(calls[0], NULL);
    }break;
    case 2: {
        // must be a destructor, and we have no constructor
        if(!firstFunctionCallsSecondFunction(calls[0], calls[1]))
            return not_found;

        return internal::CtorAndDtor(NULL, calls[1]);
    }
    case 3: {
        // we have constructor at index 0 and destructor in both other places
        if(!firstFunctionCallsSecondFunction(calls[1], calls[2]))
            return not_found;

        if((calls[0] == calls[1]) || (calls[0] == calls[2]))
            return not_found;

        return internal::CtorAndDtor(calls[0], calls[2]);
    }
    }

#elif defined(LEGACYMOCK_COMP_CLANG) || defined(LEGACYMOCK_COMP_GCC)

    std::vector<unsigned char*> uniqueCalls;
    
    for(std::vector<unsigned char*>::iterator it = calls.begin(); it != calls.end(); it++) {
        if(std::find(uniqueCalls.begin(), uniqueCalls.end(), *it) == uniqueCalls.end()) {
            uniqueCalls.push_back(*it);
        }
    }
    
    if(uniqueCalls.size() == 2) {
        return internal::CtorAndDtor(uniqueCalls[0], uniqueCalls[1]);
    }
    
    if(uniqueCalls.size() == 1) {
        if(calls.size() == 1) {
            return internal::CtorAndDtor(uniqueCalls[0], NULL);
        } else {
            return internal::CtorAndDtor(NULL, uniqueCalls[0]);
        }
    }

#else
#   error "Your compiler is not supported."
#endif

    return not_found;
}

}}}

#endif
