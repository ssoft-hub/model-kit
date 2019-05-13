#pragma once
#ifndef SCL_ACCESS_HOLDER_INTERNAL_POINTER_H
#define SCL_ACCESS_HOLDER_INTERNAL_POINTER_H

#include <memory>
#include <ModelKit/Featuring/Traits.h>
#include <ModelKit/Utility/IsMethodExists.h>

namespace SclPrivate
{
    template < typename _InstanceRefer >
    struct InstanceAccess
    {
        static_assert( ::std::is_reference< _InstanceRefer >::value,
            "The template parameter _InstanceRefer must to be a reference type." );

        using InstanceRefer = _InstanceRefer;
        using Instance = ::std::decay_t< InstanceRefer >;

        static_assert( ::ScL::is_instance< Instance >,
            "The template parameter _InstanceRefer must to be a Instance reference type." );

        using Holder = typename Instance::Holder;
        using HolderRefer = ::ScL::SimilarRefer< Holder, InstanceRefer >;

        static constexpr HolderRefer holderRefer ( InstanceRefer refer )
        {
            return ::std::forward< HolderRefer >( refer.m_holder );
        }
    };
}

namespace SclPrivate
{
    template < typename _InstanceRefer,
        typename = ::std::enable_if_t< ::ScL::is_instance< ::std::decay_t< _InstanceRefer > > > >
    constexpr ::ScL::SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Holder, _InstanceRefer && > instanceHolder ( _InstanceRefer && refer )
    {
        using InstanceRefer = _InstanceRefer &&;
        return ::SclPrivate::InstanceAccess< InstanceRefer >::holderRefer( ::std::forward< InstanceRefer >( refer ) );
    }
}

namespace SclPrivate
{
    template < typename _HolderRefer, bool has_method >
    struct GuardHelper;

    template < typename _HolderRefer >
    struct GuardHelper< _HolderRefer, false >
    {
        static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be a reference type." );
        static constexpr void guard ( _HolderRefer ) {}
    };

    template < typename _HolderRefer >
    struct GuardHelper< _HolderRefer, true >
    {
        static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be a reference type." );
        using Holder = ::std::decay_t< _HolderRefer >;
        static constexpr void guard ( _HolderRefer holder )
        {
            Holder::guard( ::std::forward< _HolderRefer >( holder ) );
        }
    };

    template < typename _HolderRefer, bool has_method >
    struct UnguardHelper;

    template < typename _HolderRefer >
    struct UnguardHelper< _HolderRefer, false >
    {
        static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be a reference type." );
        static constexpr void unguard ( _HolderRefer ) {}
    };

    template < typename _HolderRefer >
    struct UnguardHelper< _HolderRefer, true >
    {
        static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be a reference type." );
        static constexpr void unguard ( _HolderRefer holder )
        {
            using Holder = ::std::decay_t< _HolderRefer >;
            Holder::unguard( ::std::forward< _HolderRefer >( holder ) );
        }
    };
}

namespace SclPrivate
{
    struct HolderInternal
    {
        SCL_IS_METHOD_EXISTS_TRAIT( guard )
        SCL_IS_METHOD_EXISTS_TRAIT( unguard )
        SCL_IS_METHOD_EXISTS_TRAIT( value )

        template < typename _HolderRefer,
            typename = ::std::enable_if_t< is_value_method_exists< ::std::decay_t< _HolderRefer >, _HolderRefer > > >
        static constexpr void guard ( _HolderRefer holder )
        {
            using Holder = ::std::decay_t< _HolderRefer >;
            ::SclPrivate::GuardHelper< _HolderRefer, is_guard_method_exists< Holder, void( _HolderRefer ) > >::guard( ::std::forward< _HolderRefer >( holder ) );
        }

        template < typename _HolderRefer,
            typename = ::std::enable_if_t< is_value_method_exists< ::std::decay_t< _HolderRefer >, _HolderRefer > > >
        static constexpr void unguard ( _HolderRefer holder )
        {
            using Holder = ::std::decay_t< _HolderRefer >;
            ::SclPrivate::UnguardHelper< _HolderRefer, is_unguard_method_exists< Holder, void( _HolderRefer ) > >::unguard( ::std::forward< _HolderRefer >( holder ) );
        }

        template < typename _HolderRefer,
            typename = ::std::enable_if_t< is_value_method_exists< ::std::decay_t< _HolderRefer >, _HolderRefer > > >
        static constexpr decltype(auto) value ( _HolderRefer holder )
        {
            using Holder = ::std::decay_t< _HolderRefer >;
            return Holder::value( ::std::forward< _HolderRefer >( holder ) );
        }
    };
}

#endif
