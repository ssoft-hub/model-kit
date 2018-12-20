#pragma once
#ifndef GUARD_UNARY_TOOL_H
#define GUARD_UNARY_TOOL_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

namespace Guard
{
    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    struct UnaryTool
    {
        static_assert( ::std::is_reference< _InstanceRefer >::value,
            "The template parameter _InstanceRefer must to be a reference type." );

        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using InstanceGuard = ::InstanceGuard< _InstanceRefer >;
            using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
            using ResultRefer = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
            using Result = ::std::remove_reference_t< ResultRefer >;

            static_assert( ::std::is_reference< ResultRefer >::value,
                "The result of _Invokable( ValueRefer, _Arguments && ... ) must to be a reference type." );

            InstanceGuard m_feature_guard;
            ResultRefer m_result_refer;

            Holder ( _InstanceRefer instance, _Invokable && invokable, _Arguments && ... arguments )
                : m_feature_guard( ::std::forward< _InstanceRefer >( instance ) )
                , m_result_refer( ::std::forward< ResultRefer >( invokable( ::std::forward< ValueRefer >( m_feature_guard.value() ), ::std::forward< _Arguments >( arguments ) ...  ) ) )
            {
            }

            Holder ( ThisType && other )
                : m_feature_guard( ::std::forward< InstanceGuard >( other.m_feature_guard ) )
                , m_result_refer( ::std::forward< ResultRefer >( other.m_result_refer ) )
            {
            }

            Holder ( const ThisType & other ) = delete;

            template < typename _HolderRefer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _HolderRefer > >::value > >
            static constexpr ::SimilarRefer< Result, _HolderRefer && > value ( _HolderRefer && holder )
            {
                return ::std::forward< ::SimilarRefer< Result, _HolderRefer && > >( holder.m_result_refer );
            }
        };
    };
}

#endif
