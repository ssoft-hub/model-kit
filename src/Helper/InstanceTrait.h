#pragma once
#ifndef INSTANCE_TRAIT_H
#define INSTANCE_TRAIT_H

#include <type_traits>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

/*!
 * Класс для проверки совместимости типов.
 * По умолчанию, типы являются совместимыми, если типы одинаковые или
 * проверяемый тип _CheckType является производным от другого _OtherType.
 */
template < typename _CheckType, typename _OtherType >
struct IsCompatible
    : public ::std::integral_constant< bool,
        ::std::is_same< _CheckType, _OtherType >::value
        || ::std::is_base_of< _OtherType, _CheckType >::value >
{
};

/*!
 * Специализация класса для проверки совместимости Instance.
 * Типы являются совместимыми, если используются идентичные инструмент управления
 * значением Instance и вложенные типы также являются совместимыми.
 */
template < typename _ThisType, typename _OtherType, typename _ValueTool >
struct IsCompatible< Instance< _ThisType, _ValueTool >, Instance< _OtherType, _ValueTool > >
    : public ::std::integral_constant< bool, IsCompatible< _ThisType, _OtherType >::value >
{
};

/*!
 * Класс для проверки вложенности одного типа в другой.
 * По умолчанию, нет никакой вложенности.
 */
template < typename _ValueType, typename _ContainerType >
struct IsPartOf
    : public ::std::false_type
{
};

/*!
 * Специализация проверки вложенности одного Instance в другой.
 * This
 */
template < typename _ValueType, typename _ValueTool, typename _ContainerType, typename _ContainerTool >
struct IsPartOf< Instance< _ValueType, _ValueTool >, Instance< _ContainerType, _ContainerTool > >
    : public ::std::integral_constant< bool,
        IsCompatible< Instance< _ValueType, _ValueTool >, _ContainerType >::value
        || IsPartOf< Instance< _ValueType, _ValueTool >, _ContainerType >::value >
{
};


#endif
