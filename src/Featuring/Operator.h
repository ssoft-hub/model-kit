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

#define UNARY_OPERATOR_IMPLEMENTAION( symbol, name ) \
    template < typename _Type > \
    struct name \
    { \
        static_assert( ::std::is_same< _Type, ::std::decay_t< _Type > >::value, \
            "The template parameter _Type must be decayed." ); \
        template < typename _Refer, typename ... _Arguments > \
        constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... /*arguments*/ ) \
        { \
            static_assert( ::std::is_same< _Type, ::std::decay_t< _Refer > >::value, \
                "The template parameter _Refer must be a refer of template parameter _Type." ); \
            return ::std::forward< _Refer >( refer )/*TODO: . operator symbol ( ::std::forward< _Arguments >( arguments ) ... )*/; \
        } \
    }; \
    \
/*    template < typename _Right > \
    struct name< Instance< _Right > \
    { \
        constexpr decltype(auto) operator () ( _Right && right ) \
        { \
            return symbol ::std::forward< _Right >( right ); \
        } \
    }; */ \

#define UNARY_OPERATOR_INT_IMPLEMENTAION( symbol, name ) \
    template < typename _Type > \
    struct name \
    { \
        static_assert( ::std::is_same< _Type, ::std::decay_t< _Type > >::value, \
            "The template parameter _Type must be decayed." ); \
        template < typename _Refer > \
        constexpr decltype(auto) operator () ( _Refer && refer ) \
        { \
            static_assert( ::std::is_same< _Type, ::std::decay_t< _Refer > >::value, \
                "The template parameter _Refer must be a refer of template parameter _Type." ); \
            return ::std::forward< _Refer >( refer ) /* TODO: symbol*/; \
        } \
    }; \

#define BINARY_OPERATOR_IMPLEMENTAION( symbol, name ) \
    template < typename _Left, typename _Right > \
    struct name \
    { \
        static_assert( ::std::is_same< _Left, ::std::decay_t< _Left > >::value, \
            "The template parameter _Left must be decayed." ); \
        static_assert( ::std::is_same< _Right, ::std::decay_t< _Right > >::value, \
            "The template parameter _Right must be decayed." ); \
        template < typename _LeftRefer, typename _RightRefer > \
        constexpr decltype(auto) operator () ( _LeftRefer && left, _RightRefer && /*right*/ ) \
        { \
            return ::std::forward< _Left >( left )/* TODO: symbol ::std::forward< _Right >( right )*/; \
        } \
    }; \


namespace Operator
{
    namespace Private
    {
        UNARY_OPERATOR_IMPLEMENTAION( [], SquareBrackets )
        UNARY_OPERATOR_IMPLEMENTAION( (), RoundBrackets )
        UNARY_OPERATOR_IMPLEMENTAION( +, UnaryPrefixPlus )
        UNARY_OPERATOR_IMPLEMENTAION( -, UnaryPrefixMinus )
        UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPrefixPlusPlus )
        UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPrefixMinusMinus )
        UNARY_OPERATOR_IMPLEMENTAION( ~, UnaryPrefixBitwiseNot )
        UNARY_OPERATOR_IMPLEMENTAION( !, UnaryPrefixLogicalNot )
        UNARY_OPERATOR_INT_IMPLEMENTAION( ++, UnaryPostfixPlusPlus )
        UNARY_OPERATOR_INT_IMPLEMENTAION( --, UnaryPostfixMinusMinus )
    }
}

namespace Operator
{
    namespace Private
    {
        BINARY_OPERATOR_IMPLEMENTAION( ==, IsEqual )
        BINARY_OPERATOR_IMPLEMENTAION( !=, NotEqual )
        BINARY_OPERATOR_IMPLEMENTAION( <, Less )
        BINARY_OPERATOR_IMPLEMENTAION( <=, LessOrEqual )
        BINARY_OPERATOR_IMPLEMENTAION( >, Greater )
        BINARY_OPERATOR_IMPLEMENTAION( >=, GreaterOrEqual )

        BINARY_OPERATOR_IMPLEMENTAION( *, Multiply )
        BINARY_OPERATOR_IMPLEMENTAION( /, Divide )
        BINARY_OPERATOR_IMPLEMENTAION( %, Modulo )
        BINARY_OPERATOR_IMPLEMENTAION( +, Addition )
        BINARY_OPERATOR_IMPLEMENTAION( -, Subtraction )

        BINARY_OPERATOR_IMPLEMENTAION( <<, ShiftLeft )
        BINARY_OPERATOR_IMPLEMENTAION( >>, ShiftRight )

