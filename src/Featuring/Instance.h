#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>

namespace Inplace { struct DefaultTool; }

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
class Instance
{
    template < typename >
    friend class Private::SpecialValueGuard;

    template < typename >
    friend class Private::SpecialInstanceGuard;

    template < typename, typename >
    friend class Private::InstanceCompatibleResolver;

    using ThisType = Instance< _Value, _Tool >;

public:
    using Value = _Value;
    using Tool = _Tool;
    using Holder = typename Tool:: template Holder< _Value >;

private:
    /*
     * NOTE: В операциях присвоения (конструкторы и операторы) гарантия свойств
     * хранимого значения должна осуществляться посредством реализации Holder.
     */
    Holder m_holder;

public:
    /// Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
        : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {
    }

    /// Конструкторы ThisType
    Instance ( ThisType && other )
        : m_holder( InstanceResolver< ThisType, ThisType && >(
            ::std::forward< ThisType >( other ) ).resolve() )
    {
    }

    Instance ( const ThisType && other )
        : m_holder( InstanceResolver< ThisType, const ThisType && >(
            ::std::forward< const ThisType >( other ) ).resolve() )
    {
    }

    Instance ( ThisType & other )
        : m_holder( InstanceResolver< ThisType, ThisType & >( other ).resolve() )
    {
    }

    Instance ( const ThisType & other )
        : m_holder( InstanceResolver< ThisType, const ThisType & >( other ).resolve() )
    {
    }

    /// Конструкторы Instance< _OtherValue, _OtherTool >
    template < typename _OtherValue, typename _OtherTool >
    Instance ( Instance< _OtherValue, _OtherTool > && other )
        : m_holder( InstanceResolver< ThisType, Instance< _OtherValue, _OtherTool > && >(
            ::std::forward< Instance< _OtherValue, _OtherTool > >( other ) ).resolve() )
    {
    }

    template < typename _OtherValue, typename _OtherTool >
    Instance ( const Instance< _OtherValue, _OtherTool > && other )
        : m_holder( InstanceResolver< ThisType, const Instance< _OtherValue, _OtherTool > && >(
            ::std::forward< const Instance< _OtherValue, _OtherTool > >( other ) ).resolve() )
    {
    }

    template < typename _OtherValue, typename _OtherTool >
    Instance ( const Instance< _OtherValue, _OtherTool > & other )
        : m_holder( InstanceResolver< ThisType, const Instance< _OtherValue, _OtherTool > & >( other ).resolve() )
    {
    }

    template < typename _OtherValue, typename _OtherTool >
    constexpr Instance ( Instance< _OtherValue, _OtherTool > & other )
        : m_holder( InstanceResolver< ThisType, Instance< _OtherValue, _OtherTool > & >( other ).resolve() )
    {
    }

    /// Операторы присвоения (в т.ч. Instance< _OtherValue, _OtherTool >)
    template < typename _OtherValue >
    ThisType & operator = ( _OtherValue && other ) &
    {
        m_holder = InstanceResolver< ThisType, _OtherValue && >( ::std::forward< _OtherValue >( other ) ).resolve();
        return *this;
    }

    template < typename _OtherValue >
    ThisType && operator = ( _OtherValue && other ) &&
    {
        m_holder = InstanceResolver< ThisType, _OtherValue && >( ::std::forward< _OtherValue >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    /// Операторы присвоения ThisType
    ThisType & operator = ( ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( ThisType & other ) &
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType & other ) &
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    ThisType && operator = ( ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType && operator = ( const ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType && operator = ( ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    ThisType && operator = ( const ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    /// Операторы преобразования к типу
    operator Instance< const _Value, _Tool > & ()
    {
        return reinterpret_cast< Instance< const _Value, _Tool > & >( *this );
    }

    operator Instance< const _Value, _Tool > && () &&
    {
        return reinterpret_cast< Instance< const _Value, _Tool > && >( *this );
    }

    operator const Instance< const _Value, _Tool > & () const
    {
        return reinterpret_cast< const Instance< const _Value, _Tool > & >( *this );
    }

    operator const Instance< const _Value, _Tool > && () const &&
    {
        return reinterpret_cast< const Instance< const _Value, _Tool > && >( *this );
    }

    /// Операторы доступа к членам
    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
    ValueGuard< ThisType && > operator -> () &&
    {
        return ::std::forward< ThisType >( *this );
    }

    ValueGuard< const ThisType && > operator -> () const &&
    {
        return ::std::forward< const ThisType >( *this );
    }

    ValueGuard< ThisType & > operator -> () &
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator -> () const &
    {
        return *this;
    }

    /// Операторы извлечения адреса
    ValueGuard< ThisType && > operator & () &&
    {
        return ::std::forward< ThisType >( *this );
    }

    ValueGuard< const ThisType && > operator & () const &&
    {
        return ::std::forward< const ThisType >( *this );
    }

    ValueGuard< ThisType & > operator & () &
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator & () const &
    {
        return *this;
    }

    /// Проксирование оператора []
    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) &
    {
        auto invokable = ::Operator::Private::SquareBrackets< Value &, _Argument && >();
        return Operator::invoke< ThisType & >( *this, invokable, ::std::forward< _Argument >( argument ) );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const &
    {
        auto invokable = ::Operator::Private::SquareBrackets< const Value &, _Argument && >();
        return Operator::invoke< const ThisType & >( *this, invokable, ::std::forward< _Argument >( argument ) );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) &&
    {
        auto invokable = ::Operator::Private::SquareBrackets< Value &&, _Argument && >();
        return Operator::invoke< ThisType && >( ::std::forward< ThisType >( *this ), invokable, ::std::forward< _Argument >( argument ) );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const &&
    {
        auto invokable = ::Operator::Private::SquareBrackets< const Value &&, _Argument && >();
        return Operator::invoke< const ThisType && >( ::std::forward< const ThisType >( *this ), invokable, ::std::forward< _Argument >( argument ) );
    }

    /// Проксирование оператора ()
    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) &
    {
        auto invokable = ::Operator::Private::RoundBrackets< Value &, _Arguments && ... >();
        return Operator::invoke< ThisType & >( *this, invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const &
    {
        auto invokable = ::Operator::Private::RoundBrackets< const Value &, _Arguments && ... >();
        return Operator::invoke< const ThisType & >( *this, invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) &&
    {
        auto invokable = ::Operator::Private::RoundBrackets< Value &&, _Arguments && ... >();
        return Operator::invoke< ThisType && >( ::std::forward< ThisType >( *this ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const &&
    {
        auto invokable = ::Operator::Private::RoundBrackets< const Value &&, _Arguments && ... >();
        return Operator::invoke< const ThisType && >( ::std::forward< const ThisType >( *this ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

#endif
