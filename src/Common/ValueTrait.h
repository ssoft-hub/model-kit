#pragma once

#include <type_traits>

/*!
 * Класс для проверки того, что экземпляр данного типа специально
 * размещается в куче.
 */
template < typename _Value >
struct IsHeap
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_heap_v = IsHeap< _Value >::value;

/*!
 * Класс для проверки того, что экземпляр данного типа может быть опциональным
 * (не иметь никакого значения).
 */
template < typename _Value >
struct IsOptional
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_optional_v = IsOptional< _Value >::value;

/*!
 * Класс для проверки того, что экземпляр данного типа использует технику
 * неявного обобщение (отложенное копирование при доступе на изменение).
 */
template < typename _Value >
struct IsImplicit
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_implicit_v = IsImplicit< _Value >::value;

/*!
 * Класс для проверки того, что экземпляр данного типа является потокобезопасным.
 */
template < typename _Value >
struct IsThreadSafe
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_thread_safe_v = IsThreadSafe< _Value >::value;

/*!
 * Класс для проверки того, что экземпляр данного типа является контейнером.
 */
template < typename _Value >
struct IsContainer
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_container_v = IsContainer< _Value >::value;

/*!
 * Класс для проверки того, что экземпляр данного типа является
 * специализированной оберткой Instance.
 */
template < typename _Value >
struct IsInstance
    : public ::std::false_type
{};

template < typename _Value >
constexpr bool is_instance_v = IsInstance< _Value >::value;

/*!
 * Класс для проверки совместимости типов.
 * По умолчанию, типы являются совместимыми, если типы одинаковые или
 * проверяемый тип _TestType является производным от другого _OtherType.
 */
template < typename _TestType, typename _OtherType >
struct IsCompatible
    : public ::std::integral_constant< bool,
        ::std::is_same< _TestType, _OtherType >::value
        || ::std::is_base_of< _TestType, _OtherType >::value >
{};

template < typename _TestType, typename _OtherType >
constexpr bool is_compatible_v = IsCompatible< _TestType, _OtherType >::value;

/*!
 * Класс для проверки вложенности одного типа в другой.
 * По умолчанию, нет никакой вложенности.
 */
template < typename _ValueType, typename _ContainerType >
struct IsPartOf
    : public ::std::false_type
{};

template < typename _ValueType, typename _ContainerType >
constexpr bool is_part_of_v = IsPartOf< _ValueType, _ContainerType >::value;
