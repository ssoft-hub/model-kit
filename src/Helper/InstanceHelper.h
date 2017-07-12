#pragma once
#ifndef INSTANCE_HELPER_H
#define INSTANCE_HELPER_H

#include <Helper/ValueTrait.h>

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

template < typename _ValueType, typename _ValueTool >
struct InstanceHelper< Instance< _ValueType, _ValueTool > >
{
    using type = Instance< _ValueType, _ValueTool >;
};

// true, если _CheckType полностью покрывает свойства _OtherType
template < typename _CheckType, typename _OtherType >
struct TraitChecker
    : ::std::integral_constant< bool,
        ( !IsInstance< _OtherType >::value || ( IsInstance< _OtherType >::value && IsInstance< _CheckType >::value ) )
        && ( !IsHeap< _OtherType >::value || ( IsHeap< _OtherType >::value && IsHeap< _CheckType >::value ) )
        && ( !IsOptional< _OtherType >::value || ( IsOptional< _OtherType >::value && IsOptional< _CheckType >::value ) )
        && ( !IsImplicit< _OtherType >::value || ( IsImplicit< _OtherType >::value && IsImplicit< _CheckType >::value ) )
        && ( !IsAtomic< _OtherType >::value || ( IsAtomic< _OtherType >::value && IsAtomic< _CheckType >::value ) ) >

{
};


/*!
 * Класс выбирает тип Instance на основе сравнения свойств исходного и вложенного
 * типа Instance. Если исключение вложенного типа Instance не изменяет набор
 * необходимых свойств, то вложенный тип исключается. Если исключение исходного
 * тип Instance не изменяет набор необходимых свойств, то исходный тип исключается.
 */
template < typename _ValueType, typename _ValueTool, typename _RequaredTool >
struct InstanceHelper< Instance< Instance< _ValueType, _ValueTool >, _RequaredTool > >
{
    using FullType = Instance< Instance< _ValueType, _ValueTool >, _RequaredTool >;
    using ShortType = Instance< _ValueType, _RequaredTool >;
    using InnerType = Instance< _ValueType, _ValueTool >;

    using type = typename ::std::conditional<
        TraitChecker< ShortType, InnerType >::value,
        typename InstanceHelper< ShortType >::type,
            typename ::std::conditional<
                TraitChecker< InnerType, ShortType >::value,
                typename InstanceHelper< InnerType >::type,
                FullType
            >::type
        >::type;
};

#endif
