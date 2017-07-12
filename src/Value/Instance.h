#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

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

    enum InitializeFlag { NotInitialized };

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать
    /// \sa isNull()
    constexpr Instance ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
    : m_holder( ValueTool:: template makeHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
    }

    template < typename _Type >
    ThisType & operator = ( _Type & other )
    {
        vGuard( *this ).value().m_holder
            = ValueTool::template makeHolder< ValueType >( other );
        return *this;

    }

    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        vGuard( *this ).value().m_holder
            = ValueTool::template makeHolder< ValueType >( ::std::forward< _Type >( other ) );
        return *this;

    }

    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        vGuard( *this ).value().m_holder
            = ValueTool::template makeHolder< ValueType >( other );
        return *this;

    }

    constexpr Instance ( ThisType & other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct( other ) )
    {
    }

    ThisType & operator = ( ThisType & other )
    {
        vGuard( *this ).value().m_holder
            =  InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct( other ) ;
        return *this;
    }

    constexpr Instance ( ThisType && other )
    : m_holder(  InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct(
        ::std::forward< ThisType >( other ) ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        vGuard( *this ).value().m_holder
            =  InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct(
                ::std::forward< ThisType >( other ) );
        return *this;
    }

    constexpr Instance ( const ThisType & other )
    : m_holder(  InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct( other ) )
    {
    }

    ThisType & operator = ( const ThisType & other )
    {
        vGuard( *this ).value().m_holder
            =  InstanceBuilder< _ValueType, _ValueTool, _ValueType, _ValueTool >::construct( other ) ;
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( Instance< _OtherType, _OtherTool > & other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & other )
    {
        vGuard( *this ).value().m_holder
            = InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct( other );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( Instance< _OtherType, _OtherTool > && other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct(
        ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && other )
    {
        vGuard( *this ).value().m_holder
            = InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct(
                ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( const Instance< _OtherType, _OtherTool > & other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & other )
    {
        vGuard( *this ).value().m_holder
            = InstanceBuilder< _ValueType, _ValueTool, _OtherType, _OtherTool >::construct( other );
        return *this;
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyHolder( m_holder );
    }
};

#endif
