#pragma once
#ifndef _SCL_INSTANCE_FEATURED_H
#define _SCL_INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>
#include <ModelKit/Featuring/Traits.h>

namespace Inplace { struct DefaultTool; }
namespace SclPrivate { template < typename > struct InstanceAccess; }

namespace SclPrivate
{
    /*!
     * Класс для формирования экземпляра значения _Value, наделенными дополнительными
     * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
     */
    template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
    class Instance
    {
        static_assert( !::std::is_reference< _Tool >::value,
            "The template parameter _Tool must to be not a reference type." );

        template < typename >
        friend struct InstanceAccess;

        using ThisType = ::SclPrivate::Instance< _Value, _Tool >;

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
        SCL_CONSTRUCTOR_FOR_THIS_INSTANCE
        /* All kind of constructors for Instance< _OtherValue, _OtherTool > */
        SCL_CONSTRUCTOR_FOR_OTHER_INSTANCE // TODO: to remove

        /* All kind of assignment operators for any type (including Instance< _OtherValue, _OtherTool >) */
        SCL_BINARY_OPERATOR_FOR_ANY( =, Assignment )
        /* All kind of assignment operators for ThisType */
        SCL_ASSIGNMENT_OPERATOR_FOR_THIS( =, Assignment )

        /* Member access */
        // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
        // TODO: Заменить на оператор "." с внедрением P0416R1(N4477) или P0352 в стандарт C++,
        // а для оператора "->" использовать реализацию подобно другим.
        SCL_DEREFERENCE_OPERATOR( -> )
        SCL_ADDRESS_OF_OPERATOR( & )
        //PREFIX_UNARY_OPERATOR( &, AddressOf )
        SCL_PREFIX_UNARY_OPERATOR( *, Indirection )
        SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( ->*, MemberIndirection )
        SCL_BINARY_OPERATOR_FOR_ANY( SCL_SINGLE_ARG( , ), Comma )
        /* Subscript */
        SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT( [], SquareBrackets )

