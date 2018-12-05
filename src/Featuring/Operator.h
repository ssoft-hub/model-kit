#pragma once
#ifndef OPERATOR_RESOLVER_H
#define OPERATOR_RESOLVER_H

#include <ModelKit/Featuring/Tool/Inplace/DefaultTool.h>
#include <ModelKit/Featuring/Tool/Member/FunctionTool.h>
#include <ModelKit/Featuring/Traits.h>

#include <functional>
#include <type_traits>
#include <utility>

template < typename _Value, typename _Tool >
class Instance;
namespace Inplace { class DefaultTool; }

namespace Operator
{
    namespace Private
    {
        template < typename _Kind >
        struct InvokeHelper;

        struct FundamentalKind {};
        struct BlockedInstanceKind {};
        struct DefaultInstanceKind {};

        template < typename _Returned >
        struct OperatorKindHelper
        {
            using Returned = _Returned;
            using Type = ::std::conditional_t< ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value,
                FundamentalKind,
                ::std::conditional_t< ::std::is_reference< Returned >::value,
                    BlockedInstanceKind,
                    DefaultInstanceKind > >;
        };

        template <>
        struct InvokeHelper< FundamentalKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                return invokable( ::std::forward< ValueRefer >( InstanceGuard< _InstanceRefer >( ::std::forward< _InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct InvokeHelper< BlockedInstanceKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Invokable::FunctionTool< _InstanceRefer, _Invokable, _Arguments ... >;
                return Instance< Result, Tool >( ::std::forward< _InstanceRefer >( instance ), invokable, ::std::forward< _Arguments >( arguments ) ...  );
            }
        };

        template <>
        struct InvokeHelper< DefaultInstanceKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Inplace::DefaultTool;
                return Instance< Result, Tool >( invokable( ::std::forward< ValueRefer >( InstanceGuard< _InstanceRefer >( ::std::forward< _InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };
    }

    template < typename _Returned >
    using OperatorKind = typename Private::OperatorKindHelper< _Returned >::Type;

    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
    {
        using ValueRefer = SimilarRefer< typename ::std::remove_reference_t< _InstanceRefer >::Value, _InstanceRefer >;
        using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
        //using Returned = decltype( ::std::declval< _Invokable >().operator () ( ::std::declval< ValueRefer >(), ::std::declval< _Arguments >() ... ) );
        return Private::InvokeHelper< OperatorKind< Returned > >:: template invoke< _InstanceRefer >(
            ::std::forward< _InstanceRefer >( instance ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

namespace Operator
{
    namespace Private
    {
        template < typename _Left, typename _Argument >
        struct SquareBrackets
        {
            constexpr decltype(auto) operator () ( _Left && left, _Argument && argument )
            {
                return ::std::forward< _Left >( left )[ ::std::forward< _Argument >( argument ) ];
            }
        };
    }

    namespace Private
    {
        template < typename _Left, typename ... _Arguments >
        struct RoundBrackets
        {
            constexpr decltype(auto) operator () ( _Left && left, _Arguments && ... arguments )
            {
                return ::std::forward< _Left >( left )( ::std::forward< _Arguments >( arguments ) ... );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixPlus
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return + ::std::forward< _Right >( right );
            }
        };
    }

    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixMinus
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return - ::std::forward< _Right >( right );
            }
        };
    }

    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixPlusPlus
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return ++ ::std::forward< _Right >( right );
            }
        };
    }

    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixMinusMinus
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return -- ::std::forward< _Right >( right );
            }
        };
    }

    namespace Private
    {
        template < typename _Left >
        struct UnaryPostfixPlusPlus
        {
            constexpr decltype(auto) operator () ( _Left && left )
            {
                return ::std::forward< _Left >( left ) ++;
            }
        };
    }

    namespace Private
    {
        template < typename _Left >
        struct UnaryPostfixMinusMinus
        {
            constexpr decltype(auto) operator () ( _Left && left )
            {
                return ::std::forward< _Left >( left ) --;
            }
        };
    }

    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixBitwiseNot
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return ~ ::std::forward< _Right >( right );
            }
        };
    }

    namespace Private
    {
        template < typename _Right >
        struct UnaryPrefixLogicalNot
        {
            constexpr decltype(auto) operator () ( _Right && right )
            {
                return ! ::std::forward< _Right >( right );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
        template < typename _Left, typename _Right >
        struct IsEqual
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) == ::std::forward< _Right >( right );
            }
        };

        template < typename _Left, typename _Right >
        struct NotEqual
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) != ::std::forward< _Right >( right );
            }
        };

        template < typename _Left, typename _Right >
        struct Less
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) < ::std::forward< _Right >( right );
            }
        };

        template < typename _Left, typename _Right >
        struct LessOrEqual
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) <= ::std::forward< _Right >( right );
            }
        };

        template < typename _Left, typename _Right >
        struct Greater
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) > ::std::forward< _Right >( right );
            }
        };

        template < typename _Left, typename _Right >
        struct GreaterOrEqual
        {
            constexpr decltype(auto) operator () ( _Left && left, _Right && right )
            {
                return ::std::forward< _Left >( left ) >= ::std::forward< _Right >( right );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
    }
}

