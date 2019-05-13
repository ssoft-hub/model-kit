#pragma once
#ifndef SCL_FEATURING_OPERATOR_BINARY_H
#define SCL_FEATURING_OPERATOR_BINARY_H

#include <ModelKit/Utility/IsMethodExists.h>
#include <ModelKit/Utility/IsOperatorExists.h>
#include <ModelKit/Utility/SingleArgument.h>
#include "ResultSwitch.h"

namespace SclPrivate
{
    namespace Operator
    {
        namespace Binary
        {
            /* Cases for Holder method existing */
            struct HolderHasOperatorCase {};
            struct HolderHasNoOperatorCase {};
        }
    }
}

namespace SclPrivate
{
    namespace Operator
    {
        namespace Global
        {
            struct DefaultCase;
        }
    }
}

#define SCL_IS_BINARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
    template < typename _Kind, typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    using Is ## Invokable ## OperatorExistsTest = Is ## Invokable ## OperatorExistsTestHelper< ::SclPrivate::Operator::InstanceSwitchCase< _LeftRefer, _RightRefer >, _LeftRefer, _RightRefer >; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    static constexpr bool is_ ## Invokable ## _operator_exists_test = Is ## Invokable ## OperatorExistsTest< _LeftRefer, _RightRefer >::value; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::SclPrivate::Operator::NoneInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
     \
        static const bool value = ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists< _LeftRefer, _RightRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::SclPrivate::Operator::LeftInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftInstanceRefer = _LeftRefer; \
        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
        using LeftHolder = typename LeftInstance::Holder; \
        using LeftHolderRefer = ::ScL::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
        using LeftValue = typename LeftInstance::Value; \
        using LeftValueRefer = ::ScL::SimilarRefer< LeftValue, LeftInstanceRefer >; \
        using RightRefer = _RightRefer; \
     \
        static const bool value = ::SclPrivate::Operator::Binary::is_operator ## Invokable ## Left_method_exists< LeftHolder, void(LeftHolderRefer,RightRefer) > \
            || ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists< LeftValueRefer, RightRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::SclPrivate::Operator::RightInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftRefer = _LeftRefer; \
        using RightInstanceRefer = _RightRefer; \
        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
        using RightHolder = typename RightInstance::Holder; \
        using RightHolderRefer = ::ScL::SimilarRefer< RightHolder, RightInstanceRefer >; \
        using RightValue = typename RightInstance::Value; \
        using RightValueRefer = ::ScL::SimilarRefer< RightValue, RightInstanceRefer >; \
     \
        static const bool value = ::SclPrivate::Operator::Binary::is_operator ## Invokable ## Right_method_exists< RightHolder, void(LeftRefer,RightHolderRefer) > \
            || ::SclPrivate::Operator::Binary::is_ ## Invokable ## _operator_exists< LeftRefer, RightValueRefer >; \
    }; \
     \
    template < typename _LeftRefer, typename _RightRefer > \
    struct Is ## Invokable ## OperatorExistsTestHelper< ::SclPrivate::Operator::BothInstanceCase, _LeftRefer, _RightRefer > \
    { \
        static_assert( ::std::is_reference< _LeftRefer >::value, "The template parameter _LeftRefer must to be a reference type." ); \
        static_assert( ::std::is_reference< _RightRefer >::value, "The template parameter _RightRefer must to be a reference type." ); \
        using LeftInstanceRefer = _LeftRefer; \
        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
        using LeftHolder = typename LeftInstance::Holder; \
        using LeftHolderRefer = ::ScL::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
        using LeftValue = typename LeftInstance::Value; \
        using LeftValueRefer = ::ScL::SimilarRefer< LeftValue, LeftInstanceRefer >; \
        using RightInstanceRefer = _RightRefer; \
        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
        using RightHolder = typename RightInstance::Holder; \
        using RightHolderRefer = ::ScL::SimilarRefer< RightHolder, RightInstanceRefer >; \
        using RightValue = typename RightInstance::Value; \
        using RightValueRefer = ::ScL::SimilarRefer< RightValue, RightInstanceRefer >; \
     \
        static const bool is_compatible_value = ::ScL::is_compatible< LeftInstance, RightInstance > \
            && ( ::SclPrivate::Operator::Binary::is_operator ## Invokable ## _method_exists< LeftHolder, void(LeftHolderRefer,RightHolderRefer) > \
                || is_ ## Invokable ## _operator_exists_test< LeftValueRefer, LeftValueRefer > ); \
     \
        static const bool is_left_path_of_right_value = ::ScL::is_this_part_of_other< LeftInstance, RightInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftInstanceRefer, RightValueRefer >; \
     \
        static const bool is_right_path_of_left_value = ::ScL::is_this_part_of_other< RightInstance, LeftInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftValueRefer, RightInstanceRefer >; \
     \
        static const bool is_not_compatible_value = !::ScL::is_compatible< LeftInstance, RightInstance > \
            && is_ ## Invokable ## _operator_exists_test< LeftValueRefer, LeftValueRefer >; \
     \
        static const bool value = is_compatible_value \
            || is_left_path_of_right_value \
            || is_right_path_of_left_value \
            || is_not_compatible_value; \
    }; \

#define SCL_BINARY_OPERATOR_IMPLEMENTAION( symbol, Invokable ) \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Binary \
            { \
                SCL_IS_BINARY_OPERATOR_EXISTS_TRAIT( SCL_SINGLE_ARG( symbol ), Invokable ) \
                SCL_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ) \
                SCL_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Left ) \
                SCL_IS_METHOD_EXISTS_TRAIT( operator ## Invokable ## Right ) \
                SCL_IS_BINARY_OPERATOR_EXISTS_TEST_TRAIT( Invokable ) \
            } \
        } \
    } \
     \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Global \
            { \
                struct Invokable ## Case; \
            } \
        } \
    } \
     \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Binary \
            { \
                template < typename > \
                struct Invokable ## Operator; \
                 \
                template <> \
                struct Invokable ## Operator< ::SclPrivate::Operator::Global::DefaultCase > \
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
                            ::ScL::is_instance< ::std::decay_t< RightRefer > >, \
                                ::SclPrivate::Operator::Global::Invokable ## Case, \
                                ::SclPrivate::Operator::Global::DefaultCase >; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Operator< Invokable ## OperatorSwitchCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                    } \
                }; \
            } \
        } \
    } \
     \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Binary \
            { \
                template < typename, typename > \
                struct Invokable ## Switch; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::LeftInstanceCase, ::SclPrivate::Operator::Binary::HolderHasOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder; \
                        using RightRefer = _Right &&; \
                        LeftHolder::operator ## Invokable ## Left( ::SclPrivate::instanceHolder( ::std::forward< LeftInstanceRefer >( left ) ), ::std::forward< RightRefer >( right ) ); \
                        return ::std::forward< LeftInstanceRefer >( left ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::LeftInstanceCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                        using RightRefer = _Right &&; \
                        using Returned = ::std::result_of_t< ::SclPrivate::Operator::Binary::Invokable( LeftValueRefer, RightRefer ) >; \
                        return ::SclPrivate::Operator::ResultSwitch< ::SclPrivate::Operator::LeftInstanceCase, ::SclPrivate::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                            ::invoke( ::SclPrivate::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::RightInstanceCase, ::SclPrivate::Operator::Binary::HolderHasOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftRefer = _Left &&; \
                        using RightInstanceRefer = _Right &&; \
                        using RightHolder = typename ::std::decay_t< RightInstanceRefer >::Holder; \
                        RightHolder::operator ## Invokable ## Right( ::std::forward< LeftRefer >( left ), ::SclPrivate::instanceHolder( ::std::forward< RightInstanceRefer >( right ) ) ); \
                        return ::std::forward< RightInstanceRefer >( right ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::RightInstanceCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftRefer = _Left &&; \
                        using RightInstanceRefer = _Right &&; \
                        using RightValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
                        using Returned = ::std::result_of_t< ::SclPrivate::Operator::Binary::Invokable( LeftRefer, RightValueRefer ) >; \
                        return ::SclPrivate::Operator::ResultSwitch< ::SclPrivate::Operator::RightInstanceCase, ::SclPrivate::Operator::ResultSwitchCase< Returned, RightValueRefer > > \
                            ::invoke( ::SclPrivate::Operator::Binary::Invokable(), ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::BothInstanceCase, ::SclPrivate::Operator::Binary::HolderHasOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftHolder = typename ::std::decay_t< LeftInstanceRefer >::Holder; \
                        using RightInstanceRefer = _Right &&; \
                        LeftHolder::operator ## Invokable ( ::SclPrivate::instanceHolder( ::std::forward< LeftInstanceRefer >( left ) ), ::SclPrivate::instanceHolder( ::std::forward< RightInstanceRefer >( right ) ) ); \
                        return ::std::forward< RightInstanceRefer >( right ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::BothInstanceCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using RightInstanceRefer = _Right &&; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Switch< ::SclPrivate::Operator::ExposingSwitchCase< LeftInstanceRefer, RightInstanceRefer >, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                            ::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::BothExposingCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                        using RightInstanceRefer = _Right &&; \
                        using RightValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
         \
                        using Returned = ::std::result_of_t< ::SclPrivate::Operator::Binary::Invokable( LeftValueRefer, RightValueRefer ) >; \
                        return ::SclPrivate::Operator::ResultSwitch< ::SclPrivate::Operator::BothExposingCase, ::SclPrivate::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                            ::invoke( ::SclPrivate::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::LeftExposingCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< LeftInstanceRefer >::Value, LeftInstanceRefer >; \
                        using RightInstanceRefer = _Right &&; \
         \
                        using Returned = ::std::result_of_t< ::SclPrivate::Operator::Binary::Invokable( LeftValueRefer, RightInstanceRefer ) >; \
                        return ::SclPrivate::Operator::ResultSwitch< ::SclPrivate::Operator::LeftExposingCase, ::SclPrivate::Operator::ResultSwitchCase< Returned, LeftValueRefer > > \
                            ::invoke( ::SclPrivate::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## Switch< ::SclPrivate::Operator::RightExposingCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using RightInstanceRefer = _Right &&; \
                        using RightValueRefer = ::ScL::SimilarRefer< typename ::std::decay_t< RightInstanceRefer >::Value, RightInstanceRefer >; \
         \
                        using Returned = ::std::result_of_t< ::SclPrivate::Operator::Binary::Invokable( LeftInstanceRefer, RightValueRefer ) >; \
                        return ::SclPrivate::Operator::ResultSwitch< ::SclPrivate::Operator::RightExposingCase, ::SclPrivate::Operator::ResultSwitchCase< Returned, LeftInstanceRefer > > \
                            ::invoke( ::SclPrivate::Operator::Binary::Invokable(), ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
            } \
        } \
    } \
     \
    namespace SclPrivate \
    { \
        namespace Operator \
        { \
            namespace Binary \
            { \
                template < typename > \
                struct Invokable ## InstanceSwitch; \
         \
                template <> \
                struct Invokable ## InstanceSwitch < ::SclPrivate::Operator::LeftInstanceCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
                        using LeftHolder = typename LeftInstance::Holder; \
                        using LeftHolderRefer = ::ScL::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                        using RightRefer = _Right &&; \
         \
                        constexpr bool holder_has_method_for_operator = ::SclPrivate::Operator::Binary::is_operator ## Invokable ## Left_method_exists< LeftHolder, void( LeftHolderRefer, RightRefer ) >; \
                        using OperatorSwitchCase = ::std::conditional_t< holder_has_method_for_operator, ::SclPrivate::Operator::Binary::HolderHasOperatorCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase >; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Switch< LeftInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## InstanceSwitch < ::SclPrivate::Operator::RightInstanceCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using RightInstanceRefer = _Right &&; \
                        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
                        using RightHolder = typename RightInstance::Holder; \
                        using RightHolderRefer = ::ScL::SimilarRefer< RightHolder, RightInstanceRefer >; \
                        using LeftRefer = _Left &&; \
         \
                        constexpr bool holder_has_method_for_operator = ::SclPrivate::Operator::Binary::is_operator ## Invokable ## Right_method_exists< RightHolder, void( LeftRefer, RightHolderRefer ) >; \
                        using OperatorSwitchCase = ::std::conditional_t< holder_has_method_for_operator, ::SclPrivate::Operator::Binary::HolderHasOperatorCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase >; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Switch< RightInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
         \
                template <> \
                struct Invokable ## InstanceSwitch < ::SclPrivate::Operator::BothInstanceCase > \
                { \
                    template < typename _Left, typename _Right > \
                    static constexpr decltype(auto) invoke ( _Left && left, _Right && right ) \
                    { \
                        using LeftInstanceRefer = _Left &&; \
                        using LeftInstance = ::std::decay_t< LeftInstanceRefer >; \
                        using LeftHolder = typename LeftInstance::Holder; \
                        using LeftHolderRefer = ::ScL::SimilarRefer< LeftHolder, LeftInstanceRefer >; \
                        using RightInstanceRefer = _Right &&; \
                        using RightInstance = ::std::decay_t< RightInstanceRefer >; \
                        using RightHolder = typename RightInstance::Holder; \
                        using RightHolderRefer = ::ScL::SimilarRefer< RightHolder, RightInstanceRefer >; \
         \
                        constexpr bool is_left_compatible_to_right = ::ScL::is_compatible< LeftHolder, RightHolder >; \
                        constexpr bool holder_has_method_for_operator = ::SclPrivate::Operator::Binary::is_operator ## Invokable ## _method_exists< LeftHolder, void( LeftHolderRefer, RightHolderRefer ) >; \
                        using OperatorSwitchCase = ::std::conditional_t< is_left_compatible_to_right && holder_has_method_for_operator, ::SclPrivate::Operator::Binary::HolderHasOperatorCase, ::SclPrivate::Operator::Binary::HolderHasNoOperatorCase >; \
                        return ::SclPrivate::Operator::Binary::Invokable ## Switch< BothInstanceCase, OperatorSwitchCase >::invoke( ::std::forward< LeftInstanceRefer >( left ), ::std::forward< RightInstanceRefer >( right ) ); \
                    } \
                }; \
            } \
        } \
    } \
     \
    namespace SclPrivate \
    { \
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
                        return ::SclPrivate::Operator::Binary::Invokable ## InstanceSwitch< ::SclPrivate::Operator::InstanceSwitchCase< LeftRefer, RightRefer > > \
                            ::invoke( ::std::forward< LeftRefer >( left ), ::std::forward< RightRefer >( right ) ); \
                    } \
                }; \
            } \
        } \
    } \

#endif
