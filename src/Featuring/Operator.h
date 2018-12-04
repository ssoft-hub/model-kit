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
#endif
