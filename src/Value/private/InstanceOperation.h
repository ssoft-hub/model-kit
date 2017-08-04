#pragma once

template < typename _ValueType, typename _ValueTool >
class Instance;

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator < (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left < *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator <= (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left <= *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator > (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left > *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator >= (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left >= *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator == (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left = *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType >
inline constexpr bool operator != (
    const Instance< _LeftType, _LeftTool > & left,
    const _RightType & right )
{
    return *&left != *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator < (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left < *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator <= (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left <= *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator > (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left > *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator >= (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left >= *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator == (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left = *&right;
}

template < typename _LeftType, typename _RightType, typename _RightTool >
inline constexpr bool operator != (
    const _LeftType & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left != *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator < (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left < *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator <= (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left <= *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator > (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left > *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator >= (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left >= *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator == (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left = *&right;
}

template < typename _LeftType, typename _LeftTool, typename _RightType, typename _RightTool >
inline constexpr bool operator != (
    const Instance< _LeftType, _LeftTool > & left,
    const Instance< _RightType, _RightTool > & right )
{
    return *&left != *&right;
}
