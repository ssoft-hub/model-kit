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
using OptimalHelper_t = typename OptimalHelper< _ValueType >::type;

template < typename _ValueType, typename _ValueTool >
struct OptimalHelper< Instance< _ValueType, _ValueTool > >
{
    using type = Instance< _ValueType, _ValueTool >;
};

// true, если _TestType полностью покрывает свойства _OtherType
template < typename _TestType, typename _OtherType >
struct OptimalTraitChecker
    : ::std::integral_constant< bool,
        ( !IsInstance< _OtherType >::value || ( IsInstance< _OtherType >::value && IsInstance< _TestType >::value ) )
        && ( !IsHeap< _OtherType >::value || ( IsHeap< _OtherType >::value && IsHeap< _TestType >::value ) )
        && ( !IsOptional< _OtherType >::value || ( IsOptional< _OtherType >::value && IsOptional< _TestType >::value ) )
        && ( !IsImplicit< _OtherType >::value || ( IsImplicit< _OtherType >::value && IsImplicit< _TestType >::value ) )
        && ( !IsAtomic< _OtherType >::value || ( IsAtomic< _OtherType >::value && IsAtomic< _TestType >::value ) ) >

{
};

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
        OptimalTraitChecker< ShortType, InnerType >::value,
        OptimalHelper_t< ShortType >,
            ::std::conditional_t<
                OptimalTraitChecker< InnerType, ShortType >::value,
                OptimalHelper_t< InnerType >,
                FullType > >;
};


template < typename _ValueType >
using OptimalType = OptimalHelper_t< _ValueType >;
