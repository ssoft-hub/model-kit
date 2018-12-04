#pragma once
#ifndef FEATURED_TRAITS_H
#define FEATURED_TRAITS_H

#include <type_traits>

// Предопределение
template < typename _Value, typename _Tool > class Instance;
template < typename _Test > struct IsInstance;
template < typename _Test, typename _Other > struct IsCompatible;
template < typename _Test, typename _Other > struct IsThisPartOfOther;
template < typename _Type, typename _Other > struct IsSimilar;

namespace Private
{
    template < typename _Type, typename _Refer > struct SimilarReferHelper;
}

/*!
 * Признак того, что экземпляр данного типа является Instance.
 */
template < typename _Test >
static constexpr bool is_instance = ::IsInstance< _Test >::value;

/*!
 * Признак совместимости типов.
 */
template < typename _Test, typename _Other >
static constexpr bool is_compatible = ::IsCompatible< _Test, _Other >::value;

/*!
 * Признак вложенности одного типа в другой.
 */
template < typename _Test, typename _Other >
static constexpr bool is_this_part_of_other = ::IsThisPartOfOther< _Test, _Other >::value;

/*!
 * Признак подобия типов. Типы считаются подобными, если у них одинаковые
 * признаки const/volatile и rvalue/lvalue.
 */
template < typename _Type, typename _Other >
static constexpr bool is_similar = IsSimilar< _Type, _Other >::value;

/*!
 * Наделение типа свойствами ссылки.
 */
template < typename _Type, typename _Refer >
using SimilarRefer = typename Private::SimilarReferHelper< _Type, _Refer >::Type;

template < typename _Type, typename _Other >
struct IsSimilar
    : public ::std::integral_constant< bool,
           ::std::is_rvalue_reference< _Type >::value == ::std::is_rvalue_reference< _Other >::value
        && ::std::is_lvalue_reference< _Type >::value == ::std::is_lvalue_reference< _Other >::value
        && ::std::is_const< _Type >::value == ::std::is_const< _Other >::value
        && ::std::is_volatile< _Type >::value == ::std::is_volatile< _Other >::value >
{
};

////////////////////////////////////////////////////////////////////////////////

/*!
 * По умолчанию, типы не являются Instance.
 */
template < typename _Test >
struct IsInstance
    : public ::std::false_type
{};

/*!
 * Типы Instance являются Instance.
 */
template < typename _Test, typename _Tool >
struct IsInstance< Instance< _Test, _Tool > >
    : public ::std::true_type
{};

/*!
 * По умолчанию, типы являются совместимыми, если они одинаковые или
 * проверяемый тип _Test является производным от другого _Other.
 */
template < typename _Test, typename _Other >
struct IsCompatible
    : public ::std::integral_constant< bool,
           ::std::is_same< _Test, _Other >::value
        || ::std::is_base_of< _Test, _Other >::value >
{};

/*!
 * Типы Instance являются совместимыми, если в них используется идентичный
 * инструмент _Tool, и вложенные типы также являются совместимыми.
 */
template < typename _This, typename _Other, typename _Tool >
struct IsCompatible< Instance< _This, _Tool >, Instance< _Other, _Tool > >
    : public ::std::integral_constant< bool, is_compatible< _This, _Other > >
{};

/*!
 * По умолчанию, типы, не являющиеся Instance, не могут быть вложенными.
 */
template < typename _Test, typename _Other >
struct IsThisPartOfOther
    : public ::std::false_type
{};

/*!
 * Один тип Instance является частью другого, если он совместим с любой
 * вложенной частью другого.
 */
template < typename _Test, typename _Tool, typename _Other, typename _OtherTool >
struct IsThisPartOfOther< Instance< _Test, _Tool >, Instance< _Other, _OtherTool > >
    : public ::std::integral_constant< bool,
           is_compatible< Instance< _Test, _Tool >, _Other >
        || is_this_part_of_other< Instance< _Test, _Tool >, _Other > >
{};

namespace Private
{
    template < typename _Type, typename _Refer >
    struct SimilarReferHelper
    {
        static_assert( !::std::is_reference< _Type >::value, "The template parameter _Type must be a not reference." );
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must be a reference." );

        using EtalonValue = ::std::remove_reference_t< _Refer >;
        static constexpr bool is_lvalue = ::std::is_lvalue_reference< _Refer >::value;
        static constexpr bool is_rvalue = ::std::is_rvalue_reference< _Refer >::value;
        static constexpr bool is_const = ::std::is_const< EtalonValue >::value;
        static constexpr bool is_volitile = ::std::is_volatile< EtalonValue >::value;

        using Type = ::std::conditional_t< is_volitile,
            ::std::conditional_t< is_rvalue,
                ::std::add_rvalue_reference_t< ::std::add_volatile_t< _Type > >,
                ::std::conditional_t < is_lvalue,
                    ::std::add_lvalue_reference_t< ::std::add_volatile_t< _Type > >,
                    ::std::add_volatile_t< _Type > > >,
            ::std::conditional_t< is_const,
                ::std::conditional_t< is_rvalue,
                    ::std::add_rvalue_reference_t< ::std::add_const_t< _Type > >,
                    ::std::conditional_t < is_lvalue,
                        ::std::add_lvalue_reference_t< ::std::add_const_t< _Type > >,
                        ::std::add_const_t< _Type > > >,
                ::std::conditional_t< is_rvalue,
                    ::std::add_rvalue_reference_t< _Type >,
                    ::std::conditional_t< is_lvalue,
                        ::std::add_lvalue_reference_t< _Type >,
                        _Type > > > >;
    };
}

#endif
