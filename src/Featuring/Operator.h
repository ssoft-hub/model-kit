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
        template < typename _Refer, typename _Argument >
        struct SquareBrackets
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return refer.operator[] ( ::std::forward< _Argument >( argument ) );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer, typename ... _Arguments >
        struct RoundBrackets
        {
            decltype(auto) operator () ( _Refer refer, _Arguments && ... arguments )
            {
                return refer.operator() ( ::std::forward< _Arguments >( arguments ) ... );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixPlus
        {
            static_assert( !is_instance< ::std::decay_t< _Refer > >, "sds" );

            using Result = decltype( + std::declval< _Refer >() );
            Result operator () ( _Refer refer )
            {
                return + ::std::forward< _Refer >( refer );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixMinus
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return - ::std::forward< _Refer >( refer );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixPlusPlus
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return ++ ::std::forward< _Refer >( refer );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixMinusMinus
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return -- ::std::forward< _Refer >( refer );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPostfixPlusPlus
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return ::std::forward< _Refer >( refer ) ++;
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPostfixMinusMinus
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return ::std::forward< _Refer >( refer ) --;
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixBitwiseNot
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return ~ ::std::forward< _Refer >( refer );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer >
        struct UnaryPrefixLogicalNot
        {
            decltype(auto) operator () ( _Refer refer )
            {
                return ! ::std::forward< _Refer >( refer );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
        template < typename _Right, typename _Left >
        struct IsEqual
        {
            decltype(auto) operator () ( _Right && refer, _Left && argument )
            {
                return ::std::forward< _Right >( refer ) == ::std::forward< _Left >( argument );
            }
        };

        template < typename _Refer, typename _Argument >
        struct NotEqual
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return ::std::forward< _Refer >( refer ) = ::std::forward< _Argument >( argument );
            }
        };

        template < typename _Refer, typename _Argument >
        struct BinaryRightLess
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return refer.operator < ( ::std::forward< _Argument >( argument ) );
            }
        };

        template < typename _Refer, typename _Argument >
        struct LessOrEqual
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return refer.operator <= ( ::std::forward< _Argument >( argument ) );
            }
        };

        template < typename _Refer, typename _Argument >
        struct Greater
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return refer.operator > ( ::std::forward< _Argument >( argument ) );
            }
        };

        template < typename _Refer, typename _Argument >
        struct GreaterOrEqual
        {
            decltype(auto) operator () ( _Refer refer, _Argument && argument )
            {
                return refer.operator >= ( ::std::forward< _Argument >( argument ) );
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

namespace Operator
{
    template < typename _Instance,
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Instance > > > >
    using AnyKingOfInstance = _Instance;

    template < typename _Argument,
        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _Argument > > > >
    using NotInstance = _Argument;
}

// Unary operators
#define INSTANCE_PREFIX_UNARY_OPERATOR( symbol, invokable ) \
    template < typename _RightInstance > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _RightInstance > && value ) \
    { \
        using RightInstance = ::std::decay_t< _RightInstance >; \
        using RightInstanceRefer = ::std::add_rvalue_reference_t< ::Operator::AnyKingOfInstance< _RightInstance > >; \
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
    template < typename _LeftInstance > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && value, int ) \
    { \
        using LeftInstance = ::std::decay_t< _LeftInstance >; \
        using LeftInstanceRefer = ::std::add_rvalue_reference_t< Operator::AnyKingOfInstance< _LeftInstance > >; \
        using LeftValueRefer = ::SimilarRefer< typename LeftInstance::Value, LeftInstanceRefer >; \
        return Operator::invoke< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( value ), invokable< LeftValueRefer >() ); \
    } \

INSTANCE_POSTFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPostfixPlusPlus )
INSTANCE_POSTFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPostfixMinusMinus )

#undef INSTANCE_POSTFIX_UNARY_OPERATOR

#define INSTANCE_BINARY_OPERATOR( symbol, invokable ) \
    template < typename _LeftInstance, typename _RightInstance, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _LeftInstance > > && ::is_instance< ::std::decay_t< _RightInstance > > > > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && /*left*/, ::Operator::AnyKingOfInstance< _RightInstance > && /*right*/ ) \
    { \
        return; \
    } \

/*
    \
    template < typename _LeftInstance, typename _RightArgument, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _LeftInstance > > && !::is_instance< ::std::decay_t< _RightArgument > > > > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && left, ::Operator::NotInstance< _RightArgument > && right ) \
    { \
        return; \
    } \
    \
    template < typename _LeftArgument, typename _RightInstance, \
        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _LeftArgument > > && ::is_instance< ::std::decay_t< _RightInstance > > > > \
    inline decltype(auto) operator symbol ( ::Operator::NotInstance< _LeftArgument > && left, ::Operator::AnyKingOfInstance< _RightInstance > && right ) \
    { \
        return; \
    } \
*/

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
