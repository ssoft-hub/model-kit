#pragma once

#include <ModelKit/Common/ValueTrait.h>
#include <ModelKit/Value/Tool/Cpp/Inplace/DefaultTool.h>
#include <ModelKit/Value/Tool/Cpp/Raw/ImplicitTool.h>

// Предварительная декларация Instance
template < typename _ValueType, typename _ValueTool > class Instance;

/*!
 * Класс-помошник для вычисления типа обертки Instance для наделения
 * значения типа _ValueType необходимыми свойствами.
 *
 * Все типы Instance, не добавляющие своему внутреннему значению
 * дополнительных свойств, игнорируются.
 *
 * По умолчанию все типы оборачиваются типом Instance c размещением значения
 * по месту (::Cpp::Inplace::DefaultTool).
 */
template < typename _ValueType >
struct OptimalHelper
{
    using DefaultTool = ::Cpp::Inplace::DefaultTool;
    using ImplicitTool = ::Cpp::Raw::ImplicitTool;

    using type = ::std::conditional_t<
        ::std::is_trivially_copyable< _ValueType >::value
            || !::std::is_copy_constructible< _ValueType >::value
            || !::std::is_move_constructible< _ValueType >::value,
        Instance< _ValueType, DefaultTool >,
        Instance< _ValueType, ImplicitTool > >;
};
template < typename _ValueType >
using Optimal = typename OptimalHelper< _ValueType >::type;

template < typename _ValueType, typename _ValueTool >
struct OptimalHelper< Instance< _ValueType, _ValueTool > >
{
    using type = Instance< _ValueType, _ValueTool >;
};

// true, если _TestType полностью покрывает свойства _OtherType
template < typename _TestType, typename _OtherType >
struct OptimalTraitChecker
    : ::std::integral_constant< bool, ( !is_instance< _OtherType > || ( is_instance< _OtherType > && is_instance< _TestType > ) ) >
{};

template < typename _TestType, typename _OtherType >
constexpr bool is_optimal = OptimalTraitChecker<_TestType, _OtherType >::value;

/*!
 * Класс выбирает тип Instance на основе сравнения свойств исходного и вложенного
 * типа Instance. Если исключение вложенного типа Instance не изменяет набор
 * необходимых свойств, то вложенный тип исключается. Если исключение исходного
 * тип Instance не изменяет набор необходимых свойств, то исходный тип исключается.
 */
template < typename _ValueType, typename _ValueTool, typename _RequaredTool >
struct OptimalHelper< Instance< Instance< _ValueType, _ValueTool >, _RequaredTool > >
{
    using FullType = Instance< Instance< _ValueType, _ValueTool >, _RequaredTool >;
    using ShortType = Instance< _ValueType, _RequaredTool >;
    using InnerType = Instance< _ValueType, _ValueTool >;

    using type = ::std::conditional_t<
        is_optimal< ShortType, InnerType >,
        Optimal< ShortType >,
            ::std::conditional_t<
                is_optimal< InnerType, ShortType >,
                Optimal< InnerType >,
                FullType > >;
};


template < typename _ValueType >
using OptimalType = Optimal< _ValueType >;
