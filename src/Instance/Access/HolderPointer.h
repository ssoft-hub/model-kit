#pragma once
#ifndef GUARD_HOLDER_POINTER_H
#define GUARD_HOLDER_POINTER_H

#include <utility>

template < typename _Refer, typename _Tool >
struct HolderPointer
{
    using Refer = _Refer;
    using Tool = _Tool;

    Refer m_holder;

    HolderPointer ( Refer holder )
        : m_holder( ::std::forward< Refer >( holder ) )
    {
        static_assert( ::std::is_reference< Refer >::value
            , "The template parameter must be a reference." );
        Tool::guardHolder( ::std::forward< Refer >( m_holder ) );
    }

    HolderPointer ( HolderPointer & holder ) = delete;
    HolderPointer ( const HolderPointer & holder ) = delete;
    HolderPointer ( HolderPointer && holder ) = delete;
    HolderPointer ( const HolderPointer && holder ) = delete;

    ~HolderPointer ()
    {
        Tool::unguardHolder( ::std::forward< Refer >( m_holder ) );
    }
};

#endif
