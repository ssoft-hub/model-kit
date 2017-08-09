#pragma once

#include <ModelKit/Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи композитной (уникальной) агрегации.
 */
template < typename _ValueType, typename _RelationTool >
class AssociationEnd< _ValueType, AggregationKind::Shared, _RelationTool >
{
    using ThisType = AssociationEnd< _ValueType, AggregationKind::Composite, _RelationTool >;

public:
    constexpr AssociationEnd ()
    {
    }

    AssociationEnd ( const ThisType & other ) = delete;
    ThisType & operator = ( const ThisType & other ) = delete;

    AssociationEnd ( ThisType && /*other*/ )
    {
    }

    ThisType & operator = ( ThisType && /*other*/ )
    {
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    AssociationEnd ( AssociationEnd< _OtherType, AggregationKind::Shared, _OtherTool > && /*other*/ )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( AssociationEnd< _OtherType, AggregationKind::Shared, _OtherTool > && /*other*/ )
    {
        return *this;
    }

    bool operator ! () const
    {
        return false;
    }

public:
    template < typename ... _Arguments >
    static constexpr ThisType make ( _Arguments && ... /*arguments*/ )
    {
        return ThisType();
    }

    template < typename _OtherType, typename _OtherTool >
    static ThisType share ( const AssociationEnd< _OtherType, AggregationKind::Shared, _OtherTool > & /*other*/ )
    {
        return ThisType();
    }
};
