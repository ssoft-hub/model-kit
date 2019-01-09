#pragma once
#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Utility/IsMethodExists.h>
#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "ResultSwitch.h"

namespace Operator
{
    namespace Unary
    {
        /* Cases for Holder method existing */
        struct HolderHasOperatorCase {};
        struct HolderHasNoOperatorCase {};
    }
}

#define POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            struct Invokable \
            { \
                template < typename _Value > \
                decltype(auto) operator () ( _Value && value ) \
                { \
                    return ::std::forward< _Value && >( value ) symbol; \
                } \
            }; \
     \
            template < typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasOperatorCase > \
            { \
                template < typename _Instance > \
                static decltype(auto) invoke ( _Instance && instance ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using Holder = typename ::std::decay_t< InstanceRefer >::Holder; \
                    Holder::operator ## Invokable( ::instanceHolder( ::std::forward< InstanceRefer >( instance ) ) ); \
                    return ::std::forward< InstanceRefer >( instance ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasNoOperatorCase > \
            { \
                template < typename _Instance > \
                static decltype(auto) invoke ( _Instance && instance ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >; \
                    using Returned = ::std::result_of_t< Invokable( ValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultCase< Returned, ValueRefer > > \
                        ::invoke( ::Operator::Unary::Invokable(), ::std::forward< InstanceRefer >( instance ) ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Unary \
        { \
            IS_POSTFIX_UNARY_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
        } \
    } \

#define PREFIX_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            struct Invokable \
            { \
                template < typename _Value > \
                decltype(auto) operator () ( _Value && value ) \
                { \
                    return symbol ::std::forward< _Value >( value ); \
                } \
            }; \
     \
            template < typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasOperatorCase > \
            { \
                template < typename _Instance > \
                static decltype(auto) invoke ( _Instance && instance ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using Holder = typename ::std::decay_t< InstanceRefer >::Holder; \
                    Holder::operator ## Invokable( ::instanceHolder( ::std::forward< InstanceRefer >( instance ) ) ); \
                    return ::std::forward< InstanceRefer >( instance ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasNoOperatorCase > \
            { \
                template < typename _Instance > \
                static decltype(auto) invoke ( _Instance && instance ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >; \
                    using Returned = ::std::result_of_t< Invokable( ValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultCase< Returned, ValueRefer > > \
                        ::invoke( ::Operator::Unary::Invokable(), ::std::forward< InstanceRefer >( instance ) ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Unary \
        { \
            IS_PREFIX_UNARY_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
        } \
    } \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            struct Invokable \
            { \
                template < typename _Value, typename _Argument > \
                decltype(auto) operator () ( _Value && value, _Argument && argument ) \
                { \
                    return ::std::forward< _Value >( value ).operator symbol ( ::std::forward< _Argument >( argument ) ); \
                } \
            }; \
     \
            template < typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasOperatorCase > \
            { \
                template < typename _Instance, typename _Argument > \
                static decltype(auto) invoke ( _Instance && instance, _Argument && argument ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using Holder = typename ::std::decay_t< InstanceRefer >::Holder; \
                    Holder::operator ## Invokable( ::instanceHolder( ::std::forward< InstanceRefer >( instance ) ), ::std::forward< _Argument >( argument ) ); \
                    return ::std::forward< InstanceRefer >( instance ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasNoOperatorCase > \
            { \
                template < typename _Instance, typename _Argument > \
                static decltype(auto) invoke ( _Instance && instance, _Argument && argument ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >; \
                    using Returned = ::std::result_of_t< Invokable( ValueRefer, _Argument && ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultCase< Returned, ValueRefer > > \
                        ::invoke( ::Operator::Unary::Invokable(), ::std::forward< InstanceRefer >( instance ), ::std::forward< _Argument >( argument ) ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Unary \
        { \
            IS_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
            IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
        } \
    } \

#define POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            struct Invokable \
            { \
                template < typename _Value, typename ... _Arguments > \
                decltype(auto) operator () ( _Value && value, _Arguments && ... arguments ) \
                { \
                    return ::std::forward< _Value >( value ).operator symbol ( ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            }; \
     \
            template < typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasOperatorCase > \
            { \
                template < typename _Instance, typename ... _Arguments > \
                static decltype(auto) invoke ( _Instance && instance, _Arguments && ... arguments ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using Holder = typename ::std::decay_t< InstanceRefer >::Holder; \
                    Holder::operator ## Invokable( ::instanceHolder( ::std::forward< InstanceRefer >( instance ) ), ::std::forward< _Arguments >( arguments ) ... ); \
                    return ::std::forward< InstanceRefer >( instance ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< HolderHasNoOperatorCase > \
            { \
                template < typename _Instance, typename ... _Arguments > \
                static decltype(auto) invoke ( _Instance && instance, _Arguments && ... arguments ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >; \
                    using Returned = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultCase< Returned, ValueRefer > > \
                        ::invoke( ::Operator::Unary::Invokable(), ::std::forward< InstanceRefer >( instance ), ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
       namespace Unary \
       { \
           IS_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
           IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
       } \
    } \

#endif
