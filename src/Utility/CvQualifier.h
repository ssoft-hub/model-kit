#pragma once
#ifndef UTILITY_AS_CONST_H
#define UTILITY_AS_CONST_H

#include <memory>
#include <type_traits>

namespace Mdk
{
    template < typename _Type >
    constexpr ::std::add_const_t< _Type > & castConst ( _Type & value ) noexcept
    {
        return value;
    }

    template < typename _Type >
    constexpr ::std::add_const_t< _Type > && castConst ( _Type && value ) noexcept
    {
        return ::std::forward< ::std::add_const_t< _Type > && >( value );
    }

    template < typename _Type >
    constexpr ::std::add_volatile_t< _Type > & castVolatile ( _Type & value ) noexcept
    {
        return value;
    }

    template < typename _Type >
    constexpr ::std::add_volatile_t< _Type > && castVolatile ( _Type && value ) noexcept
    {
        return ::std::forward< ::std::add_volatile_t< _Type > && >( value );
    }

    template < typename _Type >
    constexpr ::std::add_cv_t< _Type > & castConstVolatile ( _Type & value ) noexcept
    {
        return value;
    }

    template < typename _Type >
    constexpr ::std::add_cv_t< _Type > && castConstVolatile ( _Type && value ) noexcept
    {
        return ::std::forward< ::std::add_cv_t< _Type > && >( value );
    }

}

#endif
