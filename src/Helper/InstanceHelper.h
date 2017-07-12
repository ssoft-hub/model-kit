#pragma once
#ifndef INSTANCE_HELPER_H
#define INSTANCE_HELPER_H

#include <type_traits>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

// Базовый инструмент для размещения значения
namespace Cpp { namespace Inplace { struct InplaceTool; } }

/*!
 * Класс-помошник для вычисления типа обертки Instance для наделения
 * значения типа _ValueType необходимыми свойствами.
 *
 * Все типы Instance, не добавляющие своему внутреннему значению
 * дополнительных свойств, игнорируются.
 *
 * По умолчанию все типы оборачиваются типом Instance c размещением значения
 * по месту (::Cpp::Inplace::InplaceTool).
 */
template < typename _ValueType >
struct InstanceHelper
{
    using DefaultTool = ::Cpp::Inplace::InplaceTool;
    using type = Instance< _ValueType, DefaultTool >;
};

/*!
 * Общая специализация помошника для значений вида Instance.
 */
template < typename _ValueType, typename _ValueTool >
struct InstanceHelper< Instance< _ValueType, _ValueTool > >
{
    using type = Instance< _ValueType, _ValueTool >;
};

#endif