        /* Functional forms */
        SCL_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS( (), RoundBrackets )
        /* Arithmetic operators */
        SCL_PREFIX_UNARY_OPERATOR( +, UnaryPrefixPlus )
        SCL_PREFIX_UNARY_OPERATOR( -, UnaryPrefixMinus )
        SCL_BINARY_OPERATOR_FOR_ANY( *, Multiply )
        SCL_BINARY_OPERATOR_FOR_ANY( /, Divide )
        SCL_BINARY_OPERATOR_FOR_ANY( %, Modulo )
        SCL_BINARY_OPERATOR_FOR_ANY( +, Addition )
        SCL_BINARY_OPERATOR_FOR_ANY( -, Subtraction )
        /* Compound assignment */
        SCL_BINARY_OPERATOR_FOR_ANY( *=, MultiplyAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( /=, DivideAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( %=, ModuloAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( +=, AdditionAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( -=, SubtractionAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( <<=, ShiftLeftAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( >>=, ShiftRightAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( &=, BitwiseAndAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( ^=, BitwiseXorAssignment )
        SCL_BINARY_OPERATOR_FOR_ANY( |=, BitwiseOrAssignment )
        /* Increment and decrement */
        SCL_PREFIX_UNARY_OPERATOR( ++, UnaryPrefixPlusPlus )
        SCL_PREFIX_UNARY_OPERATOR( --, UnaryPrefixMinusMinus )
        SCL_POSTFIX_UNARY_OPERATOR_WITH_INT( ++, UnaryPostfixPlusPlus )
        SCL_POSTFIX_UNARY_OPERATOR_WITH_INT( --, UnaryPostfixMinusMinus )
        /* Relational and comparison operators */
        SCL_BINARY_OPERATOR_FOR_ANY( ==, IsEqual )
        SCL_BINARY_OPERATOR_FOR_ANY( !=, NotEqual )
        SCL_BINARY_OPERATOR_FOR_ANY( <, Less )
        SCL_BINARY_OPERATOR_FOR_ANY( <=, LessOrEqual )
        SCL_BINARY_OPERATOR_FOR_ANY( >, Greater )
        SCL_BINARY_OPERATOR_FOR_ANY( >=, GreaterOrEqual )
        /* Logical operators */
        SCL_PREFIX_UNARY_OPERATOR( !, UnaryPrefixLogicalNot )
        SCL_BINARY_OPERATOR_FOR_ANY( &&, LogicalAnd )
        SCL_BINARY_OPERATOR_FOR_ANY( ||, LogicalOr )
        /* Bitwise operators */
        SCL_PREFIX_UNARY_OPERATOR( ~, UnaryPrefixBitwiseNot )
        SCL_BINARY_OPERATOR_FOR_ANY( &, BitwiseAnd )
        SCL_BINARY_OPERATOR_FOR_ANY( ^, BitwiseXor )
        SCL_BINARY_OPERATOR_FOR_ANY( |, BitwiseOr )
        SCL_BINARY_OPERATOR_FOR_ANY( <<, ShiftLeft )
        SCL_BINARY_OPERATOR_FOR_ANY( >>, ShiftRight )
        /* Type-casting */
        SCL_CAST_OPERATOR
    };
}

/* RIGHT-SIDE INSTANCE OPERATORS */

/* Arithmetic operators */
SCL_GLOBAL_BINARY_OPERATOR( *, Multiply )
SCL_GLOBAL_BINARY_OPERATOR( /, Divide )
SCL_GLOBAL_BINARY_OPERATOR( %, Modulo )
SCL_GLOBAL_BINARY_OPERATOR( +, Addition )
SCL_GLOBAL_BINARY_OPERATOR( -, Subtraction )
/* Compound assignment */
SCL_GLOBAL_BINARY_OPERATOR_SPECIALIZATION( Assignment ) // NOTE: The 'operator =' cannot be implemented as global method.
SCL_GLOBAL_BINARY_OPERATOR( *=, MultiplyAssignment )
SCL_GLOBAL_BINARY_OPERATOR( /=, DivideAssignment )
SCL_GLOBAL_BINARY_OPERATOR( %=, ModuloAssignment )
SCL_GLOBAL_BINARY_OPERATOR( +=, AdditionAssignment )
SCL_GLOBAL_BINARY_OPERATOR( -=, SubtractionAssignment )
SCL_GLOBAL_BINARY_OPERATOR( <<=, ShiftLeftAssignment )
SCL_GLOBAL_BINARY_OPERATOR( >>=, ShiftRightAssignment )
SCL_GLOBAL_BINARY_OPERATOR( &=, BitwiseAndAssignment )
SCL_GLOBAL_BINARY_OPERATOR( ^=, BitwiseXorAssignment )
SCL_GLOBAL_BINARY_OPERATOR( |=, BitwiseOrAssignment )
/* Relational and comparison operators */
SCL_GLOBAL_BINARY_OPERATOR( ==, IsEqual )
SCL_GLOBAL_BINARY_OPERATOR( !=, NotEqual )
SCL_GLOBAL_BINARY_OPERATOR( <, Less )
SCL_GLOBAL_BINARY_OPERATOR( <=, LessOrEqual )
SCL_GLOBAL_BINARY_OPERATOR( >, Greater )
SCL_GLOBAL_BINARY_OPERATOR( >=, GreaterOrEqual )
/* Logical operators */
SCL_GLOBAL_BINARY_OPERATOR( &&, LogicalAnd )
SCL_GLOBAL_BINARY_OPERATOR( ||, LogicalOr )
/* Bitwise operators */
SCL_GLOBAL_BINARY_OPERATOR( &, BitwiseAnd )
SCL_GLOBAL_BINARY_OPERATOR( ^, BitwiseXor )
SCL_GLOBAL_BINARY_OPERATOR( |, BitwiseOr )
SCL_GLOBAL_BINARY_OPERATOR( <<, ShiftLeft )
SCL_GLOBAL_BINARY_OPERATOR( >>, ShiftRight )

#endif
