
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#include "stdafx.h"
#include "citrix/Config.h"
#include "gtest/gtest.h"
#include "AssemblerTestUtils.h"

namespace unittests {

    static std::vector<unsigned char> getExpectedAsm();
    static std::vector<unsigned char> getActualAsm();

   
    
    TEST(Assembler, SelfTest) {
        std::vector<unsigned char> actual = getActualAsm();
        const char* foundation = "50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 8b f8 | 8b e5 | ff e0 | ff e6 | e9 78 56 34 12 | b8 78 56 34 12 | b9 78 56 34 12 | be 21 43 65 87 | ";

        ASSERT_NE(0u, actual.size());
        
#ifdef _MSC_VER
        // MSVC produces the same binary code through all recent versions, which makes
        // it the best candidate for generating the expected byte sequence.
        // Other platforms will just use the "foundation" string, which is equivalent
        // the the expected byte sequence anyway.
        // GCC & LLVM generate slightly different bytecode and we support only one,
        // the one from MSVC...
        std::vector<unsigned char> expected = getExpectedAsm();
        std::string expectedStr = AssemblerX86::toString(expected);
        
        ASSERT_NE(0u, expected.size());
        ASSERT_EQ(foundation, expectedStr);
        ASSERT_EQ(expectedStr, AssemblerX86::toString(actual));
#endif
        
        ASSERT_EQ(foundation, AssemblerX86::toString(actual));
    }


    static std::vector<unsigned char> getActualAsm() {
        struct RP : public AssemblerX86 {
            RP() {
                push(eax);
                push(ecx);
                push(edx);
                push(ebx);
                push(esp);
                push(ebp);
                push(esi);
                push(edi);

                mov(edi, eax);
                mov(esp, ebp);

                jmp(eax);
                jmp(esi);

                jmp(0x12345678);

                mov(eax, 0x12345678);
                mov(ecx, 0x12345678);
                mov(esi, 0x87654321);
            }
        }actual;

        return actual.toBytes();
    }

    std::vector<unsigned char> expectedAssembly() {
        static int64_t alwaysTrue = 1;
        unsigned char* startOfAsm = ::citrix::hooking::Utils::getInstructionPointer();
        
        if(!(alwaysTrue++)) {
#   ifdef _MSC_VER
#       pragma warning (push, 0)
        __asm {
            // magic signature, to automatically determine code size
            _emit 0d5h 
            _emit 0d4h 
            _emit 06bh 
            _emit 0e4h

            push eax
            push ecx
            push edx
            push ebx
            push esp
            push ebp
            push esi
            push edi

            mov edi, eax
            mov esp, ebp

            jmp eax
            jmp esi

            jmp jmp_target + 12345678h
            jmp_target:

            mov eax, 12345678h
            mov ecx, 12345678h
            mov esi, 87654321h

            // magic signature, to automatically determine code size
            _emit 0d5h 
            _emit 0d4h 
            _emit 06bh 
            _emit 0e4h
        }
#       pragma warning (pop)
#   endif
        }
        unsigned char* endOfAsm = ::citrix::hooking::Utils::getInstructionPointer();
        
        std::vector<unsigned char> result(startOfAsm, endOfAsm);
        return result;
    }

    std::vector<unsigned char> getExpectedAsm() {

        std::vector<unsigned char> result, ptr = expectedAssembly();

        size_t i = 0;
        bool found = false;
        for (; i < ptr.size(); i++) {
            boost::int32_t magic = *((boost::int32_t*)&ptr[i]);
            if (magic == 0xE46BD4D5) {
                found = true;
                i += 4;
                break;
            }
        }

        if (found) {
            for (; i < ptr.size(); i++) {
                boost::int32_t magic = *((boost::int32_t*)&ptr[i]);
                if (magic == 0xE46BD4D5)
                    return result;

                result.push_back(ptr[i]);
            }
        }

        return std::vector<unsigned char>();
    }
}