// Unary operators
#define INSTANCE_PREFIX_UNARY_OPERATOR( symbol, invokable ) \
    template < typename _Right, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Right > > > > \
    inline decltype(auto) operator symbol ( _Right && value ) \
    { \
        using RightInstance = ::std::decay_t< _Right >; \
        using RightInstanceRefer = ::std::add_rvalue_reference_t< _Right >; \
        using RightValueRefer = ::SimilarRefer< typename RightInstance::Value, RightInstanceRefer >; \
        return Operator::invoke< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( value ), invokable< RightValueRefer >() ); \
    } \

INSTANCE_PREFIX_UNARY_OPERATOR( +, ::Operator::Private::UnaryPrefixPlus )
INSTANCE_PREFIX_UNARY_OPERATOR( -, ::Operator::Private::UnaryPrefixMinus )
INSTANCE_PREFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPrefixPlusPlus )
INSTANCE_PREFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPrefixMinusMinus )
INSTANCE_PREFIX_UNARY_OPERATOR( ~, ::Operator::Private::UnaryPrefixBitwiseNot )
INSTANCE_PREFIX_UNARY_OPERATOR( !, ::Operator::Private::UnaryPrefixLogicalNot )

#undef INSTANCE_PREFIX_UNARY_OPERATOR

#define INSTANCE_POSTFIX_UNARY_OPERATOR( symbol, invokable ) \
    template < typename _Left, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Left > > > > \
    inline decltype(auto) operator symbol ( _Left && value, int ) \
    { \
        using LeftInstance = ::std::decay_t< _Left >; \
        using LeftInstanceRefer = ::std::add_rvalue_reference_t< _Left >; \
        using LeftValueRefer = ::SimilarRefer< typename LeftInstance::Value, LeftInstanceRefer >; \
        return Operator::invoke< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( value ), invokable< LeftValueRefer >() ); \
    } \

INSTANCE_POSTFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPostfixPlusPlus )
INSTANCE_POSTFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPostfixMinusMinus )

#undef INSTANCE_POSTFIX_UNARY_OPERATOR

#define INSTANCE_BINARY_OPERATOR( symbol, invokable ) \
    template < typename _Left, typename _Right, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Right > > || ::is_instance< ::std::decay_t< _Left > > > > \
    inline decltype(auto) operator symbol ( _Left && /*left*/, _Right && /*right*/ ) \
    { \
        return; \
    } \

// Binary operators
INSTANCE_BINARY_OPERATOR( ==, ??? )
INSTANCE_BINARY_OPERATOR( !=, ??? )
INSTANCE_BINARY_OPERATOR( <, ??? )
INSTANCE_BINARY_OPERATOR( <=, ??? )
INSTANCE_BINARY_OPERATOR( >, ??? )
INSTANCE_BINARY_OPERATOR( >=, ??? )

INSTANCE_BINARY_OPERATOR( *, ??? )
INSTANCE_BINARY_OPERATOR( /, ??? )
INSTANCE_BINARY_OPERATOR( %, ??? )
INSTANCE_BINARY_OPERATOR( +, ??? )
INSTANCE_BINARY_OPERATOR( -, ??? )

INSTANCE_BINARY_OPERATOR( <<, ??? )
INSTANCE_BINARY_OPERATOR( >>, ??? )

INSTANCE_BINARY_OPERATOR( &, ??? )
INSTANCE_BINARY_OPERATOR( ^, ??? )
INSTANCE_BINARY_OPERATOR( |, ??? )
INSTANCE_BINARY_OPERATOR( &&, ??? )
INSTANCE_BINARY_OPERATOR( ||, ??? )

INSTANCE_BINARY_OPERATOR( *=, ??? )
INSTANCE_BINARY_OPERATOR( /=, ??? )
INSTANCE_BINARY_OPERATOR( %=, ??? )
INSTANCE_BINARY_OPERATOR( +=, ??? )
INSTANCE_BINARY_OPERATOR( -=, ??? )
INSTANCE_BINARY_OPERATOR( <<=, ??? )
INSTANCE_BINARY_OPERATOR( >>=, ??? )
INSTANCE_BINARY_OPERATOR( &=, ??? )
INSTANCE_BINARY_OPERATOR( ^=, ??? )
INSTANCE_BINARY_OPERATOR( |=, ??? )

#undef INSTANCE_BINARY_OPERATOR

#endif
