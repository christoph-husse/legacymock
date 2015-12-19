#ifndef _CITRIX_MOCKING_UNITTESTS_STDAFX_H_
#define _CITRIX_MOCKING_UNITTESTS_STDAFX_H_

#include "citrix/srclink/LegacyMock-stdafx.h"
#include "citrix/Platform.h"

#include "../citrix.hooking/include/citrix/hooking/Hooking.h"
#include "../citrix.mocking/include/citrix/mocking/MockManager.h"
#include "../citrix.mocking/include/citrix/mocking/VirtualCallDetector.h"

#if LEGACYMOCK_SUPPORTS_TRACING
    #include "../citrix.tracing/include/citrix/tracing/CallTrace.h"
    #include "../citrix.tracing/include/citrix/tracing/DefaultTranslators.h"
#endif

#include "../citrix.mocking.generator/src/stdafx.h"

#include <gtest/gtest.h>

#endif