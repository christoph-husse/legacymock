
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_CUSTOMTYPESFORTESTING_H_
#define _UNITTEST_MOCK_CUSTOMTYPESFORTESTING_H_

#if LEGACYMOCK_SUPPORTS_TRACING
#   include "../citrix.tracing/include/citrix/tracing/Translator.h"
#endif

#include "citrix/type_traits/normalize_type.hpp"

namespace SomeEnums {
    enum SomeEnum {
        MemberA = 23,
        MemberB = -8,
    };
}

struct MyOwnStruct {
    int number;
    std::string str;
    
    struct MySubStruct {
        int number;
        std::string str;
    };
    
    MySubStruct sub;
    
    MyOwnStruct(bool isUpdated = false) {
        if(!isUpdated) {
            number = 123;
            str = "Hello MyStruct!";
            sub.number = -178;
            sub.str = "Hello SubStruct!";
        } else {
            number = 123;
            str = "Hello MyStruct!";
            sub.number = -178;
            sub.str = "Hello SubStruct Updated!";
        }
    }

    static void internal_copy_accessor(MyOwnStruct* dst, const MyOwnStruct* src) {
        *dst = *src;
    }

    bool operator ==(const MyOwnStruct& other) const {
        return (sub.number == other.sub.number) && (sub.str == other.sub.str) &&
            (number == other.number) && (str == other.str);
    }

    bool operator !=(const MyOwnStruct& other) const {
        return !(*this == other);
    }

private:
    MyOwnStruct(const MyOwnStruct& src) : sub(src.sub), number(src.number), str(src.str) {}
    MyOwnStruct& operator=(const MyOwnStruct& src) {
        this->number = src.number;
        this->sub = src.sub;
        this->str = src.str;
        return *this;
    }
};


template<class TDst, class TSrc>
static typename boost::enable_if<boost::is_pointer<TDst>, void>::type forceAssign(TDst dst, const TSrc& src) {
    **((typename citrix::type_traits::normalize_type<TDst>::type*)&dst) = *src;
}

template<class TDst, class TSrc>
static typename boost::disable_if<boost::is_pointer<TDst>, void>::type forceAssign(const TDst& dst, const TSrc& src) {
    *((typename citrix::type_traits::normalize_type<TDst>::type*)&dst) = (typename citrix::type_traits::normalize_type<TDst>::type)src;
}

static inline void forceAssign(const MyOwnStruct& dst, const MyOwnStruct& src) {
    MyOwnStruct::internal_copy_accessor((MyOwnStruct*)&dst, &src);
}

static inline void forceAssign(const MyOwnStruct* dst, MyOwnStruct* src) {
    MyOwnStruct::internal_copy_accessor((MyOwnStruct*)dst, src);
}

static inline void forceAssign(MyOwnStruct* dst, MyOwnStruct* src) {
    MyOwnStruct::internal_copy_accessor((MyOwnStruct*)dst, src);
}

#if LEGACYMOCK_SUPPORTS_TRACING

namespace citrix { namespace tracing {
    MAKE_TRANSLATOR_FOR(MyOwnStruct::MySubStruct) {
        
        MyOwnStruct::MySubStruct fromJson(const ptree& json) {
            return MyOwnStruct::MySubStruct();
        }
        
        ptree toJson(const MyOwnStruct::MySubStruct& value) {
            ptree json;
            json.put("number", value.number);
            json.put("str", value.str);
            
            ptree result;
            result.put_child("value", json);
            return result;
        }
    };
    
    MAKE_TRANSLATOR_FOR(MyOwnStruct) {
        
        MyOwnStruct fromJson(const ptree& json) {
        }
        
        ptree toJson(const MyOwnStruct& value) {
            ptree json;
            json.put("number", value.number);
            json.put("str", value.str);
            json.put_child("sub", translate(value.sub));
            
            ptree result;
            result.put_child("value", json);
            return result;
        }
    };
}}

#endif

#endif
