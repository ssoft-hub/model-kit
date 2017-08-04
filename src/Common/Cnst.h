#pragma once

template < typename _Type >
inline constexpr const _Type & cnst ( const _Type & value )
{
    return value;
}
