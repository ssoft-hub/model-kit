#pragma once
#ifndef ACCESS_ACCESSING_POINTER_H
#define ACCESS_ACCESSING_POINTER_H

#include <memory>
#include <ModelKit/Utility/HasMethod.h>
#include <type_traits>

HAS_METHOD_TRAIT( guard )
HAS_METHOD_TRAIT( unguard )
HAS_METHOD_TRAIT( value )

template < typename _HolderRefer, bool has_method >
struct GuardHelper;

template < typename _HolderRefer >
struct GuardHelper< _HolderRefer, false >
{
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be of reference type." );
    static constexpr void guard ( _HolderRefer ) {}
};

template < typename _HolderRefer >
struct GuardHelper< _HolderRefer, true >
{
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be of reference type." );
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
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be of reference type." );
    static constexpr void unguard ( _HolderRefer ) {}
};

template < typename _HolderRefer >
struct UnguardHelper< _HolderRefer, true >
{
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be of reference type." );
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
    static_assert( ::std::is_reference< _ValueRefer >::value, "The template parameter _ValueRefer type must to be of reference type." );
    static_assert( ::std::is_reference< _HolderRefer >::value, "The template parameter _HolderRefer type must to be of reference type." );
    static constexpr _ValueRefer value ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        return Holder::value( ::std::forward< _HolderRefer >( holder ) );
    }
};

struct Access
{
    template < typename _HolderRefer >
    static constexpr void guard ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        ::GuardHelper< _HolderRefer, ::has_guard_method< Holder, void( _HolderRefer ) > >::guard( ::std::forward< _HolderRefer >( holder ) );
    }

    template < typename _HolderRefer >
    static constexpr void unguard ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        ::UnguardHelper< _HolderRefer, ::has_unguard_method< Holder, void( _HolderRefer ) > >::unguard( ::std::forward< _HolderRefer >( holder ) );
    }

    template < typename _ValueRefer, typename _HolderRefer >
    static constexpr _ValueRefer value ( _HolderRefer holder )
    {
        using Holder = ::std::decay_t< _HolderRefer >;
        static_assert( ::has_value_method< Holder, _ValueRefer( _HolderRefer ) >,
            "The Holder type must to have static method '_ValueRefer value( _HolderRefer )'" );
        return ::ValueHelper< _ValueRefer, _HolderRefer, ::has_value_method< Holder, _ValueRefer( _HolderRefer ) > >::value( ::std::forward< _HolderRefer >( holder ) );
    }
};

#endif
