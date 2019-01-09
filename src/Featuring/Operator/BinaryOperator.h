#pragma once
#ifndef FEATURING_OPERATOR_BINARY_H
#define FEATURING_OPERATOR_BINARY_H

#include <ModelKit/Utility/IsMethodExists.h>
#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "ResultSwitch.h"

namespace Operator
{
    namespace Binary
    {
        /* Cases for Holder method existing */
        struct HolderHasOperatorCase {};
        struct HolderHasNoOperatorCase {};
    }
}

#define BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Binary \
        { \
            IS_BINARY_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Left ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Right ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Both ) \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            struct Invokable \
            { \
                template < typename _Left, typename _Right > \
                decltype(auto) operator () ( _Left && left, _Right && right ) \
                { \
                    return ::std::forward< _Left && >( left ) symbol ::std::forward< _Right && >( right ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            template < typename, typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< LeftInstanceCase, HolderHasOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder; \
                    using RightRefer = _Right &&; \
                    LeftHolder::operator ## Invokable ## Left( ::instanceHolder( ::std::forward< LeftInstanceRefer >( left ) ), ::std::forward< RightRefer >( right ) ); \
                    return ::std::forward< LeftInstanceRefer >( left ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< LeftInstanceCase, HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                    using RightRefer = _Right &&; \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftValueRefer, RightRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultCase< Returned, LeftValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< RightInstanceCase, HolderHasOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightInstanceRefer = _Right &&; \
                    using RightHolder = typename ::std::decay_t< RightInstanceRefer >::Holder; \
                    RightHolder::operator ## Invokable ## Right( ::std::forward< LeftRefer >( left ), ::instanceHolder( ::std::forward< RightInstanceRefer >( right ) ) ); \
                    return ::std::forward< RightInstanceRefer >( right ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< RightInstanceCase, HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightInstanceRefer = _Right &&; \
                    using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftRefer, RightValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::RightInstanceCase, ::Operator::ResultCase< Returned, RightValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< BothInstanceCase, HolderHasOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder; \
                    using RightInstanceRefer = _Right &&; \
                    LeftHolder::operator ## Invokable ## Both( ::instanceHolder( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ) ); \
                    return ::std::forward< RightInstanceRefer >( right ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< BothInstanceCase, HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                    using RightInstanceRefer = _Right &&; \
                    using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftValueRefer, RightValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::BothInstanceCase, ::Operator::ResultCase< Returned, LeftValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            template < typename > \
            struct Invokable ## InstanceSwitch; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < LeftInstanceCase > \
            { \
                template < typename _Left, typename _Right > \
                static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
                    using LeftHolder = typename LeftInstance::Holder; \
                    using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                    using RightRefer = _Right &&; \
     \
                    constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operator ## Invokable ## Left_method_exists< LeftHolder, void( LeftHolderRefer, RightRefer ) >; \
                    using OperatorCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< LeftInstanceCase, OperatorCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < RightInstanceCase > \
            { \
                template < typename _Left, typename _Right > \
                static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using RightInstanceRefer = _Right &&; \
                    using RightInstance = ::std::decay_t< RightInstanceRefer >; \
                    using RightHolder = typename RightInstance::Holder; \
                    using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
                    using LeftRefer = _Left &&; \
     \
                    constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operator ## Invokable ## Right_method_exists< RightHolder, void( LeftRefer, RightHolderRefer ) >; \
                    using OperatorCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< RightInstanceCase, OperatorCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < BothInstanceCase > \
            { \
                template < typename _Left, typename _Right > \
                static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
                    using LeftHolder = typename LeftInstance::Holder; \
                    using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                    using RightInstanceRefer = _Right &&; \
                    using RightInstance = ::std::decay_t< RightInstanceRefer >; \
                    using RightHolder = typename RightInstance::Holder; \
                    using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
     \
                    constexpr bool is_left_compatible_to_right = ::is_compatible< LeftHolder, RightHolder >; \
                    constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operator ## Invokable ## Both_method_exists< LeftHolder, void( LeftHolderRefer, RightHolderRefer ) >; \
                    using OperatorCase = ::std::conditional_t< is_left_compatible_to_right && holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< BothInstanceCase, OperatorCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            template < typename _LeftRefer, typename _RightRefer > \
            struct Invokable ## Helper \
            { \
                static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
                static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
     \
                using LeftRefer = _LeftRefer; \
                using RightRefer = _RightRefer; \
     \
                static constexpr decltype(auto) invoke( LeftRefer left, RightRefer right ) \
                { \
                    return ::Operator::Binary::Invokable ## InstanceSwitch< InstanceCase< LeftRefer, RightRefer > > \
                        ::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
        } \
    } \

#endif
