#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_MUTEX_H
#define INSTANCE_TOOL_THREAD_SAFE_MUTEX_H

#include <ModelKit/Instance/Tool/ThreadSafe/LockTool.h>
#include <mutex>

namespace ThreadSafe
{
    using MutexTool = LockTool< ::std::mutex >;
}

#endif
