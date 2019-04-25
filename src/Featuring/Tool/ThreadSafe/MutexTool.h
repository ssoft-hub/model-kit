#pragma once
#ifndef SCL_INSTANCE_TOOL_THREAD_SAFE_MUTEX_H
#define SCL_INSTANCE_TOOL_THREAD_SAFE_MUTEX_H

#include <ModelKit/Featuring/Tool/ThreadSafe/LockTool.h>
#include <mutex>

namespace ThreadSafe
{
    template < typename _Holder >
    struct Locking< ::std::mutex, _Holder >
    {
        template < typename _LockRefer >
        static constexpr void lock ( _LockRefer && lock )
        {
            using LockRefer = _LockRefer &&;
            ::std::forward< LockRefer >( lock ).lock();
        }

        template < typename _LockRefer >
        static constexpr void unlock ( _LockRefer && lock )
        {
            using LockRefer = _LockRefer &&;
            ::std::forward< LockRefer >( lock ).unlock();
        }
    };
}

namespace ThreadSafe
{
    using MutexTool = LockTool< ::std::mutex >;
}

#endif
