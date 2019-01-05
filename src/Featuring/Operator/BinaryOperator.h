#pragma once
#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include <ModelKit/Featuring/Tool/Guard/BinaryTool.h>
#include <ModelKit/Featuring/Tool/Guard/UnaryTool.h>
#include <ModelKit/Featuring/Tool/Inplace/DefaultTool.h>
#include <ModelKit/Featuring/Traits.h>
#include <ModelKit/Utility/IsMethodExists.h>
#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include <type_traits>
#include <utility>

namespace Operator
{
    namespace Binary
    {
        template < typename >
        struct InstanceSwitch;

        struct NoneInstanceCase {};
        struct LeftInstanceCase {};
        struct RightInstanceCase {};
        struct BothInstanceCase {};

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

        template < typename _Left, typename _Right >
        using InstanceCase = typename InstanceCaseHelper< _Left, _Right >::Type;
    }
}

namespace Operator
{
    namespace BINARY
    {
        /* Cases for operator result type */
        struct FundamentalCase {};
        struct ThisCase {};
        struct DefaultCase {};
        struct BlockedCase {};

        template < typename _Returned, typename _Refer >
        struct ResultCaseHelper
        {
            using Returned = _Returned;
            using Value = ::std::decay_t< _Refer >;

            static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
            static constexpr bool returned_is_same_value = ::std::is_same< _Returned, _Refer >::value;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalCase,
                ::std::conditional_t< returned_is_same_value,
                    ThisCase,
                    ::std::conditional_t< returned_is_reference,
                        BlockedCase,
                        DefaultCase > > >;
        };

        template < typename _Returned, typename _Value >
        using ResultCase = typename ResultCaseHelper< _Returned, _Value >::Type;
    }
}

namespace Operator
{
    namespace BINARY
    {
        using LeftInstanceCase = ::Operator::Binary::LeftInstanceCase;
        using RightInstanceCase = ::Operator::Binary::RightInstanceCase;
        using BothInstanceCase = ::Operator::Binary::BothInstanceCase;

        struct UnaryInstanceCase;
        struct BinaryInstanceCase;

        template < typename, typename >
        struct ResultSwitch {};

