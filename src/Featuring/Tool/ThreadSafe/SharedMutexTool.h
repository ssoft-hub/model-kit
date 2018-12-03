#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_SHARED_MUTEX_H
#define INSTANCE_TOOL_THREAD_SAFE_SHARED_MUTEX_H

#include <ModelKit/Featuring/Tool/ThreadSafe/SharedLockTool.h>

#if __cplusplus > 201700L
#   include <shared_mutex>
#else
#   include "private/SharedMutex.h"
#endif

namespace ThreadSafe
{
    using SharedMutexTool = SharedLockTool<
#if __cplusplus > 201700L
        ::std::shared_mutex
#else
        _Mdk::SharedMutex
#endif
    >;
}

#endif
