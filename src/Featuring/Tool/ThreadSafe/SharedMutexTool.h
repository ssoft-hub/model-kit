#pragma once
#ifndef SCL_INSTANCE_TOOL_THREAD_SAFE_SHARED_MUTEX_H
#define SCL_INSTANCE_TOOL_THREAD_SAFE_SHARED_MUTEX_H

#include <ModelKit/Featuring/Tool/ThreadSafe/LockTool.h>

#if __cplusplus > 201700L
#   include <shared_mutex>
#else
#   include "private/SharedMutex.h"
#endif

namespace ThreadSafe
{
    using SharedMutex =
#if __cplusplus > 201700L
        ::std::shared_mutex
#else
    ::_Scl::SharedMutex
#endif
    ;
}

namespace ThreadSafe
{
    template < typename _Holder >
    struct Locking< ::ThreadSafe::SharedMutex, _Holder >
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

    template < typename _Holder >
    struct Locking< ::ThreadSafe::SharedMutex, const _Holder >
    {
        template < typename _LockRefer >
        static constexpr void lock ( _LockRefer && lock )
        {
            using LockRefer = _LockRefer &&;
            ::std::forward< LockRefer >( lock ).lock_shared();
        }

        template < typename _LockRefer >
        static constexpr void unlock ( _LockRefer && lock )
        {
            using LockRefer = _LockRefer &&;
            ::std::forward< LockRefer >( lock ).unlock_shared();
        }
    };
}

namespace ThreadSafe
{
    using SharedMutexTool = LockTool< SharedMutex >;
}

#endif
