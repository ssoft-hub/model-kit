#pragma once
#ifndef GUARD_HOLDER_POINTER_H
#define GUARD_HOLDER_POINTER_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <type_traits>
#include <utility>

template < typename _HolderRefer >
class HolderGuard
{
    using ThisType = HolderGuard< _HolderRefer >;

public:
    using HolderRefer = _HolderRefer;
    using Holder = ::std::decay_t< HolderRefer >;

    static_assert( ::std::is_reference< HolderRefer >::value, "The template parameter _HolderRefer must to be of reference type." );

private:
    HolderRefer m_holder;

private:
    HolderGuard ( ThisType && ) = delete;
    HolderGuard ( const ThisType && ) = delete;
    HolderGuard ( volatile ThisType && ) = delete;
    HolderGuard ( const volatile ThisType && ) = delete;
    HolderGuard ( ThisType & ) = delete;
    HolderGuard ( const ThisType & ) = delete;
    HolderGuard ( volatile ThisType & ) = delete;
    HolderGuard ( const volatile ThisType & ) = delete;

public:
    HolderGuard ( HolderRefer holder )
        : m_holder( ::std::forward< HolderRefer >( holder ) )
    {
        ::HolderInternal::guard< HolderRefer >( ::std::forward< HolderRefer >( m_holder ) );
    }

    ~HolderGuard ()
    {
        ::HolderInternal::unguard< HolderRefer >( ::std::forward< HolderRefer >( m_holder ) );
    }
};

#endif
