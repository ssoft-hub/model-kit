#pragma once
#ifndef MDK_UNARY_OPERATOR_H
#define MDK_UNARY_OPERATOR_H

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

#define MDK_IS_UNARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
    template < typename _Kind, typename _Refer, typename ... _Arguments > \
    struct Is ## Invokable ## OperatorExistsTestHelper; \
     \
    template < typename _Refer, typename ... _Arguments > \
    using Is ## Invokable ## OperatorExistsTest = Is ## Invokable ## OperatorExistsTestHelper< ::Operator::InstanceSwitchCase< _Refer >, _Refer, _Arguments ... >; \
     \
    template < typename _Refer, typename ... _Arguments > \
    static constexpr bool is_ ## Invokable ## _operator_exists_test = Is ## Invokable ## OperatorExistsTest< _Refer, _Arguments ... >::value; \
     \
    template < typename _Refer, typename ... _Arguments > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::NoneInstanceCase, _Refer, _Arguments ... > \
    { \
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." ); \
     \
        static const bool value = ::Operator::Unary::is_ ## Invokable ## _operator_exists< _Refer, _Arguments ... >; \
    }; \
     \
    template < typename _Refer, typename ... _Arguments > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::UnaryInstanceCase, _Refer, _Arguments ...  > \
    { \
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." ); \
        using InstanceRefer = _Refer; \
        using Instance = ::std::decay_t< InstanceRefer >; \
        using Holder = typename Instance::Holder; \
        using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
        using Value = typename Instance::Value; \
        using ValueRefer = ::SimilarRefer< Value, InstanceRefer >; \
     \
        static const bool value = ::Operator::Unary::is_operator ## Invokable ## _method_exists< Holder, void(HolderRefer, _Arguments ...) > \
            || ::Operator::Unary::is_ ## Invokable ## _operator_exists< ValueRefer, _Arguments ...  >; \
    }; \
     \

#define MDK_COMMON_UNARY_OPERATOR_IMPLEMENTAION( Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            template < typename > \
            struct Invokable ## Switch; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::Unary::HolderHasOperatorCase > \
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
            struct Invokable ## Switch< ::Operator::Unary::HolderHasNoOperatorCase > \
            { \
                template < typename _Instance, typename ... _Arguments > \
                static decltype(auto) invoke ( _Instance && instance, _Arguments && ... arguments ) \
                { \
                    using InstanceRefer = _Instance &&; \
                    using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >; \
                    using Returned = ::std::result_of_t< Invokable( ValueRefer, _Arguments && ... ) >; \
                    return ::Operator::ResultSwitch< ::Operator::UnaryInstanceCase, ::Operator::ResultSwitchCase< Returned, ValueRefer > > \
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
            template < typename > \
            struct Invokable ## InstanceSwitch; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < ::Operator::UnaryInstanceCase > \
            { \
                template < typename _Refer, typename ... _Arguments > \
                static constexpr decltype(auto) invoke ( _Refer && value, _Arguments && ... arguments ) \
                { \
                    using InstanceRefer = _Refer &&; \
                    using Instance = ::std::decay_t< InstanceRefer >; \
                    using Holder = typename Instance::Holder; \
                    using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
     \
                    constexpr bool holder_has_method_for_operator = ::Operator::Unary::is_operator ## Invokable ## _method_exists< Holder, void( HolderRefer, _Arguments && ... ) >; \
                    using OperatorSwitchCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Unary::HolderHasOperatorCase, ::Operator::Unary::HolderHasNoOperatorCase >; \
                    return ::Operator::Unary::Invokable ## Switch< OperatorSwitchCase >::invoke( ::std::forward< InstanceRefer >( value ), ::std::forward< _Arguments >( arguments ) ... ); \
                } \
            }; \
        } \
    } \
     \
    namespace Operator \
    { \
       namespace Unary \
       { \
           template < typename _Refer, typename ... _Arguments > \
           struct Invokable ## Helper \
           { \
               static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." ); \
               using Refer = _Refer; \
     \
                static constexpr decltype(auto) invoke( Refer value, _Arguments && ... arguments ) \
                { \
                    return ::Operator::Unary::Invokable ## InstanceSwitch< ::Operator::InstanceSwitchCase< Refer > > \
                        ::invoke( ::std::forward< Refer >( value ), ::std::forward< _Arguments && >( arguments ) ... ); \
                } \
            }; \
        } \
    } \

#define MDK_POSTFIX_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            MDK_IS_POSTFIX_UNARY_OPERATOR_EXISTS_TRAIT( MDK_SINGLE_ARG( symbol ), Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            MDK_IS_UNARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
        } \
    } \
     \
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
        } \
    } \
     \
    MDK_COMMON_UNARY_OPERATOR_IMPLEMENTAION( Invokable ) \

#define MDK_PREFIX_UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            MDK_IS_PREFIX_UNARY_OPERATOR_EXISTS_TRAIT( MDK_SINGLE_ARG( symbol ), Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            MDK_IS_UNARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
        } \
    } \
     \
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
        } \
    } \
     \
    MDK_COMMON_UNARY_OPERATOR_IMPLEMENTAION( Invokable ) \


#define MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            MDK_IS_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENT_EXISTS_TRAIT( MDK_SINGLE_ARG( symbol ), Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            MDK_IS_UNARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
        } \
    } \
     \
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
        } \
    } \
     \
    MDK_COMMON_UNARY_OPERATOR_IMPLEMENTAION( Invokable ) \

#define MDK_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Unary \
        { \
            MDK_IS_POSTFIX_UNARY_OPERATOR_WITH_ARGUMENTS_EXISTS_TRAIT( MDK_SINGLE_ARG( symbol ), Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            MDK_IS_UNARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
        } \
    } \
     \
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
        } \
    } \
     \
    MDK_COMMON_UNARY_OPERATOR_IMPLEMENTAION( Invokable ) \

#endif
