#pragma once
#ifndef GUARD_HOLDER_POINTER_H
#define GUARD_HOLDER_POINTER_H

#include <ModelKit/Featuring/Access/Accessing.h>
#include <type_traits>
#include <utility>

template < typename _HolderRefer >
struct HolderGuard
{
    using HolderRefer = _HolderRefer;
    using Holder = ::std::decay_t< HolderRefer >;

    static_assert( ::std::is_reference< HolderRefer >::value, "The template parameter _HolderRefer must to be of reference type." );

    HolderRefer m_holder;

    HolderGuard ( HolderRefer holder )
        : m_holder( ::std::forward< HolderRefer >( holder ) )
    {
        ::Access::guard< HolderRefer >( ::std::forward< HolderRefer >( m_holder ) );
    }

    ~HolderGuard ()
    {
        ::Access::unguard< HolderRefer >( ::std::forward< HolderRefer >( m_holder ) );
    }

    HolderGuard ( HolderGuard && ) = delete;
    HolderGuard ( const HolderGuard && ) = delete;
    HolderGuard ( volatile HolderGuard && ) = delete;
    HolderGuard ( const volatile HolderGuard && ) = delete;
    HolderGuard ( HolderGuard & ) = delete;
    HolderGuard ( const HolderGuard & ) = delete;
    HolderGuard ( volatile HolderGuard & ) = delete;
    HolderGuard ( const volatile HolderGuard & ) = delete;
};

#endif
