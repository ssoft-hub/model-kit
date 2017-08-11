#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Common/Cnst.h>
#include <ModelKit/Common/InitializeType.h>
#include "private/InstanceFeatureGuard.h"
#include "private/InstanceOperation.h"
#include "private/InstanceResolver.h"
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

    template < typename, typename >
    friend class InstanceCompatibleResolver;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using HolderType = typename ValueTool:: template HolderType< _ValueType >;

private:
    /*
     * NOTE: В операциях присвоения (конструкторы и операторы) гарантия свойств
     * хранимого значения должна осуществляться посредством реализации HolderType.
     */
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать.
    constexpr Instance ( InitializeType flag )
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
    : m_holder( InstanceResolver< ThisType, ThisType && >(
        ::std::forward< ThisType >( other ) ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > && other )
    : m_holder( InstanceResolver< ThisType, Instance< _OtherType, _OtherTool > && >(
        ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ).resolve() )
    {
    }

    //! Конструкторы копирования
    Instance ( const ThisType & other )
    : m_holder( InstanceResolver< ThisType, const ThisType & >(
        other ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( const Instance< _OtherType, _OtherTool > & other )
    : m_holder( InstanceResolver< ThisType, const Instance< _OtherType, _OtherTool > & >(
        other ).resolve() )
    {
    }

    // NOTE: Определен из-за неоднозначного разрешения
    Instance ( ThisType & other )
    : Instance( const_cast< const ThisType & >( other ) ) {}

    // NOTE: Определен из-за неоднозначного разрешения
    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > & other )
    : Instance( const_cast< const Instance< _OtherType, _OtherTool > & >( other ) )
    {
    }

    //! Оператор присвоения rvalue значений
    template < typename _OtherType >
    ThisType & operator = ( _OtherType && other )
    {
        m_holder = InstanceResolver< ThisType, _OtherType && >(
            ::std::forward< _OtherType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = InstanceResolver< ThisType, ThisType && >(
            ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    /// Операторы присвоения lvalue значения
    template < typename _OtherType >
    ThisType & operator = ( const _OtherType & other )
    {
        m_holder = InstanceResolver< ThisType, const _OtherType & >( other ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    // NOTE: Определен из-за неоднозначного разрешения
    template < typename _OtherType >
    ThisType & operator = ( _OtherType & other )
    {
        return *this = const_cast< const _OtherType & >( other );
    }

    // NOTE: Определен из-за неоднозначного разрешения
    ThisType & operator = ( ThisType & other )
    {
        return *this = const_cast< const ThisType & >( other );
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

    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
    ValueGuard< ThisType & > operator -> ()
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator -> () const
    {
        return *this;
    }

    ValueGuard< ThisType & > operator & ()
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator & () const
    {
        return *this;
    }
};
