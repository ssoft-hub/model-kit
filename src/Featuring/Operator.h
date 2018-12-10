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
namespace Inplace { struct DefaultTool; }

namespace Operator
{
    namespace Private
    {
        /*
         * Если результат воздействия оператора возвращает
         *      - значение не оборачиваемого типа (фундаментальный или перечисляемый), тогда возвращается само значение не оборачиваемого типа;
         *      - значение оборачиваемого типа Type, тогда возвращается Instance< Type, DefaultTool >;
         *      - ссылку на значение любого типа Type, тогда
         *          - если Instance является и правым, и левым операндом, Instance< Type, BothOperandTool >
         *          - если Instance является только правым операндом, Instance< Type, RightOperandTool >
         *          - если Instance является только левым операндом, Instance< Type, LeftOperandTool >
         */

        template < typename _Kind >
        struct InvokeHelper;

        struct FundamentalKind {};
        struct DefaultInstanceKind {};
        struct RightBlockedInstanceKind {};
        struct LeftBlockedInstanceKind {};
        struct BothBlockedInstanceKind {};

        template < typename _Returned, typename _Right, typename _Left >
        struct OperatorKindHelper
        {
            using Returned = _Returned;
            using Right = ::std::decay_t< _Right >;
            using Left = ::std::decay_t< _Left >;

            constexpr static bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            constexpr static bool returned_is_reference = ::std::is_reference< Returned >::value;
            constexpr static bool right_is_instance = ::is_instance< Right >;
            constexpr static bool left_is_instance = ::is_instance< Left >;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalKind,
                ::std::conditional_t< !returned_is_reference || ( !right_is_instance && !left_is_instance ),
                    DefaultInstanceKind,
                    ::std::conditional_t< right_is_instance && left_is_instance,
                        BothBlockedInstanceKind,
                        ::std::conditional_t< right_is_instance,
                            RightBlockedInstanceKind,
                            LeftBlockedInstanceKind > > > >;
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
        struct InvokeHelper< RightBlockedInstanceKind >
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

