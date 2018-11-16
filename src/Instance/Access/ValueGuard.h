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
    template < typename _Refer,
        // The template parameter must be a reference!
        typename = ::std::enable_if_t< ::std::is_reference< _Refer >::value > >
    class DefaultValueGuard
    {
        using ThisType = DefaultValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Guard = FeaturedGuard< _Refer >;
        using Access = Refer;

    private:
        Guard m_featured_guard;

    public:
        constexpr DefaultValueGuard ()
            : m_featured_guard()
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValueGuard ( Refer refer )
            : m_featured_guard( ::std::forward< Refer >( refer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValueGuard ( Guard && other )
            : m_featured_guard( ::std::forward< Guard >( other ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValueGuard ( ThisType && other )
            : m_featured_guard( ::std::forward< Guard >( other.m_featured_guard ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        constexpr bool operator ! () const
        {
            return !m_featured_guard;
        }

        constexpr Access operator * () const
        {
            return ::std::forward< Access >( *m_featured_guard );
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
     * тип вложенного экземпляра значения сам не является Featured.
     */
    template < typename _Refer,
        // The template parameter must be a reference!
        typename = ::std::enable_if_t< ::std::is_reference< _Refer >::value > >
    class SpecialValueGuard

    {
        using ThisType = SpecialValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Featured = ::std::decay_t< Refer >;
        using Tool = typename Featured::Tool;

        using Value = typename Featured::Value;
        using ValueRefer = ::Similar< Value, Refer >;
        using Holder = typename Featured::Holder;
        using HolderRefer = ::Similar< Holder, Refer >;
        using FeaturedGuard = ::FeaturedGuard< Refer >;
        using ValueGuard = ::ValueGuard< ValueRefer >;
        using Access = typename ValueGuard::Access;

    private:
        FeaturedGuard m_featured_guard;
        ValueGuard m_value_guard;

    public:
        constexpr SpecialValueGuard ()
            : m_featured_guard()
            , m_value_guard()
        {
        }

        SpecialValueGuard ( Refer refer )
            : m_featured_guard( ::std::forward< Refer >( refer ) )
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

        constexpr Access operator * () const
        {
            return ::std::forward< Access >( *m_value_guard );
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
