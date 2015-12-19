#ifndef __GTEST_UTILITIES_H__
#define __GTEST_UTILITIES_H__

#include "ECThread.h"

#if defined(_WIN32)
#  include "ECProcessManager.h"
#endif
#if defined(_WIN32) || defined(DARWIN)
#  include "ECGlobalMemoryLeaks.h"
#endif
#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>




class ECActiveThreadsDetector {
    template<class> friend class ::CitrixMockingAccessor;

public:
    ECActiveThreadsDetector() {
#ifdef _WIN32
        EXPECT_NO_ERROR(ECProcessManager::get()->getCurrentProcessActiveThreadsCount(_number));
#endif
    };

    ~ECActiveThreadsDetector() {
#ifdef _WIN32
        size_t number;
        EXPECT_NO_ERROR(ECProcessManager::get()->getCurrentProcessActiveThreadsCount(number));

        EXPECT_EQ(_number, number);
#endif
    };
private:
#ifdef _WIN32
    size_t _number;
#endif
};




#if defined(ECMEMORYLEAKSDETECTION)
#  undef SCOPED_TRACE
#  define SCOPED_TRACE(arg)
#endif

class ECMemoryLeaksDetector {
    template<class> friend class ::CitrixMockingAccessor;
    
public:
    ECMemoryLeaksDetector() : _leaks(0) {
#if defined(ECMEMORYLEAKSDETECTION)
        ECGlobalMemoryLeaks::get()->reset();
#endif
    }

    ~ECMemoryLeaksDetector() {
#if defined(ECMEMORYLEAKSDETECTION)
		if (!::testing::UnitTest::GetInstance()->current_test_case()->Failed()) {
            TThreadId threadId;
            ECThread::getThreadID(threadId);
            EXPECT_EQ(_leaks, ECGlobalMemoryLeaks::get()->leaksFromThread(threadId));
		}
#endif		
    }

    
    void knownLeaks(size_t leaks) {
        _leaks += leaks;
    }

private:
    size_t _leaks;
};

#endif 

