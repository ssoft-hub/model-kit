#pragma once
#ifndef ACCESS_ACCESSING_POINTER_H
#define ACCESS_ACCESSING_POINTER_H

#include <type_traits>
#include <memory>

template < typename _Refer > struct Guarding;
template < typename _Refer > struct GuardingSpec;
template < typename _Refer > struct Unuarding;
template < typename _Refer > struct UnuardingSpec;
template < typename _Refer > struct Accessing;
template < typename _Refer > struct AccessingSpec;

template < typename _Refer >
struct Guarding
{
    static_assert( ::std::is_reference< _Refer >::value, "The _Refer type must be a reference." );
    static constexpr void apply ( _Refer ) {}
};

template < typename _Refer >
struct GuardingSpec
{
    static_assert( ::std::is_reference< _Refer >::value, "The _Refer type must be a reference." );
    static constexpr void apply ( _Refer holder )
    {
        return ::Guarding< _Refer >::apply( ::std::forward< _Refer >( holder ) );
    }
};

template < typename _Refer >
struct Unguarding
{
    static_assert( ::std::is_reference< _Refer >::value, "The _Refer type must be a reference." );
    static constexpr void apply ( _Refer ) {}
};

template < typename _Refer >
struct UnguardingSpec
{
    static_assert( ::std::is_reference< _Refer >::value, "The _Refer type must be a reference." );
    static constexpr void apply ( _Refer holder )
    {
        return ::Unguarding< _Refer >::apply( ::std::forward< _Refer >( holder ) );
    }
};

template < typename _Refer >
struct AccessingSpec
{
    static_assert( ::std::is_reference< _Refer >::value );
    static constexpr decltype(auto) value ( _Refer && holder )
    {
        using Holder = ::std::decay_t< _Refer >;
        using HolderRefer = _Refer &&;
        return ::Accessing< Holder >::template value< HolderRefer >( ::std::forward< HolderRefer >( holder ) );
    }
};

struct Access
{
    template < typename _Holder >
    static constexpr void guarding ( _Holder && holder )
    {
        using HolderRefer = _Holder &&;
        ::GuardingSpec< HolderRefer >::apply( ::std::forward< HolderRefer >( holder ) );
    }

    template < typename _Holder >
    static constexpr void unguarding ( _Holder && holder )
    {
        using HolderRefer = _Holder &&;
        ::UnguardingSpec< HolderRefer >::apply( ::std::forward< HolderRefer >( holder ) );
    }

    template < typename _Holder >
    static constexpr decltype(auto) value ( _Holder && holder )
    {
        using HolderRefer = _Holder &&;
        return ::AccessingSpec< HolderRefer >::value( ::std::forward< HolderRefer >( holder ) );
    }
};

#endif
