#pragma once
#ifndef MDK_FEATURING_OPERATOR_BINARY_H
#define MDK_FEATURING_OPERATOR_BINARY_H

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

namespace Operator
{
    namespace Global
    {
        struct DefaultCase;
    }
}

#define MDK_IS_BINARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
    template < typename _Kind, typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    using Is ## Invokable ## OperatorExistsTest = Is ## Invokable ## OperatorExistsTestHelper< ::Operator::InstanceSwitchCase< _LeftRefer, _RightRefer >, _LeftRefer, _RightRefer >; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    static constexpr bool is_ ## Invokable ## _operator_exists_test = Is ## Invokable ## OperatorExistsTest< _LeftRefer, _RightRefer >::value; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::NoneInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
     \
        static const bool value = ::Operator::Binary::is_ ## Invokable ## _operator_exists< _LeftRefer, _RightRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::LeftInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftInstanceRefer = _LeftRefer; \
        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
        using LeftHolder = typename LeftInstance::Holder; \
        using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
        using LeftValue = typename LeftInstance::Value; \
        using LeftValueRefer = ::SimilarRefer< LeftValue, LeftInstanceRefer >; \
        using RightRefer = _RightRefer; \
     \
        static const bool value = ::Operator::Binary::is_operator ## Invokable ## Left_method_exists< LeftHolder, void(LeftHolderRefer,RightRefer) > \
            || ::Operator::Binary::is_ ## Invokable ## _operator_exists< LeftValueRefer, RightRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::RightInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftRefer = _LeftRefer; \
        using RightInstanceRefer = _RightRefer; \
        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
        using RightHolder = typename RightInstance::Holder; \
        using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
        using RightValue = typename RightInstance::Value; \
        using RightValueRefer = ::SimilarRefer< RightValue, RightInstanceRefer >; \
     \
        static const bool value = ::Operator::Binary::is_operator ## Invokable ## Right_method_exists< RightHolder, void(LeftRefer,RightHolderRefer) > \
            || ::Operator::Binary::is_ ## Invokable ## _operator_exists< LeftRefer, RightValueRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::Operator::BothInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftInstanceRefer = _LeftRefer; \
        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
        using LeftHolder = typename LeftInstance::Holder; \
        using LeftHolderRefer = ::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
        using LeftValue = typename LeftInstance::Value; \
        using LeftValueRefer = ::SimilarRefer< LeftValue, LeftInstanceRefer >; \
        using RightInstanceRefer = _RightRefer; \
        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
        using RightHolder = typename RightInstance::Holder; \
        using RightHolderRefer = ::SimilarRefer< RightHolder, RightInstanceRefer >; \
        using RightValue = typename RightInstance::Value; \
        using RightValueRefer = ::SimilarRefer< RightValue, RightInstanceRefer >; \
     \
        static const bool is_compatible_value = ::is_compatible< LeftInstance, RightInstance > \
            && ( ::Operator::Binary::is_operator ## Invokable ## _method_exists< LeftHolder, void(LeftHolderRefer,RightHolderRefer) > \
                || is_ ## Invokable ## _operator_exists_test< LeftValueRefer, LeftValueRefer > ); \
     \
        static const bool is_left_path_of_right_value = ::is_this_part_of_other< LeftInstance, RightInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftInstanceRefer, RightValueRefer >; \
     \
        static const bool is_right_path_of_left_value = ::is_this_part_of_other< RightInstance, LeftInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftValueRefer, RightInstanceRefer >; \
     \
        static const bool is_not_compatible_value = !::is_compatible< LeftInstance, RightInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftValueRefer, LeftValueRefer >; \
     \
        static const bool value = is_compatible_value \
            || is_left_path_of_right_value \
            || is_right_path_of_left_value \
            || is_not_compatible_value; \
    }; \

