#pragma once
#ifndef TYPE_HELPER_H
#define TYPE_HELPER_H

template < bool condition, typename _TypeTrue, typename _TypeFalse >
struct TypeHelper;

template < typename _TypeTrue, typename _TypeFalse >
struct TypeHelper < true, _TypeTrue, _TypeFalse >
{
    using Type = _TypeTrue;
};

template < typename _TypeTrue, typename _TypeFalse >
struct TypeHelper < false, _TypeTrue, _TypeFalse >
{
    using Type = _TypeFalse;
};

#endif
