#pragma once

#include <Common/InitializeFlag.h>
#include <Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи не агрегации.
 *
 * Для конкретного использования требуется специализация инструмента, с помощью
 * которого данный полюс реализуется.
 */
template < typename _ValueType, typename _RelationTool >
class ReferEnd
{
//    template < typename >
//    friend struct ValueGuard;

//    template < typename >
//    friend struct FeatureGuard;

    using ThisType = ReferEnd< _ValueType, _RelationTool >;

public:
    using ValueType = _ValueType;
    using RelationTool = _RelationTool;
    using HolderType = typename RelationTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать.
    constexpr ReferEnd ( InitializeFlag = NotInitialized )
    {
    }

    ReferEnd ( ThisType && other )
    : m_holder( ::std::forward< HolderType >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< HolderType >( other );
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

public:
    template < typename _Other, typename _Tool >
    static ThisType refer ( const AssociationEnd< _Other, _Tool > & /*other*/ )
    {
        // TODO: other не обязан иметь m_holder
        return ReferEnd( /*Holder( other.m_holder )*/ );
    }
};
