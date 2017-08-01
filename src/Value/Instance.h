#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Common/InitializeFlag.h>
#include "private/InstanceBuilder.h"
#include "private/InstanceFeatureGuard.h"
#include "private/InstanceValueGuard.h"

/*!
 * Класс для формирования экземпляра значения, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством заданного
 * _ValueTool.
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename >
    friend struct InstanceValueGuard;

    template < typename >
    friend struct InstanceFeatureGuard;

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
    constexpr Instance ( InitializeFlag flag )
    : m_holder( flag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам.
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
    : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {
    }

    //! Конструкторы перемещения
    Instance ( ThisType && other )
    : m_holder( ::std::forward< HolderType >( other.m_holder ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > && other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool, Instance< _OtherType, _OtherTool > && >
        ::construct( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    //! Конструкторы копирования
    Instance ( const ThisType & other )
    : m_holder( other.m_holder )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( const Instance< _OtherType, _OtherTool > & other )
    : m_holder( InstanceBuilder< _ValueType, _ValueTool,
        const Instance< _OtherType, _OtherTool > & >::construct( other ) )
    {
    }

    // NOTE: Определен из-за наличия Instance ( _Arguments && ... ).
    Instance ( ThisType & other )
    : Instance( const_cast< const ThisType & >( other ) ) {}

    // NOTE: Определен из-за наличия Instance ( _Arguments && ... ).
    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > & other )
    : Instance( const_cast< const Instance< _OtherType, _OtherTool > & >( other ) )
    {
    }

    //! Оператор присвоения rvalue значений
    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        m_holder = ::std::forward< _Type >( other );
        return *this;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< HolderType >( other.m_holder );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && other )
    {
        using OtherType = Instance< _OtherType, _OtherTool >;
        m_holder = InstanceBuilder< _ValueType, _ValueTool, OtherType && >
            ::construct( ::std::forward< OtherType >( other ) );
        return *this;
    }

    /// Операторы присвоения lvalue значения
    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        m_holder = other;
        return *this;
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = other.m_holder;
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & other )
    {
        using OtherType = Instance< _OtherType, _OtherTool >;
        m_holder = InstanceBuilder< _ValueType, _ValueTool, const OtherType & >
            ::construct( other ) ;
        return *this;
    }

    // NOTE: Определен из-за наличия operator = ( _Type && ).
    ThisType & operator = ( ThisType & other )
    {
        return *this = const_cast< const ThisType & >( other );
    }

    // NOTE: Определен из-за наличия operator = ( _Type && ).
    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & other )
    {
        using OtherType = Instance< _OtherType, _OtherTool >;
        return *this = const_cast< const OtherType & >( other );
    }

    /// Операторы преобразования к типу
    operator Instance< const _ValueType, _ValueTool > & ()
    {
        return reinterpret_cast< Instance< const _ValueType, _ValueTool > & >( *this );
    }

    operator const Instance< const _ValueType, _ValueTool > & () const
    {
        return reinterpret_cast< const Instance< const _ValueType, _ValueTool > & >( *this );
    }

    /// Альтернативные методы доступа к значению
    // NOTE: Требуется оценка необходимости, так как методы не совместимы
    // с типами, отличными от Instance.
    ValueGuard< ThisType & > operator -> ()
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator -> () const
    {
        return *this;
    }

    ValueGuard< ThisType & > operator * ()
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator * () const
    {
        return *this;
    }

    const ThisType & cnst () const
    {
        return *this;
    }
};

template < typename _Type >
inline constexpr const _Type & cnst ( const _Type & value )
{
    return value;
}
