#pragma once
#ifndef SCL_FEATURING_OPERATOR_H
#define SCL_FEATURING_OPERATOR_H

#include <ModelKit/Featuring/Access/ValuePointer.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "Operator/BinaryOperator.h"
#include "Operator/UnaryOperator.h"

SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( & , AddressOf )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( * , Indirection )
SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( ->*, MemberIndirection )
SCL_BINARY_OPERATOR_IMPLEMENTAION( SCL_SINGLE_ARG( , ), Comma )

SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( [], SquareBrackets )
SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( (), RoundBrackets )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( +, UnaryPrefixPlus )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( -, UnaryPrefixMinus )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPrefixPlusPlus )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPrefixMinusMinus )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( ~, UnaryPrefixBitwiseNot )
SCL_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( !, UnaryPrefixLogicalNot )

SCL_POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( ++, UnaryPostfixPlusPlus )
SCL_POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( --, UnaryPostfixMinusMinus )

SCL_BINARY_OPERATOR_IMPLEMENTAION( ==, IsEqual )
SCL_BINARY_OPERATOR_IMPLEMENTAION( !=, NotEqual )
SCL_BINARY_OPERATOR_IMPLEMENTAION( <, Less )
SCL_BINARY_OPERATOR_IMPLEMENTAION( <=, LessOrEqual )
SCL_BINARY_OPERATOR_IMPLEMENTAION( >, Greater )
SCL_BINARY_OPERATOR_IMPLEMENTAION( >=, GreaterOrEqual )

SCL_BINARY_OPERATOR_IMPLEMENTAION( *, Multiply )
SCL_BINARY_OPERATOR_IMPLEMENTAION( /, Divide )
SCL_BINARY_OPERATOR_IMPLEMENTAION( %, Modulo )
SCL_BINARY_OPERATOR_IMPLEMENTAION( +, Addition )
SCL_BINARY_OPERATOR_IMPLEMENTAION( -, Subtraction )

SCL_BINARY_OPERATOR_IMPLEMENTAION( <<, ShiftLeft )
SCL_BINARY_OPERATOR_IMPLEMENTAION( >>, ShiftRight )

SCL_BINARY_OPERATOR_IMPLEMENTAION( &, BitwiseAnd )
SCL_BINARY_OPERATOR_IMPLEMENTAION( |, BitwiseOr )
SCL_BINARY_OPERATOR_IMPLEMENTAION( ^, BitwiseXor )

SCL_BINARY_OPERATOR_IMPLEMENTAION( &&, LogicalAnd )
SCL_BINARY_OPERATOR_IMPLEMENTAION( ||, LogicalOr )

