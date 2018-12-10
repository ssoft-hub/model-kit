#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>
#include <ModelKit/Featuring/Traits.h>

namespace Inplace { struct DefaultTool; }

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
class Instance
{
    template < typename >
    friend class Private::SpecialValueGuard;

    template < typename >
    friend class Private::SpecialInstanceGuard;

    template < typename, typename >
    friend class Private::InstanceCompatibleResolver;

    using ThisType = Instance< _Value, _Tool >;

public:
    using Value = _Value;
    using Tool = _Tool;
    using Holder = typename Tool:: template Holder< _Value >;

private:
    Holder m_holder;

public:
    /// Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
        : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {}

    /* All kind of constructors for ThisType */
    CONSTRUCTOR_FOR_THIS_INSTANCE
    /* All kind of constructors for Instance< _OtherValue, _OtherTool > */
    CONSTRUCTOR_FOR_OTHER_INSTANCE

    /* All kind of assignment operators for any type (including Instance< _OtherValue, _OtherTool >) */
    BINARY_OPERATOR_FOR_ANY( =, ??? )
    /* All kind of assignment operators for ThisType */
    BINARY_OPERATOR_FOR_THIS_INSTANCE( =, ??? )

    /* Member access */
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++,
    // а для оператора "->" использовать реализацию подобно другим.
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( -> )
    /* Reference */
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( & )
    /* Subscript */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], ??? )
    /* Functional forms */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), ??? )
    /* Arithmetic operators */
    PREFIX_UNARY_OPERATOR( +, ::Operator::Private::UnaryPrefixPlus )
    PREFIX_UNARY_OPERATOR( -, ::Operator::Private::UnaryPrefixMinus )
    BINARY_OPERATOR_FOR_ANY( *, ??? )
    BINARY_OPERATOR_FOR_ANY( /, ??? )
    BINARY_OPERATOR_FOR_ANY( %, ??? )
    BINARY_OPERATOR_FOR_ANY( +, ??? )
    BINARY_OPERATOR_FOR_ANY( -, ??? )
    /* Compound assignment */
    BINARY_OPERATOR_FOR_ANY( *=, ??? )
    BINARY_OPERATOR_FOR_ANY( /=, ??? )
    BINARY_OPERATOR_FOR_ANY( %=, ??? )
    BINARY_OPERATOR_FOR_ANY( +=, ??? )
    BINARY_OPERATOR_FOR_ANY( -=, ??? )
    BINARY_OPERATOR_FOR_ANY( <<=, ??? )
    BINARY_OPERATOR_FOR_ANY( >>=, ??? )
    BINARY_OPERATOR_FOR_ANY( &=, ??? )
    BINARY_OPERATOR_FOR_ANY( ^=, ??? )
    BINARY_OPERATOR_FOR_ANY( |=, ??? )
    /* Increment and decrement */
    PREFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPrefixPlusPlus )
    PREFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPrefixMinusMinus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( ++, ::Operator::Private::UnaryPostfixPlusPlus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( --, ::Operator::Private::UnaryPostfixMinusMinus )
    /* Relational and comparison operators */
    BINARY_OPERATOR_FOR_ANY( ==, ??? )
    BINARY_OPERATOR_FOR_ANY( !=, ??? )
    BINARY_OPERATOR_FOR_ANY( <, ??? )
    BINARY_OPERATOR_FOR_ANY( <=, ??? )
    BINARY_OPERATOR_FOR_ANY( >, ??? )
    BINARY_OPERATOR_FOR_ANY( >=, ??? )
    /* Logical operators */
    PREFIX_UNARY_OPERATOR( !, ::Operator::Private::UnaryPrefixLogicalNot )
    BINARY_OPERATOR_FOR_ANY( &&, ??? )
    BINARY_OPERATOR_FOR_ANY( ||, ??? )
    /* Bitwise operators */
    PREFIX_UNARY_OPERATOR( ~, ::Operator::Private::UnaryPrefixBitwiseNot )
    BINARY_OPERATOR_FOR_ANY( &, ??? )
    BINARY_OPERATOR_FOR_ANY( ^, ??? )
    BINARY_OPERATOR_FOR_ANY( |, ??? )
    BINARY_OPERATOR_FOR_ANY( <<, ??? )
    BINARY_OPERATOR_FOR_ANY( >>, ??? )
    /* Type-casting */
    CAST_OPERATOR

//    /// Проксирование оператора []
//    template< typename _Argument >
//    constexpr decltype(auto) operator [] ( _Argument && argument ) &&
//    {
//        auto invokable = ::Operator::Private::SquareBrackets< Value &&, _Argument && >();
//        return Operator::invoke< ThisType && >( ::std::forward< ThisType >( *this ), invokable, ::std::forward< _Argument >( argument ) );
//    }
};

#endif
