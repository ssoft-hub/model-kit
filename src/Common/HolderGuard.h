#pragma once

#include <utility>

template < typename _ReferType, typename _ValueTool >
struct HolderGuard
{
    using ReferType = _ReferType;
    using ValueTool = _ValueTool;

    ReferType m_holder;

    HolderGuard ( ReferType holder )
    : m_holder( ::std::forward< ReferType >( holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardHolder( ::std::forward< ReferType >( m_holder ) );
    }

    ~HolderGuard ()
    {
        ValueTool::unguardHolder( ::std::forward< ReferType >( m_holder ) );
    }
};
