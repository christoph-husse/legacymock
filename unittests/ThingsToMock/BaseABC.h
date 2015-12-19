
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_BASEABC_H_
#define _UNITTEST_MOCK_BASEABC_H_

#include "citrix/Config.h"


namespace MockMe {

    struct BaseA {
        enum { member_MAGIC = 9208 };
        enum { inAOverriddenByB_MAGIC = 7736 };
        enum { inAOverriddenByBABC_MAGIC = 6786 };
        enum { inANotOverridden_MAGIC = 2444 };

        virtual ~BaseA() { }
        virtual int inAOverriddenByB(int a) { return member * inAOverriddenByB_MAGIC * a; }
        virtual int inAOverriddenByBABC(int a) { return member * inAOverriddenByBABC_MAGIC * a; }
        virtual int inCImplementsAOverriddenByABC(int a) = 0;
        virtual int inANotOverridden(int a) { return member * inANotOverridden_MAGIC * a; }

        BaseA(int factor = 1) : member(member_MAGIC * factor) { }

        virtual int getMember() { return member; }

        int member;
    };

    namespace internal {
        struct BaseA_ProbingInstance : public BaseA {
            virtual int inCImplementsAOverriddenByABC(int a) { return a; }
        };
    }

    struct BaseB : public BaseA {
        enum { member_MAGIC = 9369 };
        enum { inAOverriddenByB_MAGIC = 5895 };
        enum { inAOverriddenByBABC_MAGIC = 4937 };

        virtual int inAOverriddenByB(int a) { return member * inAOverriddenByB_MAGIC * a; }
        virtual int inAOverriddenByBABC(int a) { return member * inAOverriddenByBABC_MAGIC * a; }

        BaseB(int factor = 1) : 
            BaseA(member_MAGIC * factor),
            member(member_MAGIC * factor) { }

        virtual int getMember() { return member; }

        int member;
    };

    namespace internal {
        struct BaseB_ProbingInstance : public BaseB {
            virtual int inCImplementsAOverriddenByABC(int a) { return a; }
        };
    }

    struct BaseC : public BaseA {
        enum { member_MAGIC = 1868 };
        enum { inCImplementsAOverriddenByABC_MAGIC = 3455 };

        virtual int inCImplementsAOverriddenByABC(int a) { return member * inCImplementsAOverriddenByABC_MAGIC * a; }

        virtual int inCOverriddenByABC(int a) { return a;}

        BaseC(int factor = 1) : 
            BaseA(member_MAGIC * factor),
            member(member_MAGIC * factor) { }

        virtual int getMember() { return member; }

        int member;
    };

    struct BaseABC : public BaseB, public BaseC {
        enum { member_MAGIC = 9475 };
        enum { inCImplementsAOverriddenByABC_MAGIC = 2789 };
        enum { inAOverriddenByBABC_MAGIC = 12378 };
        enum { inCOverriddenByABC_MAGIC = 8269 };
        enum { inABCNotOverridden_MAGIC = 3567 };

        virtual int inCImplementsAOverriddenByABC(int a) { return member * inCImplementsAOverriddenByABC_MAGIC * a; }
        virtual int inAOverriddenByBABC(int a) { return member * inAOverriddenByBABC_MAGIC * a; }
        virtual int inCOverriddenByABC(int a) { return member * inCOverriddenByABC_MAGIC * a; }
        virtual int inABCNotOverridden(int a) { return member * inABCNotOverridden_MAGIC * a; }

        BaseABC() : 
            BaseB(member_MAGIC),
            BaseC(member_MAGIC),
            member(member_MAGIC) { }

        virtual int getMember() { return member; }

        int member;
    };
 }


#endif