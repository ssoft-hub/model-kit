#pragma once

#include <utility>

template < typename _ReferType, typename _ValueTool >
struct HolderPointer
{
    using ReferType = _ReferType;
    using ValueTool = _ValueTool;

    ReferType m_holder;

    HolderPointer ( ReferType holder )
        : m_holder( ::std::forward< ReferType >( holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardHolder( ::std::forward< ReferType >( m_holder ) );
    }

    ~HolderPointer ()
    {
        ValueTool::unguardHolder( ::std::forward< ReferType >( m_holder ) );
    }
};
