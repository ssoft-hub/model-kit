#pragma once
#ifndef GUARD_FEATURE_POINTER_H
#define GUARD_FEATURE_POINTER_H

#include "ReferPointer.h"

// Предопределение
template < typename _Value, typename _Tool >
class Featured;

namespace Private
{
    template < typename _Refer >
    struct FeaturedPointerHelper;
}

/*!
 * Указатель на значение, к которому применено дополнительное свойство.
 */
template < typename _Refer >
using FeaturedPointer = typename Private::FeaturedPointerHelper< _Refer >::type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Featured значения, к которому применена
     * особенность, реализуемая данным Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Featured.
     */
    template < typename _Refer >
    class DefaultFeaturedPointer
    {
        using ThisType = DefaultFeaturedPointer< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;

    private:
        Pointer m_pointer;

    public:
        constexpr DefaultFeaturedPointer ()
            : m_pointer()
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultFeaturedPointer ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultFeaturedPointer ( ThisType && other )
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
    class SpecialFeaturedPointer
    {
        using ThisType = SpecialFeaturedPointer< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;
        using Featured = ::std::remove_reference_t< Refer >;
        using FeaturedTool = typename Featured::Tool;

        using HolderRefer = ::std::conditional_t<
            ::std::is_const< Featured >::value,
            ::std::conditional_t<
                ::std::is_rvalue_reference< Refer >::value,
                const typename Featured::Holder &&,
                const typename Featured::Holder & >,
            ::std::conditional_t<
                ::std::is_rvalue_reference< Refer >::value,
                typename Featured::Holder &&,
                typename Featured::Holder & > >;

    private:
        Pointer m_pointer;

    public:
        constexpr SpecialFeaturedPointer ()
            : m_pointer()
        {
        }

        SpecialFeaturedPointer ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
            FeaturedTool::guardHolder( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }

        SpecialFeaturedPointer ( ThisType && other )
            : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
        {
        }

        ~SpecialFeaturedPointer ()
        {
            if ( !!m_pointer )
                FeaturedTool::unguardHolder( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
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

        constexpr decltype(auto) value () const
        {
            assert( m_pointer );
            return FeaturedTool::value( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }
    };
}

namespace Private
{
    template < typename _Refer >
    struct FeaturedPointerHelper
    {
        using type = Private::DefaultFeaturedPointer< _Refer >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< Featured< _Value, _Tool > & >
    {
        using type = Private::SpecialFeaturedPointer< Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< Featured< _Value, _Tool > && >
    {
        using type = Private::SpecialFeaturedPointer< Featured< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< const Featured< _Value, _Tool > & >
    {
        using type = Private::SpecialFeaturedPointer< const Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< const Featured< _Value, _Tool > && >
    {
        using type = Private::SpecialFeaturedPointer< const Featured< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< Featured< _Value, _Tool > > {};

    template < typename _Value, typename _Tool >
    struct FeaturedPointerHelper< const Featured< _Value, _Tool > > {};
}

#endif
