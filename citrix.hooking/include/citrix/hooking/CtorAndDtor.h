
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_HOOKING_CTORANDDTOR_H_
#define _CITRIX_HOOKING_CTORANDDTOR_H_

#include "citrix/type_traits/member_function_pointer_traits.hpp"

namespace citrix { namespace hooking {

template<class TConstructorSignature>
struct TypedCtorAndDtor {
    typedef TConstructorSignature TConstructor;
    typedef typename type_traits::member_function_pointer_traits<TConstructor>::class_type TMemberType;
    typedef void (TMemberType::*TDestructor)();

    TConstructor ctor;
    TDestructor dtor;

    bool hasConstructor() const {
        return ctor != (TConstructorSignature)NULL;
    }

    bool hasDestructor() const {
        return dtor != (TDestructor)NULL;
    }

    TypedCtorAndDtor() : ctor(NULL), dtor(NULL) { }
    TypedCtorAndDtor(TConstructor ctor, TDestructor dtor) : ctor(ctor), dtor(dtor) {
        BOOST_STATIC_ASSERT_MSG((boost::is_member_function_pointer<TConstructorSignature>::value), "TConstructorSignature must refer to a member function pointer.");
    }
};

namespace internal {
    struct CtorAndDtor {
        void* ctor;
        void* dtor;

        CtorAndDtor() : ctor(NULL), dtor(NULL) { }
        CtorAndDtor(void* ctor, void* dtor) : ctor(ctor), dtor(dtor) { }

        template<class T>
        TypedCtorAndDtor<T> asTyped() { 
            typedef TypedCtorAndDtor<T> TTyped;
            return TTyped(*((T*)&ctor), *((typename TTyped::TDestructor*)&dtor));
        }
    };
}

}}

#endif
