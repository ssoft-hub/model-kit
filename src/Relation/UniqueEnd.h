#pragma once

#include <Helper/InitializeFlag.h>
#include <Relation/AggregationEnd.h>
#include <Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи уникальной агрегации.
 *
 * Для конкретного использования требуется специализация инструмента, с помощью
 * которого данный полюс реализуется.
 */
template < typename _ValueType, typename _RelationTool >
class UniqueEnd
{
    template < typename >
    friend struct ValueGuard;

    template < typename >
    friend struct FeatureGuard;

    using ThisType = UniqueEnd< _ValueType, _RelationTool >;

public:
    using ValueType = _ValueType;
    using RelationTool = _RelationTool;
    using HolderType = typename RelationTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать.
    constexpr UniqueEnd ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr UniqueEnd ( _Arguments && ... arguments )
    : m_holder( RelationTool:: template makeHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
    }

    UniqueEnd ( ThisType && other )
    : m_holder( ::std::forward< HolderType >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< HolderType >( other );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    UniqueEnd ( UniqueEnd< _OtherType, _OtherTool > && /*other*/ )
    : m_holder() // ::std::forward< typename UniqueEnd< _OtherType, _OtherTool >::HolderType >( other.m_holder ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( UniqueEnd< _OtherType, _OtherTool > && /*other*/ )
    {
        // m_holder = ::std::forward< typename UniqueEnd< _OtherType, _OtherTool >::HolderType >( other.m_holder );
        return *this;
    }

    operator AssociationEnd< _ValueType, _RelationTool > ()
    {
        return AssociationEnd< _ValueType, _RelationTool >();
    }

    operator const AssociationEnd< _ValueType, _RelationTool > () const
    {
        return AssociationEnd< _ValueType, _RelationTool >();
    }

    operator AggregationEnd< _ValueType, _RelationTool > ()
    {
        return AggregationEnd< _ValueType, _RelationTool >();
    }

    operator const AggregationEnd< _ValueType, _RelationTool > () const
    {
        return AggregationEnd< _ValueType, _RelationTool >();
    }
};
