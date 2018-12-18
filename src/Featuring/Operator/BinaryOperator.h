#pragma once
#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include "Resolver.h"
#include "Trait.h"

#define BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    BOTH_INVOKABLE_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator { template < typename, typename > struct Invokable; } \
    namespace Operator { namespace LeftSpec { template < typename, typename > struct Invokable; } } \
    namespace Operator { namespace RightSpec { template < typename, typename > struct Invokable; } } \
    namespace Operator { namespace BothSpec { template < typename, typename > struct Invokable; } } \
    \
    namespace Operator \
    { \
        namespace LeftSpec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Left, template < typename > class _LeftHolder, typename _Right > \
            struct Invokable< _LeftHolder< _Left >, _Right > \
            { \
                using Left = ::std::remove_cv_t< _Left >; \
                using Right = ::std::remove_cv_t< _Right >; \
                using LeftHolder = _LeftHolder< _Left >; \
                template < typename _LeftRefer, typename _RightRefer > \
                /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
                { \
                    using LeftHolderRefer = _LeftRefer &&; \
                    using RightRefer = _RightRefer &&; \
                    using LeftValueRefer = ::SimilarRefer< Left, LeftHolderRefer >; \
                    static_assert( ::std::is_same< LeftHolder, ::std::decay_t< LeftHolderRefer > >::value, \
                        "The template parameter _LeftRefer must be a refer of template parameter _LeftHolder< _Left >." ); \
                    return ::Operator::Invokable< Left, Right >()( \
                        ::std::forward< LeftValueRefer >( ::Access::value( ::std::forward< LeftHolderRefer >( left ) ) ), \
                        ::std::forward< RightRefer >( right ) ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        namespace RightSpec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Left, typename _Right, template < typename > class _RightHolder > \
            struct Invokable< _Left, _RightHolder< _Right > > \
            { \
                using Left = ::std::remove_cv_t< _Left >; \
                using Right = ::std::remove_cv_t< _Right >; \
                using RightHolder = _RightHolder< _Right >; \
                template < typename _LeftRefer, typename _RightRefer > \
                /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
                { \
                    using LeftRefer = _LeftRefer &&; \
                    using RightHolderRefer = _RightRefer &&; \
                    using RightValueRefer = ::SimilarRefer< Right, RightHolderRefer >; \
                    static_assert( ::std::is_same< RightHolder, ::std::decay_t< RightHolderRefer > >::value, \
                        "The template parameter _RightRefer must be a refer of template parameter _RightHolder< _Right >." ); \
                    return ::Operator::Invokable< Left, Right >()( \
                        ::std::forward< LeftRefer >( left ), \
                        ::std::forward< RightValueRefer >( ::Access::value( ::std::forward< RightHolderRefer >( right ) ) ) ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        namespace BothSpec \
        { \
            /* For unguarded Holder only */ \
            template < typename _Left, template < typename > class _LeftHolder, typename _Right, template < typename > class _RightHolder > \
            struct Invokable< _LeftHolder< _Left >, _RightHolder< _Right > > \
            { \
                using Left = ::std::remove_cv_t< _Left >; \
                using Right = ::std::remove_cv_t< _Right >; \
                using LeftHolder = _LeftHolder< _Left >; \
                using RightHolder = _RightHolder< _Right >; \
                template < typename _LeftRefer, typename _RightRefer > \
                /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
                { \
                    using LeftHolderRefer = _LeftRefer &&; \
                    using LeftValueRefer = ::SimilarRefer< Left, LeftHolderRefer >; \
                    using RightHolderRefer = _RightRefer &&; \
                    using RightValueRefer = ::SimilarRefer< Right, RightHolderRefer >; \
                    static_assert( ::std::is_same< LeftHolder, ::std::decay_t< LeftHolderRefer > >::value, \
                        "The template parameter _LeftRefer must be a refer of template parameter _LeftHolder< _Left >." ); \
                    static_assert( ::std::is_same< RightHolder, ::std::decay_t< RightHolderRefer > >::value, \
                        "The template parameter _RightRefer must be a refer of template parameter _RightHolder< _Right >." ); \
                    return ::Operator::Invokable< Left, Right >()( \
                        ::std::forward< LeftValueRefer >( ::Access::value( ::std::forward< LeftHolderRefer >( left ) ) ), \
                        ::std::forward< RightValueRefer >( ::Access::value( ::std::forward< RightHolderRefer >( right ) ) ) ); \
                } \
            };\
        } \
    } \
    \
    namespace Operator \
    { \
        template < typename _Left, typename _Right > \
        struct Invokable \
        { \
            using Left = _Left; \
            using Right = _Right; \
            static_assert( ::std::is_same< Left, ::std::decay_t< Left > >::value, \
                "The template parameter _Left must be decayed." ); \
            static_assert( ::std::is_same< Right, ::std::decay_t< Right > >::value, \
                "The template parameter _Right must be decayed." ); \
            template < typename _LeftRefer, typename _RightRefer > \
            /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
            { \
                using LeftRefer = _LeftRefer &&; \
                using RightRefer = _RightRefer &&; \
                static_assert( ::std::is_same< Left, ::std::decay_t< LeftRefer > >::value, \
                    "The template parameter _LeftRefer must be a refer of template parameter _Left." ); \
                static_assert( ::std::is_same< Right, ::std::decay_t< RightRefer > >::value, \
                    "The template parameter _RightRefer must be a refer of template parameter _Right." ); \
                return ::std::forward< LeftRefer >( left ) symbol ::std::forward< RightRefer >( right ); \
            } \
        }; \
    } \
    \
    namespace Operator \
    { \
        template < typename _LeftValue, typename _LeftTool, typename _Right > \
        struct Invokable< ::Instance< _LeftValue, _LeftTool >, _Right > \
        { \
            using LeftInstance = ::Instance< _LeftValue, _LeftTool >; \
            using LeftHolder = typename LeftInstance::Holder; \
            using Right = _Right; \
            static_assert( ::std::is_same< Right, ::std::decay_t< Right > >::value, \
                "The template parameter _Right must be decayed." ); \
            template < typename _LeftRefer, typename _RightRefer > \
            /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
            { \
                using LeftInstanceRefer = _LeftRefer &&; \
                using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                using RightRefer = _RightRefer &&; \
                static_assert( ::std::is_same< LeftInstance, ::std::decay_t< LeftInstanceRefer > >::value, \
                    "The template parameter _LeftRefer must be a refer of template parameter Instance< _LeftValue, _LeftTool >." ); \
                static_assert( ::std::is_same< Right, ::std::decay_t< RightRefer > >::value, \
                    "The template parameter _RightRefer must be a refer of template parameter _Right." ); \
    \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::LeftSpec::Invokable< LeftHolder, Right >()( ::std::forward< LeftHolderRefer >( left.m_holder ), ::std::forward< RightRefer >( right ) ); \
            } \
        };\
    } \
    \
    namespace Operator \
    { \
        template < typename _Left, typename _RightValue, typename _RightTool > \
        struct Invokable< _Left, ::Instance< _RightValue, _RightTool > > \
        { \
            using Left = _Left; \
            using RightInstance = ::Instance< _RightValue, _RightTool >; \
            using RightHolder = typename RightInstance::Holder; \
            static_assert( ::std::is_same< Left, ::std::decay_t< Left > >::value, \
                "The template parameter _Left must be decayed." ); \
            template < typename _LeftRefer, typename _RightRefer > \
            /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
            { \
                using LeftRefer = _LeftRefer &&; \
                using RightInstanceRefer = _RightRefer &&; \
                using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
                static_assert( ::std::is_same< Left, ::std::decay_t< LeftRefer > >::value, \
                    "The template parameter _LeftRefer must be a refer of template parameter _Left." ); \
                static_assert( ::std::is_same< RightInstance, ::std::decay_t< RightInstanceRefer > >::value, \
                    "The template parameter _RightRefer must be a refer of template parameter Instance< _RightValue, _RightTool >." ); \
    \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::RightSpec::Invokable< Left, RightHolder >()( ::std::forward< LeftRefer >( left ), ::std::forward< RightHolderRefer >( right.m_holder ) ); \
            } \
        };\
    } \
    \
    namespace Operator \
    { \
        template < typename _LeftValue, typename _LeftTool, typename _RightValue, typename _RightTool > \
        struct Invokable< ::Instance< _LeftValue, _LeftTool >, ::Instance< _RightValue, _RightTool > > \
        { \
            using LeftInstance = ::Instance< _LeftValue, _LeftTool >; \
            using LeftHolder = typename LeftInstance::Holder; \
            using RightInstance = ::Instance< _RightValue, _RightTool >; \
            using RightHolder = typename RightInstance::Holder; \
    \
            template < typename _LeftRefer, typename _RightRefer > \
            /*constexpr*/ decltype(auto) operator () ( _LeftRefer && left, _RightRefer && right ) \
            { \
                using LeftInstanceRefer = _LeftRefer &&; \
                using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                using RightInstanceRefer = _RightRefer &&; \
                using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
                static_assert( ::std::is_same< LeftInstance, ::std::decay_t< LeftInstanceRefer > >::value, \
                    "The template parameter _LeftRefer must be a refer of template parameter Instance< _LeftValue, _LeftTool >." ); \
                static_assert( ::std::is_same< RightInstance, ::std::decay_t< RightInstanceRefer > >::value, \
                    "The template parameter _RightRefer must be a refer of template parameter Instance< _RightValue, _RightTool >." ); \
    \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::BothSpec::Invokable< LeftHolder, RightHolder >()( ::std::forward< LeftHolderRefer >( left.m_holder ), ::std::forward< RightHolderRefer >( right.m_holder ) ); \
            } \
        };\
    } \

#endif
