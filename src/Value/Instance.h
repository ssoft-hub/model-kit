#pragma once

#include <Helper/InitializeFlag.h>
#include "private/InstanceBuilder.h"
#include "private/InstanceGuard.h"

/*!
 * Класс для формирования экземпляра значения, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством заданного
 * _ValueTool.
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename >
    friend struct ValueGuard;

    template < typename >
    friend struct FeatureGuard;

    template < InstanceBuildSwitchType >
    friend struct InstanceBuildSwither;

    template < typename, typename >
    friend class Instance;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using HolderType = typename ValueTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать.
    constexpr Instance ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам.
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, _Arguments ... >
        ::construct( ::std::forward< _Arguments >( arguments ) ... ) )
    {
    }

    //! Конструктор перемещения
    constexpr Instance ( ThisType && other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, ThisType >::construct(
        ::std::forward< ThisType >( other ) ) )
    {
    }

    //! Конструктор копирования
    constexpr Instance ( const ThisType & other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, ThisType >::construct( other ) )
    {
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyHolder( m_holder );
    }

    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        m_holder = InstanceBuilder< _ValueType, _ValueTool, _Type && >
            ::construct( ::std::forward< _Type >( other ) );
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        m_holder = InstanceBuilder< _ValueType, _ValueTool, const _Type & >
            ::construct( other ) ;
        return *this;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = InstanceBuilder< _ValueType, _ValueTool, ThisType && >
            ::construct( ::std::forward< ThisType >( other ) );
        return *this;
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = InstanceBuilder< _ValueType, _ValueTool, const ThisType & >
            ::construct( other ) ;
        return *this;
    }
};
