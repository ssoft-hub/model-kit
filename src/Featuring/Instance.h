#pragma once
#ifndef MDK_INSTANCE_FEATURED_H
#define MDK_INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>
#include <ModelKit/Featuring/Traits.h>

#include "Operator/UnaryOperator.h"
#include "Operator/BinaryOperator.h"

namespace Inplace { struct DefaultTool; }
template < typename > struct InstanceAccess;

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
class Instance
{
    //static_assert( !::std::is_reference< _Value >::value,
    //    "The template parameter _Value must to be not a reference type." );
    static_assert( !::std::is_reference< _Tool >::value,
        "The template parameter _Tool must to be not a reference type." );

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
    MDK_CONSTRUCTOR_FOR_THIS_INSTANCE
    /* All kind of constructors for Instance< _OtherValue, _OtherTool > */
    MDK_CONSTRUCTOR_FOR_OTHER_INSTANCE // TODO: to remove

    /* All kind of assignment operators for any type (including Instance< _OtherValue, _OtherTool >) */
    MDK_BINARY_OPERATOR_FOR_ANY( =, Assignment )
    /* All kind of assignment operators for ThisType */
    MDK_ASSIGNMENT_OPERATOR_FOR_THIS( =, Assignment )

    /* Member access */
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++,
    // а для оператора "->" использовать реализацию подобно другим.
    MDK_DEREFERENCE_OPERATOR( -> )
    MDK_ADDRESS_OF_OPERATOR( & )
    //PREFIX_UNARY_OPERATOR( &, AddressOf )
    MDK_PREFIX_UNARY_OPERATOR( *, Indirection )
    MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( ->*, MemberIndirection )
    MDK_BINARY_OPERATOR_FOR_ANY( MDK_SINGLE_ARG( , ), Comma )
    /* Subscript */
    MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], SquareBrackets )

    /* Functional forms */
    MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), RoundBrackets )
    /* Arithmetic operators */
    MDK_PREFIX_UNARY_OPERATOR( +, UnaryPrefixPlus )
    MDK_PREFIX_UNARY_OPERATOR( -, UnaryPrefixMinus )
    MDK_BINARY_OPERATOR_FOR_ANY( *, Multiply )
    MDK_BINARY_OPERATOR_FOR_ANY( /, Divide )
    MDK_BINARY_OPERATOR_FOR_ANY( %, Modulo )
    MDK_BINARY_OPERATOR_FOR_ANY( +, Addition )
    MDK_BINARY_OPERATOR_FOR_ANY( -, Subtraction )
    /* Compound assignment */
    MDK_BINARY_OPERATOR_FOR_ANY( *=, MultiplyAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( /=, DivideAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( %=, ModuloAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( +=, AdditionAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( -=, SubtractionAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( <<=, ShiftLeftAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( >>=, ShiftRightAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( &=, BitwiseAndAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( ^=, BitwiseXorAssignment )
    MDK_BINARY_OPERATOR_FOR_ANY( |=, BitwiseOrAssignment )
    /* Increment and decrement */
    MDK_PREFIX_UNARY_OPERATOR( ++, UnaryPrefixPlusPlus )
    MDK_PREFIX_UNARY_OPERATOR( --, UnaryPrefixMinusMinus )
    MDK_POSTFIX_UNARY_OPERATOR_WITH_INT( ++, UnaryPostfixPlusPlus )
    MDK_POSTFIX_UNARY_OPERATOR_WITH_INT( --, UnaryPostfixMinusMinus )
    /* Relational and comparison operators */
    MDK_BINARY_OPERATOR_FOR_ANY( ==, IsEqual )
    MDK_BINARY_OPERATOR_FOR_ANY( !=, NotEqual )
    MDK_BINARY_OPERATOR_FOR_ANY( <, Less )
    MDK_BINARY_OPERATOR_FOR_ANY( <=, LessOrEqual )
    MDK_BINARY_OPERATOR_FOR_ANY( >, Greater )
    MDK_BINARY_OPERATOR_FOR_ANY( >=, GreaterOrEqual )
    /* Logical operators */
    MDK_PREFIX_UNARY_OPERATOR( !, UnaryPrefixLogicalNot )
    MDK_BINARY_OPERATOR_FOR_ANY( &&, LogicalAnd )
    MDK_BINARY_OPERATOR_FOR_ANY( ||, LogicalOr )
    /* Bitwise operators */
    MDK_PREFIX_UNARY_OPERATOR( ~, UnaryPrefixBitwiseNot )
    MDK_BINARY_OPERATOR_FOR_ANY( &, BitwiseAnd )
    MDK_BINARY_OPERATOR_FOR_ANY( ^, BitwiseXor )
    MDK_BINARY_OPERATOR_FOR_ANY( |, BitwiseOr )
    MDK_BINARY_OPERATOR_FOR_ANY( <<, ShiftLeft )
    MDK_BINARY_OPERATOR_FOR_ANY( >>, ShiftRight )
    /* Type-casting */
    MDK_CAST_OPERATOR
};

/* RIGHT-SIDE INSTANCE OPERATORS */

/* Arithmetic operators */
MDK_GLOBAL_BINARY_OPERATOR( *, Multiply )
MDK_GLOBAL_BINARY_OPERATOR( /, Divide )
MDK_GLOBAL_BINARY_OPERATOR( %, Modulo )
MDK_GLOBAL_BINARY_OPERATOR( +, Addition )
MDK_GLOBAL_BINARY_OPERATOR( -, Subtraction )
/* Compound assignment */
MDK_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Assignment ) // NOTE: The 'operator =' cannot be implemented as global method.
MDK_GLOBAL_BINARY_OPERATOR( *=, MultiplyAssignment )
MDK_GLOBAL_BINARY_OPERATOR( /=, DivideAssignment )
MDK_GLOBAL_BINARY_OPERATOR( %=, ModuloAssignment )
MDK_GLOBAL_BINARY_OPERATOR( +=, AdditionAssignment )
MDK_GLOBAL_BINARY_OPERATOR( -=, SubtractionAssignment )
MDK_GLOBAL_BINARY_OPERATOR( <<=, ShiftLeftAssignment )
MDK_GLOBAL_BINARY_OPERATOR( >>=, ShiftRightAssignment )
MDK_GLOBAL_BINARY_OPERATOR( &=, BitwiseAndAssignment )
MDK_GLOBAL_BINARY_OPERATOR( ^=, BitwiseXorAssignment )
MDK_GLOBAL_BINARY_OPERATOR( |=, BitwiseOrAssignment )
/* Relational and comparison operators */
MDK_GLOBAL_BINARY_OPERATOR( ==, IsEqual )
MDK_GLOBAL_BINARY_OPERATOR( !=, NotEqual )
MDK_GLOBAL_BINARY_OPERATOR( <, Less )
MDK_GLOBAL_BINARY_OPERATOR( <=, LessOrEqual )
MDK_GLOBAL_BINARY_OPERATOR( >, Greater )
MDK_GLOBAL_BINARY_OPERATOR( >=, GreaterOrEqual )
/* Logical operators */
MDK_GLOBAL_BINARY_OPERATOR( &&, LogicalAnd )
MDK_GLOBAL_BINARY_OPERATOR( ||, LogicalOr )
/* Bitwise operators */
MDK_GLOBAL_BINARY_OPERATOR( &, BitwiseAnd )
MDK_GLOBAL_BINARY_OPERATOR( ^, BitwiseXor )
MDK_GLOBAL_BINARY_OPERATOR( |, BitwiseOr )
MDK_GLOBAL_BINARY_OPERATOR( <<, ShiftLeft )
MDK_GLOBAL_BINARY_OPERATOR( >>, ShiftRight )

#endif
