#pragma once
#ifndef UTILITY_IS_OPERATOR_EXISTS_H
#define UTILITY_IS_OPERATOR_EXISTS_H

#include <type_traits>

#define IS_PREFIX_OPERATOR_EXISTS_TRAIT( symbol, Invokable ) \
    template < typename _Right > \
    class Is ## Invokable ## OperatorExistsHelper \
    { \
        static_assert( ::std::is_reference< _Right >::value, \
            "The template parameter _Right must to be a reference type." ); \
        template < typename _RightArg, \
            typename = decltype( symbol ::std::declval< _RightArg >() ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Right >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type > \
    using Is ## Invokable ## OperatorExists = typename Is ## Invokable ## OperatorExistsHelper< _Type >::Type; \
    template < typename _Type > \
    static constexpr bool is_ ## Invokable ## _operator_exists = Is ## Invokable ## OperatorExists< _Type >::value; \

#define IS_POSTFIX_OPERATOR_EXISTS_TRAIT( symbol, Invokable ) \
    template < typename _Left > \
    class Is ## Invokable ## OperatorExistsHelper \
    { \
        static_assert( ::std::is_reference< _Left >::value, \
            "The template parameter _Left must to be a reference type." ); \
        template < typename _LeftArg, \
            typename = decltype( ::std::declval< _LeftArg >() symbol ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Left >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type > \
    using Is ## Invokable ## OperatorExists = typename Is ## Invokable ## OperatorExistsHelper< _Type >::Type; \
    template < typename _Type > \
    static constexpr bool is_ ## Invokable ## _operator_exists = Is ## Invokable ## OperatorExists< _Type >::value; \

#define IS_BINARY_OPERATOR_EXISTS_TRAIT( symbol, Invokable ) \
    template < typename _Left, typename _Right > \
    class Is ## Invokable ## OperatorExistsHelper \
    { \
        static_assert( ::std::is_reference< _Right >::value, \
            "The template parameter _Right must to be a reference type." ); \
        static_assert( ::std::is_reference< _Left >::value, \
            "The template parameter _Left must to be a reference type." ); \
        template < typename _LeftArg, typename _RightArg, \
            typename = decltype( ::std::declval< _LeftArg >() symbol ::std::declval< _RightArg >() ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Left, _Right >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Left, typename _Right > \
    using Is ## Invokable ## OperatorExists = typename Is ## Invokable ## OperatorExistsHelper< _Left, _Right >::Type; \
    template < typename _Left, typename _Right > \
    static constexpr bool is_ ## Invokable ## _operator_exists = Is ## Invokable ## OperatorExists< _Left, _Right >::value; \

#define IS_POSTFIX_OPERATOR_WITH_ARGUMENT_EXISTS_TRAIT( symbol, Invokable ) \
    template < typename _Type, typename _Argument > \
    class Is ## Invokable ## OperatorExistsHelper \
    { \
        template < typename _TypeArg, typename _Arg, \
            typename = decltype( ::std::declval< _TypeArg >(). operator symbol ( ::std::declval< _Arg >() ) ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type, _Argument >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type, typename _Argument > \
    using Is ## Invokable ## OperatorExists = typename Is ## Invokable ## OperatorExistsHelper< _Type, _Argument >::Type; \
    template < typename _Type, typename _Argument > \
    static constexpr bool is_ ## Invokable ## _operator_exists = Is ## Invokable ## OperatorExists< _Type, _Argument >::value; \

#define IS_POSTFIX_OPERATOR_WITH_ARGUMENTS_EXISTS_TRAIT( symbol, Invokable ) \
    template < typename _Type, typename ... _Arguments > \
    class Is ## Invokable ## OperatorExistsHelper \
    { \
        template < typename _TypeArg, typename ... _Args, \
            typename = decltype( ::std::declval< _TypeArg >(). operator symbol ( ::std::declval< _Args >() ... ) ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename ... > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Type, _Arguments ... >( ::std::declval< int >() ) ); \
    }; \
    \
    template < typename _Type, typename ... _Arguments > \
    using Is ## Invokable ## OperatorExists = typename Is ## Invokable ## OperatorExistsHelper< _Type, _Arguments ... >::Type; \
    template < typename _Type, typename ... _Arguments > \
    static constexpr bool is_ ## Invokable ## _operator_exists = Is ## Invokable ## OperatorExists< _Type, _Arguments ... >::value; \

#endif
