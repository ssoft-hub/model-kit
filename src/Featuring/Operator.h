#pragma once
#ifndef MDK_OPERATOR_BLA_H
#define MDK_OPERATOR_BLA_H

#include <ModelKit/Featuring/Access/ValuePointer.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "Operator/BinaryOperator.h"
#include "Operator/UnaryOperator.h"

MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( & , AddressOf )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( * , Indirection )
MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( ->*, MemberIndirection )
MDK_BINARY_OPERATOR_IMPLEMENTAION( MDK_SINGLE_ARG( , ), Comma )

MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( [], SquareBrackets )
MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( (), RoundBrackets )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( +, UnaryPrefixPlus )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( -, UnaryPrefixMinus )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPrefixPlusPlus )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPrefixMinusMinus )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ~, UnaryPrefixBitwiseNot )
MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( !, UnaryPrefixLogicalNot )

MDK_POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPostfixPlusPlus )
MDK_POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPostfixMinusMinus )

MDK_BINARY_OPERATOR_IMPLEMENTAION( ==, IsEqual )
MDK_BINARY_OPERATOR_IMPLEMENTAION( !=, NotEqual )
MDK_BINARY_OPERATOR_IMPLEMENTAION( <, Less )
MDK_BINARY_OPERATOR_IMPLEMENTAION( <=, LessOrEqual )
MDK_BINARY_OPERATOR_IMPLEMENTAION( >, Greater )
MDK_BINARY_OPERATOR_IMPLEMENTAION( >=, GreaterOrEqual )

MDK_BINARY_OPERATOR_IMPLEMENTAION( *, Multiply )
MDK_BINARY_OPERATOR_IMPLEMENTAION( /, Divide )
MDK_BINARY_OPERATOR_IMPLEMENTAION( %, Modulo )
MDK_BINARY_OPERATOR_IMPLEMENTAION( +, Addition )
MDK_BINARY_OPERATOR_IMPLEMENTAION( -, Subtraction )

MDK_BINARY_OPERATOR_IMPLEMENTAION( <<, ShiftLeft )
MDK_BINARY_OPERATOR_IMPLEMENTAION( >>, ShiftRight )

MDK_BINARY_OPERATOR_IMPLEMENTAION( &, BitwiseAnd )
MDK_BINARY_OPERATOR_IMPLEMENTAION( |, BitwiseOr )
MDK_BINARY_OPERATOR_IMPLEMENTAION( ^, BitwiseXor )

MDK_BINARY_OPERATOR_IMPLEMENTAION( &&, LogicalAnd )
MDK_BINARY_OPERATOR_IMPLEMENTAION( ||, LogicalOr )

