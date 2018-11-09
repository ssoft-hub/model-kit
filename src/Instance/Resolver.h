#pragma once
#ifndef INSTANCE_RESOLVER_H
#define INSTANCE_RESOLVER_H

#include "Access/FeaturedPointer.h"
#include "Access/ValuePointer.h"
#include "Traits.h"

namespace Private
{
    template < typename _Featured, typename _OtherRefer >
    struct FeaturedResolverHelper;
}

/*!
 * Вычислитель необходимого способа предоставления доступа к экземпляру значения
 * посредством метода resolve().
 *
 * Если типы _Featured и _Other между собой являются совместимыми,
 * или _Other совместим с любой внутренней частью _Featured,
 *
 * то операция выполняется между внутренними представлениями Featured.
 * Если данный тип совместим с внутренней частью другого, то другой
 * "разрешается" до типа совместимого с этим. Во всех других случаях
 * другой "разрешается" до базового внутреннего значения.
 */
template < typename _Featured, typename _OtherRefer >
using FeaturedResolver = typename Private::FeaturedResolverHelper< _Featured, _OtherRefer >::Type;

namespace Private
{
    template < typename _Featured, typename _OtherRefer >
    class FeaturedCompatibleResolver;

    template < typename _Featured, typename _OtherRefer >
    class FeaturedFeatureResolver;

    template < typename _Featured, typename _OtherRefer >
    class FeaturedValueResolver;
}

namespace Private
{
    template < typename _Featured, typename _OtherRefer >
    struct FeaturedResolverHelper
    {
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeatured = ::std::decay_t< _OtherRefer >;

        using Type = ::std::conditional_t<
            is_compatible< Featured, OtherFeatured >
                || is_this_part_of_other< OtherFeatured, Featured >,
            FeaturedCompatibleResolver< Featured, OtherRefer >,
            ::std::conditional_t<
                is_this_part_of_other< Featured, OtherFeatured >,
                FeaturedFeatureResolver< Featured, OtherRefer >,
                FeaturedValueResolver< Featured, OtherRefer > > >;
    };
}


namespace Private
{
    /*!
     * Используется, если _Other совместим с _Featured или вложенной частью _Featured.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedCompatibleResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeatured = ::std::remove_reference_t< OtherRefer >;
        using OtherHolder = typename OtherFeatured::Holder;
        using OtherHolderRefer = ::Similar< OtherHolder, OtherRefer >;

    private:
        OtherRefer m_other_refer;

    public:
        FeaturedCompatibleResolver ( OtherRefer other )
            : m_other_refer( ::std::forward< OtherRefer >( other ) )
        {
        }

        OtherHolderRefer resolve () const
        {
            return ::std::forward< OtherHolderRefer >( m_other_refer.m_holder );
        }
    };

}

namespace Private
{
    /*!
     * Используется, если _Featured совместим с вложенной частью _Other.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedFeatureResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeaturedPointer = FeaturedPointer< OtherRefer >;
        using OtherFeatured = ::std::remove_reference_t< OtherRefer >;
        using OtherValue = typename OtherFeatured::Value;
        using OtherValueRefer = ::Similar< OtherValue, OtherRefer >;
        using NextResolver = FeaturedResolver< _Featured, OtherValueRefer >;

    private:
        OtherFeaturedPointer m_featured_pointer;
        NextResolver m_next_resolver;

    public:
        FeaturedFeatureResolver ( OtherRefer other )
            : m_featured_pointer( ::std::forward< OtherRefer >( other ) )
            , m_next_resolver( ::std::forward< OtherValueRefer >( m_featured_pointer.value() ) )
        {
        }

        decltype(auto) resolve () const
        {
            return m_next_resolver.resolve();
        }
    };
}

namespace Private
{
    /*!
     * Используется, если _Featured не совместим с _Other.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedValueResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherValuePointer = ValuePointer< OtherRefer >;

    private:
        OtherValuePointer m_value_guard;

    public:
        FeaturedValueResolver ( OtherRefer other )
            : m_value_guard( ::std::forward< OtherRefer >( other ) )
        {
        }

        decltype(auto) resolve () const
        {
            return *m_value_guard;
        }
    };
}

#endif
