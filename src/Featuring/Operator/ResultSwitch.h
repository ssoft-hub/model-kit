#pragma once
#ifndef MDK_FEATURING_OPERATOR_RESULT_SWITCH_H
#define MDK_FEATURING_OPERATOR_RESULT_SWITCH_H

#include <ModelKit/Featuring/Tool/Guard/OperatorTool.h>
#include <ModelKit/Featuring/Tool/Inplace/DefaultTool.h>
#include <ModelKit/Featuring/Traits.h>
#include <type_traits>
#include <utility>

namespace Operator
{
    struct NoneInstanceCase {};
    struct LeftInstanceCase {};
    struct RightInstanceCase {};
    struct BothInstanceCase {};
    struct BothExposingCase;
    struct RightExposingCase;
    struct LeftExposingCase;

    using UnaryInstanceCase = LeftInstanceCase;

    template < typename _Left, typename _Right >
    struct InstanceCaseHelper
    {
        using Left = ::std::decay_t< _Left >;
        using Right = ::std::decay_t< _Right >;
        using Type = ::std::conditional_t< ::is_instance< Left > && ::is_instance< Right >,
            BothInstanceCase,
            ::std::conditional_t< ::is_instance< Left >,
                LeftInstanceCase,
                ::std::conditional_t< ::is_instance< Right >,
                    RightInstanceCase,
                    NoneInstanceCase > > >;
    };

    template < typename _Left, typename _Right = void >
    using InstanceSwitchCase = typename InstanceCaseHelper< _Left, _Right >::Type;

    template < typename _Left, typename _Right >
    struct ExposingCaseHelper
    {
        using Left = ::std::decay_t< _Left >;
        using Right = ::std::decay_t< _Right >;
        using Type = ::std::conditional_t< ::is_this_part_of_other< Left, Right >,
                RightExposingCase,
                ::std::conditional_t< ::is_this_part_of_other< Right, Left >,
                    LeftExposingCase,
                    BothExposingCase > >;
    };

    template < typename _Left, typename _Right >
    using ExposingSwitchCase = typename ExposingCaseHelper< _Left, _Right >::Type;
}

namespace Operator
{
    /* Cases for operator result type */
    struct FundamentalCase {};
    struct ThisCase {};
    struct DefaultCase {};
    struct BlockedCase {};

    template < bool, typename _Returned, typename _Refer >
    struct ResultCaseInstanceHelper;

    template < typename _Returned, typename _Refer >
    struct ResultCaseInstanceHelper< true, _Returned, _Refer >
    {
        using Returned = _Returned;
        using Instance = ::std::decay_t< Returned >;
        static_assert( ::is_instance< Instance >, "Bla" );
        using Value = typename Instance::Value;

        using Type = ::std::conditional_t< ::std::is_reference< Value >::value,
            ::Operator::BlockedCase,
            ::Operator::DefaultCase >;
    };

    template < typename _Returned, typename _Refer >
    struct ResultCaseInstanceHelper< false, _Returned, _Refer >
    {
        using Returned = _Returned;
        using Value = ::std::decay_t< _Refer >;

        static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
        static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
        static constexpr bool returned_is_same_value = ::std::is_same< _Returned, _Refer >::value;

        using Type = ::std::conditional_t< returned_is_not_wrappable,
            ::Operator::FundamentalCase,
            ::std::conditional_t< returned_is_same_value,
                ::Operator::ThisCase,
                ::std::conditional_t< returned_is_reference,
                    ::Operator::BlockedCase,
                    ::Operator::DefaultCase > > >;
    };

    template < typename _Returned, typename _Refer >
    struct ResultCaseHelper
    {
        using Type = typename ResultCaseInstanceHelper< ::is_instance< _Returned >, _Returned, _Refer >::Type;
    };

    template < typename _Returned, typename _Value >
    using ResultSwitchCase = typename ResultCaseHelper< _Returned, _Value >::Type;
}

namespace Operator
{
    template < typename, typename >
    struct ResultSwitch;

