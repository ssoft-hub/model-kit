#pragma once
#ifndef OPERATOR_BLA_H
#define OPERATOR_BLA_H

#include <ModelKit/Featuring/Access/ValuePointer.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "Operator/BinaryOperator.h"
#include "Operator/UnaryOperator.h"

PREFIX_UNARY_OPERATOR_IMPLEMENTAION( & , AddressOf )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( * , Indirection )
POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( ->*, MemberIndirection )
BINARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( , ), Comma )

POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( [], SquareBrackets )
POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( (), RoundBrackets )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( +, UnaryPrefixPlus )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( -, UnaryPrefixMinus )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPrefixPlusPlus )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPrefixMinusMinus )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ~, UnaryPrefixBitwiseNot )
PREFIX_UNARY_OPERATOR_IMPLEMENTAION( !, UnaryPrefixLogicalNot )

POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPostfixPlusPlus )
POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPostfixMinusMinus )

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

#define GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, Invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool, \
        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _Left > > \
            && ::Operator::Binary::is_ ## Invokable ## _operator_exists_test< _Left &&, Instance< _RightValue, _RightTool > right_refer > > > \
    constexpr decltype(auto) operator symbol ( _Left && left, Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        using LeftRefer = _Left &&; \
        using RightRefer = Instance< _RightValue, _RightTool > right_refer; \
        return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
    } \

#define GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
    namespace Operator \
    { \
        namespace Binary \
        { \
            template <> \
            struct Invokable ## Operator< ::Operator::Global::Invokable ## Case > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightRefer = _Right &&; \
                    return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
             \
        } \
    } \

#define GLOBAL_BINARY_OPERATOR( symbol, Invokable ) \
    GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    GLOBAL_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer, Invokable ) \
    /*constexpr*/ decltype(auto) operator symbol ( ThisType other_refer /*other*/ ) this_refer \
    { \
        /* TODO: */ \
        /*::std::forward< Holder this_refer >( m_holder ) symbol InstanceResolver< ThisType, ThisType other_refer >( ::std::forward< ThisType other_refer >( other ) ).resolve();*/ \
        /*return ::Operator::Invokable< ThisType, ThisType >()( \
            ::std::forward< ThisType this_refer >( *this ), \
            ::std::forward< ThisType other_refer >( other ) );*/ \
    } \

#define BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, this_refer, Invokable ) \
    template < typename _Right, \
        typename = ::std::enable_if_t< ::Operator::Binary::is_ ## Invokable ## _operator_exists_test< ThisType this_refer, _Right && > > > \
    constexpr decltype(auto) operator symbol ( _Right && right ) this_refer \
    { \
        using LeftRefer = ThisType this_refer; \
        using RightRefer = _Right &&; \
        return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( *this ), ::std::forward< RightRefer >( right ) ); \
    } \

#define POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, Invokable ) \
    template < typename _Argument, \
        typename = ::std::enable_if_t< ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Argument && > > > \
    constexpr decltype(auto) operator symbol ( _Argument && argument ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer, _Argument && >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Argument && >( argument ) ); \
    } \

#define POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( _Arguments && ... arguments ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer, _Arguments && ... >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Arguments && >( arguments ) ... ); \
    } \

#define PREFIX_UNARY_OPERATOR_PROTOTYPE( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol () this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( int ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define ADDRESS_OF_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    constexpr ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define DEREFERENCE_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    constexpr ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( other_refer ) \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE_V2( other_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::std::is_constructible< ThisType, ThisType other_refer >::value && \
            sizeof...( _Arguments ) == 0 > > \
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

#define ADDRESS_OF_OPERATOR( symbol ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    ADDRESS_OF_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

#define DEREFERENCE_OPERATOR( symbol ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    DEREFERENCE_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

#define PREFIX_UNARY_OPERATOR( symbol, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), &&, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const &&, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), &, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const &, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    PREFIX_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( symbol, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), const &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), const &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( symbol, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), const &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), const &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define POSTFIX_UNARY_OPERATOR_WITH_INT( symbol, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), const &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), const &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define BINARY_OPERATOR_FOR_ANY( symbol, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define FRIEND_OPERATOR_ACCESS( Invokable ) \
    template < typename > friend struct ::Operator::Invokable; \

/*
 * NOTE: Закомментирована реализация из-за проблем со сборкой ::std::map< Instance, Value >
 * Для оператора присвоения '=' константный экземпляр слева является экзотикой,
 * поэтому такое решение может быть хоть как-то оправдано.
 */
#define ASSIGNMENT_OPERATOR_FOR_THIS( symbol, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const volatile &, Invokable )*/ \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const volatile &, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const volatile &&, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, volatile &, Invokable ) \
    BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const volatile &, Invokable )*/ \

#endif
