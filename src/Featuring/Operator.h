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
                using Tool = ::_Invokable::FunctionTool< _InstanceRefer, _Invokable, _Arguments ... >;
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

#define UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator { template < typename > struct Invokable; } \
    namespace Operator { namespace Spec { template < typename > struct Invokable; } } \
    \
    namespace Operator \
    { \
        namespace Spec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Value, template < typename > class _Holder > \
            struct Invokable< _Holder< _Value > > \
            { \
                using Holder = _Holder< _Value >; \
                using Value = _Value; \
                template < typename _Refer, typename ... _Arguments > \
                constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
                { \
                    using HolderRefer= ::std::add_rvalue_reference_t< _Refer >; \
                    static_assert( ::std::is_same< Holder, ::std::decay_t< HolderRefer > >::value, \
                        "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >::Holder." ); \
                    using ValueRefer = ::SimilarRefer< Value, HolderRefer >; \
                    return ::Operator::Invokable< Value >()( ::std::forward< ValueRefer >( Holder::value( ::std::forward< HolderRefer >( refer ) ) ), ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Type = _Value; \
            static_assert( ::std::is_same< Type, ::std::decay_t< Type > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer, typename ... _Arguments > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using Refer= ::std::add_rvalue_reference_t< _Refer >; \
                static_assert( ::std::is_same< Type, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ). operator symbol ( ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        }; \
    } \
    \
    namespace Operator \
    { \
        template < typename _Value, typename _Tool > \
        struct Invokable< ::Instance< _Value, _Tool > > \
        { \
            using Instance = ::Instance< _Value, _Tool >; \
            using Holder = typename Instance::Holder; \
            template < typename _Refer, typename ... _Arguments > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using InstanceRefer= ::std::add_rvalue_reference_t< _Refer >; \
                static_assert( ::std::is_same< Instance, ::std::decay_t< InstanceRefer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >." ); \
                using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::Spec::Invokable< Holder >()( ::std::forward< HolderRefer >( refer.m_holder ), ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        };\
    } \

#define UNARY_OPERATOR_INT_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator { template < typename > struct Invokable; } \
    namespace Operator { namespace Spec { template < typename > struct Invokable; } } \
    \
    namespace Operator \
    { \
        namespace Spec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Value, template < typename > class _Holder > \
            struct Invokable< _Holder< _Value > > \
            { \
                using Holder = _Holder< _Value >; \
                using Value = _Value; \
                template < typename _Refer, typename ... _Arguments > \
                constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
                { \
                    using HolderRefer= ::std::add_rvalue_reference_t< _Refer >; \
                    static_assert( ::std::is_same< Holder, ::std::decay_t< HolderRefer > >::value, \
                        "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >::Holder." ); \
                    using ValueRefer = ::SimilarRefer< Value, HolderRefer >; \
                    return ::Operator::Invokable< Value >()( ::std::forward< ValueRefer >( Holder::value( ::std::forward< HolderRefer >( refer ) ) ), ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Type = _Value; \
            static_assert( ::std::is_same< Type, ::std::decay_t< Type > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer, typename ... _Arguments > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using Refer= ::std::add_rvalue_reference_t< _Refer >; \
                static_assert( ::std::is_same< Type, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ) symbol; \
            } \
        }; \
    } \
    \
    namespace Operator \
    { \
        template < typename _Value, typename _Tool > \
        struct Invokable< ::Instance< _Value, _Tool > > \
        { \
            using Instance = ::Instance< _Value, _Tool >; \
            using Holder = typename Instance::Holder; \
            template < typename _Refer, typename ... _Arguments > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using InstanceRefer= ::std::add_rvalue_reference_t< _Refer >; \
                static_assert( ::std::is_same< Instance, ::std::decay_t< InstanceRefer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >." ); \
                using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::Spec::Invokable< Holder >()( ::std::forward< HolderRefer >( refer.m_holder ), ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        };\
    } \

#define BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        template < typename _Left, typename _Right > \
        struct Invokable \
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
    } \


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

/*
// Unary prefix operators
#define INSTANCE_PREFIX_UNARY_OPERATOR( symbol, Invokable ) \
    template < typename _Right, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Right > > > > \
    inline decltype(auto) operator symbol ( _Right && value ) \
    { \
        using RightInstance = ::std::decay_t< _Right >; \
        using RightInstanceRefer = ::std::add_rvalue_reference_t< _Right >; \
        using RightValueRefer = ::SimilarRefer< typename RightInstance::Value, RightInstanceRefer >; \
        return Operator::invoke< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( value ), Invokable< RightValueRefer >() ); \
    } \

// Unary postfix operators
#define INSTANCE_POSTFIX_UNARY_OPERATOR( symbol, Invokable ) \
    template < typename _Left, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Left > > > > \
    inline decltype(auto) operator symbol ( _Left && value, int ) \
    { \
        using LeftInstance = ::std::decay_t< _Left >; \
        using LeftInstanceRefer = ::std::add_rvalue_reference_t< _Left >; \
        using LeftValueRefer = ::SimilarRefer< typename LeftInstance::Value, LeftInstanceRefer >; \
        return Operator::invoke< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( value ), Invokable< LeftValueRefer >() ); \
    } \
*/

// Binary operators
#define GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, Invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool > \
    constexpr decltype(auto) operator symbol ( _Left && /*left*/, Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        /* TODO: */ \
        return ::std::forward< Instance< _RightValue, _RightTool > right_refer >( right ); \
    } \

#define GLOBAL_BINARY_OPERATOR( symbol, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, volatile &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const volatile &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, volatile &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, const volatile &, Invokable ) \

#define BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer, Invokable ) \
    constexpr decltype(auto) operator symbol ( ThisType other_refer other ) this_refer \
    { \
        /* TODO: */ \
        ::std::forward< Holder this_refer >( m_holder ) symbol InstanceResolver< ThisType, ThisType other_refer >( ::std::forward< ThisType other_refer >( other ) ).resolve(); \
        return ::std::forward< ThisType this_refer >( *this ); \
    } \

#define BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, this_refer, Invokable ) \
    template < typename _Other > \
    constexpr decltype(auto) operator symbol ( _Other && other ) this_refer \
    { \
        /* TODO: */ \
        return Invokable< ThisType, ::std::decay_t< _Other > >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< _Other && >( other ) ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, Invokable ) \
    template < typename _Argument > \
    constexpr decltype(auto) operator symbol ( _Argument && argument ) this_refer \
    { \
        return Invokable< ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< _Argument >( argument ) ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments > \
    constexpr decltype(auto) operator symbol ( _Arguments && ... arguments ) this_refer \
    { \
        return Invokable< ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< _Arguments >( arguments ) ... ); \
    } \

#define PREFIX_OPERATOR_PROTOTYPE( symbol, this_refer, Invokable ) \
    constexpr decltype(auto) operator symbol () this_refer \
    { \
        return Invokable< ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, this_refer, Invokable ) \
    constexpr decltype(auto) operator symbol ( int ) this_refer \
    { \
        return Invokable< ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ) ); \
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

#define CAST_OPERATOR_PROTOTYPE( refer_Value ) \
    template < typename _OtherValue, \
        typename = ::std::enable_if_t< \
            ::std::is_same< ::std::decay_t< _OtherValue >, ::std::decay_t< _Value > >::value \
            && ::is_similar< _OtherValue, _Value > > > \
    operator Instance< _OtherValue, _Tool > refer_Value () refer_Value \
    { \
        return static_cast< Instance< _OtherValue, _Tool > refer_Value >( *this ); \
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

#define FRIEND_OPERATOR_ACCESS( Invokable ) \
    template < typename > friend struct Invokable; \

#define PREFIX_UNARY_OPERATOR( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, &&, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const &&, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, volatile &&, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const volatile &&, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, &, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const &, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, volatile &, Invokable ) \
    PREFIX_OPERATOR_PROTOTYPE( symbol, const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, volatile &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, volatile &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_INT( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const volatile &&, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, volatile &, Invokable ) \
    POSTFIX_OPERATOR_PROTOTYPE_WITH_INT( symbol, const volatile &, Invokable ) \

#define BINARY_OPERATOR_FOR_ANY( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, const volatile &, Invokable ) \

#define BINARY_OPERATOR_FOR_THIS_INSTANCE( symbol, Invokable ) \
    FRIEND_OPERATOR_ACCESS( Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &&, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &&, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &&, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &&, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, &, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const &, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, volatile &, const volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, const volatile &, const volatile &, Invokable ) \

/* RIGHT-SIDE INSTANCE OPERATORS */

/* Arithmetic operators */
GLOBAL_BINARY_OPERATOR( *, ::Operator::Multiply )
GLOBAL_BINARY_OPERATOR( /, ::Operator::Divide )
GLOBAL_BINARY_OPERATOR( %, ::Operator::Modulo )
GLOBAL_BINARY_OPERATOR( +, ::Operator::Addition )
GLOBAL_BINARY_OPERATOR( -, ::Operator::Subtraction )
/* Compound assignment */
GLOBAL_BINARY_OPERATOR( *=, ::Operator::MultiplyAssignment )
GLOBAL_BINARY_OPERATOR( /=, ::Operator::DivideAssignment )
GLOBAL_BINARY_OPERATOR( %=, ::Operator::ModuloAssignment )
GLOBAL_BINARY_OPERATOR( +=, ::Operator::AdditionAssignment )
GLOBAL_BINARY_OPERATOR( -=, ::Operator::SubtractionAssignment )
GLOBAL_BINARY_OPERATOR( <<=, ::Operator::ShiftLeftAssignment )
GLOBAL_BINARY_OPERATOR( >>=, ::Operator::ShiftRightAssignment )
GLOBAL_BINARY_OPERATOR( &=, ::Operator::BitwiseAndAssignment )
GLOBAL_BINARY_OPERATOR( ^=, ::Operator::BitwiseXorAssignment )
GLOBAL_BINARY_OPERATOR( |=, ::Operator::BitwiseOrAssignment )
/* Relational and comparison operators */
GLOBAL_BINARY_OPERATOR( ==, ::Operator::IsEqual )
GLOBAL_BINARY_OPERATOR( !=, ::Operator::NotEqual )
GLOBAL_BINARY_OPERATOR( <, ::Operator::Less )
GLOBAL_BINARY_OPERATOR( <=, ::Operator::LessOrEqual )
GLOBAL_BINARY_OPERATOR( >, ::Operator::Greater )
GLOBAL_BINARY_OPERATOR( >=, ::Operator::GreaterOrEqual )
/* Logical operators */
GLOBAL_BINARY_OPERATOR( &&, ::Operator::LogicalAnd )
GLOBAL_BINARY_OPERATOR( ||, ::Operator::LogicalOr )
/* Bitwise operators */
GLOBAL_BINARY_OPERATOR( &, ::Operator::BitwiseAnd )
GLOBAL_BINARY_OPERATOR( ^, ::Operator::BitwiseXor )
GLOBAL_BINARY_OPERATOR( |, ::Operator::BitwiseOr )
GLOBAL_BINARY_OPERATOR( <<, ::Operator::ShiftLeft )
GLOBAL_BINARY_OPERATOR( >>, ::Operator::ShiftRight )

#undef INSTANCE_PREFIX_UNARY_OPERATOR
#undef INSTANCE_POSTFIX_UNARY_OPERATOR
#undef GLOBAL_BINARY_OPERATOR

#endif
