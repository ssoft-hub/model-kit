#pragma once
#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Tool/Guard/UnaryTool.h>
#include <ModelKit/Utility/IsMethodExists.h>
#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "Resolver.h"

namespace Operator
{
    namespace Unary
    {
        template < typename >
        struct ResultSwitch;

        /* Cases for operator result type */
        struct FundamentalCase {};
        struct ThisInstanceCase {};
        struct DefaultInstanceCase {};
        struct BlockedInstanceCase {};

        template < typename _Returned, typename _Refer >
        struct ResultCaseHelper
        {
            using Returned = _Returned;
            using Value = ::std::decay_t< _Refer >;

            static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
            static constexpr bool value_is_instance = ::is_instance< Value >;
            static constexpr bool returned_and_refer_are_same = ::std::is_same< _Returned, _Refer >::value;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalCase,
                ::std::conditional_t< returned_and_refer_are_same,
                    ThisInstanceCase,
                    ::std::conditional_t< returned_is_reference,
                        BlockedInstanceCase,
                        DefaultInstanceCase > > >;
        };

        template < typename _Returned, typename _Value >
        using ResultCase = typename ResultCaseHelper< _Returned, _Value >::Type;

        template <>
        struct ResultSwitch< FundamentalCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                return invokable( ::std::forward< ValueRefer >( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct ResultSwitch< ThisInstanceCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                invokable( ::std::forward< ValueRefer >( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... );
                return ::std::forward< InstanceRefer >( instance );
            }
        };

        template <>
        struct ResultSwitch< DefaultInstanceCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( ::std::forward< ValueRefer >( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };

        template <>
        struct ResultSwitch< BlockedInstanceCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                static_assert( ::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a reference type." );
                using Tool = ::Guard::UnaryTool< InstanceRefer, _Invokable, _Arguments ... >;
                using Result = ::std::remove_reference_t< Returned >;
                return Instance< Result, Tool >( ::std::forward< InstanceRefer >( instance ), ::std::forward< _Invokable >( invokable ), ::std::forward< _Arguments >( arguments ) ... );
            }
        };


    }
}

namespace Operator
{
    namespace Unary
    {
        /* Cases for Holder method existing */
        struct HolderHasOperatorCase {};
        struct HolderHasNoOperatorCase {};
    }
}

namespace Operator
{
    namespace Unary
    {
        struct SquareBrackets
        {
            template < typename _Value, typename ... _Arguments >
            decltype(auto) operator () ( _Value && value, _Arguments && ... arguments )
            {
                return ::std::forward< _Value >( value ).operator [] ( ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template < typename >
        struct SquareBracketsSwitch;

        template <>
        struct SquareBracketsSwitch< HolderHasOperatorCase >
        {
            template < typename _Instance, typename ... _Arguments >
            static decltype(auto) invoke ( _Instance && instance, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using Holder = typename ::std::decay_t< InstanceRefer >::Holder;
                Holder::operatorSquareBrackets( ::instanceHolder( ::std::forward< InstanceRefer >( instance ) ), ::std::forward< _Arguments >( arguments ) ... );
                return ::std::forward< InstanceRefer >( instance );
            }
        };

        template <>
        struct SquareBracketsSwitch< HolderHasNoOperatorCase >
        {
            template < typename _Instance, typename ... _Arguments >
            static decltype(auto) invoke ( _Instance && instance, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                using Returned = ::std::result_of_t< SquareBrackets( ValueRefer, _Arguments && ... ) >;
                return ResultSwitch< ResultCase< Returned, ValueRefer > >::invoke(
                    ::std::forward< InstanceRefer >( instance ), SquareBrackets(), ::std::forward< _Arguments >( arguments ) ... );
            }
        };
    }
}

namespace Operator
{
    namespace Unary
    {
        IS_POSTFIX_OPERATOR_WITH_ARGUMENT_EXISTS_TRAIT( [], SquareBrackets )
        IS_METHOD_EXISTS_TRAIT( operatorSquareBrackets )
    }
}

#define UNARY_OPERATOR_IMPLEMENTAION_EXP( symbol, Invokable ) \
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
                constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
                { \
                    using HolderRefer = _Refer &&; \
                    static_assert( ::std::is_same< Holder, ::std::decay_t< HolderRefer > >::value, \
                        "The template parameter _Refer must to be a refer of template parameter Instance< _Value, _Tool >::Holder." ); \
                    using ValueRefer = ::SimilarRefer< Value, HolderRefer >; \
                    return ::Operator::Invokable< Value >()( ::std::forward< ValueRefer >( ::HolderInternal::value< ValueRefer, HolderRefer >( ::std::forward< HolderRefer >( refer ) ) ), ::std::forward< _Arguments >( arguments ) ... ); \
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
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using InstanceRefer = _Refer &&; \
                static_assert( ::std::is_same< Instance, ::std::decay_t< InstanceRefer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter Instance< _Value, _Tool >." ); \
                using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::Spec::Invokable< Holder >()( ::std::forward< HolderRefer >( refer.m_holder ), ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        };\
    } \




#define UNARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
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
                constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
                { \
                    using HolderRefer = _Refer &&; \
                    static_assert( ::std::is_same< Holder, ::std::decay_t< HolderRefer > >::value, \
                        "The template parameter _Refer must to be a refer of template parameter Instance< _Value, _Tool >::Holder." ); \
                    using ValueRefer = ::SimilarRefer< Value, HolderRefer >; \
                    return ::Operator::Invokable< Value >()( ::std::forward< ValueRefer >( ::HolderInternal::value< ValueRefer, HolderRefer >( ::std::forward< HolderRefer >( refer ) ) ), ::std::forward< _Arguments >( arguments ) ... ); \
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
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using InstanceRefer = _Refer &&; \
                static_assert( ::std::is_same< Instance, ::std::decay_t< InstanceRefer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter Instance< _Value, _Tool >." ); \
                using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >; \
                /* TODO: вернуть Instance с заблокированным Holder, или Instance над значением, или фундаментальное значение */ \
                return ::Operator::Spec::Invokable< Holder >()( ::std::forward< HolderRefer >( refer.m_holder ), ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        };\
    } \


#define POSTFIX_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    IS_POSTFIX_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must to be decayed." ); \
            template < typename _Refer > \
            constexpr decltype(auto) operator () ( _Refer && refer ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ) symbol; \
            } \
        }; \
    } \

#define PREFIX_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    IS_PREFIX_OPERATOR_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must to be decayed." ); \
            template < typename _Refer > \
            constexpr decltype(auto) operator () ( _Refer && refer ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter _Value." ); \
                return symbol ::std::forward< Refer >( refer ); \
            } \
        }; \
    } \

#define POSTFIX_OPERATOR_WITH_ARGUMENT_IMPLEMENTAION( symbol, Invokable ) \
    IS_POSTFIX_OPERATOR_WITH_ARGUMENT_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must to be decayed." ); \
            template < typename _Refer, typename _Argument > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Argument && argument ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ). operator symbol ( ::std::forward< _Argument >( argument ) ); \
            } \
        }; \
    } \

#define POSTFIX_OPERATOR_WITH_ARGUMENTS_IMPLEMENTAION( symbol, Invokable ) \
    IS_POSTFIX_OPERATOR_WITH_ARGUMENTS_EXISTS_TRAIT( SINGLE_ARG( symbol ), Invokable ) \
    UNARY_OPERATOR_IMPLEMENTAION( SINGLE_ARG( symbol ), Invokable ) \
    namespace Operator \
    { \
        template < typename _Value > \
        struct Invokable \
        { \
            using Value = _Value; \
            static_assert( ::std::is_same< Value, ::std::decay_t< Value > >::value, \
                "The template parameter _Value must to be decayed." ); \
            template < typename _Refer, typename ... _Arguments > \
            constexpr decltype(auto) operator () ( _Refer && refer, _Arguments && ... arguments ) \
            { \
                using Refer = _Refer &&; \
                static_assert( ::std::is_same< Value, ::std::decay_t< Refer > >::value, \
                    "The template parameter _Refer must to be a refer of template parameter _Value." ); \
                return ::std::forward< Refer >( refer ). operator symbol ( ::std::forward< _Arguments >( arguments ) ... ); \
            } \
        }; \
    } \

#endif
