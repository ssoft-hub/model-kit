#pragma once

#include <ModelKit/Common/AggregationKind.h>

template < typename _Type, AggregationKind _Kind, typename _RelationTool >
class AssociationEnd;

#define UPDATE_ASSOCIATION_UNARY_OPERATOR( sym ) \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool > \
    inline constexpr AssociationEnd< _LeftType, _LeftKind, _LeftTool > & operator sym ( \
        AssociationEnd< _LeftType, _LeftKind, _LeftTool > & value ) \
    { \
        sym(*&value); \
        return value; \
    } \

#define CONST_ASSOCIATION_UNARY_OPERATOR( sym ) \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool > \
    inline constexpr decltype(auto) operator sym ( \
        const AssociationEnd< _LeftType, _LeftKind, _LeftTool > & value ) \
    { \
        return sym(*&value); \
    } \

#define UPDATE_ASSOCIATION_BINARY_OPERATOR( sym ) \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool, \
        typename _RightType > \
    inline constexpr AssociationEnd< _LeftType, _LeftKind, _LeftTool > & operator sym ( \
        AssociationEnd< _LeftType, _LeftKind, _LeftTool > & left, \
        const _RightType & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \
 \
    template < typename _LeftType, \
        typename _RightType, AggregationKind _RightKind, typename _RightTool > \
    inline constexpr _LeftType & operator sym ( \
        _LeftType & left, \
        const AssociationEnd< _RightType, _RightKind, _RightTool > & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \
 \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool, \
        typename _RightType, AggregationKind _RightKind, typename _RightTool > \
    inline constexpr AssociationEnd< _LeftType, _LeftKind, _LeftTool > &  operator sym ( \
        AssociationEnd< _LeftType, _LeftKind, _LeftTool > & left, \
        const AssociationEnd< _RightType, _RightKind, _RightTool > & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \

#define STREAM_ASSOCIATION_OPERATOR( sym ) \
    template < typename _LeftType, typename _RightType, AggregationKind _RightKind, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        _LeftType & left, \
        const AssociationEnd< _RightType, _RightKind, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \

#define CONST_ASSOCIATION_BINARY_OPERATOR( sym ) \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool, typename _RightType > \
    inline constexpr decltype(auto) operator sym ( \
        const AssociationEnd< _LeftType, _LeftKind, _LeftTool > & left, \
        const _RightType & right ) \
    { \
        return (*&left) sym (*&right); \
    } \
 \
    template < typename _LeftType, typename _RightType, AggregationKind _RightKind, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        const _LeftType & left, \
        const AssociationEnd< _RightType, _RightKind, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \
 \
    template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool, typename _RightType, AggregationKind _RightKind, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        const AssociationEnd< _LeftType, _LeftKind, _LeftTool > & left, \
        const AssociationEnd< _RightType, _RightKind, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \

// Сложение
CONST_ASSOCIATION_BINARY_OPERATOR( + )

// Вычитание
CONST_ASSOCIATION_BINARY_OPERATOR( - )

// Унарный плюс
CONST_ASSOCIATION_UNARY_OPERATOR( + )

// Унарный минус
CONST_ASSOCIATION_UNARY_OPERATOR( - )

// Умножение
CONST_ASSOCIATION_BINARY_OPERATOR( * )

// Деление
CONST_ASSOCIATION_BINARY_OPERATOR( / )

// Операция модуль (остаток от деления целых чисел)
CONST_ASSOCIATION_BINARY_OPERATOR( % )

// Префиксный инкремент
UPDATE_ASSOCIATION_UNARY_OPERATOR( ++ )

// Префиксный декремент
UPDATE_ASSOCIATION_UNARY_OPERATOR( -- )

// Суффиксный инкремент
template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool >
inline constexpr decltype(auto) operator ++ (
    AssociationEnd< _LeftType, _LeftKind, _LeftTool > & value, int )
{
    return (*&value)++;
}

// Суффиксный декремент
template < typename _LeftType, AggregationKind _LeftKind, typename _LeftTool >
inline constexpr decltype(auto) operator -- (
    AssociationEnd< _LeftType, _LeftKind, _LeftTool > & value, int )
{
    return (*&value)--;
}

// Равенство
CONST_ASSOCIATION_BINARY_OPERATOR( == )

// Неравенство
CONST_ASSOCIATION_BINARY_OPERATOR( != )

// Больше
CONST_ASSOCIATION_BINARY_OPERATOR( > )

// Меньше
CONST_ASSOCIATION_BINARY_OPERATOR( < )

// Больше или равно
CONST_ASSOCIATION_BINARY_OPERATOR( >= )

// Меньше или равно
CONST_ASSOCIATION_BINARY_OPERATOR( <= )

// Логическое отрицание, НЕ
CONST_ASSOCIATION_UNARY_OPERATOR( ! )

// Логическое умножение, И
CONST_ASSOCIATION_BINARY_OPERATOR( && )

// Логическое сложение, ИЛИ
CONST_ASSOCIATION_BINARY_OPERATOR( || )

// Побитовая инверсия
CONST_ASSOCIATION_UNARY_OPERATOR( ~ )

// Побитовое И
CONST_ASSOCIATION_BINARY_OPERATOR( & )

// Побитовое ИЛИ (or)
CONST_ASSOCIATION_BINARY_OPERATOR( | )

// Побитовое исключающее ИЛИ (xor)
CONST_ASSOCIATION_BINARY_OPERATOR( ^ )

// Побитовый сдвиг влево
CONST_ASSOCIATION_BINARY_OPERATOR( << )

// Побитовый сдвиг вправо
CONST_ASSOCIATION_BINARY_OPERATOR( >> )

// Потоковый вывод
STREAM_ASSOCIATION_OPERATOR( << )

// Потоковый ввод
STREAM_ASSOCIATION_OPERATOR( >> )

// Сложение, совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( += )

// Вычитание, совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( -= )

// Умножение, совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( *= )

// Деление, совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( /= )

// Вычисление остатка от деления, совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( %= )

// Побитовое «И» (AND), совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( &= )

// Побитовое «ИЛИ» (or), совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( |= )

// Побитовое «исключающее ИЛИ» (xor), совмещённое с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( ^= )

// Побитовый сдвиг влево, совмещённый с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( <<= )

// Побитовый сдвиг вправо, совмещённый с присваиванием
UPDATE_ASSOCIATION_BINARY_OPERATOR( >>= )

#undef UPDATE_ASSOCIATION_UNARY_OPERATOR
#undef UPDATE_ASSOCIATION_BINARY_OPERATOR
#undef CONST_ASSOCIATION_UNARY_OPERATOR
#undef CONST_ASSOCIATION_BINARY_OPERATOR
#undef STREAM_ASSOCIATION_OPERATOR
