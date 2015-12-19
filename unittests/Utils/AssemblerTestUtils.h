
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_UNITTESTS_ASSEMBLERTESTUTILS_H_
#define _CITRIX_MOCK_UNITTESTS_ASSEMBLERTESTUTILS_H_

#include <map>
#include <boost/cstdint.hpp>

#include "Utils/StringUtils.h"
#include "../citrix.hooking/include/citrix/hooking/x86_32/X86Utils.h"

namespace unittests {
	using namespace citrix::hooking::x86_32;
	using namespace citrix::hooking;

	class AssemblerBase  {
	private:
		AssemblerBase(const AssemblerBase&);
		AssemblerBase& operator=(const AssemblerBase&);
	protected:
		std::vector<unsigned char> code;
		std::map<std::string, int> labels;
		std::vector<unsigned char> relocatedCode;
		std::map<std::string, size_t> importedLabels;

		void emit(int byte) {
			code.push_back((unsigned char)byte);
		}

		void emit32(boost::int64_t imm) {
			boost::int32_t imm32 = (boost::int32_t)imm;
			unsigned char* ptr = (unsigned char*)&imm32;
			emit(ptr[0]);
			emit(ptr[1]);
			emit(ptr[2]);
			emit(ptr[3]);
		}

		void makeLabel(std::string name) {
			labels[name] = code.size();
		}

		int label(std::string name) {
			if (labels.find(name) == labels.end()) {
				return importedLabels[name];
			} else {
				return (int)(&relocatedCode[0] + labels[name]);
			}
		}

	public:

		AssemblerBase() {
			code.reserve(1024);
			relocatedCode.resize(1024);
		}

		void import(const AssemblerBase& imports) {
			for(std::map<std::string, int>::const_iterator it = imports.labels.begin(); it != imports.labels.end(); it++) {
				importedLabels[it->first] = (size_t)(&imports.code[0] + it->second);
			}
		}

		std::string relocated() {
			int relocatedSize = X86Utils::relocate(&toBytes()[0], toBytes().size(), &relocatedCode[0], relocatedCode.size());

			if (relocatedSize <= 0)
				return "[UNABLE TO RELOCATE]";

			return toString(relocatedCode, relocatedSize);
		}

		std::string toString() {
			return toString(toBytes());
		}
	    
		static std::string toString(unsigned char* codePtr) {
			std::vector<unsigned char> code(codePtr, codePtr + 20);
			return toString(code);
		}

		static std::string toString(const std::vector<unsigned char>& bytes, int byteCount = -1) {
			std::ostringstream sout;
			int i = 0;

			if (byteCount < 0)
				byteCount = (int)bytes.size();

			while (i < byteCount) {
				int len = X86Utils::getInstructionLength(&bytes[i]);

				for (int j = i; (j < i + len) && (j < byteCount); j++) {
					sout << std::hex << std::setfill('0') << std::setw(2) << (int)bytes[j] << " ";
				}
				sout << "| ";
				i += len;
			}

			return sout.str();
		}

		const std::vector<unsigned char>& toBytes() {
			return code;
		}
	};

	#define entry_point             struct EP : public AssemblerX86 { void run() { 
	#define expect_relocated        }}; struct RP : public AssemblerX86 { void run() {  
	#define exit_point              }}; { EP ep; ep.run(); RP rp; rp.import(ep); rp.run(); ASSERT_EQ(rp.toString(), ep.relocated()); }


	class AssemblerX86 : public AssemblerBase {
	protected:
		enum ERegisters {
			eax = 0,
			ecx = 1,
			edx = 2,
			ebx = 3,
			esp = 4,
			ebp = 5,
			esi = 6,
			edi = 7,
		};

		void push(ERegisters reg) {
			emit(0x50 + (int)reg);
		}

		void mov(ERegisters dst, ERegisters src) {
			emit(0x8b);
			emit(0xc0 + (((int)src) & 0x7) + ((((int)dst) & 0x7) << 3));
		}

		void mov(ERegisters dst, boost::int64_t imm) {
			emit(0xb8 + (((int)dst) & 0x7));
			emit32(imm);
		}

		void jmp(ERegisters reg) {
			emit(0xff);
			emit(0xe0 + (((int)reg) & 0x7));
		}

		void jmp(boost::int64_t imm) {
			emit(0xe9);
			emit32(imm);
		}
	};
}

#endif
