#pragma once

template < typename _ValueType, typename _ValueTool >
class Instance;

#define UPDATE_INSTANCE_UNARY_OPERATOR( sym ) \
    template < typename _LeftType, typename _LeftTool > \
    inline constexpr Instance< _LeftType, _LeftTool > & operator sym ( \
        Instance< _LeftType, _LeftTool > & value ) \
    { \
        sym(*&value); \
        return value; \
    } \

#define CONST_INSTANCE_UNARY_OPERATOR( sym ) \
    template < typename _LeftType, typename _LeftTool > \
    inline constexpr decltype(auto) operator sym ( \
        const Instance< _LeftType, _LeftTool > & value ) \
    { \
        return sym(*&value); \
    } \

#define UPDATE_INSTANCE_BINARY_OPERATOR( sym ) \
    template < typename _LeftType, typename _LeftTool, typename _RightType > \
    inline constexpr Instance< _LeftType, _LeftTool > & operator sym ( \
        Instance< _LeftType, _LeftTool > & left, \
        const _RightType & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \
 \
    template < typename _LeftType, typename _RightType, typename _RightTool > \
    inline constexpr _LeftType & operator sym ( \
        _LeftType & left, \
        const Instance< _RightType, _RightTool > & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \
 \
    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
    inline constexpr Instance< _LeftType, _LeftTool > &  operator sym ( \
        Instance< _LeftType, _LeftTool > & left, \
        const Instance< _RightType, _RightTool > & right ) \
    { \
        (*&left) sym (*&right); \
        return left; \
    } \

#define STREAM_INSTANCE_OPERATOR( sym ) \
    template < typename _LeftType, typename _RightType, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        _LeftType & left, \
        const Instance< _RightType, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \

#define CONST_INSTANCE_BINARY_OPERATOR( sym ) \
    template < typename _LeftType, typename _LeftTool, typename _RightType > \
    inline constexpr decltype(auto) operator sym ( \
        const Instance< _LeftType, _LeftTool > & left, \
        const _RightType & right ) \
    { \
        return (*&left) sym (*&right); \
    } \
 \
    template < typename _LeftType, typename _RightType, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        const _LeftType & left, \
        const Instance< _RightType, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \
 \
    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
    inline constexpr decltype(auto) operator sym ( \
        const Instance< _LeftType, _LeftTool > & left, \
        const Instance< _RightType, _RightTool > & right ) \
    { \
        return (*&left) sym (*&right); \
    } \

// Сложение
CONST_INSTANCE_BINARY_OPERATOR( + )

// Вычитание
CONST_INSTANCE_BINARY_OPERATOR( - )

// Унарный плюс
CONST_INSTANCE_UNARY_OPERATOR( + )

// Унарный минус
CONST_INSTANCE_UNARY_OPERATOR( - )

// Умножение
CONST_INSTANCE_BINARY_OPERATOR( * )

// Деление
CONST_INSTANCE_BINARY_OPERATOR( / )

// Операция модуль (остаток от деления целых чисел)
CONST_INSTANCE_BINARY_OPERATOR( % )

// Префиксный инкремент
UPDATE_INSTANCE_UNARY_OPERATOR( ++ )

// Префиксный декремент
UPDATE_INSTANCE_UNARY_OPERATOR( -- )

// Суффиксный инкремент
template < typename _LeftType, typename _LeftTool >
inline constexpr decltype(auto) operator ++ (
    Instance< _LeftType, _LeftTool > & value, int )
{
    return (*&value)++;
}

// Суффиксный декремент
template < typename _LeftType, typename _LeftTool >
inline constexpr decltype(auto) operator -- (
    Instance< _LeftType, _LeftTool > & value, int )
{
    return (*&value)--;
}

// Равенство
CONST_INSTANCE_BINARY_OPERATOR( == )

// Неравенство
CONST_INSTANCE_BINARY_OPERATOR( != )

// Больше
CONST_INSTANCE_BINARY_OPERATOR( > )

// Меньше
CONST_INSTANCE_BINARY_OPERATOR( < )

// Больше или равно
CONST_INSTANCE_BINARY_OPERATOR( >= )

// Меньше или равно
CONST_INSTANCE_BINARY_OPERATOR( <= )

// Логическое отрицание, НЕ
CONST_INSTANCE_UNARY_OPERATOR( ! )

// Логическое умножение, И
CONST_INSTANCE_BINARY_OPERATOR( && )

// Логическое сложение, ИЛИ
CONST_INSTANCE_BINARY_OPERATOR( || )

// Побитовая инверсия
CONST_INSTANCE_UNARY_OPERATOR( ~ )

// Побитовое И
CONST_INSTANCE_BINARY_OPERATOR( & )

// Побитовое ИЛИ (or)
CONST_INSTANCE_BINARY_OPERATOR( | )

// Побитовое исключающее ИЛИ (xor)
CONST_INSTANCE_BINARY_OPERATOR( ^ )

// Побитовый сдвиг влево
CONST_INSTANCE_BINARY_OPERATOR( << )

// Побитовый сдвиг вправо
CONST_INSTANCE_BINARY_OPERATOR( >> )

// Потоковый вывод
STREAM_INSTANCE_OPERATOR( << )

// Потоковый ввод
STREAM_INSTANCE_OPERATOR( >> )

// Сложение, совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( += )

// Вычитание, совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( -= )

// Умножение, совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( *= )

// Деление, совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( /= )

// Вычисление остатка от деления, совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( %= )

// Побитовое «И» (AND), совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( &= )

// Побитовое «ИЛИ» (or), совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( |= )

// Побитовое «исключающее ИЛИ» (xor), совмещённое с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( ^= )

// Побитовый сдвиг влево, совмещённый с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( <<= )

// Побитовый сдвиг вправо, совмещённый с присваиванием
UPDATE_INSTANCE_BINARY_OPERATOR( >>= )

#undef UPDATE_INSTANCE_UNARY_OPERATOR
#undef UPDATE_INSTANCE_BINARY_OPERATOR
#undef CONST_INSTANCE_UNARY_OPERATOR
#undef CONST_INSTANCE_BINARY_OPERATOR
