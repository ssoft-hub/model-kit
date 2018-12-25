#pragma once
#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "Resolver.h"

namespace Operator
{
    namespace Binary
    {
        template < typename >
        struct InstanceSwitch;

        struct NoInstanceCase {};
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
                        NoInstanceCase > > >;
        };

        template < typename _Left, typename _Right >
        using InstanceCase = typename InstanceCaseHelper< _Left, _Right >::Type;
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
        struct ResultSwitch< NoInstanceCase, FundamentalCase >
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
        struct ResultSwitch< LeftInstanceCase, FundamentalCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftInstanceRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                return invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).value(), ::std::forward< RightRefer >( right ) );
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
                return invokable( ::std::forward< LeftRefer >( left ), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).value() );
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
                return invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).value(), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).value() );
            }
        };

        template <>
        struct ResultSwitch< NoInstanceCase, DefaultCase >
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
                return Instance< Returned, Tool >( invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).value(), ::std::forward< RightRefer >( right ) ) );
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
                return Instance< Returned, Tool >( invokable( ::std::forward< LeftRefer >( left ), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).value() ) );
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
                return Instance< Returned, Tool >( invokable( InstanceGuard< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( left ) ).value(), InstanceGuard< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( right ) ).value() ) );
            }
        };

        template <>
        struct ResultSwitch< NoInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
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
        struct ResultSwitch< RightInstanceCase, LeftCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
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
        struct ResultSwitch< NoInstanceCase, RightCase >
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
        struct ResultSwitch< RightInstanceCase, RightCase >
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
        struct ResultSwitch< NoInstanceCase, BlockedCase >
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
        struct ResultSwitch< RightInstanceCase, BlockedCase >
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
        struct InstanceSwitch< NoInstanceCase >
        {
            template < typename _LeftRefer, typename _RightRefer, typename _Invokable >
            static constexpr decltype(auto) invoke ( _LeftRefer && left, _RightRefer && right, _Invokable && invokable )
            {
                using LeftRefer = _LeftRefer &&;
                using RightRefer = _RightRefer &&;
                using Invokable = _Invokable &&;
                using Returned = ::std::result_of_t< _Invokable( LeftRefer, RightRefer ) >;
                return ResultSwitch< NoInstanceCase, ResultCase< Returned, LeftRefer, RightRefer > >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ), ::std::forward< Invokable >( invokable ) );
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

#endif
