/*
Copyright Redshift Software, Inc. 2008-2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_OS_LINUX_H
#define BOOST_PREDEF_OS_LINUX_H

#include <citrix/predef/version_number.h>
#include <citrix/predef/make.h>

/*`
[heading `BOOST_OS_LINUX`]

[@http://en.wikipedia.org/wiki/Linux Linux] operating system.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`linux`] [__predef_detection__]]
    [[`__linux`] [__predef_detection__]]
    ]
 */

#define BOOST_OS_LINUX BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if !BOOST_PREDEF_DETAIL_OS_DETECTED && ( \
    defined(linux) || defined(__linux) \
    )
#   undef BOOST_OS_LINUX
#   define BOOST_OS_LINUX BOOST_VERSION_NUMBER_AVAILABLE
#endif

#if BOOST_OS_LINUX
#   define BOOST_OS_LINUX_AVAILABLE
#   include <citrix/predef/detail/os_detected.h>
#endif

#define BOOST_OS_LINUX_NAME "Linux"

#include <citrix/predef/detail/test.h>
BOOST_PREDEF_DECLARE_TEST(BOOST_OS_LINUX,BOOST_OS_LINUX_NAME)


#endif
