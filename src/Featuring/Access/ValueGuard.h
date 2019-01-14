#pragma once
#ifndef MDK_FEATURING_ACCESS_VALUE_GUARD_H
#define MDK_FEATURING_ACCESS_VALUE_GUARD_H

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
using ValueGuard = typename ::Private::ValueGuardHelper< _Refer >::Type;

namespace Private
{
    /*!
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
        using InstanceGuard = ::InstanceGuard< _Refer >;

        using ValueAccess = typename InstanceGuard::ValueAccess;
        using PointerAccess = typename InstanceGuard::PointerAccess;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( !::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must to be a not Instance type reference!" );

    private:
        InstanceGuard m_instance_guard;

    public:
        DefaultValueGuard ( Refer refer )
            : m_instance_guard( ::std::forward< Refer >( refer ) )
        {
        }

        DefaultValueGuard ( InstanceGuard && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other ) )
        {
        }

        DefaultValueGuard ( ThisType && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other.m_instance_guard ) )
        {
        }

        ValueAccess valueAccess () const
        {
            return m_instance_guard.valueAccess();
        }

        PointerAccess pointerAccess () const
        {
            return m_instance_guard.pointerAccess();
        }

        operator ValueAccess () const
        {
            return valueAccess();
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
        using InstanceRefer = _Refer;
        using Instance = ::std::decay_t< InstanceRefer >;
        using Value = typename Instance::Value;
        using ValueRefer = ::SimilarRefer< Value, InstanceRefer >;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >;

        using InstanceGuard = ::InstanceGuard< InstanceRefer >;
        using ValueGuard = ::ValueGuard< ValueRefer >;

        using InstanceAccess = typename InstanceGuard::InstanceAccess;
        using ValueAccess =  typename ValueGuard::ValueAccess;
        using PointerAccess = typename ValueGuard::PointerAccess;

        static_assert( ::std::is_reference< InstanceRefer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( ::is_instance< Instance >, "The template parameter _Refer must to be a Instance type reference!" );
        //static_assert( ::is_similar< ValueRefer, InstanceRefer >, "The Refer and ValueRefer must to be similar types!" );
        static_assert( ::is_similar< HolderRefer, InstanceRefer >, "The Refer and HolderRefer must to be similar types!" );

    private:
        InstanceGuard m_instance_guard;
        ValueGuard m_value_guard;

    public:
        SpecialValueGuard ( InstanceRefer refer )
            : m_instance_guard( ::std::forward< InstanceRefer >( refer ) )
            , m_value_guard( ::HolderInternal::value< ValueRefer, HolderRefer >( m_instance_guard.holderAccess() ) )
        {
        }

        SpecialValueGuard ( InstanceGuard && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other ) )
            , m_value_guard( ::HolderInternal::value< ValueRefer, HolderRefer >( m_instance_guard.holderAccess() ) )
        {
        }

        SpecialValueGuard ( ThisType && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other.m_instance_guard ) )
            , m_value_guard( ::std::forward< ValueGuard >( other.m_value_guard ) )
        {
        }

        InstanceAccess instanceAccess () const
        {
            return m_instance_guard.instanceAccess();
        }

        ValueAccess valueAccess () const
        {
            return m_value_guard.valueAccess();
        }

        PointerAccess pointerAccess () const
        {
            return m_value_guard.pointerAccess();
        }

        operator ValueAccess () const
        {
            return valueAccess();
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
