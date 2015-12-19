
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_VIRTUALCALLDETECTOR_H_
#define _CITRIX_MOCKING_VIRTUALCALLDETECTOR_H_

#include "citrix/SystemPreincludeFixes.h"
#include "citrix/Config.h"

#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Relocator.h"

#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace citrix { namespace mocking { namespace internal {

class VirtualCallDetector {
protected:
    typedef citrix::hooking::x86_32::X86Utils Utils;
    typedef citrix::hooking::x86_32::X86Relocator Relocator;

    VirtualCallDetector();
    bool tryInitialize(void* startAddr_, void* endAddr_, void* stubEaxReceiver);
    void dummyCallForMoreBytes();
    void failFastRelativeCalls();

    static size_t getVirtualCallAddressFromTls();

    template<class T>
    static void* methodPtrToPVoid(T ptr) {
        BOOST_STATIC_ASSERT_MSG(boost::is_member_function_pointer<T>::value, 
            "This method requires a member function pointer.");

        return *((void**)&ptr);
    }
    
private:
    void* virtualCallAddr;
    void* dummyAddr;
    int callRegister;
    std::vector<unsigned char> patchStubBuffer;

    static boost::shared_ptr<size_t> tlsVirtualCallAddress;

    void reset();
    bool findVirtualAddress(uint16_t* startAddr, uint16_t* endAddr);
    bool findDummyAddress(uint16_t* startAddr, uint16_t* endAddr);
    bool detectVirtualCallAndDummy(void* startAddr_, void* endAddr_);
    void shiftSandwichCodeIntoDummyCall();
    void rerouteVirtualCallToPatchedStub(void* stubAddr);

    static void LEGACYMOCK_CALLER_CLEANS_STACK writeVirtualCallAddressToTls(void* address);
};

}}}

#endif