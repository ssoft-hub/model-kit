#pragma once
#ifndef FEATURING_TOOL_GUARD_OPERATOR_TOOL_H
#define FEATURING_TOOL_GUARD_OPERATOR_TOOL_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

namespace Guard
{
    template < typename _Invokable, typename _InstanceRefer, typename ... _Arguments >
    struct LeftTool
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
            using ResultRefer = _Type;

            static_assert( ::std::is_reference< _Type >::value,
                "The template parameter _Type must to be a reference type." );
            static_assert( ::std::is_same< ResultRefer, ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) > >::value,
                "The result of _Invokable( _LeftRefer, _InstanceRefer ) must to be a _Type" );

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
            static constexpr ::SimilarRefer< ResultRefer, _HolderRefer && > value ( _HolderRefer && holder )
            {
                return ::std::forward< ::SimilarRefer< ResultRefer, _HolderRefer && > >( holder.m_result_refer );
            }
        };
    };
}

namespace Guard
{
    template < typename _Invokable, typename _LeftRefer, typename _InstanceRefer >
    struct RightTool
    {
        using LeftRefer = _LeftRefer;
        using InstanceRefer = _InstanceRefer;
        using Invokable = _Invokable;

        static_assert( ::std::is_reference< _LeftRefer >::value,
            "The template parameter _LeftRefer must to be a reference type." );
        static_assert( ::std::is_reference< _InstanceRefer >::value,
            "The template parameter _InstanceRefer must to be a reference type." );

        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using ResultRefer = _Type;

            static_assert( ::std::is_reference< _Type >::value,
                "The template parameter _Type must to be a reference type." );
            static_assert( ::std::is_same< ResultRefer, ::std::result_of_t< Invokable( LeftRefer, ValueRefer ) > >::value,
                "The result of _Invokable( _LeftRefer, _InstanceRefer ) must to be a _Type" );

            InstanceGuard m_feature_guard;
            ResultRefer m_result_refer;

            Holder (  Invokable && invokable, LeftRefer left, InstanceRefer instance )
                : m_feature_guard( ::std::forward< InstanceRefer >( instance ) )
                , m_result_refer( ::std::forward< ResultRefer >( invokable(  ::std::forward< LeftRefer >( left ), ::std::forward< ValueRefer >( m_feature_guard.instanceAccess() ) ) ) )
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
            static constexpr ::SimilarRefer< ResultRefer, _HolderRefer && > value ( _HolderRefer && holder )
            {
                return ::std::forward< ::SimilarRefer< ResultRefer, _HolderRefer && > >( holder.m_result_refer );
            }
        };
    };
}

namespace Guard
{
    template < typename _Invokable, typename _LeftInstanceRefer, typename _RightInstanceRefer >
    struct BothTool
    {
        using Invokable = _Invokable;
        using LeftInstanceRefer = _LeftInstanceRefer;
        using RightInstanceRefer = _RightInstanceRefer;
        static_assert( ::std::is_reference< LeftInstanceRefer >::value,
            "The template parameter _LeftInstanceRefer must to be a reference type." );
        static_assert( ::std::is_reference< RightInstanceRefer >::value,
            "The template parameter _RightInstanceRefer must to be a reference type." );

        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
            using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
            using ResultRefer = _Type;

            static_assert( ::std::is_reference< _Type >::value,
                "The template parameter _Type must to be a reference type." );
            static_assert( ::std::is_same< ResultRefer, ::std::result_of_t< Invokable( LeftValueRefer, RightValueRefer ) > >::value,
                "The result of _Invokable( _LeftRefer, _InstanceRefer ) must to be a _Type" );
            static_assert( ::std::is_reference< ResultRefer >::value,
                "The result of _Invokable( LeftValueRefer, RightValueRefer ) must to be a reference type." );

            LeftInstanceGuard m_left_feature_guard;
            RightInstanceGuard m_right_feature_guard;
            ResultRefer m_result_refer;

            Holder (  Invokable && invokable, LeftInstanceRefer left, RightInstanceRefer right )
                : m_left_feature_guard( ::std::forward< LeftInstanceRefer >( left ) )
                , m_right_feature_guard( ::std::forward< RightInstanceRefer >( right ) )
                , m_result_refer( invokable( ::std::forward< LeftValueRefer >( m_left_feature_guard.instanceAccess() ), ::std::forward< RightValueRefer >( m_right_feature_guard.instanceAccess() ) ) )
            {
            }

            Holder ( ThisType && other )
                : m_left_feature_guard( ::std::forward< InstanceGuard >( other.m_left_feature_guard ) )
                , m_right_feature_guard( ::std::forward< InstanceGuard >( other.m_right_feature_guard ) )
                , m_result_refer( ::std::forward< ResultRefer >( other.m_result_refer ) )
            {
                assert( false ); // Restricted functionality
            }

            Holder ( const ThisType & other ) = delete;

            template < typename _HolderRefer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _HolderRefer > >::value > >
            static constexpr ::SimilarRefer< ResultRefer, _HolderRefer && > value ( _HolderRefer && holder )
            {
                return ::std::forward< ::SimilarRefer< ResultRefer, _HolderRefer && > >( holder.m_result_refer );
            }
        };
    };
}

#endif
