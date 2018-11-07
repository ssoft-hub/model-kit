#pragma once

#include <ModelKit/Common/FeaturePointer.h>

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
class AssociationEnd;

/*!
 * Класс - защитник свойства верхнего уровня для экземпляра значения.
 * Реализует специализацию для значений обернутых в Association.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
struct AssociationFeaturePointer
{
    using ThisType = AssociationFeaturePointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = ReferType;

    using AssociationPointer = ReferPointer< ReferType >;
    using AssociationType = ::std::remove_reference_t< ReferType >;
    using AssociationTool = typename AssociationType::RelationTool;

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

private:
    AssociationPointer m_pointer;

public:
    constexpr AssociationFeaturePointer ()
        : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AssociationFeaturePointer ( ReferType refer )
        : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        AssociationTool::guardHolder( ::std::forward< HolderReferType >( m_pointer->m_holder ) );
    }

    AssociationFeaturePointer ( ThisType && other )
        : m_pointer( ::std::forward< AssociationPointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~AssociationFeaturePointer ()
    {
        if ( !!m_pointer )
            AssociationTool::unguardHolder( ::std::forward< HolderReferType >( m_pointer->m_holder ) );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr const AssociationPointer & operator -> () const
    {
        return m_pointer;
    }
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationFeaturePointer< AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationFeaturePointer< AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > & >
{
    using type = AssociationFeaturePointer< const AssociationEnd< _ValueType, _kind, _RelationTool > & >;
};

template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > && >
{
    using type = AssociationFeaturePointer< const AssociationEnd< _ValueType, _kind, _RelationTool > && >;
};

// disabled
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< AssociationEnd< _ValueType, _kind, _RelationTool > > {};
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
struct FeaturePointerHelper< const AssociationEnd< _ValueType, _kind, _RelationTool > > {};
