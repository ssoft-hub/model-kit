//#pragma once
//#ifndef INSTANCE_OPERATION_H

//#include "OperatorRoundBrackets.h"

//#include <memory>

///*! TODO:
// * Может быть определен любой результат действия унарного оператора над операндом.
// *
// * Если оператор приводит к изменению внутреннего содержимого операнда,
// * то должен быть возвращен входящий параметр Featured. Иначе должен быть
// * возвращен результат воздействия на внутреннее содержимое операнда.
// */

//template < typename _Value, typename _Tool >
//class Featured;

//#define RVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && value ) \
//    { \
//        /*возвращается Featured*/ \
//        sym(*&value); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( value ); \
//    } \

//#define LVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & value ) \
//    { \
//        /*возвращается Featured*/ \
//        sym(*&value); \
//        return value; \
//    } \

//#define CRVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && value ) \
//    { \
//        /*возвращается результат воздействия на содержимое Featured*/ \
//        return sym(*&value); \
//    } \

//#define CLVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & value ) \
//    { \
//        /*возвращается результат воздействия на содержимое Featured*/ \
//        return sym(*&value); \
//    } \

//#define INSTANCE_PREFIX_UNARY_OPERATOR( sym ) \
//    RVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    CRVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    LVALUE_PREFIX_UNARY_OPERATOR( sym ) \
//    CLVALUE_PREFIX_UNARY_OPERATOR( sym ) \

//#define RVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        _RightType & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( left ); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        const _RightType & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        _RightType && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        const _RightType && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CRVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        _RightType & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        const _RightType & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        _RightType && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        const _RightType && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        _RightType & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        const _RightType & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        _RightType && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        const _RightType && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CLVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        _RightType & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        const _RightType & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        _RightType && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _LeftTool, typename _RightType > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        const _RightType && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType & left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType & left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType && left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType && left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType & left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType & left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType && left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType && left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType & left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType & left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType && left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType && left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType & left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType & left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        _LeftType && left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \
// \
//    template < typename _LeftType, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const _LeftType && left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define RVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( left ); \
//    } \

//#define CRVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CLVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define RVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( left ); \
//    } \

//#define CRVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CLVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        const Featured< _RightType, _RightTool > && right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define RVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( left ); \
//    } \

//#define CRVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CLVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define RVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > && operator sym ( \
//        Featured< _LeftType, _LeftTool > && left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return ::std::forward< Featured< _LeftType, _LeftTool > && >( left ); \
//    } \

//#define CRVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > && left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \

//#define LVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline Featured< _LeftType, _LeftTool > & operator sym ( \
//        Featured< _LeftType, _LeftTool > & left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        (*&left) sym (*&right); \
//        return left; \
//    } \

//#define CLVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool > \
//    inline decltype(auto) operator sym ( \
//        const Featured< _LeftType, _LeftTool > & left, \
//        const Featured< _RightType, _RightTool > & right ) \
//    { \
//        return (*&left) sym (*&right); \
//    } \


//#define INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_LEFT_INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_LEFT_RVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_LEFT_CRVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_LEFT_LVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    RVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CRVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    LVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \
//    CLVALUE_LEFT_CLVALUE_RIGHT_INSTANCE_BINARY_OPERATOR( sym ) \

//// Сложение
//INSTANCE_BINARY_OPERATOR( + )

//// Вычитание
//INSTANCE_BINARY_OPERATOR( - )

//// Унарный плюс
//INSTANCE_PREFIX_UNARY_OPERATOR( + )

//// Унарный минус
//INSTANCE_PREFIX_UNARY_OPERATOR( - )

//// Умножение
//INSTANCE_BINARY_OPERATOR( * )

//// Деление
//INSTANCE_BINARY_OPERATOR( / )

//// Операция модуль (остаток от деления целых чисел)
//INSTANCE_BINARY_OPERATOR( % )

//// Префиксный инкремент
//INSTANCE_PREFIX_UNARY_OPERATOR( ++ )

//// Префиксный декремент
//INSTANCE_PREFIX_UNARY_OPERATOR( -- )

//// Суффиксный инкремент
//template < typename _LeftType, typename _LeftTool >
//inline decltype(auto) operator ++ (
//    Featured< _LeftType, _LeftTool > & value, int )
//{
//    return (*&value)++;
//}

//// Суффиксный декремент
//template < typename _LeftType, typename _LeftTool >
//inline decltype(auto) operator -- (
//    Featured< _LeftType, _LeftTool > & value, int )
//{
//    return (*&value)--;
//}

//// Равенство
//INSTANCE_BINARY_OPERATOR( == )

//// Неравенство
//INSTANCE_BINARY_OPERATOR( != )

//// Больше
//INSTANCE_BINARY_OPERATOR( > )

//// Меньше
//INSTANCE_BINARY_OPERATOR( < )

//// Больше или равно
//INSTANCE_BINARY_OPERATOR( >= )

//// Меньше или равно
//INSTANCE_BINARY_OPERATOR( <= )

//// Логическое отрицание, НЕ
//INSTANCE_PREFIX_UNARY_OPERATOR( ! )

//// Логическое умножение, И
//INSTANCE_BINARY_OPERATOR( && )

//// Логическое сложение, ИЛИ
//INSTANCE_BINARY_OPERATOR( || )

//// Побитовая инверсия
//INSTANCE_PREFIX_UNARY_OPERATOR( ~ )

//// Побитовое И
//INSTANCE_BINARY_OPERATOR( & )

//// Побитовое ИЛИ (or)
//INSTANCE_BINARY_OPERATOR( | )

//// Побитовое исключающее ИЛИ (xor)
//INSTANCE_BINARY_OPERATOR( ^ )

//// Побитовый сдвиг влево
//INSTANCE_BINARY_OPERATOR( << )

//// Побитовый сдвиг вправо
//INSTANCE_BINARY_OPERATOR( >> )

////// Потоковый вывод
////STREAM_INSTANCE_OPERATOR( << )

////// Потоковый ввод
////STREAM_INSTANCE_OPERATOR( >> )

//// Сложение, совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( += )

//// Вычитание, совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( -= )

//// Умножение, совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( *= )

//// Деление, совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( /= )

//// Вычисление остатка от деления, совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( %= )

//// Побитовое «И» (AND), совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( &= )

//// Побитовое «ИЛИ» (or), совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( |= )

//// Побитовое «исключающее ИЛИ» (xor), совмещённое с присваиванием
//INSTANCE_BINARY_OPERATOR( ^= )

//// Побитовый сдвиг влево, совмещённый с присваиванием
//INSTANCE_BINARY_OPERATOR( <<= )

//// Побитовый сдвиг вправо, совмещённый с присваиванием
//INSTANCE_BINARY_OPERATOR( >>= )

//#undef INSTANCE_PREFIX_UNARY_OPERATOR
//#undef INSTANCE_BINARY_OPERATOR
//#undef INSTANCE_BINARY_OPERATOR
//#undef STREAM_INSTANCE_OPERATOR

//#endif