#define MDK_BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace Operator \
    { \
        namespace Binary \
        { \
            MDK_IS_BINARY_OPERATOR_EXISTS_TRAIT( MDK_SINGLE_ARG( symbol ), Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Left ) \
            MDK_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Right ) \
            MDK_IS_BINARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Global \
        { \
            struct Invokable ## Case; \
        } \
    } \
     \
    namespace Operator \
    { \
        namespace Binary \
        { \
            template < typename > \
            struct Invokable ## Operator; \
             \
            template <> \
            struct Invokable ## Operator< ::Operator::Global::DefaultCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightRefer = _Right &&; \
                    return ::std::forward< LeftRefer >( left ) symbol ::std::forward< RightRefer >( right ); \
                } \
            }; \
             \
            struct Invokable \
            { \
                template < typename _Left, typename _Right > \
                decltype(auto) operator () ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightRefer = _Right &&; \
                    using Invokable ## OperatorSwitchCase = ::std::conditional_t< \
                        ::is_instance< ::std::decay_t< RightRefer > >, \
                            ::Operator::Global::Invokable ## Case, \
                            ::Operator::Global::DefaultCase >; \
                    return ::Operator::Binary::Invokable ## Operator< Invokable ## OperatorSwitchCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
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
            struct Invokable ## Switch< ::Operator::LeftInstanceCase, ::Operator::Binary::HolderHasOperatorCase > \
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
            struct Invokable ## Switch< ::Operator::LeftInstanceCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                    using RightRefer = _Right &&; \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftValueRefer, RightRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftInstanceCase, ::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::RightInstanceCase, ::Operator::Binary::HolderHasOperatorCase > \
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
            struct Invokable ## Switch< ::Operator::RightInstanceCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftRefer = _Left &&; \
                    using RightInstanceRefer = _Right &&; \
                    using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftRefer, RightValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::RightInstanceCase, ::Operator::ResultSwitchCase< Returned, RightValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::BothInstanceCase, ::Operator::Binary::HolderHasOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder; \
                    using RightInstanceRefer = _Right &&; \
                    LeftHolder::operator ## Invokable ( ::instanceHolder( ::std::forward< LeftInstanceRefer >( left ) ), ::instanceHolder( ::std::forward< RightInstanceRefer >( right ) ) ); \
                    return ::std::forward< RightInstanceRefer >( right ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::BothInstanceCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using RightInstanceRefer = _Right &&; \
                    return ::Operator::Binary::Invokable ## Switch< ::Operator::ExposingSwitchCase< LeftInstanceRefer, RightInstanceRefer >, ::Operator::Binary::HolderHasNoOperatorCase > \
                        ::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::BothExposingCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                    using RightInstanceRefer = _Right &&; \
                    using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
     \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftValueRefer, RightValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::BothExposingCase, ::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::LeftExposingCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using LeftValueRefer = ::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                    using RightInstanceRefer = _Right &&; \
     \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftValueRefer, RightInstanceRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::LeftExposingCase, ::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                        ::invoke( ::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## Switch< ::Operator::RightExposingCase, ::Operator::Binary::HolderHasNoOperatorCase > \
            { \
                template < typename _Left, typename _Right > \
                static decltype(auto) invoke ( _Left && left, _Right && right ) \
                { \
                    using LeftInstanceRefer = _Left &&; \
                    using RightInstanceRefer = _Right &&; \
                    using RightValueRefer = ::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
     \
                    using Returned = ::std::result_of_t< ::Operator::Binary::Invokable( LeftInstanceRefer, RightValueRefer ) >; \
                    return ::Operator::ResultSwitch< ::Operator::RightExposingCase, ::Operator::ResultSwitchCase< Returned, LeftInstanceRefer > > \
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
            struct Invokable ## InstanceSwitch < ::Operator::LeftInstanceCase > \
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
                    using OperatorSwitchCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< LeftInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < ::Operator::RightInstanceCase > \
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
                    using OperatorSwitchCase = ::std::conditional_t< holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< RightInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                } \
            }; \
     \
            template <> \
            struct Invokable ## InstanceSwitch < ::Operator::BothInstanceCase > \
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
                    constexpr bool holder_has_method_for_operator = ::Operator::Binary::is_operator ## Invokable ## _method_exists< LeftHolder, void( LeftHolderRefer, RightHolderRefer ) >; \
                    using OperatorSwitchCase = ::std::conditional_t< is_left_compatible_to_right && holder_has_method_for_operator, ::Operator::Binary::HolderHasOperatorCase, ::Operator::Binary::HolderHasNoOperatorCase >; \
                    return ::Operator::Binary::Invokable ## Switch< BothInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
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
                    return ::Operator::Binary::Invokable ## InstanceSwitch< ::Operator::InstanceSwitchCase< LeftRefer, RightRefer > > \
                        ::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                } \
            }; \
        } \
    } \

#endif
