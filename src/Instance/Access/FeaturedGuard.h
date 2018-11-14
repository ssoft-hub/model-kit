#pragma once
#ifndef GUARD_FEATURE_POINTER_H
#define GUARD_FEATURE_POINTER_H

#include <ModelKit/Instance/Traits.h>
#include "ReferPointer.h"

// Предопределение
template < typename _Value, typename _Tool >
class Featured;

namespace Private
{
    template < typename _Refer >
    struct FeaturedGuardHelper;
}

/*!
 * Указатель на значение, к которому применено дополнительное свойство.
 */
template < typename _Refer >
using FeaturedGuard = typename Private::FeaturedGuardHelper< _Refer >::Type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Featured значения, к которому применена
     * особенность, реализуемая данным Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Featured.
     */
    template < typename _Refer >
    class DefaultFeaturedGuard
    {
        using ThisType = DefaultFeaturedGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;

    private:
        Pointer m_pointer;

    public:
        constexpr DefaultFeaturedGuard ()
            : m_pointer()
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultFeaturedGuard ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultFeaturedGuard ( ThisType && other )
            : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        constexpr bool operator ! () const
        {
            return !m_pointer;
        }

        constexpr Refer operator * () const
        {
            return ::std::forward< Refer >( *m_pointer );
        }

        constexpr const Pointer & operator -> () const
        {
            return m_pointer;
        }
    };
}

namespace Private
{
    /*!
     * Указатель на экземпляр вложенного в Featured значения, к которому применена
     * особенность, реализуемая данным Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам является Featured.
     */
    template < typename _Refer,
        // The template parameter must be a reference!
        typename = ::std::enable_if_t< ::std::is_reference< _Refer >::value > >
    class SpecialFeaturedGuard
    {
        using ThisType = SpecialFeaturedGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;
        using Featured = ::std::decay_t< Refer >;
        using Tool = typename Featured::Tool;
        using Holder = typename Featured::Holder;
        using HolderRefer = ::Similar< Holder, _Refer >;
        using Value = typename Featured::Value;
        using ValueRefer = ::Similar< Value, _Refer >;

    private:
        Pointer m_pointer;

    public:
        constexpr SpecialFeaturedGuard ()
            : m_pointer()
        {
        }

        SpecialFeaturedGuard ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
            Tool::guardHolder( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }

        SpecialFeaturedGuard ( ThisType && other )
            : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
        {
        }

        ~SpecialFeaturedGuard ()
        {
            if ( !!m_pointer )
                Tool::unguardHolder( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }

        constexpr bool operator ! () const
        {
            return !m_pointer;
        }

        constexpr Refer operator * () const
        {
            return ::std::forward< Refer >( *m_pointer );
        }

        constexpr const Pointer & operator -> () const
        {
            return m_pointer;
        }

        constexpr ValueRefer value () const
        {
            assert( m_pointer );
            return ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( m_pointer->m_holder ) ) );
        }
    };
}

namespace Private
{
    template < typename _Refer >
    struct FeaturedGuardHelper
    {
        using Type = Private::DefaultFeaturedGuard< _Refer >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialFeaturedGuard< Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialFeaturedGuard< Featured< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< const Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialFeaturedGuard< const Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< const Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialFeaturedGuard< const Featured< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< Featured< _Value, _Tool > > {};

    template < typename _Value, typename _Tool >
    struct FeaturedGuardHelper< const Featured< _Value, _Tool > > {};
}

#endif