MDK_BINARY_OPERATOR_IMPLEMENTAION( =, Assignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( *=, MultiplyAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( /=, DivideAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( %=, ModuloAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( +=, AdditionAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( -=, SubtractionAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( <<=, ShiftLeftAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( >>=, ShiftRightAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( &=, BitwiseAndAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( |=, BitwiseOrAssignment )
MDK_BINARY_OPERATOR_IMPLEMENTAION( ^=, BitwiseXorAssignment )

#define MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, Invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool, \
        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _Left > > \
            && ::Operator::Binary::is_ ## Invokable ## _operator_exists_test< _Left &&, Instance< _RightValue, _RightTool > right_refer > > > \
    constexpr decltype(auto) operator symbol ( _Left && left, Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        using LeftRefer = _Left &&; \
        using RightRefer = Instance< _RightValue, _RightTool > right_refer; \
        return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
    } \

#define MDK_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
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

#define MDK_GLOBAL_BINARY_OPERATOR( symbol, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_GLOBAL_BINARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer, Invokable ) \
    /*template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Binary::is_ ## Invokable ## _operator_exists_test< ThisType this_refer, ThisType other_refer > > >*/ \
    constexpr decltype(auto) operator symbol ( ThisType other_refer right ) this_refer \
    { \
        using LeftRefer = ThisType this_refer; \
        using RightRefer = ThisType other_refer; \
        return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( *this ), ::std::forward< RightRefer >( right ) ); \
    } \

#define MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, this_refer, Invokable ) \
    template < typename _Right, \
        typename = ::std::enable_if_t< ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Binary::is_ ## Invokable ## _operator_exists_test< ThisType this_refer, _Right && > > > \
    constexpr decltype(auto) operator symbol ( _Right && right ) this_refer \
    { \
        using LeftRefer = ThisType this_refer; \
        using RightRefer = _Right &&; \
        return ::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( *this ), ::std::forward< RightRefer >( right ) ); \
    } \

#define MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, Invokable ) \
    template < typename _Argument, \
        typename = ::std::enable_if_t< ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Argument && > > > \
    constexpr decltype(auto) operator symbol ( _Argument && argument ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer, _Argument && >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Argument && >( argument ) ); \
    } \

#define MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( _Arguments && ... arguments ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer, _Arguments && ... >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Arguments && >( arguments ) ... ); \
    } \

#define MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol () this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( int ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > > > \
    constexpr ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define MDK_DEREFERENCE_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::HolderInternal::is_value_method_exists< Holder, Holder this_refer > > > \
    constexpr ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( other_refer ) \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE_V2( other_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::std::is_constructible< ThisType, ThisType other_refer >::value && \
            sizeof...( _Arguments ) == 0 > > \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( other_refer ) \
    template < typename _OtherValue, typename _OtherTool > \
    constexpr Instance ( Instance< _OtherValue, _OtherTool > other_refer other ) \
        : m_holder( InstanceResolver< ThisType, Instance< _OtherValue, _OtherTool > other_refer >( \
            ::std::forward< Instance< _OtherValue, _OtherTool > other_refer >( other ) ).resolve() ) \
    { \
    } \

#define MDK_CAST_OPERATOR_PROTOTYPE( refer_Value ) \
    template < typename _OtherValue, \
        typename = ::std::enable_if_t< \
            ::std::is_same< ::std::decay_t< _OtherValue >, ::std::decay_t< _Value > >::value \
            && ::is_similar< _OtherValue, _Value > > > \
    operator Instance< _OtherValue, _Tool > refer_Value () refer_Value \
    { \
        return static_cast< Instance< _OtherValue, _Tool > refer_Value >( *this ); \
    } \

#define MDK_CONSTRUCTOR_FOR_THIS_INSTANCE \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( && ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const && ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile && ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile && ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( & ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const & ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile & ) \
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile & ) \

#define MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( && ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const && ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile && ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile && ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( & ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const & ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile & ) \
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile & ) \

#define MDK_CAST_OPERATOR \
    MDK_CAST_OPERATOR_PROTOTYPE( && ) \
    MDK_CAST_OPERATOR_PROTOTYPE( const && ) \
    MDK_CAST_OPERATOR_PROTOTYPE( volatile && ) \
    MDK_CAST_OPERATOR_PROTOTYPE( const volatile && ) \
    MDK_CAST_OPERATOR_PROTOTYPE( & ) \
    MDK_CAST_OPERATOR_PROTOTYPE( const & ) \
    MDK_CAST_OPERATOR_PROTOTYPE( volatile & ) \
    MDK_CAST_OPERATOR_PROTOTYPE( const volatile & ) \

#define MDK_ADDRESS_OF_OPERATOR( symbol ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), && ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const && ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile && ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile && ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), & ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const & ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile & ) \
    MDK_ADDRESS_OF_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile & ) \

#define MDK_DEREFERENCE_OPERATOR( symbol ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), && ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const && ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile && ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile && ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), & ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const & ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile & ) \
    MDK_DEREFERENCE_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile & ) \

#define MDK_PREFIX_UNARY_OPERATOR( symbol, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_PREFIX_UNARY_OPERATOR_PROTOTYPE( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( symbol, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( symbol, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_POSTFIX_UNARY_OPERATOR_WITH_INT( symbol, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_BINARY_OPERATOR_FOR_ANY( symbol, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( MDK_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define MDK_FRIEND_OPERATOR_ACCESS( Invokable ) \
    template < typename > friend struct ::Operator::Invokable; \

/*
 * NOTE: Закомментирована реализация из-за проблем со сборкой ::std::map< Instance, Value >
 * Для оператора присвоения '=' константный экземпляр слева является экзотикой,
 * поэтому такое решение может быть хоть как-то оправдано.
 */
#define MDK_ASSIGNMENT_OPERATOR_FOR_THIS( symbol, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &&, const volatile &, Invokable )*/ \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), &, const volatile &, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), volatile &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, const &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, const volatile &&, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, const &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, volatile &, Invokable ) \
    MDK_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( MDK_SINGLE_ARG( symbol ), const volatile &, const volatile &, Invokable )*/ \

#endif
