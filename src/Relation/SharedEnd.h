#pragma once

#include <Helper/InitializeFlag.h>
#include <Relation/AggregationEnd.h>
#include <Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи обобщенной агрегации.
 *
 * Для конкретного использования требуется специализация инструмента, с помощью
 * которого данный полюс реализуется.
 */
template < typename _ValueType, typename _RelationTool >
class SharedEnd
{
    template < typename >
    friend struct ValueGuard;

    template < typename >
    friend struct FeatureGuard;

    using ThisType = SharedEnd< _ValueType, _RelationTool >;

public:
    using ValueType = _ValueType;
    using RelationTool = _RelationTool;
    using HolderType = typename RelationTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать.
    constexpr SharedEnd ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr SharedEnd ( _Arguments && ... arguments )
    : m_holder( RelationTool:: template makeHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
    }

    SharedEnd ( ThisType && other )
    : m_holder( ::std::forward< HolderType >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< HolderType >( other );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    SharedEnd ( SharedEnd< _OtherType, _OtherTool > && /*other*/ )
    : m_holder() // ::std::forward< typename SharedEnd< _OtherType, _OtherTool >::HolderType >( other.m_holder ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( SharedEnd< _OtherType, _OtherTool > && /*other*/ )
    {
        // m_holder = ::std::forward< typename SharedEnd< _OtherType, _OtherTool >::HolderType >( other.m_holder );
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

public:
    template < typename _OtherType, typename _OtherTool >
    static ThisType share ( const SharedEnd< _OtherType, _OtherTool > & /*other*/ )
    {
        // TODO: other не обязан иметь m_holder
        return ThisType();// Holder( other.m_holder ) );
    }
};
