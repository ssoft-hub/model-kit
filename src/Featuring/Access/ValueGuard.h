#pragma once
#ifndef GUARD_VALUE_POINTER_H
#define GUARD_VALUE_POINTER_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

// Предопределение
template < typename _Value, typename _Tool >
class Instance;

namespace Private
{
    template < typename _Refer >
    struct ValueGuardHelper;
}

/*!
 * Указатель на экземпляр вложенного в Instance базового значения, к которому применены
 * все особенности, реализуемые посредством используемых Instance.
 */
template < typename _Refer >
using ValueGuard = typename Private::ValueGuardHelper< _Refer >::Type;

namespace Private
{
    /*
     * Указатель на экземпляр вложенного в Instance базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Instance. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Instance.
     */
    template < typename _Refer >
    class DefaultValueGuard
    {
        using ThisType = DefaultValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Guard = InstanceGuard< _Refer >;
        using AccessRefer = Refer;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );
        static_assert( !::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must be a not Instance type reference!" );

    private:
        Guard m_instance_guard;

    public:
        constexpr DefaultValueGuard ()
            : m_instance_guard()
        {
        }

        DefaultValueGuard ( Refer refer )
            : m_instance_guard( ::std::forward< Refer >( refer ) )
        {
        }

        DefaultValueGuard ( Guard && other )
            : m_instance_guard( ::std::forward< Guard >( other ) )
        {
        }

        DefaultValueGuard ( ThisType && other )
            : m_instance_guard( ::std::forward< Guard >( other.m_instance_guard ) )
        {
        }

        constexpr bool operator ! () const
        {
            return !m_instance_guard;
        }

        constexpr AccessRefer operator * () const
        {
            return ::std::forward< AccessRefer >( *m_instance_guard );
        }

        constexpr const Guard & operator -> () const &
        {
            return m_instance_guard;
        }

        constexpr const Guard && operator -> () const &&
        {
            return ::std::forward< const Guard >( m_instance_guard );
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
     * Указатель на экземпляр вложенного в Instance базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Instance. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам является Instance.
     */
    template < typename _Refer >
    class SpecialValueGuard

    {
        using ThisType = SpecialValueGuard< _Refer >;

    public:
        using Refer = _Refer;
        using Instance = ::std::decay_t< Refer >;
        using Tool = typename Instance::Holder;

        using Value = typename Instance::Value;
        using ValueRefer = ::SimilarRefer< Value, Refer >;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::SimilarRefer< Holder, Refer >;
        using InstanceGuard = ::InstanceGuard< Refer >;
        using ValueGuard = ::ValueGuard< ValueRefer >;
        using AccessRefer = typename ValueGuard::AccessRefer;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );
        static_assert( ::is_instance< Instance >, "The template parameter _Refer must be a Instance type reference!" );
        static_assert( ::is_similar< ValueRefer, Refer >, "The Refer and ValueRefer must be similar types!" );
        static_assert( ::is_similar< HolderRefer, Refer >, "The Refer and HolderRefer must be similar types!" );

    private:
        InstanceGuard m_instance_guard;
        ValueGuard m_value_guard;

    public:
        constexpr SpecialValueGuard ()
            : m_instance_guard()
            , m_value_guard()
        {
        }

        SpecialValueGuard ( _Refer refer )
            : m_instance_guard( ::std::forward< _Refer >( refer ) )
            , m_value_guard( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_instance_guard).m_holder ) ) ) )
        {
        }

        SpecialValueGuard ( InstanceGuard && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other ) )
            , m_value_guard( ::std::forward< ValueRefer >( Tool::value( ::std::forward< HolderRefer >( (*m_instance_guard).m_holder ) ) ) )
        {
        }

        SpecialValueGuard ( ThisType && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other.m_instance_guard ) )
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
    struct ValueGuardHelper< Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialValueGuard< Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialValueGuard< Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Instance< _Value, _Tool > & >
    {
        using Type = Private::SpecialValueGuard< const Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Instance< _Value, _Tool > && >
    {
        using Type = Private::SpecialValueGuard< const Instance< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< Instance< _Value, _Tool > > {};
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const Instance< _Value, _Tool > > {};
}

#endif
