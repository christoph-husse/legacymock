/*
Copyright Redshift Software Inc 2011-2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_ARCHITECTURE_PYRAMID_H
#define BOOST_PREDEF_ARCHITECTURE_PYRAMID_H

#include <citrix/predef/version_number.h>
#include <citrix/predef/make.h>

/*`
[heading `BOOST_ARCH_PYRAMID`]

Pyramid 9810 architecture.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`pyr`] [__predef_detection__]]
    ]
 */

#define BOOST_ARCH_PYRAMID BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(pyr)
#   undef BOOST_ARCH_PYRAMID
#   define BOOST_ARCH_PYRAMID BOOST_VERSION_NUMBER_AVAILABLE
#endif

#if BOOST_ARCH_PYRAMID
#   define BOOST_ARCH_PYRAMID_AVAILABLE
#endif

#define BOOST_ARCH_PYRAMID_NAME "Pyramid 9810"

#include <citrix/predef/detail/test.h>
BOOST_PREDEF_DECLARE_TEST(BOOST_ARCH_PYRAMID,BOOST_ARCH_PYRAMID_NAME)


#endif
