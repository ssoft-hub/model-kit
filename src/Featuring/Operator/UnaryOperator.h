#pragma once
#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H

#include <ModelKit/Featuring/Access/Accessing.h>
#include "Resolver.h"
#include "Trait.h"

#define _UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator { template < typename > struct Invokable; } \
    namespace Operator { namespace Spec { template < typename > struct Invokable; } } \
    \
    namespace Operator \
    { \
        namespace Spec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Value, template < typename > class _Holder > \
            struct Invokable< _Holder< _Value > > \
            { \
                using Holder = _Holder< _Value >; \
                using Value = _Value; \
                template < typename _Refer, typename ... _Arguments > \
                /*constexpr*/ decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
                { \
                    using HolderRefer = _Refer &&; \
                    static_assert( ::std::is_same< Holder, ::std::decay_t< HolderRefer > >::value, \
                        "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >::Holder." ); \
                    using ValueRefer = ::SimilarRefer< Value, HolderRefer >; \
                    return ::Operator::Invokable< Value >()( ::std::forward< ValueRefer >( ::Access::value( ::std::forward< HolderRefer >( refer ) ) ), ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        template < typename _Value, typename _Tool > \
        struct Invokable< ::Instance< _Value, _Tool > > \
        { \
            using Instance = ::Instance< _Value, _Tool >; \
            using Holder = typename Instance::Holder; \
            template < typename _Refer, typename ... _Arguments > \
            /*constexpr*/ decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using InstanceRefer = _Refer &&; \
                static_assert( ::std::is_same< Instance, ::std::decay_t< InstanceRefer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter Instance< _Value, _Tool >." ); \
                using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::Spec::Invokable< Holder >()( ::std::forward< HolderRefer >( refer.m_holder ), ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        };\
    } \


#define LEFT_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    LEFT_INVOKABLE_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    _UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer > \
            /*constexpr*/ decltype(auto) operator () ( _Refer && refer ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ) symbol; \
            } \
        }; \
    } \

#define RIGHT_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    RIGHT_INVOKABLE_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    _UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer > \
            /*constexpr*/ decltype(auto) operator () ( _Refer && refer ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return symbol ::std::forward< Refer >( refer ); \
            } \
        }; \
    } \

#define ARGUMENT_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    ARGUMENT_INVOKABLE_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    _UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer, typename _Argument > \
            /*constexpr*/ decltype(auto) operator () ( _Refer && refer, _Argument && argument ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ). operator symbol ( ::std::forward< _Argument >( argument ) ); \
            } \
        }; \
    } \

#define ARGUMENTS_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    ARGUMENTS_INVOKABLE_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    _UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must be decayed." ); \
            template < typename _Refer, typename ... _Arguments > \
            /*constexpr*/ decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ). operator symbol ( ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        }; \
    } \

#endif
