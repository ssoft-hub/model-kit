#pragma once

template < typename _Type >
inline constexpr const _Type & asConst ( const _Type & value )
{
    return value;
}
