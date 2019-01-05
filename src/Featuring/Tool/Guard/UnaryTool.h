#pragma once
#ifndef GUARD_UNARY_TOOL_H
#define GUARD_UNARY_TOOL_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

namespace Guard
{
    template < typename _Invokable, typename _InstanceRefer, typename ... _Arguments >
    struct UnaryTool
    {
        using InstanceRefer = _InstanceRefer;
        using Invokable = _Invokable;
        static_assert( ::std::is_reference< InstanceRefer >::value,
            "The template parameter _InstanceRefer must to be a reference type." );

        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using ResultRefer = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >;
            using Result = ::std::remove_reference_t< ResultRefer >;

            static_assert( ::std::is_reference< ResultRefer >::value,
                "The result of _Invokable( ValueRefer, _Arguments && ... ) must to be a reference type." );

            InstanceGuard m_feature_guard;
            ResultRefer m_result_refer;

            Holder (  Invokable && invokable, InstanceRefer instance, _Arguments && ... arguments )
                : m_feature_guard( ::std::forward< InstanceRefer >( instance ) )
                , m_result_refer( ::std::forward< ResultRefer >( invokable( ::std::forward< ValueRefer >( m_feature_guard.instanceAccess() ), ::std::forward< _Arguments >( arguments ) ...  ) ) )
            {
            }

            Holder ( ThisType && other )
                : m_feature_guard( ::std::forward< InstanceGuard >( other.m_feature_guard ) )
                , m_result_refer( ::std::forward< ResultRefer >( other.m_result_refer ) )
            {
                assert( false ); // Restricted functionality
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
