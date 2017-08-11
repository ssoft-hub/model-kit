#pragma once

#include <ModelKit/Common/ValueGuard.h>

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
class AssociationEnd;

/*!
 * Класс - защитник всех свойств для экземпляра значения.
 * Реализует специализацию для значений обернутых в Association.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
struct AssociationValueGuard
{
    using ThisType = AssociationValueGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using AssociationType = ::std::remove_reference_t< ReferType >;
    using ValueTool = typename AssociationType::RelationTool;

    using ValueReferType = ::std::conditional_t<
        ::std::is_const< AssociationType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename AssociationType::ValueType &&,
            const typename AssociationType::ValueType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            typename AssociationType::ValueType &&,
            typename AssociationType::ValueType & > >;

    using HolderReferType = ::std::conditional_t<
        ::std::is_const< AssociationType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename AssociationType::HolderType &&,
            const typename AssociationType::HolderType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            typename AssociationType::HolderType &&,
            typename AssociationType::HolderType & > >;

    using FeatureGuardType = FeatureGuard< ReferType >;
    using ValueGuardType = ValueGuard< ValueReferType >;
    using AccessType = typename ValueGuardType::AccessType;

private:
    FeatureGuardType m_feature_guard;
    ValueGuardType m_internal_guard;

public:
    constexpr AssociationValueGuard ()
    : m_feature_guard()
    , m_internal_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValueGuard ( FeatureGuardType && other )
    : m_feature_guard( ::std::forward< FeatureGuardType >( other ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< FeatureGuardType >( other.m_feature_guard ) )
    , m_internal_guard( ::std::forward< ValueGuardType >( other.m_internal_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_feature_guard;
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_internal_guard );
    }

    constexpr const ValueGuardType & operator -> () const
    {
        return m_internal_guard;
    }
};


template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationValueGuard< AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationValueGuard< AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationValueGuard< const AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationValueGuard< const AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

// disabled
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< AssociationEnd< _ValueType, _kind, _RelationTool > > {};
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValueGuardHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > > {};
