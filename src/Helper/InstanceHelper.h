#pragma once
#ifndef INSTANCE_HELPER_H
#define INSTANCE_HELPER_H

#include <type_traits>

// Вспомогательные классы для проверки свойств Instance

template < typename _Value >
struct IsHeapInstance
{
    static constexpr bool result = false;
};

template < typename _Value >
struct IsOptionalInstance
{
    static constexpr bool result = false;
};

template < typename _Value >
struct IsImplicitInstance
{
    static constexpr bool result = false;
};

template < typename _Value >
struct IsAtomicInstance
{
    static constexpr bool result = false;
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

#endif
