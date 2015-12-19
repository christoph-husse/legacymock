
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_PRIVATEANDPROTECTED_H_
#define _UNITTEST_MOCK_PRIVATEANDPROTECTED_H_

#include "citrix/Config.h"
#include <string>

template<typename> class CitrixMockingAccessor;

namespace MockMe {

    
    class PrivateAndProtected {
        template<typename> friend class CitrixMockingAccessor;
        friend class PrivateAndProtectedForwarder;
    public:
        typedef std::string string;

        int publicVariable;

        string test(string a) {
            CITRIX_MOCK_UNREFPARAM(a);
            return string();
        }

        int disableMockTest_myProtectedFunc() {
            return myProtectedFunc();
        }

        int disableMockTest_myPrivateFunc() {
            return myPrivateFunc();
        }

    protected:
        int protectedVariable;

        int myProtectedFunc() {
            return 1;
        }
    private:
        int privateVariable;

        int myPrivateFunc() {
            return 2;
        }
    };

    class PrivateAndProtectedForwarder {
    public:
        static int forward_myProtectedFunc(PrivateAndProtected& instance) {
            return instance.myProtectedFunc();
        }

        static int forward_myPrivateFunc(PrivateAndProtected& instance) {
            return instance.myPrivateFunc();
        }
    };
}

#endif