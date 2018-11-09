#pragma once
#ifndef GUARD_VALUE_POINTER_H
#define GUARD_VALUE_POINTER_H

#include <ModelKit/Instance/Access/FeaturedPointer.h>
#include <ModelKit/Instance/Traits.h>

// Предопределение
template < typename _Value, typename _Tool >
class Featured;

namespace Private
{
    template < typename _Refer >
    struct ValuePointerHelper;
}

/*!
 * Указатель на экземпляр вложенного в Featured базового значения, к которому применены
 * все особенности, реализуемые посредством используемых Featured.
 */
template < typename _Refer >
using ValuePointer = typename Private::ValuePointerHelper< _Refer >::Type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Featured базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Featured. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Featured.
     */
    template < typename _Refer >
    class DefaultValuePointer
    {
        using ThisType = DefaultValuePointer< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = FeaturedPointer< _Refer >;
        using Access = Refer;

    private:
        Pointer m_featured_pointer;

    public:
        constexpr DefaultValuePointer ()
            : m_featured_pointer()
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValuePointer ( Refer refer )
            : m_featured_pointer( ::std::forward< Refer >( refer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValuePointer ( Pointer && other )
            : m_featured_pointer( ::std::forward< Pointer >( other ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        DefaultValuePointer ( ThisType && other )
            : m_featured_pointer( ::std::forward< Pointer >( other.m_featured_pointer ) )
        {
            static_assert( ::std::is_reference< Refer >::value
                , "The template parameter must be a reference." );
        }

        constexpr bool operator ! () const
        {
            return !m_featured_pointer;
        }

        constexpr Access operator * () const
        {
            return ::std::forward< Access >( *m_featured_pointer );
        }

        constexpr const Pointer & operator -> () const
        {
            return m_featured_pointer;
        }
    };
}

namespace Private
{
    template < typename _Refer >
    struct ValuePointerHelper
    {
        using Type = DefaultValuePointer< _Refer >;
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
    class SpecialValuePointer

    {
        using ThisType = SpecialValuePointer< _Refer >;

    public:
        using Refer = _Refer;
        using Featured = ::std::decay_t< Refer >;
        using Tool = typename Featured::Tool;

        using Value = typename Featured::Value;
        using ValueRefer = ::Similar< Value, Refer >;
        using Holder = typename Featured::Holder;
        using HolderRefer = ::Similar< Holder, Refer >;
        using FeaturedPointer = ::FeaturedPointer< Refer >;
        using ValuePointer = ::ValuePointer< ValueRefer >;
        using Access = typename ValuePointer::Access;

    private:
        FeaturedPointer m_featured_pointer;
        ValuePointer m_value_pointer;

    public:
        constexpr SpecialValuePointer ()
            : m_featured_pointer()
            , m_value_pointer()
        {
        }

        SpecialValuePointer ( Refer refer )
            : m_featured_pointer( ::std::forward< Refer >( refer ) )
            , m_value_pointer( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_featured_pointer).m_holder ) ) ) )
        {
        }

        SpecialValuePointer ( FeaturedPointer && other )
            : m_featured_pointer( ::std::forward< FeaturedPointer >( other ) )
            , m_value_pointer( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_featured_pointer).m_holder ) ) ) )
        {
        }

        SpecialValuePointer ( ThisType && other )
            : m_featured_pointer( ::std::forward< FeaturedPointer >( other.m_featured_pointer ) )
            , m_value_pointer( ::std::forward< ValuePointer >( other.m_value_pointer ) )
        {
        }

        constexpr bool operator ! () const
        {
            return !m_value_pointer;
        }

        constexpr Access operator * () const
        {
            return ::std::forward< Access >( *m_value_pointer );
        }

        constexpr const ValuePointer & operator -> () const
        {
            return m_value_pointer;
        }
    };
}

namespace Private
{
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialValuePointer< Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialValuePointer< Featured< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > & >
    {
        using Type = Private::SpecialValuePointer< const Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > && >
    {
        using Type = Private::SpecialValuePointer< const Featured< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > > {};
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > > {};
}

#endif
