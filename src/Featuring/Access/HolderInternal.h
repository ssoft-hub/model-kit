#pragma once
#ifndef ACCESS_HOLDER_INTERNAL_POINTER_H
#define ACCESS_HOLDER_INTERNAL_POINTER_H

#include <memory>
#include <ModelKit/Featuring/Traits.h>
#include <ModelKit/Utility/IsMethodExists.h>

// TODO: To move into private
template < typename _InstanceRefer >
struct InstanceAccess
{
    static_assert( ::std::is_reference< _InstanceRefer >::value,
        "The template parameter _InstanceRefer must to be a reference type." );

    using InstanceRefer = _InstanceRefer;
    using Instance = ::std::decay_t< InstanceRefer >;

    static_assert( ::is_instance< Instance >,
        "The template parameter _InstanceRefer must to be a Instance reference type." );

    using Holder = typename Instance::Holder;
    using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >;

    static constexpr HolderRefer holderRefer ( InstanceRefer refer )
    {
        return ::std::forward< HolderRefer >( refer.m_holder );
    }
};

template < typename _InstanceRefer,
    typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _InstanceRefer > > > >
constexpr ::SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Holder, _InstanceRefer && > instanceHolder ( _InstanceRefer && refer )
{
    using InstanceRefer = _InstanceRefer &&;
    return InstanceAccess< InstanceRefer >::holderRefer( ::std::forward< InstanceRefer >( refer ) );
}


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

template < typename _ValueRefer, typename _HolderRefer, bool has_method >
struct ValueHelper;

template < typename _ValueRefer, typename _HolderRefer >
struct ValueHelper< _ValueRefer, _HolderRefer, true >
{
    static_assert( ::std::is_reference< _ValueRefer >::value, "The template parameter _ValueRefer type must to be a reference type." );
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be a reference type." );
    static constexpr _ValueRefer value ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        return Holder::value( ::std::forward< _HolderRefer >( holder ) );
    }
};

struct HolderInternal
{
    IS_METHOD_EXISTS_TRAIT( guard )
    IS_METHOD_EXISTS_TRAIT( unguard )
    IS_METHOD_EXISTS_TRAIT( value )

    template < typename _HolderRefer >
    static constexpr void guard ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        ::GuardHelper< _HolderRefer, is_guard_method_exists< Holder, void( _HolderRefer ) > >::guard( ::std::forward< _HolderRefer >( holder ) );
    }

    template < typename _HolderRefer >
    static constexpr void unguard ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        ::UnguardHelper< _HolderRefer, is_unguard_method_exists< Holder, void( _HolderRefer ) > >::unguard( ::std::forward< _HolderRefer >( holder ) );
    }

    template < typename _ValueRefer, typename _HolderRefer,
        typename = ::std::enable_if_t< is_value_method_exists< ::std::decay_t< _HolderRefer >, _ValueRefer( _HolderRefer ) > > >
    static constexpr _ValueRefer value ( _HolderRefer holder )
    {
        //using Holder = ::std::decay_t< _HolderRefer >;
        return ::ValueHelper< _ValueRefer, _HolderRefer, true >::value( ::std::forward< _HolderRefer >( holder ) );
    }
};

#endif
