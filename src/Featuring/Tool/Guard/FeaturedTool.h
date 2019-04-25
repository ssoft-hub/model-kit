#pragma once
#ifndef SCL_GUARD_FEATURED_TOOL_H
#define SCL_GUARD_FEATURED_TOOL_H

#include <ModelKit/Featuring/Access/ValueGuard.h>

namespace Guard
{
    template < typename _Instance >
    struct FeaturedTool
    {
        using InstanceRefer = _Instance;

        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;
            using ValueGuard = ::Scl::ValueGuard< InstanceRefer >;

            ValueGuard m_value_guard;

            Holder ( InstanceRefer refer )
                : m_value_guard( ::std::forward< InstanceRefer >( refer ) )
            {
            }

            //! Access to internal value of Holder for any king of referencies.
            template < typename _HolderRefer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _HolderRefer > >::value > >
            static constexpr decltype(auto) value ( _HolderRefer && holder )
            {
                using HolderRefer = _HolderRefer &&;
                using ValueRefer = ::Scl::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( holder.m_value_guard.valueAccess() );
            }
        };
    };
}

#endif
