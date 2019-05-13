#pragma once
#ifndef SCL_FEATURING_ACCESS_VALUE_GUARD_H
#define SCL_FEATURING_ACCESS_VALUE_GUARD_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

namespace SclPrivate { template < typename _Value, typename _Tool > class Instance; }
namespace SclPrivate { template < typename > struct ValueGuardHelper; }

namespace ScL
{
    //! Указатель на экземпляр вложенного в Instance базового значения, к которому применены
    /// все особенности, реализуемые посредством используемых Instance.
    template < typename _Refer >
    using ValueGuard = typename ::SclPrivate::ValueGuardHelper< _Refer >::Type;
}

namespace SclPrivate
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
        using InstanceGuard = ::SclPrivate::InstanceGuard< _Refer >;

        using ValueAccess = typename InstanceGuard::ValueAccess;
        using PointerAccess = typename InstanceGuard::PointerAccess;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( !::ScL::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must to be a not Instance type reference!" );

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

namespace SclPrivate
{
    template < typename _Refer >
    struct ValueGuardHelper
    {
        using Type = DefaultValueGuard< _Refer >;
    };
}

namespace SclPrivate
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
        using ValueRefer = ::ScL::SimilarRefer< Value, InstanceRefer >;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::ScL::SimilarRefer< Holder, InstanceRefer >;

        using InstanceGuard = ::SclPrivate::InstanceGuard< InstanceRefer >;
        using ValueGuard = ::ScL::ValueGuard< ValueRefer >;

        using InstanceAccess = typename InstanceGuard::InstanceAccess;
        using ValueAccess =  typename ValueGuard::ValueAccess;
        using PointerAccess = typename ValueGuard::PointerAccess;

        static_assert( ::std::is_reference< InstanceRefer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( ::ScL::is_instance< Instance >, "The template parameter _Refer must to be a Instance type reference!" );
        //static_assert( ::ScL::is_similar< ValueRefer, InstanceRefer >, "The Refer and ValueRefer must to be similar types!" );
        static_assert( ::ScL::is_similar< HolderRefer, InstanceRefer >, "The Refer and HolderRefer must to be similar types!" );

    private:
        InstanceGuard m_instance_guard;
        ValueGuard m_value_guard;

    public:
        SpecialValueGuard ( InstanceRefer refer )
            : m_instance_guard( ::std::forward< InstanceRefer >( refer ) )
            , m_value_guard( ::SclPrivate::HolderInternal::value< HolderRefer >( m_instance_guard.holderAccess() ) )
        {
        }

        SpecialValueGuard ( InstanceGuard && other )
            : m_instance_guard( ::std::forward< InstanceGuard >( other ) )
            , m_value_guard( ::SclPrivate::HolderInternal::value< HolderRefer >( m_instance_guard.holderAccess() ) )
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

namespace SclPrivate
{
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< ::SclPrivate::Instance< _Value, _Tool > & >
    {
        using Type = ::SclPrivate::SpecialValueGuard< ::SclPrivate::Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< ::SclPrivate::Instance< _Value, _Tool > && >
    {
        using Type = ::SclPrivate::SpecialValueGuard< ::SclPrivate::Instance< _Value, _Tool > && >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const ::SclPrivate::Instance< _Value, _Tool > & >
    {
        using Type = ::SclPrivate::SpecialValueGuard< const ::SclPrivate::Instance< _Value, _Tool > & >;
    };

    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const ::SclPrivate::Instance< _Value, _Tool > && >
    {
        using Type = ::SclPrivate::SpecialValueGuard< const ::SclPrivate::Instance< _Value, _Tool > && >;
    };

    // disabled
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< ::SclPrivate::Instance< _Value, _Tool > > {};
    template < typename _Value, typename _Tool >
    struct ValueGuardHelper< const ::SclPrivate::Instance< _Value, _Tool > > {};
}

#endif
