#pragma once
#ifndef VALUE_TRAIT_H
#define VALUE_TRAIT_H

#include <type_traits>

template < typename _Value >
struct IsHeap
    : public ::std::false_type
{
};

template < typename _Value >
struct IsOptional
    : public ::std::false_type
{
};

template < typename _Value >
struct IsImplicit
    : public ::std::false_type
{
};

template < typename _Value >
struct IsAtomic
    : public ::std::false_type
{
};

template < typename _Value >
struct IsContainer
    : public ::std::false_type
{
};

#endif
