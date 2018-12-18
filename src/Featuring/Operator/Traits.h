#pragma once
#ifndef OPERATOR_TRAIT_H
#define OPERATOR_TRAIT_H

#include <ModelKit/Featuring/Traits.h>

#define RIGHT_INVOKABLE_TRAIT( symbol, Invokable ) \
    template < typename _Right > \
    class Has ## Invokable ## Helper \
    { \
        template < typename _RightArg, \
            typename = decltype( symbol ::std::declval< _RightArg >() ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Right >(0) ); \
    }; \
    \
    template < typename _Type > \
    using Has ## Invokable = typename Has ## Invokable ## Helper< _Type >::Type; \
    template < typename _Type > \
    static constexpr bool has_ ## Invokable = Has ## Invokable< _Type >::value; \

#define LEFT_INVOKABLE_TRAIT( symbol, Invokable ) \
    template < typename _Left > \
    class Has ## Invokable ## Helper \
    { \
        template < typename _LeftArg, \
            typename = decltype( ::std::declval< _LeftArg >() symbol ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Left >(0) ); \
    }; \
    \
    template < typename _Type > \
    using Has ## Invokable = typename Has ## Invokable ## Helper< _Type >::Type; \
    template < typename _Type > \
    static constexpr bool has_ ## Invokable = Has ## Invokable< _Type >::value; \

#define BOTH_INVOKABLE_TRAIT( symbol, Invokable ) \
    template < typename _Left, typename _Right > \
    class Has ## Invokable ## Helper \
    { \
        template < typename _LeftArg, typename _RightArg, \
            typename = decltype( ::std::declval< _LeftArg >() symbol ::std::declval< _RightArg >() ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test< _Left, _Right >(0) ); \
    }; \
    \
    template < typename _Left, typename _Right > \
    using Has ## Invokable = typename Has ## Invokable ## Helper< _Left, _Right >::Type; \
    template < typename _Left, typename _Right > \
    static constexpr bool has_ ## Invokable = Has ## Invokable< _Left, _Right >::value; \

#define ARGUMENT_INVOKABLE_TRAIT( symbol, Invokable ) \
    template < typename _Type, typename _Argument > \
    class Has ## Invokable ## Helper \
    { \
        template < typename _TypeArg, typename _Arg, \
            typename = decltype( ::std::declval< _TypeArg >(). operator symbol ( ::std::declval< _Arg >() ) ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test<_Type, _Argument >(0) ); \
    }; \
    \
    template < typename _Type, typename _Argument > \
    using Has ## Invokable = typename Has ## Invokable ## Helper< _Type, _Argument >::Type; \
    template < typename _Type, typename _Argument > \
    static constexpr bool has_ ## Invokable = Has ## Invokable< _Type, _Argument >::value; \

#define ARGUMENTS_INVOKABLE_TRAIT( symbol, Invokable ) \
    template < typename _Type, typename ... _Arguments > \
    class Has ## Invokable ## Helper \
    { \
        template < typename _TypeArg, typename ... _Args, \
            typename = decltype( ::std::declval< _TypeArg >(). operator symbol ( ::std::declval< _Args >() ... ) ) > \
        static ::std::true_type __test ( int ); \
    \
        template < typename, typename ... > \
        static ::std::false_type __test ( ... ); \
    \
    public: \
        using Type = decltype( __test<_Type, _Arguments ... >(0) ); \
    }; \
    \
    template < typename _Type, typename ... _Arguments > \
    using Has ## Invokable = typename Has ## Invokable ## Helper< _Type, _Arguments ... >::Type; \
    template < typename _Type, typename ... _Arguments > \
    static constexpr bool has_ ## Invokable = Has ## Invokable< _Type, _Arguments ... >::value; \

#endif