SCL_BINARY_OPERATOR_IMPLEMENTAION( =, Assignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( *=, MultiplyAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( /=, DivideAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( %=, ModuloAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( +=, AdditionAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( -=, SubtractionAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( <<=, ShiftLeftAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( >>=, ShiftRightAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( &=, BitwiseAndAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( |=, BitwiseOrAssignment )
SCL_BINARY_OPERATOR_IMPLEMENTAION( ^=, BitwiseXorAssignment )

#define SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer, Invokable ) \
    template < typename _Left, typename _RightValue, typename _RightTool, \
        typename = ::std::enable_if_t< !::ScL::is_instance< ::std::decay_t< _Left > > \
            && ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists_test< _Left &&, ::SclPrivate::Instance< _RightValue, _RightTool > right_refer > > > \
    constexpr decltype(auto) operator symbol ( _Left && left, ::SclPrivate::Instance< _RightValue, _RightTool > right_refer right ) \
    { \
        using LeftRefer = _Left &&; \
        using RightRefer = ::SclPrivate::Instance< _RightValue, _RightTool > right_refer; \
        return ::SclPrivate::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
    } \

#define SCL_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Binary \
            { \
                template <> \
                struct Invokable ## Operator< ::SclPrivate::Operator::Global::Invokable ## Case > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftRefer = _Left &&; \
                        using RightRefer = _Right &&; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                    } \
                }; \
                 \
            } \
        } \
    }

#define SCL_GLOBAL_BINARY_OPERATOR( symbol, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer, Invokable ) \
    /*template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists_test< ThisType this_refer, ThisType other_refer > > >*/ \
    constexpr decltype(auto) operator symbol ( ThisType other_refer right ) this_refer \
    { \
        using LeftRefer = ThisType this_refer; \
        using RightRefer = ThisType other_refer; \
        return ::SclPrivate::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( *this ), ::std::forward< RightRefer >( right ) ); \
    } \

#define SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( symbol, this_refer, Invokable ) \
    template < typename _Right, \
        typename = ::std::enable_if_t< ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists_test< ThisType this_refer, _Right && > > > \
    constexpr decltype(auto) operator symbol ( _Right && right ) this_refer \
    { \
        using LeftRefer = ThisType this_refer; \
        using RightRefer = _Right &&; \
        return ::SclPrivate::Operator::Binary::Invokable ## Helper< LeftRefer, RightRefer >::invoke( ::std::forward< LeftRefer >( *this ), ::std::forward< RightRefer >( right ) ); \
    } \

#define SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( symbol, this_refer, Invokable ) \
    template < typename _Argument, \
        typename = ::std::enable_if_t< ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Argument && > > > \
    constexpr decltype(auto) operator symbol ( _Argument && argument ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::SclPrivate::Operator::Unary::Invokable ## Helper< ThisRefer, _Argument && >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Argument && >( argument ) ); \
    } \

#define SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( _Arguments && ... arguments ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::SclPrivate::Operator::Unary::Invokable ## Helper< ThisRefer, _Arguments && ... >::invoke( ::std::forward< ThisRefer >( *this ), ::std::forward< _Arguments && >( arguments ) ... ); \
    } \

#define SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol () this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::SclPrivate::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( symbol, this_refer, Invokable ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > \
            && ::SclPrivate::Operator::Unary::is_ ## Invokable ## _operator_exists_test< Value this_refer, _Arguments && ... > > > \
    constexpr decltype(auto) operator symbol ( int ) this_refer \
    { \
        using ThisRefer = ThisType this_refer; \
        return ::SclPrivate::Operator::Unary::Invokable ## Helper< ThisRefer >::invoke( ::std::forward< ThisRefer >( *this ) ); \
    } \

#define SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > > > \
    constexpr ::ScL::ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ::ScL::ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define SCL_DEREFERENCE_OPERATOR_PROTOTYPE( symbol, this_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< sizeof...( _Arguments ) == 0 \
            && ::SclPrivate::HolderInternal::is_value_method_exists< Holder, Holder this_refer > > > \
    constexpr ::ScL::ValuePointer< ThisType this_refer > operator symbol () this_refer \
    { \
        return ::ScL::ValuePointer< ThisType this_refer >( ::std::forward< ThisType this_refer >( *this ) ); \
    } \

#define SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( other_refer ) \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( ::SclPrivate::InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE_V2( other_refer ) \
    template < typename ... _Arguments, \
        typename = ::std::enable_if_t< ::std::is_constructible< ThisType, ThisType other_refer >::value && \
            sizeof...( _Arguments ) == 0 > > \
    constexpr Instance ( ThisType other_refer other ) \
        : m_holder( ::SclPrivate::InstanceResolver< ThisType, ThisType other_refer >( \
            ::std::forward< ThisType other_refer >( other ) ).resolve() ) \
    { \
    } \

#define SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( other_refer ) \
    template < typename _OtherValue, typename _OtherTool > \
    constexpr Instance ( Instance< _OtherValue, _OtherTool > other_refer other ) \
        : m_holder( ::SclPrivate::InstanceResolver< ThisType, Instance< _OtherValue, _OtherTool > other_refer >( \
            ::std::forward< Instance< _OtherValue, _OtherTool > other_refer >( other ) ).resolve() ) \
    { \
    } \

#define SCL_CAST_OPERATOR_PROTOTYPE( refer_Value ) \
    template < typename _OtherValue, \
        typename = ::std::enable_if_t< \
            ::std::is_same< ::std::decay_t< _OtherValue >, ::std::decay_t< _Value > >::value \
            && ::ScL::is_similar< _OtherValue, _Value > > > \
    operator Instance< _OtherValue, _Tool > refer_Value () refer_Value \
    { \
        return static_cast< Instance< _OtherValue, _Tool > refer_Value >( *this ); \
    } \

#define SCL_CONSTRUCTOR_FOR_THIS_INSTANCE \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( && ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const && ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile && ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile && ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( & ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const & ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( volatile & ) \
    SCL_CONSTRUCTOR_FOR_THIS_INSTANCE_PROTOTYPE( const volatile & ) \

#define SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( && ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const && ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile && ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile && ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( & ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const & ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( volatile & ) \
    SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE_PROTOTYPE( const volatile & ) \

#define SCL_CAST_OPERATOR \
    SCL_CAST_OPERATOR_PROTOTYPE( && ) \
    SCL_CAST_OPERATOR_PROTOTYPE( const && ) \
    SCL_CAST_OPERATOR_PROTOTYPE( volatile && ) \
    SCL_CAST_OPERATOR_PROTOTYPE( const volatile && ) \
    SCL_CAST_OPERATOR_PROTOTYPE( & ) \
    SCL_CAST_OPERATOR_PROTOTYPE( const & ) \
    SCL_CAST_OPERATOR_PROTOTYPE( volatile & ) \
    SCL_CAST_OPERATOR_PROTOTYPE( const volatile & ) \

#define SCL_ADDRESS_OF_OPERATOR( symbol ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    SCL_ADDRESS_OF_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

#define SCL_DEREFERENCE_OPERATOR( symbol ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    SCL_DEREFERENCE_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

#define SCL_PREFIX_UNARY_OPERATOR( symbol, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_PREFIX_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( symbol, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENT( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( symbol, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_ARGUMENTS( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_POSTFIX_UNARY_OPERATOR_WITH_INT( symbol, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_POSTFIX_UNARY_OPERATOR_PROTOTYPE_WITH_INT( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_BINARY_OPERATOR_FOR_ANY( symbol, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_ANY( SCL_SINGLE_ARG( symbol ), const volatile &, Invokable ) \

#define SCL_FRIEND_OPERATOR_ACCESS( Invokable ) \
    template < typename > friend struct ::SclPrivate::Operator::Invokable; \

/*
 * NOTE: Закомментирована реализация из-за проблем со сборкой ::std::map< Instance, Value >
 * Для оператора присвоения '=' константный экземпляр слева является экзотикой,
 * поэтому такое решение может быть хоть как-то оправдано.
 */
#define SCL_ASSIGNMENT_OPERATOR_FOR_THIS( symbol, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const volatile &, Invokable )*/ \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const volatile &, Invokable ) \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const volatile &, Invokable )*/ \
    /*BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const volatile &&, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, volatile &, Invokable ) \
    SCL_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const volatile &, Invokable )*/ \

#endif