        template <>
        struct ResultSwitch< UnaryInstanceCase, FundamentalCase >
        {
            template < typename _Invokable, typename _Instance, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                return invokable( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct ResultSwitch< UnaryInstanceCase, ThisCase >
        {
            template < typename _Invokable, typename _Instance, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                invokable( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
                return ::std::forward< InstanceRefer >( instance );
            }
        };

        template <>
        struct ResultSwitch< UnaryInstanceCase, DefaultCase >
        {
            template < typename _Invokable, typename _Instance, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Invokable && invokable, _Instance && instance, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                using Invokable = _Invokable;
                using Returned = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >;

                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );

                using InstanceGuard = ::InstanceGuard< InstanceRefer >;
                using DefaultTool = ::Inplace::DefaultTool;
                return ::Instance< Returned, DefaultTool >( invokable( InstanceGuard( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };

        template <>
        struct ResultSwitch< UnaryInstanceCase, BlockedCase >
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

                using GuardTool = ::Guard::UnaryTool< Invokable, InstanceRefer, _Arguments ... >;
                using Result = ::std::remove_reference_t< Returned >;
                return ::Instance< Result, GuardTool >( ::std::forward< InstanceRefer >( instance ), ::std::forward< InvokableRefer >( invokable ), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, FundamentalCase >
        {
            template < typename _Invokable, typename _Left, typename _Right >
            static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using RightInstanceRefer = _Right &&;
                // TODO: check - this part of
                using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
                using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
                return invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                    RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, ThisCase >
        {
            template < typename _Invokable, typename _Left, typename _Right >
            static constexpr decltype(auto) invoke ( _Invokable && invokable, _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using RightInstanceRefer = _Right &&;
                // TODO: check - this part of
                using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
                using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
                invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                    RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
                return ::std::forward< LeftInstanceRefer >( left );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, DefaultCase >
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
                    "The type of return parameter must to be a non reference type." );

                // TODO: check - this part of
                using LeftInstanceGuard = ::InstanceGuard< LeftInstanceRefer >;
                using RightInstanceGuard = ::InstanceGuard< RightInstanceRefer >;
                using DefaultTool = ::Inplace::DefaultTool;
                return ::Instance< Returned, DefaultTool >(invokable( LeftInstanceGuard( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(),
                    RightInstanceGuard( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() ) );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, BlockedCase >
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

                static_assert( ::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a reference type." );

                using GuardTool = ::Guard::BinaryTool< Invokable, LeftInstanceRefer, RightInstanceRefer >;
                using Result = ::std::remove_reference_t< Returned >;
                return ::Instance< Result, GuardTool >( ::std::forward< InvokableRefer >( invokable ), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
            }
        };

    }
}

namespace Operator
{
    namespace Binary
    {
        /* Cases for Holder method existing */
        struct HolderHasOperatorCase {};
        struct HolderHasNoOperatorCase {};
    }
}

namespace Operator
{
    namespace BINARY
    {
        using HolderHasOperatorCase = ::Operator::Binary::HolderHasOperatorCase;
        using HolderHasNoOperatorCase = ::Operator::Binary::HolderHasNoOperatorCase;
    }
}

namespace Operator
{
    namespace Binary
    {
        template < typename, typename >
        struct ResultSwitch;

        /* Cases for operator result type */
        struct FundamentalCase {};
        struct LeftCase {};
        struct RightCase {};
        struct DefaultCase {};
        struct BlockedCase {};

        template < typename _Returned, typename _LeftRefer, typename _RightRefer >
        struct ResultCaseHelper
        {
            using Returned = _Returned;
            using LeftValue = ::std::decay_t< _LeftRefer >;
            using RightValue = ::std::decay_t< _RightRefer >;

            static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
            static constexpr bool returned_is_same_left_value = ::std::is_same< _Returned, _LeftRefer >::value;
            static constexpr bool returned_is_same_right_value = ::std::is_same< _Returned, _RightRefer >::value;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalCase,
                ::std::conditional_t< returned_is_same_left_value,
                    LeftCase,
                    ::std::conditional_t< returned_is_same_right_value,
                        RightCase,
                        ::std::conditional_t< returned_is_reference,
                            BlockedCase,
                            DefaultCase > > > >;
        };

        template < typename _Returned, typename _LeftRefer, typename _RightRefer >
        using ResultCase = typename ResultCaseHelper< _Returned, _LeftRefer, _RightRefer >::Type;

    }
}

namespace Operator
{
    namespace Binary
    {
        template <>
        struct ResultSwitch< NoneInstanceCase, FundamentalCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                return invokable( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) );
            }
        };

        template <>
        struct ResultSwitch< NoneInstanceCase, DefaultCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                using Returned = ::std::result_of_t< _Invokable( LeftRefer, RightRefer ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ) );
            }
        };

        template <>
        struct ResultSwitch< NoneInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< NoneInstanceCase, RightCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< NoneInstanceCase, BlockedCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< LeftInstanceCase, FundamentalCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                return invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(), ::std::forward< RightRefer >( right ) );
            }
        };

        template <>
        struct ResultSwitch< LeftInstanceCase, DefaultCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightRefer = _RightRefer &&;
                using Returned = ::std::result_of_t< _Invokable( LeftValueRefer, RightRefer ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(), ::std::forward< RightRefer >( right ) ) );
            }
        };

        template <>
        struct ResultSwitch< LeftInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< LeftInstanceCase, RightCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< LeftInstanceCase, BlockedCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< RightInstanceCase, FundamentalCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightInstanceRefer = _RightRefer &&;
                return invokable( ::std::forward< LeftRefer >( left ), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
            }
        };

        template <>
        struct ResultSwitch< RightInstanceCase, DefaultCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightInstanceRefer = _RightRefer &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( LeftRefer, RightValueRefer ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( ::std::forward< LeftRefer >( left ), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() ) );
            }
        };

        template <>
        struct ResultSwitch< RightInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< RightInstanceCase, RightCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< RightInstanceCase, BlockedCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, FundamentalCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using RightInstanceRefer = _RightRefer &&;
                return invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, DefaultCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightInstanceRefer = _RightRefer &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( LeftValueRefer, RightValueRefer ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).instanceAccess(), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).instanceAccess() ) );
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, RightCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };

        template <>
        struct ResultSwitch< BothInstanceCase, BlockedCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
            }
        };
    }
}

