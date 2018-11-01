#pragma once

#include <ModelKit/Common/FeatureGuard.h>
#include <ModelKit/Common/ValueGuard.h>
#include <ModelKit/Common/ValueTrait.h>

template < typename _InstanceType, typename _OtherReferType >
class InstanceCompatibleResolver;

template < typename _InstanceType, typename _OtherReferType >
class InstanceFeatureResolver;

template < typename _InstanceType, typename _OtherReferType >
class InstanceValueResolver;

/*!
 * Помошник вычисляет необходимый способ предоставления доступа к значению.
 * Если типы Instance совместимы или другой совместим с внутренней частью этого,
 * то операция выполняется между внутренними представлениями Instance.
 * Если данный тип совместим с внутренней частью другого, то другой
 * "разрешается" до типа совместимого с этим. Во всех других случаях
 * другой "разрешается" до базового внутреннего значения.
 */
template < typename _InstanceType, typename _OtherReferType >
struct InstanceResolverHelper
{
    using InstanceType = _InstanceType;
    using OtherReferType = _OtherReferType;
    using OtherInstanceType = ::std::decay_t< OtherReferType >;

    using type = ::std::conditional_t<
        is_compatible_v< InstanceType, OtherInstanceType >
            || is_part_of_v< OtherInstanceType, InstanceType >,
        InstanceCompatibleResolver< _InstanceType, _OtherReferType >,
        ::std::conditional_t<
            is_part_of_v< InstanceType, OtherInstanceType >,
            InstanceFeatureResolver< _InstanceType, _OtherReferType >,
            InstanceValueResolver< _InstanceType, _OtherReferType > > >;
};

template < typename _InstanceType, typename _OtherReferType >
using InstanceResolver = typename InstanceResolverHelper< _InstanceType, _OtherReferType >::type;

/*!
 *
 */
template < typename _InstanceType, typename _OtherReferType >
class InstanceCompatibleResolver
{
public:
    using InstanceType = _InstanceType;
    using OtherReferType = _OtherReferType;
    using OtherInstanceType = ::std::remove_reference_t< OtherReferType >;
    using OtherHolderReferType = ::std::conditional_t<
        ::std::is_const< OtherInstanceType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherReferType >::value,
            const typename OtherInstanceType::HolderType &&,
            const typename OtherInstanceType::HolderType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherReferType >::value,
            typename OtherInstanceType::HolderType &&,
            typename OtherInstanceType::HolderType & > >;

private:
    OtherReferType m_other_refer;

public:
    InstanceCompatibleResolver ( OtherReferType other )
    : m_other_refer( ::std::forward< OtherReferType >( other ) )
    {
    }

    OtherHolderReferType resolve () const
    {
        return ::std::forward< OtherHolderReferType >( m_other_refer.m_holder );
    }
};

/*!
 *
 */
template < typename _InstanceType, typename _OtherReferType >
class InstanceFeatureResolver
{
public:
    using InstanceType = _InstanceType;
    using OtherReferType = _OtherReferType;
    using OtherFeatureGuardType = FeatureGuard< OtherReferType >;
    using OtherInstanceType = ::std::remove_reference_t< OtherReferType >;
    using OtherValueReferType = ::std::conditional_t<
        ::std::is_const< OtherInstanceType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherReferType >::value,
            const typename OtherInstanceType::ValueType &&,
            const typename OtherInstanceType::ValueType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherReferType >::value,
            typename OtherInstanceType::ValueType &&,
            typename OtherInstanceType::ValueType & > >;

    using NextResolverType = InstanceResolver< _InstanceType, OtherValueReferType >;

private:
    OtherFeatureGuardType m_feature_guard;
    NextResolverType m_next_resolver;

public:
    InstanceFeatureResolver ( OtherReferType other )
    : m_feature_guard( ::std::forward< OtherReferType >( other ) )
    , m_next_resolver( ::std::forward< OtherValueReferType >( m_feature_guard.value() ) )
    {
    }

    decltype(auto) resolve () const
    {
        return m_next_resolver.resolve();
    }
};

/*!
 *
 */
template < typename _InstanceType, typename _OtherReferType >
class InstanceValueResolver
{
public:
    using InstanceType = _InstanceType;
    using OtherReferType = _OtherReferType;
    using OtherValueGuardType = ValueGuard< OtherReferType >;

private:
    OtherValueGuardType m_value_guard;

public:
    InstanceValueResolver ( OtherReferType other )
    : m_value_guard( ::std::forward< OtherReferType >( other ) )
    {
    }

    decltype(auto) resolve () const
    {
        return *m_value_guard;
    }
};
