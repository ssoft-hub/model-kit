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
    BINARY_OPERATOR_FOR_ANY( =, ::Operator::Private::Assignment )
    /* All kind of assignment operators for ThisType */
    BINARY_OPERATOR_FOR_THIS_INSTANCE( =, ::Operator::Private::Assignment )

    /* Member access */
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++,
    // а для оператора "->" использовать реализацию подобно другим.
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( -> )
    /* Reference */
    POSTFIX_UNARY_OPERATOR_FOR_ACCESS( & )
    /* Subscript */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], ::Operator::Private::SquareBrackets )
    /* Functional forms */
    POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), ::Operator::Private::RoundBrackets )
    /* Arithmetic operators */
    PREFIX_UNARY_OPERATOR( +, ::Operator::Private::UnaryPrefixPlus )
    PREFIX_UNARY_OPERATOR( -, ::Operator::Private::UnaryPrefixMinus )
    BINARY_OPERATOR_FOR_ANY( *, ::Operator::Private::Multiply )
    BINARY_OPERATOR_FOR_ANY( /, ::Operator::Private::Divide )
    BINARY_OPERATOR_FOR_ANY( %, ::Operator::Private::Modulo )
    BINARY_OPERATOR_FOR_ANY( +, ::Operator::Private::Addition )
    BINARY_OPERATOR_FOR_ANY( -, ::Operator::Private::Subtraction )
    /* Compound assignment */
    BINARY_OPERATOR_FOR_ANY( *=, ::Operator::Private::MultiplyAssignment )
    BINARY_OPERATOR_FOR_ANY( /=, ::Operator::Private::DivideAssignment )
    BINARY_OPERATOR_FOR_ANY( %=, ::Operator::Private::ModuloAssignment )
    BINARY_OPERATOR_FOR_ANY( +=, ::Operator::Private::AdditionAssignment )
    BINARY_OPERATOR_FOR_ANY( -=, ::Operator::Private::SubtractionAssignment )
    BINARY_OPERATOR_FOR_ANY( <<=, ::Operator::Private::ShiftLeftAssignment )
    BINARY_OPERATOR_FOR_ANY( >>=, ::Operator::Private::ShiftRightAssignment )
    BINARY_OPERATOR_FOR_ANY( &=, ::Operator::Private::BitwiseAndAssignment )
    BINARY_OPERATOR_FOR_ANY( ^=, ::Operator::Private::BitwiseXorAssignment )
    BINARY_OPERATOR_FOR_ANY( |=, ::Operator::Private::BitwiseOrAssignment )
    /* Increment and decrement */
    PREFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPrefixPlusPlus )
    PREFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPrefixMinusMinus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( ++, ::Operator::Private::UnaryPostfixPlusPlus )
    POSTFIX_UNARY_OPERATOR_WITH_INT( --, ::Operator::Private::UnaryPostfixMinusMinus )
    /* Relational and comparison operators */
    BINARY_OPERATOR_FOR_ANY( ==, ::Operator::Private::IsEqual )
    BINARY_OPERATOR_FOR_ANY( !=, ::Operator::Private::NotEqual )
    BINARY_OPERATOR_FOR_ANY( <, ::Operator::Private::Less )
    BINARY_OPERATOR_FOR_ANY( <=, ::Operator::Private::LessOrEqual )
    BINARY_OPERATOR_FOR_ANY( >, ::Operator::Private::Greater )
    BINARY_OPERATOR_FOR_ANY( >=, ::Operator::Private::GreaterOrEqual )
    /* Logical operators */
    PREFIX_UNARY_OPERATOR( !, ::Operator::Private::UnaryPrefixLogicalNot )
    BINARY_OPERATOR_FOR_ANY( &&, ::Operator::Private::LogicalAnd )
    BINARY_OPERATOR_FOR_ANY( ||, ::Operator::Private::LogicalOr )
    /* Bitwise operators */
    PREFIX_UNARY_OPERATOR( ~, ::Operator::Private::UnaryPrefixBitwiseNot )
    BINARY_OPERATOR_FOR_ANY( &, ::Operator::Private::BitwiseAnd )
    BINARY_OPERATOR_FOR_ANY( ^, ::Operator::Private::BitwiseXor )
    BINARY_OPERATOR_FOR_ANY( |, ::Operator::Private::BitwiseOr )
    BINARY_OPERATOR_FOR_ANY( <<, ::Operator::Private::ShiftLeft )
    BINARY_OPERATOR_FOR_ANY( >>, ::Operator::Private::ShiftRight )
    /* Type-casting */
    CAST_OPERATOR
};

#endif