    template < typename _Returned, typename _Right, typename _Left >
    using OperatorKind = typename Private::OperatorKindHelper< _Returned, _Right, _Left >::Type;

    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
    {
        using ValueRefer = SimilarRefer< typename ::std::remove_reference_t< _InstanceRefer >::Value, _InstanceRefer >;
        using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
        //using Returned = decltype( ::std::declval< _Invokable >().operator () ( ::std::declval< ValueRefer >(), ::std::declval< _Arguments >() ... ) );
        return Private::InvokeHelper< OperatorKind< Returned, ValueRefer, void > >:: template invoke< _InstanceRefer >(
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

//// Unary prefix operators
//#define INSTANCE_PREFIX_UNARY_OPERATOR( symbol, invokable ) \
//    template < typename _Right, \
//        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Right > > > > \
//    inline decltype(auto) operator symbol ( _Right && value ) \
//    { \
//        using RightInstance = ::std::decay_t< _Right >; \
//        using RightInstanceRefer = ::std::add_rvalue_reference_t< _Right >; \
//        using RightValueRefer = ::SimilarRefer< typename RightInstance::Value, RightInstanceRefer >; \
//        return Operator::invoke< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( value ), invokable< RightValueRefer >() ); \
//    } \

//// Unary postfix operators
//#define INSTANCE_POSTFIX_UNARY_OPERATOR( symbol, invokable ) \
//    template < typename _Left, \
//        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Left > > > > \
//    inline decltype(auto) operator symbol ( _Left && value, int ) \
//    { \
//        using LeftInstance = ::std::decay_t< _Left >; \
//        using LeftInstanceRefer = ::std::add_rvalue_reference_t< _Left >; \
//        using LeftValueRefer = ::SimilarRefer< typename LeftInstance::Value, LeftInstanceRefer >; \
//        return Operator::invoke< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( value ), invokable< LeftValueRefer >() ); \
//    } \

// Binary operators
#define GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool > \
    constexpr decltype(auto) operator symbol ( _Left && /*left*/, Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        /* TODO: */ \
        return ::std::forward< Instance< _RightValue, _RightTool > right_refer >( right ); \
    } \

#define GLOBAL_BINARY_OPERATOR( symbol, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, &&, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const &&, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, volatile &&, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const volatile &&, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, &, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const &, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, volatile &, invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const volatile &, invokable ) \

#define BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer, invokable ) \
    constexpr decltype(auto) operator symbol ( ThisType other_refer other ) this_refer \
    { \
        /* TODO: */ \
        ::std::forward< Holder this_refer >( m_holder ) = InstanceResolver< ThisType, ThisType other_refer >( ::std::forward< ThisType other_refer >( other ) ).resolve(); \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, invokable ) \
    template < typename _Argument > \
    constexpr decltype(auto) operator symbol ( _Argument && /*argument*/ ) this_refer \
    { \
        /* TODO: */ \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, this_refer, invokable ) \
    template < typename ... _Arguments > \
    constexpr decltype(auto) operator symbol ( _Arguments && ... /*arguments*/ ) this_refer \
    { \
        /* TODO: */ \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define PREFIX_OPERATOR_PROTOTYPE( symbol, this_refer, invokable ) \
    constexpr decltype(auto) operator symbol () this_refer \
    { \
        /* TODO: */ \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, this_refer, invokable ) \
    constexpr decltype(auto) operator symbol ( int ) this_refer \
    { \
        /* TODO: */ \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, this_refer ) \
    constexpr ValueGuard< ThisType this_refer > operator symbol () this_refer \
    { \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( other_refer ) \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( other_refer ) \
    template < typename _OtherValue, typename _OtherTool > \
    constexpr Instance ( Instance< _OtherValue, _OtherTool > other_refer other ) \
        : m_holder( InstanceResolver< ThisType, Instance< _OtherValue, _OtherTool > other_refer >( \
            ::std::forward< Instance< _OtherValue, _OtherTool > other_refer >( other ) ).resolve() ) \
    { \
    } \

#define CAST_OPERATOR_PROTOTYPE( refer_type ) \
    template < typename _OtherValue, \
        typename = ::std::enable_if_t< \
            ::std::is_same< ::std::decay_t< _OtherValue >, ::std::decay_t< _Value > >::value \
            && ::is_similar< _OtherValue, _Value > > > \
    operator Instance< _OtherValue, _Tool > refer_type () refer_type \
    { \
        return static_cast< Instance< _OtherValue, _Tool > refer_type >( *this ); \
    } \

#define CONSTRUCTOR_FOR_THIS_INSTANCE \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( && ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const && ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile && ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile && ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( & ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const & ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile & ) \
    CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile & ) \

#define CONSTRUCTOR_FOR_OTHER_INSTANCE \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( && ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const && ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile && ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile && ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( & ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const & ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile & ) \
    CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile & ) \

#define CAST_OPERATOR \
    CAST_OPERATOR_PROTOTYPE( && ) \
    CAST_OPERATOR_PROTOTYPE( const && ) \
    CAST_OPERATOR_PROTOTYPE( volatile && ) \
    CAST_OPERATOR_PROTOTYPE( const volatile && ) \
    CAST_OPERATOR_PROTOTYPE( & ) \
    CAST_OPERATOR_PROTOTYPE( const & ) \
    CAST_OPERATOR_PROTOTYPE( volatile & ) \
    CAST_OPERATOR_PROTOTYPE( const volatile & ) \

#define POSTFIX_UNARY_OPERATOR_FOR_ACCESS( symbol ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, && ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, const && ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, volatile && ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, const volatile && ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, & ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, const & ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, volatile & ) \
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS_PROTOTYPE( symbol, const volatile & ) \

#define PREFIX_UNARY_OPERATOR( symbol, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, &&, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const &&, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, volatile &&, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const volatile &&, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, &, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const &, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, volatile &, invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const volatile &, invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( symbol, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &, invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( symbol, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, volatile &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const volatile &, invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_INT( symbol, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, volatile &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const volatile &, invokable ) \

#define BINARY_OPERATOR_FOR_ANY( symbol, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &&, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &, invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &, invokable ) \

#define BINARY_OPERATOR_FOR_THIS_INSTANCE( symbol, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const volatile &, invokable ) \

#define OPERATORS_DEFINITION \
    /* Subscript */ \
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], ??? ) \
    /* Functional forms */ \
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), ??? ) \
    /* Arithmetic operators */ \
    PREFIX_UNARY_OPERATOR( +, ::Operator::Private::UnaryPrefixPlus ) \
    PREFIX_UNARY_OPERATOR( -, ::Operator::Private::UnaryPrefixMinus ) \
    BINARY_OPERATOR_FOR_ANY( *, ??? ) \
    BINARY_OPERATOR_FOR_ANY( /, ??? ) \
    BINARY_OPERATOR_FOR_ANY( %, ??? ) \
    BINARY_OPERATOR_FOR_ANY( +, ??? ) \
    BINARY_OPERATOR_FOR_ANY( -, ??? ) \
    /* Compound assignment */ \
    BINARY_OPERATOR_FOR_ANY( *=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( /=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( %=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( +=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( -=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( <<=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( >>=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( &=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( ^=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( |=, ??? ) \
    /* Increment and decrement */ \
    PREFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPrefixPlusPlus ) \
    PREFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPrefixMinusMinus ) \
    POSTFIX_UNARY_OPERATOR_WITH_INT( ++, ::Operator::Private::UnaryPostfixPlusPlus ) \
    POSTFIX_UNARY_OPERATOR_WITH_INT( --, ::Operator::Private::UnaryPostfixMinusMinus ) \
    /* Relational and comparison operators */ \
    BINARY_OPERATOR_FOR_ANY( ==, ??? ) \
    BINARY_OPERATOR_FOR_ANY( !=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( <, ??? ) \
    BINARY_OPERATOR_FOR_ANY( <=, ??? ) \
    BINARY_OPERATOR_FOR_ANY( >, ??? ) \
    BINARY_OPERATOR_FOR_ANY( >=, ??? ) \
    /* Logical operators */ \
    PREFIX_UNARY_OPERATOR( !, ::Operator::Private::UnaryPrefixLogicalNot ) \
    BINARY_OPERATOR_FOR_ANY( &&, ??? ) \
    BINARY_OPERATOR_FOR_ANY( ||, ??? ) \
    /* Bitwise operators */ \
    PREFIX_UNARY_OPERATOR( ~, ::Operator::Private::UnaryPrefixBitwiseNot ) \
    BINARY_OPERATOR_FOR_ANY( &, ??? ) \
    BINARY_OPERATOR_FOR_ANY( ^, ??? ) \
    BINARY_OPERATOR_FOR_ANY( |, ??? ) \
    BINARY_OPERATOR_FOR_ANY( <<, ??? ) \
    BINARY_OPERATOR_FOR_ANY( >>, ??? ) \

/* RIGHT-SIDE INSTANCE OPERATORS */

/* Arithmetic operators */
GLOBAL_BINARY_OPERATOR( *, ??? )
GLOBAL_BINARY_OPERATOR( /, ??? )
GLOBAL_BINARY_OPERATOR( %, ??? )
GLOBAL_BINARY_OPERATOR( +, ??? )
GLOBAL_BINARY_OPERATOR( -, ??? )
/* Compound assignment */
GLOBAL_BINARY_OPERATOR( *=, ??? )
GLOBAL_BINARY_OPERATOR( /=, ??? )
GLOBAL_BINARY_OPERATOR( %=, ??? )
GLOBAL_BINARY_OPERATOR( +=, ??? )
GLOBAL_BINARY_OPERATOR( -=, ??? )
GLOBAL_BINARY_OPERATOR( <<=, ??? )
GLOBAL_BINARY_OPERATOR( >>=, ??? )
GLOBAL_BINARY_OPERATOR( &=, ??? )
GLOBAL_BINARY_OPERATOR( ^=, ??? )
GLOBAL_BINARY_OPERATOR( |=, ??? )
/* Relational and comparison operators */
GLOBAL_BINARY_OPERATOR( ==, ??? )
GLOBAL_BINARY_OPERATOR( !=, ??? )
GLOBAL_BINARY_OPERATOR( <, ??? )
GLOBAL_BINARY_OPERATOR( <=, ??? )
GLOBAL_BINARY_OPERATOR( >, ??? )
GLOBAL_BINARY_OPERATOR( >=, ??? )
/* Logical operators */
GLOBAL_BINARY_OPERATOR( &&, ??? )
GLOBAL_BINARY_OPERATOR( ||, ??? )
/* Bitwise operators */
GLOBAL_BINARY_OPERATOR( &, ??? )
GLOBAL_BINARY_OPERATOR( ^, ??? )
GLOBAL_BINARY_OPERATOR( |, ??? )
GLOBAL_BINARY_OPERATOR( <<, ??? )
GLOBAL_BINARY_OPERATOR( >>, ??? )

#undef INSTANCE_PREFIX_UNARY_OPERATOR
#undef INSTANCE_POSTFIX_UNARY_OPERATOR
#undef GLOBAL_BINARY_OPERATOR

#endif