namespace Operator
{
    namespace Binary
    {
        template <>
        struct InstanceSwitch< NoneInstanceCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                using Invokable = _Invokable &&;
                using Returned = ::std::result_of_t< _Invokable( LeftRefer, RightRefer ) >;
                return ResultSwitch< NoneInstanceCase, ResultCase< Returned, LeftRefer, RightRefer > >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ), ::std::forward< Invokable >( invokable ) );
            }
        };

        template <>
        struct InstanceSwitch< LeftInstanceCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightRefer = _RightRefer &&;
                using Invokable = _Invokable &&;
                using Returned = ::std::result_of_t< _Invokable( LeftValueRefer, RightRefer ) >;
                return ResultSwitch< LeftInstanceCase, ResultCase< Returned, LeftValueRefer, RightRefer > >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ), ::std::forward< Invokable >( invokable ) );
            }
        };

        template <>
        struct InstanceSwitch< RightInstanceCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightInstanceRefer = _RightRefer &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Invokable = _Invokable &&;
                using Returned = ::std::result_of_t< _Invokable( LeftRefer, RightValueRefer ) >;
                return ResultSwitch< RightInstanceCase, ResultCase< Returned, LeftRefer, RightValueRefer > >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ), ::std::forward< Invokable >( invokable ) );
            }
        };

        template <>
        struct InstanceSwitch< BothInstanceCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightInstanceRefer = _RightRefer &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Invokable = _Invokable &&;
                using Returned = ::std::result_of_t< _Invokable( LeftValueRefer, RightValueRefer ) >;
                return ResultSwitch< BothInstanceCase, ResultCase< Returned, LeftValueRefer, RightValueRefer > >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ), ::std::forward< Invokable >( invokable ) );
            }
        };
    }
}

#define BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Binary \
        { \
            struct Invokable \
            { \
                template < typename _Left, typename _Right > \
                decltype(auto) operator () ( _Left && left, _Right && right ) \
                { \
                    return ::std::forward< _Left && >( left ) symbol ::std::forward< _Right && >( right ); \
                } \
            }; \
     \
            template < typename _LeftRefer, typename _RightRefer > \
            struct Invokable ## Helper \
            { \
                static_assert( ::std::is_reference< _LeftRefer >::value, \
                    "The template parameter _LeftRefer must to be a reference type." ); \
                static_assert( ::std::is_reference< _RightRefer >::value, \
                    "The template parameter _RightRefer must to be a reference type." ); \
                using LeftRefer = _LeftRefer; \
                using RightRefer = _RightRefer; \
                static constexpr decltype(auto) invoke( LeftRefer left, RightRefer right ) \
                { \
                    return InstanceSwitch< InstanceCase< LeftRefer, RightRefer > >::invoke( \
                        ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ), Invokable() ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            IS_BINARY_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
        } \
    } \



namespace Operator
{
    namespace Binary
    {
        IS_BINARY_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( * ), Multiply )
        IS_METHOD_EXISTS_TRAIT( operatorMultiply )
        IS_METHOD_EXISTS_TRAIT( operatorMultiplyLeft )
        IS_METHOD_EXISTS_TRAIT( operatorMultiplyRight )
        IS_METHOD_EXISTS_TRAIT( operatorMultiplyBoth )
    }
}

namespace Operator
{
    namespace Binary
    {
        struct Multiply
        {
            template < typename _Left, typename _Right >
            decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left && >( left ) * ::std::forward< _Right && >( right );
            }
        };
    }
}

namespace Operator
{
    namespace BINARY
    {
        template < typename, typename >
        struct MultiplySwitch;

