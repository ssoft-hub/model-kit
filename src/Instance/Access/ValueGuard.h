#pragma once
#ifndef GUARD_VALUE_POINTER_H
#define GUARD_VALUE_POINTER_H

#include <ModelKit/Instance/Access/FeaturedGuard.h>
#include <ModelKit/Instance/Traits.h>

// Предопределение
template < typename _Value, typename _Tool >
class Featured;

namespace Private
{
    template < typename _Refer >
    struct ValueGuardHelper;
}

/*!
 * Указатель на экземпляр вложенного в Featured базового значения, к которому применены
 * все особенности, реализуемые посредством используемых Featured.
 */
template < typename _Refer >
using ValueGuard = typename Private::ValueGuardHelper< _Refer >::Type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Featured базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Featured.
     */
    template < typename _Refer >
    class DefaultValueGuard
    {
        using ThisType = DefaultValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Guard = FeaturedGuard< _Refer >;
        using AccessRefer = Refer;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );
        static_assert( !::is_featured< ::std::decay_t< Refer > >, "The template parameter _Refer must be a not featured type reference!" );

    private:
        Guard m_featured_guard;

    public:
        constexpr DefaultValueGuard ()
            : m_featured_guard()
        {
        }

        DefaultValueGuard ( Refer refer )
            : m_featured_guard( ::std::forward< Refer >( refer ) )
        {
        }

        DefaultValueGuard ( Guard && other )
            : m_featured_guard( ::std::forward< Guard >( other ) )
        {
        }

        DefaultValueGuard ( ThisType && other )
            : m_featured_guard( ::std::forward< Guard >( other.m_featured_guard ) )
        {
        }

        constexpr bool operator ! () const
        {
            return !m_featured_guard;
        }

        constexpr AccessRefer operator * () const
        {
            return ::std::forward< AccessRefer >( *m_featured_guard );
        }

        constexpr const Guard & operator -> () const &
        {
            return m_featured_guard;
        }

        constexpr const Guard && operator -> () const &&
        {
            return ::std::forward< const Guard >( m_featured_guard );
        }
    };
}

namespace Private
{
    template < typename _Refer >
    struct ValueGuardHelper
    {
        using Type = DefaultValueGuard< _Refer >;
    };
}

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Featured базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам является Featured.
     */
    template < typename _Refer >
    class SpecialValueGuard

    {
        using ThisType = SpecialValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Featured = ::std::decay_t< Refer >;
        using Tool = typename Featured::Holder;

        using Value = typename Featured::Value;
        using ValueRefer = ::SimilarRefer< Value, Refer >;
        using Holder = typename Featured::Holder;
        using HolderRefer = ::SimilarRefer< Holder, Refer >;
        using FeaturedGuard = ::FeaturedGuard< Refer >;
        using ValueGuard = ::ValueGuard< ValueRefer >;
        using AccessRefer = typename ValueGuard::AccessRefer;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );
        static_assert( ::is_featured< Featured >, "The template parameter _Refer must be a featured type reference!" );
        static_assert( ::is_similar< Refer, ValueRefer >, "The Refer and ValueRefer must be similar types!" );
        static_assert( ::is_similar< Refer, HolderRefer >, "The Refer and HolderRefer must be similar types!" );

    private:
        FeaturedGuard m_featured_guard;
        ValueGuard m_value_guard;

    public:
        constexpr SpecialValueGuard ()
            : m_featured_guard()
            , m_value_guard()
        {
        }

        SpecialValueGuard ( _Refer refer )
            : m_featured_guard( ::std::forward< _Refer >( refer ) )
            , m_value_guard( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_featured_guard).m_holder ) ) ) )
        {
        }

        SpecialValueGuard ( FeaturedGuard && other )
            : m_featured_guard( ::std::forward< FeaturedGuard >( other ) )
            , m_value_guard( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_featured_guard).m_holder ) ) ) )
        {
        }

        SpecialValueGuard ( ThisType && other )
            : m_featured_guard( ::std::forward< FeaturedGuard >( other.m_featured_guard ) )
            , m_value_guard( ::std::forward< ValueGuard >( other.m_value_guard ) )
        {
        }

        constexpr bool operator ! () const
        {
            return !m_value_guard;
        }

        constexpr AccessRefer operator * () const
        {
            return ::std::forward< AccessRefer >( *m_value_guard );
        }

        constexpr const ValueGuard & operator -> () const &
        {
            return m_value_guard;
        }

        constexpr const ValueGuard && operator -> () const &&
        {
            return ::std::forward< const ValueGuard >( m_value_guard );
        }
    };
}

namespace Private
{
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialValueGuard< Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialValueGuard< Featured< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialValueGuard< const Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialValueGuard< const Featured< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< Featured< _Value, _Tool > > {};
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Featured< _Value, _Tool > > {};
}

#endif
