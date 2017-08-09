#pragma once

#include <ModelKit/Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи не являющегося агрегацией.
 */
template < typename _ValueType, typename _RelationTool >
class AssociationEnd< _ValueType, AggregationKind::None, _RelationTool >
{
    using ThisType = AssociationEnd< _ValueType, AggregationKind::None, _RelationTool >;

public:
    constexpr AssociationEnd ()
    {
    }

    AssociationEnd ( const ThisType & other )
    {
    }

    ThisType & operator = ( const ThisType & other )
    {
    }

    AssociationEnd ( ThisType && /*other*/ )
    {
    }

    ThisType & operator = ( ThisType && /*other*/ )
    {
        return *this;
    }

    template < typename _OtherType, AggregationKind _kind, typename _OtherTool >
    AssociationEnd ( const AssociationEnd< _OtherType, _kind, _OtherTool > & /*other*/ )
    {
    }

    template < typename _OtherType, AggregationKind _kind, typename _OtherTool >
    ThisType & operator = ( const AssociationEnd< _OtherType, _kind, _OtherTool > & other )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    AssociationEnd ( AssociationEnd< _OtherType, AggregationKind::None, _OtherTool > && /*other*/ )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( AssociationEnd< _OtherType, AggregationKind::None, _OtherTool > && /*other*/ )
    {
        return *this;
    }
};
