#pragma once
#ifndef SCL_UTILITY_IS_FUNCTION_EXISTS_H
#define SCL_UTILITY_IS_FUNCTION_EXISTS_H

#include <type_traits>

#define SCL_IS_FUNCTION_EXISTS_TRAIT( function, shortname ) \
    template < typename _Type, typename ... _Arguments > \
    struct Is_ ## shortname ## _FunctionExistsHelper { \
    private: \
        template < typename _Test, \
            typename = decltype( function( ::std::declval< _Arguments >() ... ) ) > \
        static constexpr ::std::true_type __test ( int ); \
    \
        template< typename > \
        static constexpr ::std::false_type __test( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type, typename _Result, typename ... _Arguments > \
    struct Is_ ## shortname ## _FunctionExistsHelper< _Type, _Result( _Arguments ... ) > \
    { \
    private: \
        template < typename _Test, \
            typename = ::std::enable_if_t< ::std::is_same< decltype( function( ::std::declval< _Arguments >() ... ) ), _Result >::value > > \
        static constexpr ::std::true_type __test ( int ); \
    \
        template< typename > \
        static constexpr ::std::false_type __test( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename ... _Arguments > \
    using Is_ ## shortname ## _FunctionExists = typename Is_ ## shortname ## _FunctionExistsHelper< void, _Arguments ... >::Type; \
    template < typename ... _Arguments > \
    static constexpr bool is_ ## shortname ## _function_exists = Is_ ## shortname ## _FunctionExists< void, _Arguments ... >::value; \

#endif
