#pragma once
#ifndef GUARD_HOLDER_POINTER_H
#define GUARD_HOLDER_POINTER_H

#include <type_traits>
#include <utility>

template < typename _Refer >
struct HolderGuard
{
    using Refer = _Refer;
    using Holder = ::std::decay_t< Refer >;

    static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );

    Refer m_holder;

    HolderGuard ( Refer holder )
        : m_holder( ::std::forward< Refer >( holder ) )
    {
        Holder::guard( ::std::forward< Refer >( m_holder ) );
    }

    HolderGuard ( HolderGuard & holder ) = delete;
    HolderGuard ( const HolderGuard & holder ) = delete;
    HolderGuard ( HolderGuard && holder ) = delete;
    HolderGuard ( const HolderGuard && holder ) = delete;

    ~HolderGuard ()
    {
        Holder::unguard( ::std::forward< Refer >( m_holder ) );
    }
};

#endif
