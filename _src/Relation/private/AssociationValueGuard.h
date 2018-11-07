#pragma once

#include <ModelKit/Common/ValuePointer.h>

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
struct AssociationValuePointer
{
    using ThisType = AssociationValuePointer< _ReferType >;

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

    using FeaturePointerType = FeaturePointer< ReferType >;
    using ValuePointerType = ValuePointer< ValueReferType >;
    using AccessType = typename ValuePointerType::AccessType;

private:
    FeaturePointerType m_feature_guard;
    ValuePointerType m_internal_guard;

public:
    constexpr AssociationValuePointer ()
        : m_feature_guard()
    , m_internal_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValuePointer ( ReferType refer )
        : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValuePointer ( FeaturePointerType && other )
        : m_feature_guard( ::std::forward< FeaturePointerType >( other ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationValuePointer ( ThisType && other )
        : m_feature_guard( ::std::forward< FeaturePointerType >( other.m_feature_guard ) )
    , m_internal_guard( ::std::forward< ValuePointerType >( other.m_internal_guard ) )
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

    constexpr const ValuePointerType & operator -> () const
    {
        return m_internal_guard;
    }
};


template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationValuePointer< AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationValuePointer< AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationValuePointer< const AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationValuePointer< const AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

// disabled
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > > {};
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct ValuePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > > {};
