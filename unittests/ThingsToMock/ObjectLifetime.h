
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_OBJECTLIFETIME_H_
#define _UNITTEST_MOCK_OBJECTLIFETIME_H_

#include "citrix/Config.h"

namespace MockMe {
    class Lifetime_EmptyCtorAndDtor {
    public:
        Lifetime_EmptyCtorAndDtor() {
        }

        ~Lifetime_EmptyCtorAndDtor() {
        }
    };

    class Lifetime_CallsMemberInCtorAndDtor {
    public:
        void calledDuringConstruction() { }
        void calledDuringDestruction() { }

        Lifetime_CallsMemberDuringDtor() {
            calledDuringConstruction();
        }

        ~Lifetime_CallsMemberDuringDtor() {
            calledDuringDestruction();
        }
    };

    class Lifetime_ThrowsDuringConstruction {
    public:
        Lifetime_ThrowsDuringConstruction() {
            throw std::bad_exception();
        }

        ~Lifetime_ThrowsDuringConstruction() {
        }
    };

    class Lifetime_ThrowsDuringDestruction {
    public:
        Lifetime_ThrowsDuringDestruction() {
        }

        ~Lifetime_ThrowsDuringDestruction() {
            throw std::bad_exception();
        }
    };

    class Lifetime_HasOnlyDtor {
    public:
        ~Lifetime_HasOnlyDtor() {
        }
    };

    class Lifetime_HasOnlyCtor {
    public:
        Lifetime_HasOnlyCtor() {
        }
    };

    class Lifetime_HasNoMember {
    };

    class Lifetime_CreatesSameTypeInCtor {
    public:
        Lifetime_CreatesSameTypeInCtor(int recurse = 4) {
            if(recurse > 0) {
                Lifetime_CreatesSameTypeInCtor anotherInstance((recurse - 1));
            }
        }

        ~Lifetime_CreatesSameTypeInCtor() {
        }
    };

    class Lifetime_CreatesSameTypeInDtor {
        int recurse;
    public:

        Lifetime_CreatesSameTypeInDtor(int recurse = 4) : recurse(recurse) {
        }

        ~Lifetime_CreatesSameTypeInDtor() {
            if(recurse > 0) {
                Lifetime_CreatesSameTypeInDtor anotherInstance((recurse - 1));
            }
        }
    };
}

#endif