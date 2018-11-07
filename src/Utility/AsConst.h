#pragma once
#ifndef UTILITY_AS_CONST_H
#define UTILITY_AS_CONST_H

#include <memory>
#include <type_traits>

template < typename _Type >
constexpr ::std::add_const_t< _Type > & asConst ( _Type & value ) noexcept
{
    return value;
}

template < typename _Type >
constexpr ::std::add_const_t< _Type > && asConst ( _Type && value ) noexcept
{
    return ::std::forward< _Type >( value );
}

#endif
