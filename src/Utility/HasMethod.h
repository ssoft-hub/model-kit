#pragma once
#ifndef UTILITY_HAS_METHOD_H
#define UTILITY_HAS_METHOD_H

#include <type_traits>

#define HAS_METHOD_TRAIT( method ) \
    template < typename, typename _Method > \
    struct Has_ ## method ## _Helper { \
        static_assert( std::integral_constant< _Method, false >::value, \
            "The template parameter _Method must to be of function type."); \
    }; \
    \
    template < typename _Type, typename _Member, typename ... _Arguments > \
    struct Has_ ## method ## _Helper< _Type, _Member( _Arguments... ) > \
    { \
    private: \
        template < typename _Test > \
        static constexpr std::is_same< decltype( std::declval< _Test >(). method ( std::declval< _Arguments >() ... ) ), _Member > __test ( int ); \
    \
        template< typename > \
        static constexpr std::false_type __test( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type >( int() ) ); \
    }; \
    \
    template < typename _Type, typename _Method > \
    using Has_ ## method ## _Method = typename Has_ ## method ## _Helper< _Type, _Method >::Type; \
    template < typename _Type, typename _Method > \
    static constexpr bool has_ ## method ## _method = Has_ ## method ##_Method< _Type, _Method >::value; \

#endif
