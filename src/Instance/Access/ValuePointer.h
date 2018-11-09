#pragma once
#ifndef GUARD_VALUE_POINTER_H
#define GUARD_VALUE_POINTER_H

#include <ModelKit/Instance/Access/FeaturedPointer.h>

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
using ValuePointer = typename Private::ValuePointerHelper< _Refer >::type;

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
        using type = DefaultValuePointer< _Refer >;
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
        using Featured = ::std::remove_reference_t< Refer >;
        using Tool = typename Featured::Tool;

        using ValueRefer = ::std::conditional_t<
            ::std::is_const< Featured >::value,
            ::std::conditional_t<
                ::std::is_rvalue_reference< Refer >::value,
                const typename Featured::Value &&,
                const typename Featured::Value & >,
            ::std::conditional_t<
                ::std::is_rvalue_reference< Refer >::value,
                typename Featured::Value &&,
                typename Featured::Value & > >;

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

        using FeaturedPointer = FeaturedPointer< Refer >;
        using InternalPointer = ValuePointer< ValueRefer >;
        using Access = typename InternalPointer::Access;

    private:
        FeaturedPointer m_featured_pointer;
        InternalPointer m_internal_pointer;

    public:
        constexpr SpecialValuePointer ()
            : m_featured_pointer()
            , m_internal_pointer()
        {
        }

        SpecialValuePointer ( Refer refer )
            : m_featured_pointer( ::std::forward< Refer >( refer ) )
            , m_internal_pointer( Tool::value( ::std::forward< HolderRefer >( (*m_featured_pointer).m_holder ) ) )
        {
        }

        SpecialValuePointer ( FeaturedPointer && other )
            : m_featured_pointer( ::std::forward< FeaturedPointer >( other ) )
            , m_internal_pointer( Tool::value( ::std::forward< HolderRefer >( (*m_featured_pointer).m_holder ) ) )
        {
        }

        SpecialValuePointer ( ThisType && other )
            : m_featured_pointer( ::std::forward< FeaturedPointer >( other.m_featured_pointer ) )
            , m_internal_pointer( ::std::forward< InternalPointer >( other.m_internal_pointer ) )
        {
        }

        constexpr bool operator ! () const
        {
            return !m_internal_pointer;
        }

        constexpr Access operator * () const
        {
            return ::std::forward< Access >( *m_internal_pointer );
        }

        constexpr const InternalPointer & operator -> () const
        {
            return m_internal_pointer;
        }
    };
}

namespace Private
{
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > & >
    {
        using type = Private::SpecialValuePointer< Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > && >
    {
        using type = Private::SpecialValuePointer< Featured< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > & >
    {
        using type = Private::SpecialValuePointer< const Featured< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > && >
    {
        using type = Private::SpecialValuePointer< const Featured< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< Featured< _Value, _Tool > > {};
    template < typename _Value, typename _Tool >
    struct ValuePointerHelper< const Featured< _Value, _Tool > > {};
}

#endif
