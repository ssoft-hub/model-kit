#pragma once
#ifndef INSTANCE_RESOLVER_H
#define INSTANCE_RESOLVER_H

#include "Access/FeaturedGuard.h"
#include "Access/ValueGuard.h"
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
    class FeaturedOtherPathOfThisResolver;

    template < typename _Featured, typename _OtherRefer >
    class FeaturedThisPathOfOtherResolver;

    template < typename _Featured, typename _OtherRefer >
    class FeaturedValueResolver;
}

/*
 * NOTE: Можно было бы реализовать случай преобразования одного вида Holder
 * в другой с помощью FeaturedConvertResolver. Однако это нарушит инкапсуляцию
 * Holder и позволит получить доступ к его внутреннему содержимому,
 * что в свою очередь может изменить ожидаемое поведение.
 */
namespace Private
{
    template < typename _Featured, typename _OtherRefer >
    struct FeaturedResolverHelper
    {
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeatured = ::std::decay_t< _OtherRefer >;

        using Type = ::std::conditional_t<
            is_compatible< Featured, OtherFeatured >,
            FeaturedCompatibleResolver< Featured, OtherRefer >,
            ::std::conditional_t<
                is_this_part_of_other< OtherFeatured, Featured >,
                FeaturedOtherPathOfThisResolver< Featured, OtherRefer >,
                ::std::conditional_t<
                    is_this_part_of_other< Featured, OtherFeatured >,
                    FeaturedThisPathOfOtherResolver< Featured, OtherRefer >,
                    FeaturedValueResolver< Featured, OtherRefer > > > >;
    };
}

namespace Private
{
    /*!
     * Используется, если _Other совместим с _Featured.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedCompatibleResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeatured = ::std::decay_t< OtherRefer >;
        using OtherHolder = typename OtherFeatured::Holder;
        using OtherHolderRefer = ::SimilarRefer< OtherHolder, OtherRefer >;
        using AccessRefer = OtherHolderRefer;

    private:
        OtherRefer m_other_refer;

    public:
        FeaturedCompatibleResolver ( OtherRefer other )
            : m_other_refer( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_other_refer.m_holder );
        }
    };
}

namespace Private
{
    /*!
     * Используется, если _Other совместим c вложенной частью _Featured.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedOtherPathOfThisResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using AccessRefer = OtherRefer;

    private:
        OtherRefer m_other_refer;

    public:
        FeaturedOtherPathOfThisResolver ( OtherRefer other )
            : m_other_refer( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_other_refer );
        }
    };
}

namespace Private
{
    /*!
     * Используется, если _Featured совместим с вложенной частью _Other.
     */
    template < typename _Featured, typename _OtherRefer >
    class FeaturedThisPathOfOtherResolver
    {
    public:
        using Featured = _Featured;
        using OtherRefer = _OtherRefer;
        using OtherFeaturedGuard = FeaturedGuard< OtherRefer >;
        using OtherFeatured = ::std::decay_t< OtherRefer >;
        using OtherValue = typename OtherFeatured::Value;
        using OtherValueRefer = ::SimilarRefer< OtherValue, OtherRefer >;
        using NextResolver = FeaturedResolver< Featured, OtherValueRefer >;
        using AccessRefer = typename NextResolver::AccessRefer;

        static_assert( ::is_featured< Featured >, "The template parameter _Featured must be a featured!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must be a reference!" );
        static_assert( ::is_featured< OtherFeatured >, "The template parameter _OtherRefer must be a featured type reference!" );
        static_assert( ::is_similar< OtherRefer, OtherValueRefer >, "The OtherRefer and OtherValueRefer must be similar types!" );

    private:
        OtherFeaturedGuard m_featured_pointer;
        NextResolver m_next_resolver;

    public:
        FeaturedThisPathOfOtherResolver ( OtherRefer other )
            : m_featured_pointer( ::std::forward< OtherRefer >( other ) )
            , m_next_resolver( ::std::forward< OtherValueRefer >( m_featured_pointer.value() ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_next_resolver.resolve() );
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
        using OtherValueGuard = ValueGuard< OtherRefer >;
        using AccessRefer = typename OtherValueGuard::AccessRefer;

        static_assert( ::is_featured< Featured >, "The template parameter _Featured must be a featured!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must be a reference!" );

    private:
        OtherValueGuard m_value_guard;

    public:
        FeaturedValueResolver ( OtherRefer other )
            : m_value_guard( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( *m_value_guard );
        }
    };
}

#endif
