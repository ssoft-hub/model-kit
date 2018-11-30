#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_RECURSIVE_MUTEX_H
#define INSTANCE_TOOL_THREAD_SAFE_RECURSIVE_MUTEX_H

#include <ModelKit/Instance/Tool/ThreadSafe/LockTool.h>
#include <mutex>

namespace ThreadSafe
{
    using RecursiveMutexTool = LockTool< ::std::recursive_mutex >;
}

#endif
