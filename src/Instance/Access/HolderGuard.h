#pragma once
#ifndef GUARD_HOLDER_POINTER_H
#define GUARD_HOLDER_POINTER_H

#include <utility>

template < typename _Refer, typename _Tool >
struct HolderGuard
{
    using Refer = _Refer;
    using Tool = _Tool;

    static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );

    Refer m_holder;

    HolderGuard ( Refer holder )
        : m_holder( ::std::forward< Refer >( holder ) )
    {
        Tool::guardHolder( ::std::forward< Refer >( m_holder ) );
    }

    HolderGuard ( HolderGuard & holder ) = delete;
    HolderGuard ( const HolderGuard & holder ) = delete;
    HolderGuard ( HolderGuard && holder ) = delete;
    HolderGuard ( const HolderGuard && holder ) = delete;

    ~HolderGuard ()
    {
        Tool::unguardHolder( ::std::forward< Refer >( m_holder ) );
    }
};

#endif
