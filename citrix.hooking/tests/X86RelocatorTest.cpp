
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include <gtest/gtest.h>

#include "citrix/Config.h"
#include "../citrix.hooking/include/citrix/hooking/Hooking.h"
#include "Utils/AssemblerTestUtils.h"

namespace unittests { namespace X86Relocator {

    /*
        The tests here need to verify that a given entry point is properly
        relocated by X86Relocator::relocate(). 
        "X86RelocatorUtils.h" provides a useful testing framework for this task.

        For instance, the test:

                entry_point {
                    jmp(0x12345678);
                } expect_relocated {
                    mov(eax, 0x12345678);
                    jmp(eax);
                } end

        does the following:

        1) translate "jmp(0x12345678);" into bytecode 
        2) translate "mov(eax, 0x12345678); jmp(eax);" into bytecode
        3) Relocates (1) with X86Relocator
        4) Converts (3) and (2) to byte strings "b8 1d e3 17 01 | ff e0 | " and "b8 78 56 34 12 | ff e0 | "
        5) Will use ASSERT_EQ() on both strings. Will find out that 
                Value of : ep.relocated()
                Actual : "b8 1d e3 17 01 | ff e0 | "
                Expected : rp.toString()
                Which is : "b8 78 56 34 12 | ff e0 | "

        Which is a pretty error message for this abombination of a test. The bytes
        are grouped into instructions, using our X86Decoder.

        So this is it. Write the entry point into the first brackets and the
        expected relocated assembly code into the second brackets. You can use
        "delta" to access the runtime difference between the entry point and the
        relocation buffer. 
    */


    TEST(X86Relocator, justCopy) {
        entry_point {
            push(ebp);
            mov(ebp, esp);
        } expect_relocated {
            push(ebp);
            mov(ebp, esp);
        } exit_point
    } 

    /*
    TODO: make this work again...
    TEST(X86Relocator, relocateJump) {
        entry_point {
            jmp(0x12345678);
            makeLabel("jmp");
            push(ebp);
            mov(ebp, esp);
        } expect_relocated {
            makeLabel("jmpRelocated");
            jmp((label("jmp") + 0x12345678) - (label("jmpRelocated") + 5));
            push(ebp);
            mov(ebp, esp);
        } exit_point
    }
    */


}}