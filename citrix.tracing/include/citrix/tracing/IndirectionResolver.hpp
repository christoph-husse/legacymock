
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCK_TRACING_INDIRECTIONRESOLVER_HPP_
#define _CITRIX_MOCK_TRACING_INDIRECTIONRESOLVER_HPP_

#include "citrix/Platform.h"

#if !LEGACYMOCK_SUPPORTS_TRACING
#   error "Tracing is only supported since boost version 1.41!"
#endif


#include "Translator.h"
#include "citrix/type_traits/normalize_type.hpp"

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace citrix { namespace tracing {

    namespace internal {
        /*
            Takes any type and checks if there is a Translator<>
            for the normalized form (no specifiers, no reference).
            If there is none, a compile-time error is raised, unless
            it is a pointer. If so, then the pointer is dereferenced
            at compile time and at runtime. Deferencing at compile-time
            causes a recursion, which looks for Translator<> for the next
            level of indirection. Runtime deferencing is done safely and
            returns an empty ptree if the indirections can not be fully
            traversed due to invalid memory addresses.

            This class is the cornerstone for CallSerializer.
        */
        template<class T>
        struct IndirectionResolver {
            typedef typename citrix::type_traits::normalize_type<T>::type TNormalized;
            typedef boost::property_tree::ptree ptree;

            static ptree toPropertyTree(const T& value, int indirection = 0) {
                return toPropertyTreeFilter<TranslatorSFINAE<T>, T>(value, indirection, 0);
            } 

            template<class Y, class Unused>
            static ptree toPropertyTreeFilter(const T& value, int indirection = 0, typename Y::SFINAE sfinae = Y::SFINAE()) {
                CITRIX_MOCK_UNREFPARAM(sfinae);

                ptree result = Translator<TNormalized>().toJson((const TNormalized&)value);

                if(indirection > 0)
                    result.put("indirection", indirection);

                return result;
            } 

            template<class Y, class Unused>
            static ptree toPropertyTreeFilter(const T& value, int indirection = 0, char unused = 0) {
                CITRIX_MOCK_UNREFPARAM(unused);

                return toPropertyTreeInternal<TranslatorSFINAE<TNormalized>, TNormalized>(value, indirection, 0);
            } 

        private:
            template<class TPtr>
            static bool isInvalidPtr(TPtr* ptr) {
            #ifdef _MSC_VER
                __try {
                    boost::uint64_t test = *((boost::uint64_t*)ptr);
                    CITRIX_MOCK_UNREFPARAM(test);
                    return false;
                } __except(EXCEPTION_EXECUTE_HANDLER) {
                    return true;
                }
            #else
                return ptr == NULL;
            #endif
            }

            static ptree ptreeByPointer(const void* ptr, int indirection) {
                ptree result;
                result.put("pointer", ptr);

                if(indirection > 0)
                    result.put("indirection", indirection);

                return result;
            }

            template<class Y, class Unused>
            static ptree toPropertyTreeInternal(const T& value, int indirection = 0, typename Y::SFINAE sfinae = Y::SFINAE()) {
                CITRIX_MOCK_UNREFPARAM(sfinae);

                // There is a Translator<TNormalized>, and its called Y
                ptree result = Translator<TNormalized>().toJson((const TNormalized&)value);

                if(indirection > 0)
                    result.put("indirection", indirection);

                return result;
            } 

            template<class Unused, class P>
            static typename boost::enable_if<
                boost::integral_constant<bool, 
                        boost::is_pointer<P>::value && !(
                        boost::is_member_function_pointer<P>::value || 
                        boost::is_function<P>::value || 
                        boost::is_function<typename boost::remove_pointer<P>::type>::value
                    )
                >, ptree>::type
            toPropertyTreeInternal(const T& value, int indirection = 0, char sfinae = 0) {
                CITRIX_MOCK_UNREFPARAM(sfinae);

                // There is no Translator<TNormalized>, but T is a pointer
                if(!isInvalidPtr(value))
                    return IndirectionResolver<typename boost::remove_pointer<TNormalized>::type>::toPropertyTree(*value, indirection + 1);
                else
                    return ptreeByPointer(value, indirection);
            }

            template<class Unused, class P>
            static typename boost::enable_if<
                boost::integral_constant<bool, 
                    boost::is_member_function_pointer<P>::value || 
                    boost::is_function<P>::value || 
                    boost::is_function<typename boost::remove_pointer<P>::type>::value
                >, ptree>::type
            toPropertyTreeInternal(const T& value, int indirection = 0, char sfinae = 0) {
                CITRIX_MOCK_UNREFPARAM(sfinae);

                // There is no Translator<TNormalized>, but T is a function
                return ptreeByPointer(value, indirection);
            }

            template<class Unused, class P>
            static typename boost::disable_if<
                boost::integral_constant<bool, 
                        boost::is_pointer<P>::value ||
                        boost::is_member_function_pointer<P>::value || 
                        boost::is_function<P>::value || 
                        boost::is_function<typename boost::remove_pointer<P>::type>::value
                >, ptree>::type
            toPropertyTreeInternal(const T& value, int indirection = 0, char sfinae = 0) {
               CITRIX_MOCK_UNREFPARAM(value);
               CITRIX_MOCK_UNREFPARAM(sfinae);

               // There is no Translator<TNormalized>, and TNormalized is no pointer, which means we have hit a dead-end
               //BOOST_STATIC_ASSERT_MSG(boost::is_pointer<P>::value, "No translator could be found for this type :(!");

               return ptreeByPointer(&value, indirection);
           } 
        };

        template<>
        struct IndirectionResolver<void> {
        };
    }
}}

#endif
