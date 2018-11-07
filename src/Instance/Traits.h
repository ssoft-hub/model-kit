#pragma once
#ifndef FEATURED_TRAITS_H
#define FEATURED_TRAITS_H

#include <type_traits>

// Предварительная декларация Featured
template < typename _Test, typename _Tool > class Featured;

namespace Private
{
    template < typename _Test > struct IsFeaturedHelper;
    template < typename _Test, typename _Other > struct IsCompatibleHelper;
    template < typename _Test, typename _Other > struct IsThisPartOfOtherHelper;
}

/*!
 * Признак того, что экземпляр данного типа является Featured.
 */
template < typename _Test >
constexpr bool is_featured = Private::IsFeaturedHelper< _Test >::value;

/*!
 * Признак совместимости типов.
 */
template < typename _Test, typename _Other >
constexpr bool is_compatible = Private::IsCompatibleHelper< _Test, _Other >::value;

/*!
 * Признак вложенности одного типа в другой.
 */
template < typename _Test, typename _Other >
constexpr bool is_this_part_of_other = Private::IsThisPartOfOtherHelper< _Test, _Other >::value;

////////////////////////////////////////////////////////////////////////////////

namespace Private
{
    /*!
     * По умолчанию, типы не являются Featured.
     */
    template < typename _Test >
    struct IsFeaturedHelper
        : public ::std::false_type
    {};

    /*!
     * Типы Featured являются Featured.
     */
    template < typename _Test, typename _Tool >
    struct IsFeaturedHelper< Featured< _Test, _Tool > >
        : public ::std::true_type
    {};
}

namespace Private
{
    /*!
     * По умолчанию, типы являются совместимыми, если они одинаковые или
     * проверяемый тип _Test является производным от другого _Other.
     */
    template < typename _Test, typename _Other >
    struct IsCompatibleHelper
        : public ::std::integral_constant< bool,
               ::std::is_same< _Test, _Other >::value
            || ::std::is_base_of< _Test, _Other >::value >
    {};

    /*!
     * Типы Featured являются совместимыми, если в них используется идентичный
     * инструмент _Tool, и вложенные типы также являются совместимыми.
     */
    template < typename _This, typename _Other, typename _Tool >
    struct IsCompatibleHelper< Featured< _This, _Tool >, Featured< _Other, _Tool > >
        : public ::std::integral_constant< bool, is_compatible< _This, _Other > >
    {};
}

namespace Private
{
    /*!
     * По умолчанию, типы, не являющиеся Featured, не могут быть вложенными.
     */
    template < typename _Test, typename _Other >
    struct IsThisPartOfOtherHelper
        : public ::std::false_type
    {};

    /*!
     * Один тип Featured является частью другого, если он совместим с любой
     * вложенной частью другого.
     */
    template < typename _Test, typename _Tool, typename _Other, typename _OtherTool >
    struct IsThisPartOfOtherHelper< Featured< _Test, _Tool >, Featured< _Other, _OtherTool > >
        : public ::std::integral_constant< bool,
               is_compatible< Featured< _Test, _Tool >, _Other >
            || is_this_part_of_other< Featured< _Test, _Tool >, _Other > >
    {};
}

#endif