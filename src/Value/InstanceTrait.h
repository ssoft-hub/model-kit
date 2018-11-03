#pragma once

#include <ModelKit/Common/ValueTrait.h>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

/*!
 * Специализация кдасса для проверки того, что значение является Instance.
 */
template < typename _ValueType, typename _ValueTool >
struct IsInstanceHelper< Instance< _ValueType, _ValueTool > >
    : public ::std::true_type
{};

/*!
 * Специализация класса для проверки совместимости между Instance.
 * Типы являются совместимыми, если используются идентичные инструмент управления
 * значением Instance и вложенные типы также являются совместимыми.
 */
template < typename _ThisType, typename _OtherType, typename _ValueTool >
struct IsCompatibleHelper< Instance< _ThisType, _ValueTool >, Instance< _OtherType, _ValueTool > >
    : public ::std::integral_constant< bool, is_compatible< _ThisType, _OtherType > >
{};

/*!
 * Специализация проверки вложенности одного Instance в другой.
 */
template < typename _ValueType, typename _ValueTool, typename _ContainerType, typename _ContainerTool >
struct IsThisPartOfOtherHelper< Instance< _ValueType, _ValueTool >, Instance< _ContainerType, _ContainerTool > >
    : public ::std::integral_constant< bool,
        is_compatible< Instance< _ValueType, _ValueTool >, _ContainerType >
        || is_this_part_of_other< Instance< _ValueType, _ValueTool >, _ContainerType > >
{};
