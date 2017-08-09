#pragma once

#include <ModelKit/Common/ValueGuard.h>

template < typename _Type, AggregationKind _Kind, typename _RelationTool >
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
    using AssociationType = typename ::std::remove_reference< ReferType >::type;
    using ValueTool = typename AssociationType::RelationTool;

    using ValueReferType = typename ::std::conditional<
        ::std::is_const< AssociationType >::value,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename AssociationType::ValueType &&,
            const typename AssociationType::ValueType & >::type,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            typename AssociationType::ValueType &&,
            typename AssociationType::ValueType & >::type >::type;

    using HolderReferType = typename ::std::conditional<
        ::std::is_const< AssociationType >::value,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename AssociationType::HolderType &&,
            const typename AssociationType::HolderType & >::type,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            typename AssociationType::HolderType &&,
            typename AssociationType::HolderType & >::type >::type;

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


template < typename _Type, AggregationKind _Kind, typename _RelationTool >
struct ValueGuardHelper< AssociationEnd< _Type, _Kind, _RelationTool > & >
{
    using type = AssociationValueGuard< AssociationEnd< _Type, _Kind, _RelationTool > & >;
};

template < typename _Type, AggregationKind _Kind, typename _RelationTool >
struct ValueGuardHelper< AssociationEnd< _Type, _Kind, _RelationTool > && >
{
    using type = AssociationValueGuard< AssociationEnd< _Type, _Kind, _RelationTool > && >;
};

template < typename _Type, AggregationKind _Kind, typename _RelationTool >
struct ValueGuardHelper< const AssociationEnd< _Type, _Kind, _RelationTool > & >
{
    using type = AssociationValueGuard< const AssociationEnd< _Type, _Kind, _RelationTool > & >;
};
