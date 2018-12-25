#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>
#include <ModelKit/Featuring/Traits.h>

#include "Operator/UnaryOperator.h"

namespace Inplace { struct DefaultTool; }
template < typename > struct InstanceAccess;

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
class Instance
{
    static_assert( !::std::is_reference< _Value >::value,
        "The template parameter _Value must to be a non reference type." );
    static_assert( !::std::is_reference< _Tool >::value,
        "The template parameter _Tool must to be a non reference type." );

    template < typename >
    friend struct InstanceAccess;

    template < typename >
    friend class Private::SpecialValueGuard; // TODO: To remove. To use InstanceAccess instead.

    template < typename >
    friend class Private::SpecialInstanceGuard; // TODO: To remove. To use InstanceAccess instead.

    template < typename, typename >
    friend class Private::InstanceCompatibleResolver; // TODO: To remove. To use InstanceAccess instead.

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
    CONSTRUCTOR_FOR_OTHER_INSTANCE // TODO: to remove

    /* All kind of assignment operators for any type (including Instance< _OtherValue, _OtherTool >) */
    BINARY_OPERATOR_FOR_ANY( =, Assignment )
    /* All kind of assignment operators for ThisType */
    ASSIGNMENT_OPERATOR_FOR_THIS( =, Assignment )

    /* Member access */
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++,
    // а для оператора "->" использовать реализацию подобно другим.
    DEREFERENCE_OPERATOR( -> )
    ADDRESS_OF_OPERATOR( & )
    //PREFIX_UNARY_OPERATOR( &, AddressOf )
    //PREFIX_UNARY_OPERATOR( *, Indirection )
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( ->*, MemberIndirection )
    BINARY_OPERATOR_FOR_ANY( SINGLE_ARG( , ), Comma )
    /* Subscript */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], SquareBrackets )

    /* Functional forms */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), RoundBrackets )
    /* Arithmetic operators */
    PREFIX_UNARY_OPERATOR( +, UnaryPrefixPlus )
    PREFIX_UNARY_OPERATOR( -, UnaryPrefixMinus )
    BINARY_OPERATOR_FOR_ANY( *, Multiply )
    BINARY_OPERATOR_FOR_ANY( /, Divide )
    BINARY_OPERATOR_FOR_ANY( %, Modulo )
    BINARY_OPERATOR_FOR_ANY( +, Addition )
    BINARY_OPERATOR_FOR_ANY( -, Subtraction )
    /* Compound assignment */
    BINARY_OPERATOR_FOR_ANY( *=, MultiplyAssignment )
    BINARY_OPERATOR_FOR_ANY( /=, DivideAssignment )
    BINARY_OPERATOR_FOR_ANY( %=, ModuloAssignment )
    BINARY_OPERATOR_FOR_ANY( +=, AdditionAssignment )
    BINARY_OPERATOR_FOR_ANY( -=, SubtractionAssignment )
    BINARY_OPERATOR_FOR_ANY( <<=, ShiftLeftAssignment )
    BINARY_OPERATOR_FOR_ANY( >>=, ShiftRightAssignment )
    BINARY_OPERATOR_FOR_ANY( &=, BitwiseAndAssignment )
    BINARY_OPERATOR_FOR_ANY( ^=, BitwiseXorAssignment )
    BINARY_OPERATOR_FOR_ANY( |=, BitwiseOrAssignment )
    /* Increment and decrement */
    PREFIX_UNARY_OPERATOR( ++, UnaryPrefixPlusPlus )
    PREFIX_UNARY_OPERATOR( --, UnaryPrefixMinusMinus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( ++, UnaryPostfixPlusPlus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( --, UnaryPostfixMinusMinus )
    /* Relational and comparison operators */
    BINARY_OPERATOR_FOR_ANY( ==, IsEqual )
    BINARY_OPERATOR_FOR_ANY( !=, NotEqual )
    BINARY_OPERATOR_FOR_ANY( <, Less )
    BINARY_OPERATOR_FOR_ANY( <=, LessOrEqual )
    BINARY_OPERATOR_FOR_ANY( >, Greater )
    BINARY_OPERATOR_FOR_ANY( >=, GreaterOrEqual )
    /* Logical operators */
    PREFIX_UNARY_OPERATOR( !, UnaryPrefixLogicalNot )
    BINARY_OPERATOR_FOR_ANY( &&, LogicalAnd )
    BINARY_OPERATOR_FOR_ANY( ||, LogicalOr )
    /* Bitwise operators */
    PREFIX_UNARY_OPERATOR( ~, UnaryPrefixBitwiseNot )
    BINARY_OPERATOR_FOR_ANY( &, BitwiseAnd )
    BINARY_OPERATOR_FOR_ANY( ^, BitwiseXor )
    BINARY_OPERATOR_FOR_ANY( |, BitwiseOr )
    BINARY_OPERATOR_FOR_ANY( <<, ShiftLeft )
    BINARY_OPERATOR_FOR_ANY( >>, ShiftRight )
    /* Type-casting */
    CAST_OPERATOR
};

/* RIGHT-SIDE INSTANCE OPERATORS */

/*! NOTE: The 'operator =' cannot be implemented as global method. */

/* Arithmetic operators */
//GLOBAL_BINARY_OPERATOR( *, Multiply )
//GLOBAL_BINARY_OPERATOR( /, Divide )
//GLOBAL_BINARY_OPERATOR( %, Modulo )
//GLOBAL_BINARY_OPERATOR( +, Addition )
//GLOBAL_BINARY_OPERATOR( -, Subtraction )
///* Compound assignment */
//GLOBAL_BINARY_OPERATOR( *=, MultiplyAssignment )
//GLOBAL_BINARY_OPERATOR( /=, DivideAssignment )
//GLOBAL_BINARY_OPERATOR( %=, ModuloAssignment )
//GLOBAL_BINARY_OPERATOR( +=, AdditionAssignment )
//GLOBAL_BINARY_OPERATOR( -=, SubtractionAssignment )
//GLOBAL_BINARY_OPERATOR( <<=, ShiftLeftAssignment )
//GLOBAL_BINARY_OPERATOR( >>=, ShiftRightAssignment )
//GLOBAL_BINARY_OPERATOR( &=, BitwiseAndAssignment )
//GLOBAL_BINARY_OPERATOR( ^=, BitwiseXorAssignment )
//GLOBAL_BINARY_OPERATOR( |=, BitwiseOrAssignment )
///* Relational and comparison operators */
////GLOBAL_BINARY_OPERATOR( ==, IsEqual )
//GLOBAL_BINARY_OPERATOR( !=, NotEqual )
//GLOBAL_BINARY_OPERATOR( <, Less )
//GLOBAL_BINARY_OPERATOR( <=, LessOrEqual )
//GLOBAL_BINARY_OPERATOR( >, Greater )
//GLOBAL_BINARY_OPERATOR( >=, GreaterOrEqual )
///* Logical operators */
//GLOBAL_BINARY_OPERATOR( &&, LogicalAnd )
//GLOBAL_BINARY_OPERATOR( ||, LogicalOr )
///* Bitwise operators */
//GLOBAL_BINARY_OPERATOR( &, BitwiseAnd )
//GLOBAL_BINARY_OPERATOR( ^, BitwiseXor )
//GLOBAL_BINARY_OPERATOR( |, BitwiseOr )
//GLOBAL_BINARY_OPERATOR( <<, ShiftLeft )
//GLOBAL_BINARY_OPERATOR( >>, ShiftRight )

#endif