    template <>
    struct ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::FundamentalCase >
    {
        template < typename _Invokable, typename _Instance, typename ... _Arguments >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
        {
            using InstanceRefer = _Instance &&;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            return invokable( InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ThisCase >
    {
        template < typename _Invokable, typename _Instance, typename ... _Arguments >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
        {
            using InstanceRefer = _Instance &&;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            invokable( InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
            return ::std::forward< InstanceRefer >( instance );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::DefaultCase >
    {
        template < typename _Invokable, typename _Instance, typename ... _Arguments >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
        {
            using InstanceRefer = _Instance &&;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using Invokable = _Invokable;
            using Returned = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >;

            static_assert( !::std::is_reference< Returned >::value,
                "The type of return parameter must to be not a reference type." );

            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            using ResultInstance = ::Instance< Returned, ::Inplace::DefaultTool >;
            return ResultInstance( invokable( InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::BlockedCase >
    {
        template < typename _Invokable, typename _Instance, typename ... _Arguments >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
        {
            using InstanceRefer = _Instance &&;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using Invokable = _Invokable;
            using InvokableRefer = _Invokable &&;
            using Returned = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >;

            static_assert( ::std::is_reference< Returned >::value,
                "The type of return parameter must to be a reference type." );

            using GuardTool = ::Guard::LeftTool< Invokable, InstanceRefer, _Arguments && ... >;
            using ResultInstance = ::Instance< Returned, GuardTool >;
            return ResultInstance( ::std::forward< InvokableRefer >( invokable ), ::std::forward< InstanceRefer >( instance ), ::std::forward< _Arguments && >( arguments ) ... );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightInstanceCase, ::Operator::FundamentalCase >
    {
        template < typename _Invokable, typename _Left, typename _Instance >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Instance && instance )
        {
            using LeftRefer = _Left &&;
            using InstanceRefer = _Instance &&;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            return invokable( ::std::forward< LeftRefer >( left ), InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess() );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightInstanceCase, ::Operator::ThisCase >
    {
        template < typename _Invokable, typename _Left, typename _Instance >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Instance && instance )
        {
            using LeftRefer = _Left &&;
            using InstanceRefer = _Instance &&;
            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            invokable( ::std::forward< LeftRefer >( left ), InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess() );
            return ::std::forward< InstanceRefer >( instance );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightInstanceCase, ::Operator::DefaultCase >
    {
        template < typename _Invokable, typename _Left, typename _Instance >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Instance && instance )
        {
            using LeftRefer = _Left &&;
            using InstanceRefer = _Instance &&;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using Invokable = _Invokable;
            using Returned = ::std::result_of_t< Invokable( LeftRefer, ValueRefer ) >;

            static_assert( !::std::is_reference< Returned >::value,
                "The type of return parameter must to be not a reference type." );

            using InstanceGuard = ::InstanceGuard< InstanceRefer >;
            using ResultInstance = ::Instance< Returned, ::Inplace::DefaultTool >;
            return ResultInstance( invokable( ::std::forward< LeftRefer >( left ), InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess() ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightInstanceCase, ::Operator::BlockedCase >
    {
        template < typename _Invokable, typename _Left, typename _Instance >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Instance && instance )
        {
            using LeftRefer = _Left &&;
            using InstanceRefer = _Instance &&;
            using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
            using Invokable = _Invokable;
            using InvokableRefer = _Invokable &&;
            using Returned = ::std::result_of_t< Invokable( LeftRefer, ValueRefer ) >;

            static_assert( ::std::is_reference< Returned >::value,
                "The type of return parameter must to be a reference type." );

            using GuardTool = ::Guard::RightTool< Invokable, LeftRefer, InstanceRefer >;
            using ResultInstance = ::Instance< Returned, GuardTool >;
            return ResultInstance( ::std::forward< InvokableRefer >( invokable ), ::std::forward< LeftRefer >( left ), ::std::forward< InstanceRefer >( instance ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::BothExposingCase, ::Operator::FundamentalCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            return invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightExposingCase, ::Operator::FundamentalCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            return invokable( ::std::forward< LeftInstanceRefer >( left ),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftExposingCase, ::Operator::FundamentalCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            return invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                ::std::forward< RightInstanceRefer >( right ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::BothExposingCase, ::Operator::ThisCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
            return ::std::forward< LeftInstanceRefer >( left );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightExposingCase, ::Operator::ThisCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            invokable( ::std::forward< LeftInstanceRefer >( left ),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
            return ::std::forward< LeftInstanceRefer >( left );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftExposingCase, ::Operator::ThisCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                ::std::forward< RightInstanceRefer >( right ) );
            return ::std::forward< LeftInstanceRefer >( left );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::BothExposingCase, ::Operator::DefaultCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
            using RightInstanceRefer = _Right &&;
            using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
            using Invokable = _Invokable;
            using Returned = ::std::result_of_t< Invokable( LeftValueRefer, RightValueRefer ) >;

            static_assert( !::std::is_reference< Returned >::value,
                "The type of return parameter must to be not a reference type." );

            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            using ResultInstance = ::Instance< Returned, ::Inplace::DefaultTool >;
            return ResultInstance( invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightExposingCase, ::Operator::DefaultCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
            using Invokable = _Invokable;
            using Returned = ::std::result_of_t< Invokable( LeftInstanceRefer, RightValueRefer ) >;

            static_assert( !::std::is_reference< Returned >::value,
                "The type of return parameter must to be not a reference type." );

            using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
            using ResultInstance = ::Instance< Returned, ::Inplace::DefaultTool >;
            return ResultInstance( invokable( ::std::forward< LeftInstanceRefer >( left ),
                RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftExposingCase, ::Operator::DefaultCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
            using RightInstanceRefer = _Right &&;
            using Invokable = _Invokable;
            using Returned = ::std::result_of_t< Invokable( LeftValueRefer, RightInstanceRefer ) >;

            static_assert( !::std::is_reference< Returned >::value,
                "The type of return parameter must to be not a reference type." );

            using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
            using ResultInstance = ::Instance< Returned, ::Inplace::DefaultTool >;
            return ResultInstance( invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                ::std::forward< RightInstanceRefer >( right ) ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::BothExposingCase, ::Operator::BlockedCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
            using RightInstanceRefer = _Right &&;
            using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
            using Invokable = _Invokable;
            using InvokableRefer = _Invokable &&;
            using Returned = ::std::result_of_t< Invokable( LeftValueRefer, RightValueRefer ) >;

            using GuardTool = ::Guard::BothTool< Invokable, LeftInstanceRefer, RightInstanceRefer >;
            using ResultInstance = ::Instance< Returned, GuardTool >;
            return ResultInstance( ::std::forward< InvokableRefer >( invokable ), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::RightExposingCase, ::Operator::BlockedCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using RightInstanceRefer = _Right &&;
            using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
            using Invokable = _Invokable;
            using InvokableRefer = _Invokable &&;
            using Returned = ::std::result_of_t< Invokable( LeftInstanceRefer, RightValueRefer ) >;

            using GuardTool = ::Guard::RightTool< Invokable, LeftInstanceRefer, RightInstanceRefer >;
            using ResultInstance = ::Instance< Returned, GuardTool >;
            return ResultInstance( ::std::forward< InvokableRefer >( invokable ), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
        }
    };

    template <>
    struct ResultSwitch< ::Operator::LeftExposingCase, ::Operator::BlockedCase >
    {
        template < typename _Invokable, typename _Left, typename _Right >
        static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
        {
            using LeftInstanceRefer = _Left &&;
            using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
            using RightInstanceRefer = _Right &&;
            using Invokable = _Invokable;
            using InvokableRefer = _Invokable &&;
            using Returned = ::std::result_of_t< Invokable( LeftValueRefer, RightInstanceRefer ) >;

            using GuardTool = ::Guard::LeftTool< Invokable, LeftInstanceRefer, RightInstanceRefer >;
            using ResultInstance = ::Instance< Returned, GuardTool >;
            return ResultInstance( ::std::forward< InvokableRefer >( invokable ), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
        }
    };
}

#endif
