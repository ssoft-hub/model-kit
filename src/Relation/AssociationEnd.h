#pragma once

#include <ModelKit/Common/AggregationKind.h>
#include <ModelKit/Common/Cnst.h>
#include "private/AssociationFeatureGuard.h"
#include "private/AssociationOperation.h"
#include "private/AssociationValueGuard.h"

/*!
 * Полюс ассоциативной связи со значением. Подразумевает возможность измненения
 * внутреннего значения без изменения самого полюса.
 *
 * Первый параметр определяет тип значения, второй - средства реализации полюса
 * ассоциативной связи.
 *
 * Ассоциативные связи, реализованные посредством разных инструментов не
 * являются совместимыми. Однако в некоторых случаях можно реализовать
 * преобразование из одной реализации в другую.
 */
template < typename _ValueType, AggregationKind _Kind, typename _RelationTool >
class AssociationEnd
{
    template < typename >
    friend struct AssociationValueGuard;

    template < typename >
    friend struct AssociationFeatureGuard;

    using ThisType = AssociationEnd< _ValueType, _Kind, _RelationTool >;

public:
    using ValueType = _ValueType;
    using RelationTool = _RelationTool;
    using HolderType = typename RelationTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

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

    template < typename _OtherType >
    ThisType & operator = ( const _OtherType & /*other*/ )
    {
        return *this;
    }

    bool operator ! () const
    {
        return false;
    }

    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
//    ValueGuard< ThisType & > operator -> ()
//    {
//        return *this;
//    }

//    ValueGuard< const ThisType & > operator -> () const
//    {
//        return *this;
//    }

    ValueGuard< ThisType & > operator & ()
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator & () const
    {
        return *this;
    }

public:
    template < typename ... _Arguments >
    static constexpr ThisType make ( _Arguments && ... /*arguments*/ )
    {
        static_assert( _Kind != AggregationKind::None, "Make not supported for none aggregation type." );
        return ThisType();
    }

    template < typename _OtherType, typename _OtherTool >
    static ThisType share ( const AssociationEnd< _OtherType, AggregationKind::Shared, _OtherTool > & /*other*/ )
    {
        static_assert( _Kind == AggregationKind::Shared, "Share not supported for not shared aggregation type." );
        return ThisType();
    }

    template < typename _OtherType, AggregationKind _OtherKind, typename _OtherTool >
    static ThisType make ( const AssociationEnd< _OtherType, _OtherKind, _OtherTool > & /*other*/ )
    {
        static_assert( _Kind != AggregationKind::None, "Make not supported for none aggregation type." );
        return ThisType();
    }

    template < typename _OtherType, AggregationKind _OtherKind, typename _OtherTool >
    static ThisType refer ( const AssociationEnd< _OtherType, _OtherKind, _OtherTool > & /*other*/ )
    {
        static_assert( _Kind == AggregationKind::None, "Refer supported for none aggregation type only." );
        return ThisType();
    }

    template < typename _OtherType >
    static ThisType share ( const _OtherType & /*other*/ )
    {
        return ThisType();
    }

    template < typename _OtherType >
    static ThisType copy ( const _OtherType & /*other*/ )
    {
        return ThisType();
    }

    template < typename _OtherType >
    static ThisType refer ( const _OtherType & /*other*/ )
    {
        return ThisType();
    }

};

#include <ModelKit/Relation/Tool/Cpp/Inplace/DefaultTool.h>

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using NoneEnd = AssociationEnd< _ValueType, AggregationKind::None, _RelationTool >;

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using SharedEnd = AssociationEnd< _ValueType, AggregationKind::Shared, _RelationTool >;

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using CompositeEnd = AssociationEnd< _ValueType, AggregationKind::Composite, _RelationTool >;
