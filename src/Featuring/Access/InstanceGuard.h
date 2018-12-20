#pragma once
#ifndef GUARD_FEATURE_POINTER_H
#define GUARD_FEATURE_POINTER_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Traits.h>
#include "ReferPointer.h"

// Предопределение
template < typename _Value, typename _Tool >
class Instance;

namespace Private
{
    template < typename _Refer >
    struct InstanceGuardHelper;
}

/*!
 * Указатель на значение, к которому применено дополнительное свойство.
 */
template < typename _Refer >
using InstanceGuard = typename Private::InstanceGuardHelper< _Refer >::Type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Instance значения, к которому применена
     * особенность, реализуемая данным Instance. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Instance.
     */
    template < typename _Refer >
    class DefaultInstanceGuard
    {
        using ThisType = DefaultInstanceGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be of reference type." );
        static_assert( !::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must to be a not Instance type reference!" );

    private:
        Pointer m_pointer;

    public:
        /*constexpr*/ DefaultInstanceGuard ()
            : m_pointer()
        {
        }

        DefaultInstanceGuard ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
        }

        DefaultInstanceGuard ( ThisType && other )
            : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
        {
        }

        /*constexpr*/ bool operator ! () const
        {
            return !m_pointer;
        }

        /*constexpr*/ Refer operator * () const
        {
            return ::std::forward< Refer >( *m_pointer );
        }

        /*constexpr*/ const Pointer & operator -> () const
        {
            return m_pointer;
        }
    };
}

namespace Private
{
    /*!
     * Указатель на экземпляр вложенного в Instance значения, к которому применена
     * особенность, реализуемая данным Instance. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам является Instance.
     */
    template < typename _Refer >
    class SpecialInstanceGuard
    {
        using ThisType = SpecialInstanceGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Pointer = ReferPointer< Refer >;
        using Instance = ::std::decay_t< Refer >;
        using Tool = typename Instance::Holder;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::SimilarRefer< Holder, Refer >;
        using Value = typename Instance::Value;
        using ValueRefer = ::SimilarRefer< Value, Refer >;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be of reference type." );
        static_assert( ::is_instance< Instance >, "The template parameter _Refer must to be a Instance type reference!" );
        static_assert( ::is_similar< ValueRefer, Refer >, "The Refer and ValueRefer must to be similar types!" );
        static_assert( ::is_similar< HolderRefer, Refer >, "The Refer and HolderRefer must to be similar types!" );

    private:
        Pointer m_pointer;

    public:
        /*constexpr*/ SpecialInstanceGuard ()
            : m_pointer()
        {
        }

        SpecialInstanceGuard ( Refer refer )
            : m_pointer( ::std::forward< Refer >( refer ) )
        {
            ::HolderInternal::guard< HolderRefer >( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }

        SpecialInstanceGuard ( ThisType && other )
            : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
        {
        }

        ~SpecialInstanceGuard ()
        {
            if ( !!m_pointer )
                ::HolderInternal::unguard< HolderRefer >( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }

        /*constexpr*/ bool operator ! () const
        {
            return !m_pointer;
        }

        /*constexpr*/ Refer operator * () const
        {
            return ::std::forward< Refer >( *m_pointer );
        }

        /*constexpr*/ const Pointer & operator -> () const
        {
            return m_pointer;
        }

        /*constexpr*/ ValueRefer value () const
        {
            assert( m_pointer );
            return ::HolderInternal::value< ValueRefer, HolderRefer >( ::std::forward< HolderRefer >( m_pointer->m_holder ) );
        }
    };
}

namespace Private
{
    template < typename _Refer >
    struct InstanceGuardHelper
    {
        using Type = Private::DefaultInstanceGuard< _Refer >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialInstanceGuard< Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialInstanceGuard< Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialInstanceGuard< const Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialInstanceGuard< const Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< volatile Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialInstanceGuard< volatile Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< volatile Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialInstanceGuard< volatile Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const volatile Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialInstanceGuard< const volatile Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const volatile Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialInstanceGuard< const volatile Instance< _Value, _Tool > & >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< Instance< _Value, _Tool > > {};

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const Instance< _Value, _Tool > > {};

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< volatile Instance< _Value, _Tool > > {};

    template < typename _Value, typename _Tool >
    struct InstanceGuardHelper< const volatile Instance< _Value, _Tool > > {};
}

#endif
