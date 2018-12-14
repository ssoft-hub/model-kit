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
    BINARY_OPERATOR_FOR_ANY( =, ::Operator::Assignment )
    /* All kind of assignment operators for ThisType */
    BINARY_OPERATOR_FOR_THIS_INSTANCE( =, ::Operator::Assignment )

    /* Member access */
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++,
    // а для оператора "->" использовать реализацию подобно другим.
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( -> )
    /* Reference */
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( & )
    /* Subscript */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], ::Operator::SquareBrackets )
    /* Functional forms */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), ::Operator::RoundBrackets )
    /* Arithmetic operators */
    PREFIX_UNARY_OPERATOR( +, ::Operator::UnaryPrefixPlus )
    PREFIX_UNARY_OPERATOR( -, ::Operator::UnaryPrefixMinus )
    BINARY_OPERATOR_FOR_ANY( *, ::Operator::Multiply )
    BINARY_OPERATOR_FOR_ANY( /, ::Operator::Divide )
    BINARY_OPERATOR_FOR_ANY( %, ::Operator::Modulo )
    BINARY_OPERATOR_FOR_ANY( +, ::Operator::Addition )
    BINARY_OPERATOR_FOR_ANY( -, ::Operator::Subtraction )
    /* Compound assignment */
    BINARY_OPERATOR_FOR_ANY( *=, ::Operator::MultiplyAssignment )
    BINARY_OPERATOR_FOR_ANY( /=, ::Operator::DivideAssignment )
    BINARY_OPERATOR_FOR_ANY( %=, ::Operator::ModuloAssignment )
    BINARY_OPERATOR_FOR_ANY( +=, ::Operator::AdditionAssignment )
    BINARY_OPERATOR_FOR_ANY( -=, ::Operator::SubtractionAssignment )
    BINARY_OPERATOR_FOR_ANY( <<=, ::Operator::ShiftLeftAssignment )
    BINARY_OPERATOR_FOR_ANY( >>=, ::Operator::ShiftRightAssignment )
    BINARY_OPERATOR_FOR_ANY( &=, ::Operator::BitwiseAndAssignment )
    BINARY_OPERATOR_FOR_ANY( ^=, ::Operator::BitwiseXorAssignment )
    BINARY_OPERATOR_FOR_ANY( |=, ::Operator::BitwiseOrAssignment )
    /* Increment and decrement */
    PREFIX_UNARY_OPERATOR( ++, ::Operator::UnaryPrefixPlusPlus )
    PREFIX_UNARY_OPERATOR( --, ::Operator::UnaryPrefixMinusMinus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( ++, ::Operator::UnaryPostfixPlusPlus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( --, ::Operator::UnaryPostfixMinusMinus )
    /* Relational and comparison operators */
    BINARY_OPERATOR_FOR_ANY( ==, ::Operator::IsEqual )
    BINARY_OPERATOR_FOR_ANY( !=, ::Operator::NotEqual )
    BINARY_OPERATOR_FOR_ANY( <, ::Operator::Less )
    BINARY_OPERATOR_FOR_ANY( <=, ::Operator::LessOrEqual )
    BINARY_OPERATOR_FOR_ANY( >, ::Operator::Greater )
    BINARY_OPERATOR_FOR_ANY( >=, ::Operator::GreaterOrEqual )
    /* Logical operators */
    PREFIX_UNARY_OPERATOR( !, ::Operator::UnaryPrefixLogicalNot )
    BINARY_OPERATOR_FOR_ANY( &&, ::Operator::LogicalAnd )
    BINARY_OPERATOR_FOR_ANY( ||, ::Operator::LogicalOr )
    /* Bitwise operators */
    PREFIX_UNARY_OPERATOR( ~, ::Operator::UnaryPrefixBitwiseNot )
    BINARY_OPERATOR_FOR_ANY( &, ::Operator::BitwiseAnd )
    BINARY_OPERATOR_FOR_ANY( ^, ::Operator::BitwiseXor )
    BINARY_OPERATOR_FOR_ANY( |, ::Operator::BitwiseOr )
    BINARY_OPERATOR_FOR_ANY( <<, ::Operator::ShiftLeft )
    BINARY_OPERATOR_FOR_ANY( >>, ::Operator::ShiftRight )
    /* Type-casting */
    CAST_OPERATOR
};

/* RIGHT-SIDE INSTANCE OPERATORS */

/*! NOTE: The 'operator =' cannot be implemented as global method. */

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

#endif
