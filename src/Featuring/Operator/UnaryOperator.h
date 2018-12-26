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
        struct ThisCase {};
        struct DefaultCase {};
        struct BlockedCase {};

        template < typename _Returned, typename _Refer >
        struct ResultCaseHelper
        {
            using Returned = _Returned;
            using Value = ::std::decay_t< _Refer >;

            static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
            static constexpr bool returned_is_same_value = ::std::is_same< _Returned, _Refer >::value;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalCase,
                ::std::conditional_t< returned_is_same_value,
                    ThisCase,
                    ::std::conditional_t< returned_is_reference,
                        BlockedCase,
                        DefaultCase > > >;
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
                return invokable( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct ResultSwitch< ThisCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                invokable( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... );
                return ::std::forward< InstanceRefer >( instance );
            }
        };

        template <>
        struct ResultSwitch< DefaultCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                static_assert( !::std::is_reference< Returned >::value,
                    "The type of return parameter must to be a non reference type." );
                using Tool = ::Inplace::DefaultTool;
                return Instance< Returned, Tool >( invokable( InstanceGuard< InstanceRefer >( ::std::forward< InstanceRefer >( instance ) ).instanceAccess(), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };

        template <>
        struct ResultSwitch< BlockedCase >
        {
            template < typename _Instance, typename _Invokable, typename ... _Arguments >
            static constexpr decltype(auto) invoke ( _Instance && instance, _Invokable && invokable, _Arguments && ... arguments )
            {
                using InstanceRefer = _Instance &&;
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< InstanceRefer >::Value, InstanceRefer >;
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
                    return ResultSwitch< ResultCase< Returned, ValueRefer > >::invoke( \
                        ::std::forward< InstanceRefer >( instance ), Invokable() ); \
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
                    return ResultSwitch< ResultCase< Returned, ValueRefer > >::invoke( \
                        ::std::forward< InstanceRefer >( instance ), Invokable() ); \
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
                    return ResultSwitch< ResultCase< Returned, ValueRefer > >::invoke( \
                        ::std::forward< InstanceRefer >( instance ), Invokable(), ::std::forward< _Argument >( argument ) ); \
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
                    return ResultSwitch< ResultCase< Returned, ValueRefer > >::invoke( \
                        ::std::forward< InstanceRefer >( instance ), Invokable(), ::std::forward< _Arguments >( arguments ) ... ); \
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
