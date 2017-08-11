#pragma once

#include <ModelKit/Common/ValueTrait.h>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

/*!
 * Специализация кдасса для проверки того, что значение является Instance.
 */
template < typename _ValueType, typename _ValueTool >
struct IsInstance< Instance< _ValueType, _ValueTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация класса для проверки совместимости между Instance.
 * Типы являются совместимыми, если используются идентичные инструмент управления
 * значением Instance и вложенные типы также являются совместимыми.
 */
template < typename _ThisType, typename _OtherType, typename _ValueTool >
struct IsCompatible< Instance< _ThisType, _ValueTool >, Instance< _OtherType, _ValueTool > >
    : public ::std::integral_constant< bool, IsCompatible< _ThisType, _OtherType >::value >
{
};

/*!
 * Специализация проверки вложенности одного Instance в другой.
 */
template < typename _ValueType, typename _ValueTool, typename _ContainerType, typename _ContainerTool >
struct IsPartOf< Instance< _ValueType, _ValueTool >, Instance< _ContainerType, _ContainerTool > >
    : public ::std::integral_constant< bool,
        IsCompatible< Instance< _ValueType, _ValueTool >, _ContainerType >::value
        || IsPartOf< Instance< _ValueType, _ValueTool >, _ContainerType >::value >
{
};
