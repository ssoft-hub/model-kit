#pragma once

#include <type_traits>

/*!
 * Класс для проверки того, что экземпляр данного типа является
 * специализированной оберткой Instance.
 */
template < typename _Value >
struct IsInstanceHelper
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_instance = IsInstanceHelper< _Value >::value;

/*!
 * Класс для проверки совместимости типов.
 * По умолчанию, типы являются совместимыми, если типы одинаковые или
 * проверяемый тип _TestType является производным от другого _OtherType.
 */
template < typename _TestType, typename _OtherType >
struct IsCompatibleHelper
    : public ::std::integral_constant< bool,
           ::std::is_same< _TestType, _OtherType >::value
        || ::std::is_base_of< _TestType, _OtherType >::value >
{};

template < typename _TestType, typename _OtherType >
constexpr bool is_compatible = IsCompatibleHelper< _TestType, _OtherType >::value;

/*!
 * Класс для проверки вложенности одного типа в другой.
 * По умолчанию, нет никакой вложенности.
 */
template < typename _ValueType, typename _ContainerType >
struct IsThisPartOfOtherHelper
    : public ::std::false_type
{};

template < typename _ValueType, typename _ContainerType >
constexpr bool is_this_part_of_other = IsThisPartOfOtherHelper< _ValueType, _ContainerType >::value;
