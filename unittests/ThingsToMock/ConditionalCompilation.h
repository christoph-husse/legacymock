//  (C) Copyright Herve Bronnimann 2004.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   1 July 2004
      Split the code into two headers to lessen dependence on
      Boost.tuple. (Herve)
   26 June 2004
      Added the code for the boost minmax library. (Herve)
*/


#ifndef _UNITTEST_MOCK_CONDITIONALCOMPILATION_H_
#define _UNITTEST_MOCK_CONDITIONALCOMPILATION_H_

#include "citrix/Config.h"

#ifndef SOME_CLASS_CONDITION /* test comment 
removal */#   define SOME_CLASS_CONDITION22
#   define SOME_CLASS_CONDITION
#endif

#ifndef ENABLE_METHOD_1
#   define ENABLE_METHOD_1 /* test comment removal */
#endif

#ifndef SOME_VERSION
#   define SOME_VERSION 101 // test comment removal
#endif

template<class> class CitrixMockingAccessor;

namespace MockMe {
// test comment removal
# ifndef    SOME_CLASS_CONDITION

    class ConditionalCompilation2 {

    };

    class MyOtherChoice {
        void method0() {
        }
    };

#else

    // Define real class after empty stub, because script will pick first occurrrence if
    // precondition analysis fails
    class ConditionalCompilation {
        template<class> friend class ::CitrixMockingAccessor;

        void method0() {
        }

    #if defined(ENABLE_METHOD_1) & (SOME_VERSION > 100)
        int myVariable1;

        void method1() {
        }
    #endif
        void method2() { }
    #if defined(ENABLE_METHOD_3_4) & (SOME_VERSION > 105)
        void method3() {
        }

        void method4() {
        }
        int myVariable2;
    #endif

        void method5() {
        }
        void method6() {
        }

        int myVariable3;
    };

#endif
}

// ATTENTION: There shall be no new-line after "#endif" (regression test)
#endif