        template <>
        struct MultiplySwitch< LeftInstanceCase, HolderHasOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder;
                using RightRefer = _Right &&;
                LeftHolder::operatorMultiplyLeft( ::instanceHolder( ::std::forward< LeftInstanceRefer >( left ) ), ::std::forward< RightRefer >( right ) );
                return ::std::forward< LeftInstanceRefer >( left );
            }
        };

        template <>
        struct MultiplySwitch< LeftInstanceCase, HolderHasNoOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightRefer = _Right &&;
                using Returned = ::std::result_of_t< ::Operator::Binary::Multiply( LeftValueRefer, RightRefer ) >;
                return ::Operator::BINARY::ResultSwitch< UnaryInstanceCase, ::Operator::BINARY::ResultCase< Returned, LeftValueRefer > >
                    ::invoke( ::Operator::Binary::Multiply(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) );
            }
        };

        template <>
        struct MultiplySwitch< RightInstanceCase, HolderHasOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftRefer = _Left &&;
                using RightInstanceRefer = _Right &&;
                using RightHolder = typename ::std::decay_t< RightInstanceRefer >::Holder;
                RightHolder::operatorMultiplyRight( ::instanceHolder( ::std::forward< RightInstanceRefer >( right ) ), ::std::forward< LeftRefer >( left ) );
                return ::std::forward< RightInstanceRefer >( right );
            }
        };

        template <>
        struct MultiplySwitch< RightInstanceCase, HolderHasNoOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftRefer = _Left &&;
                using RightInstanceRefer = _Right &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Returned = ::std::result_of_t< ::Operator::Binary::Multiply( LeftRefer, RightValueRefer ) >;
                return ::Operator::BINARY::ResultSwitch< UnaryInstanceCase, ::Operator::BINARY::ResultCase< Returned, RightValueRefer > >
                    ::invoke( ::Operator::Binary::Multiply(), ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
            }
        };

        template <>
        struct MultiplySwitch< BothInstanceCase, HolderHasOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder;
                using RightInstanceRefer = _Right &&;
                LeftHolder::operatorMultiplyBoth( ::instanceHolder( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ) );
                return ::std::forward< RightInstanceRefer >( right );
            }
        };

        template <>
        struct MultiplySwitch< BothInstanceCase, HolderHasNoOperatorCase >
        {
            template < typename _Left, typename _Right >
            static decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >;
                using RightInstanceRefer = _Right &&;
                using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >;
                using Returned = ::std::result_of_t< ::Operator::Binary::Multiply( LeftValueRefer, RightValueRefer ) >;
                return ::Operator::BINARY::ResultSwitch< BothInstanceCase, ::Operator::BINARY::ResultCase< Returned, LeftValueRefer > >
                    ::invoke( ::Operator::Binary::Multiply(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
            }
        };
    }
}

namespace Operator
{
    namespace Binary
    {
        template < typename >
        struct MultiplyInstanceSwitch;

        template <>
        struct MultiplyInstanceSwitch < LeftInstanceCase >
        {
            template < typename _Left, typename _Right >
            static constexpr decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftInstance = ::std::decay_t< LeftInstanceRefer >;
                using LeftHolder = typename LeftInstance::Holder;
                using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >;
                using RightRefer = _Right &&;

                constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operatorMultiplyLeft_method_exists< LeftHolder, void( LeftHolderRefer, RightRefer ) >;
                using OperatorCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >;
                return ::Operator::BINARY::MultiplySwitch< LeftInstanceCase, OperatorCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) );
            }
        };

        template <>
        struct MultiplyInstanceSwitch < RightInstanceCase >
        {
            template < typename _Left, typename _Right >
            static constexpr decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using RightInstanceRefer = _Right &&;
                using RightInstance = ::std::decay_t< RightInstanceRefer >;
                using RightHolder = typename RightInstance::Holder;
                using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >;
                using LeftRefer = _Left &&;

                constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operatorMultiplyRight_method_exists< RightHolder, void( RightHolderRefer, LeftRefer ) >;
                using OperatorCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >;
                return ::Operator::BINARY::MultiplySwitch< RightInstanceCase, OperatorCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
            }
        };

        template <>
        struct MultiplyInstanceSwitch < BothInstanceCase >
        {
            template < typename _Left, typename _Right >
            static constexpr decltype(auto) invoke ( _Left && left, _Right && right )
            {
                using LeftInstanceRefer = _Left &&;
                using LeftInstance = ::std::decay_t< LeftInstanceRefer >;
                using LeftHolder = typename LeftInstance::Holder;
                using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >;
                using RightInstanceRefer = _Right &&;
                using RightInstance = ::std::decay_t< RightInstanceRefer >;
                using RightHolder = typename RightInstance::Holder;
                using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >;

                constexpr bool is_left_compatible_to_right = ::is_compatible< LeftHolder, RightHolder >;
                constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operatorMultiplyBoth_method_exists< LeftHolder, void( LeftHolderRefer, RightHolderRefer ) >;
                using OperatorCase = ::std::conditional_t< is_left_compatible_to_right && holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >;
                return ::Operator::BINARY::MultiplySwitch< BothInstanceCase, OperatorCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) );
            }
        };
    }
}


namespace Operator
{
    namespace Binary
    {
        template < typename _LeftRefer, typename _RightRefer >
        struct MultiplyHelper
        {
            static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." );
            static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." );

            using LeftRefer = _LeftRefer;
            using RightRefer = _RightRefer;

            static constexpr decltype(auto) invoke( LeftRefer left, RightRefer right )
            {
                return ::Operator::Binary::MultiplyInstanceSwitch< InstanceCase< LeftRefer, RightRefer > >
                    ::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) );
            }
        };
    }
}


#endif
