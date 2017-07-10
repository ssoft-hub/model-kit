#pragma once
#ifndef INSTANCE_HELPER_H
#define INSTANCE_HELPER_H

#include <type_traits>

// Вспомогательные классы для проверки свойств Instance

template < typename _Value >
struct IsHeapInstance
    : public ::std::false_type
{
};

template < typename _Value >
struct IsOptionalInstance
    : public ::std::false_type
{
};

template < typename _Value >
struct IsImplicitInstance
    : public ::std::false_type
{
};

template < typename _Value >
struct IsAtomicInstance
    : public ::std::false_type
{
};

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

// Инструмент по умолчанию для размщения значения
namespace Cpp { namespace Inplace { struct InplaceTool; } }

/*!
 * Класс-помошник для вычисления оптимального типа обертки InstanceType для наделения
 * значения типа _ValueType необходимыми свойствами.
 *
 * При применении вычисления обертки, не добавляющие значению дополнительных свойств,
 * игнорируются.
 *
 * По умолчанию все типы оборачиваются типом Instance c размещением значения по месту.
 */
template < typename _ValueType >
struct InstanceHelper
{
    using DefaultTool = ::Cpp::Inplace::InplaceTool;
    using InstanceType = Instance< _ValueType, DefaultTool >;
};

/*!
 * Специализация помошника для вычисления типа InstanceType для значений в виде
 * оберток типа Instance. Нет необходимости тип Instance дополнительно оборачивать.
 */
template < typename _ValueType, typename _ValueTool >
struct InstanceHelper< Instance< _ValueType, _ValueTool > >
{
    using InstanceType = Instance< _ValueType, _ValueTool >;
};

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