        BINARY_OPERATOR_IMPLEMENTAION( &, BitwiseAnd )
        BINARY_OPERATOR_IMPLEMENTAION( |, BitwiseOr )
        BINARY_OPERATOR_IMPLEMENTAION( ^, BitwiseXor )

        BINARY_OPERATOR_IMPLEMENTAION( &&, LogicalAnd )
        BINARY_OPERATOR_IMPLEMENTAION( ||, LogicalOr )

        BINARY_OPERATOR_IMPLEMENTAION( =, Assignment )
        BINARY_OPERATOR_IMPLEMENTAION( *=, MultiplyAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( /=, DivideAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( %=, ModuloAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( +=, AdditionAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( -=, SubtractionAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( <<=, ShiftLeftAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( >>=, ShiftRightAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( &=, BitwiseAndAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( |=, BitwiseOrAssignment )
        BINARY_OPERATOR_IMPLEMENTAION( ^=, BitwiseXorAssignment )
    }
}

/*
// Unary prefix operators
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

// Unary postfix operators
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
*/

// Binary operators
#define GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool > \
    constexpr decltype(auto) operator symbol ( _Left && /*left*/, Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        /* TODO: */ int a, b; invokable< int &, int & >( a, b ); \
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
        ::std::forward< Holder this_refer >( m_holder ) symbol InstanceResolver< ThisType, ThisType other_refer >( ::std::forward< ThisType other_refer >( other ) ).resolve(); \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, this_refer, invokable ) \
    template < typename _Other > \
    constexpr decltype(auto) operator symbol ( _Other && other ) this_refer \
    { \
        /* TODO: */ \
        return invokable< ThisType, ::std::decay_t< _Other > >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< _Other && >( other ) ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, invokable ) \
    template < typename _Argument > \
    constexpr decltype(auto) operator symbol ( _Argument && argument ) this_refer \
    { \
        /* TODO: */ \
        return invokable< ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< _Argument >( argument ) ); \
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
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const volatile &&, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, volatile &, invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const volatile &, invokable ) \

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

/* RIGHT-SIDE INSTANCE OPERATORS */

///* Arithmetic operators */
//GLOBAL_BINARY_OPERATOR( *, ::Operator::Private::Multiply )
//GLOBAL_BINARY_OPERATOR( /, ::Operator::Private::Divide )
//GLOBAL_BINARY_OPERATOR( %, ::Operator::Private::Modulo )
//GLOBAL_BINARY_OPERATOR( +, ::Operator::Private::Addition )
//GLOBAL_BINARY_OPERATOR( -, ::Operator::Private::Subtraction )
///* Compound assignment */
//GLOBAL_BINARY_OPERATOR( *=, ??? )
//GLOBAL_BINARY_OPERATOR( /=, ??? )
//GLOBAL_BINARY_OPERATOR( %=, ??? )
//GLOBAL_BINARY_OPERATOR( +=, ??? )
//GLOBAL_BINARY_OPERATOR( -=, ??? )
//GLOBAL_BINARY_OPERATOR( <<=, ??? )
//GLOBAL_BINARY_OPERATOR( >>=, ??? )
//GLOBAL_BINARY_OPERATOR( &=, ??? )
//GLOBAL_BINARY_OPERATOR( ^=, ??? )
//GLOBAL_BINARY_OPERATOR( |=, ??? )
///* Relational and comparison operators */
//GLOBAL_BINARY_OPERATOR( ==, ??? )
//GLOBAL_BINARY_OPERATOR( !=, ??? )
//GLOBAL_BINARY_OPERATOR( <, ??? )
//GLOBAL_BINARY_OPERATOR( <=, ??? )
//GLOBAL_BINARY_OPERATOR( >, ??? )
//GLOBAL_BINARY_OPERATOR( >=, ??? )
///* Logical operators */
//GLOBAL_BINARY_OPERATOR( &&, ??? )
//GLOBAL_BINARY_OPERATOR( ||, ??? )
///* Bitwise operators */
//GLOBAL_BINARY_OPERATOR( &, ??? )
//GLOBAL_BINARY_OPERATOR( ^, ??? )
//GLOBAL_BINARY_OPERATOR( |, ??? )
//GLOBAL_BINARY_OPERATOR( <<, ??? )
//GLOBAL_BINARY_OPERATOR( >>, ??? )

#undef INSTANCE_PREFIX_UNARY_OPERATOR
#undef INSTANCE_POSTFIX_UNARY_OPERATOR
#undef GLOBAL_BINARY_OPERATOR

#endif
