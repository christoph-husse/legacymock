
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_NONCOPYABLENONDEFAULTCTOR_H_
#define _UNITTEST_MOCK_NONCOPYABLENONDEFAULTCTOR_H_

#include "citrix/Config.h"
#include <boost/shared_ptr.hpp>

namespace MockMe {

    class NonCopyableNonDefaultCtor_Param {
    public:
        int value;

        NonCopyableNonDefaultCtor_Param(int value) : value(value) {}

    private:
        NonCopyableNonDefaultCtor_Param() {}

        NonCopyableNonDefaultCtor_Param(const NonCopyableNonDefaultCtor_Param&);
        NonCopyableNonDefaultCtor_Param& operator=(const NonCopyableNonDefaultCtor_Param&);
    };

    class NonCopyableNonDefaultCtor {
    public:
        NonCopyableNonDefaultCtor_Param& void__void(NonCopyableNonDefaultCtor_Param& param) { 
            static NonCopyableNonDefaultCtor_Param staticValue((123));
            param.value = 456;
            return staticValue;
        }

        NonCopyableNonDefaultCtor(const NonCopyableNonDefaultCtor_Param& param) {
            CITRIX_MOCK_UNREFPARAM(param);
        }
    
    private:
        NonCopyableNonDefaultCtor() {}

        NonCopyableNonDefaultCtor(const NonCopyableNonDefaultCtor&);
        NonCopyableNonDefaultCtor& operator=(const NonCopyableNonDefaultCtor&);
    };

}

namespace citrix {
    namespace mocking {
        namespace internal {
            template<class T> boost::shared_ptr<T> allocateDefaultInstance();

            template<>
            static boost::shared_ptr<MockMe::NonCopyableNonDefaultCtor_Param> allocateDefaultInstance<MockMe::NonCopyableNonDefaultCtor_Param>() {
                return boost::shared_ptr<MockMe::NonCopyableNonDefaultCtor_Param>(new MockMe::NonCopyableNonDefaultCtor_Param(887));
            }
        }
    }
}


#endif