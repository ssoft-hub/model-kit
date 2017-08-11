#pragma once

#include <ModelKit/Value/Tool/Cpp/Inplace/DefaultTool.h>
#include <type_traits>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

/*!
 * Класс-помошник для вычисления типа обертки Instance для наделения
 * значения типа _ValueType необходимыми свойствами.
 *
 * По умолчанию все типы оборачиваются типом Instance c размещением значения
 * по месту (::Cpp::Inplace::DefaultTool).
 */
template < typename _ValueType >
struct DefaultHelper
{
    using type = Instance< _ValueType, ::Cpp::Inplace::DefaultTool >;
};
template < typename _ValueType >
using DefaultHelper_t = typename DefaultHelper< _ValueType >::type;

template < typename _ValueType, typename _ValueTool >
struct DefaultHelper< Instance< _ValueType, _ValueTool > >
{
    using type = Instance< _ValueType, _ValueTool >;
};

/*!
 * Класс убирает вложенность идентичных Instance.
 */
template < typename _ValueType, typename _ValueTool, typename _RequaredTool >
struct DefaultHelper< Instance< Instance< _ValueType, _ValueTool >, _RequaredTool > >
{
    using type = ::std::conditional_t<
        ::std::is_same< _ValueTool, _RequaredTool >::value,
        DefaultHelper_t< Instance< _ValueType, _ValueTool > >,
        Instance< Instance< _ValueType, _ValueTool >, _RequaredTool > >;
};
