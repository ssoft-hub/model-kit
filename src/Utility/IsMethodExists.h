#pragma once
#ifndef SCL_UTILITY_SCL_IS_METHOD_EXISTS_H
#define SCL_UTILITY_SCL_IS_METHOD_EXISTS_H

#include <type_traits>

#define SCL_IS_METHOD_EXISTS_TRAIT( method ) \
    template < typename _Type, typename ... _Arguments > \
    struct Is_ ## method ## _MethodExistsHelper { \
    private: \
        template < typename _Test, \
            typename = decltype( ::std::declval< _Test >(). method ( ::std::declval< _Arguments >() ... ) ) > \
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
    struct Is_ ## method ## _MethodExistsHelper< _Type, _Result( _Arguments ... ) > \
    { \
    private: \
        template < typename _Test, \
            typename = ::std::enable_if_t< ::std::is_same< decltype( ::std::declval< _Test >(). method ( ::std::declval< _Arguments >() ... ) ), _Result >::value > > \
        static constexpr ::std::true_type __test ( int ); \
    \
        template< typename > \
        static constexpr ::std::false_type __test( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type, typename ... _Arguments > \
    using Is_ ## method ## _MethodExists = typename Is_ ## method ## _MethodExistsHelper< _Type, _Arguments ... >::Type; \
    template < typename _Type, typename ... _Arguments > \
    static constexpr bool is_ ## method ## _method_exists = Is_ ## method ## _MethodExists< _Type, _Arguments ... >::value; \

#endif
