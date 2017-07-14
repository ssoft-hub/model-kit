#pragma once

#include <type_traits>

/*!
 * Класс для проверки того, что экземпляр данного типа специально
 * размещается в куче.
 */
template < typename _Value >
struct IsHeap
    : public ::std::false_type
{
};

/*!
 * Класс для проверки того, что экземпляр данного типа может быть опциональным
 * (не иметь никакого значения).
 */
template < typename _Value >
struct IsOptional
    : public ::std::false_type
{
};

/*!
 * Класс для проверки того, что экземпляр данного типа использует технику
 * неявного обобщение (отложенное копирование при доступе на изменение).
 */
template < typename _Value >
struct IsImplicit
    : public ::std::false_type
{
};

/*!
 * Класс для проверки того, что экземпляр данного типа является потокобезопасным.
 */
template < typename _Value >
struct IsAtomic
    : public ::std::false_type
{
};

/*!
 * Класс для проверки того, что экземпляр данного типа является контейнером.
 */
template < typename _Value >
struct IsContainer
    : public ::std::false_type
{
};

/*!
 * Класс для проверки того, что экземпляр данного типа является
 * специализированной оберткой Instance.
 */
template < typename _Value >
struct IsInstance
    : public ::std::false_type
{
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
 * Класс для проверки вложенности одного типа в другой.
 * По умолчанию, нет никакой вложенности.
 */
template < typename _ValueType, typename _ContainerType >
struct IsPartOf
    : public ::std::false_type
{
};
