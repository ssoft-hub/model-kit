#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Featuring/Operator.h>
#include <ModelKit/Featuring/Resolver.h>
#include <ModelKit/Featuring/Traits.h>

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

    Instance ( volatile ThisType && other )
        : m_holder( InstanceResolver< ThisType, volatile ThisType && >(
            ::std::forward< volatile ThisType >( other ) ).resolve() )
    {
    }

    Instance ( const volatile ThisType && other )
        : m_holder( InstanceResolver< ThisType, const volatile ThisType && >(
            ::std::forward< const volatile ThisType >( other ) ).resolve() )
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

    Instance ( volatile ThisType & other )
        : m_holder( InstanceResolver< ThisType, volatile ThisType & >( other ).resolve() )
    {
    }

    Instance ( const volatile ThisType & other )
        : m_holder( InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve() )
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
    Instance ( volatile Instance< _OtherValue, _OtherTool > && other )
        : m_holder( InstanceResolver< ThisType, volatile Instance< _OtherValue, _OtherTool > && >(
            ::std::forward< volatile Instance< _OtherValue, _OtherTool > >( other ) ).resolve() )
    {
    }

    template < typename _OtherValue, typename _OtherTool >
    Instance ( const volatile Instance< _OtherValue, _OtherTool > && other )
        : m_holder( InstanceResolver< ThisType, const volatile Instance< _OtherValue, _OtherTool > && >(
            ::std::forward< const volatile Instance< _OtherValue, _OtherTool > >( other ) ).resolve() )
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

    template < typename _OtherValue, typename _OtherTool >
    Instance ( const volatile Instance< _OtherValue, _OtherTool > & other )
        : m_holder( InstanceResolver< ThisType, const volatile Instance< _OtherValue, _OtherTool > & >( other ).resolve() )
    {
    }

    template < typename _OtherValue, typename _OtherTool >
    constexpr Instance ( volatile Instance< _OtherValue, _OtherTool > & other )
        : m_holder( InstanceResolver< ThisType, volatile Instance< _OtherValue, _OtherTool > & >( other ).resolve() )
    {
    }

    // Assignment operators for (including Instance< _OtherValue, _OtherTool >)
    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) &&
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) const &&
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType & operator = ( _Argument && other ) &
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return *this;
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) const &
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    template < typename _Argument >
    ThisType && operator = ( _Argument && other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, _Argument && >( ::std::forward< _Argument >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    // Assignment operators for ThisType
    ThisType && operator = ( ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( const ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( volatile ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( const volatile ThisType && other ) &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( const ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( volatile ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType && operator = ( const volatile ThisType & other ) &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    const ThisType && operator = ( ThisType && other ) const &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( const ThisType && other ) const &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( volatile ThisType && other ) const &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( const volatile ThisType && other ) const &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( ThisType & other ) const &&
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( const ThisType & other ) const &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( volatile ThisType & other ) const &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    const ThisType && operator = ( const volatile ThisType & other ) const &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return ::std::forward< const ThisType >( *this );
    }

    volatile ThisType && operator = ( ThisType && other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( const ThisType && other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( volatile ThisType && other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( const volatile ThisType && other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( ThisType & other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( const ThisType & other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( volatile ThisType & other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    volatile ThisType && operator = ( const volatile ThisType & other ) volatile &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return ::std::forward< volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( ThisType && other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( const ThisType && other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( volatile ThisType && other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( const volatile ThisType && other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( ThisType & other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( const ThisType & other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( volatile ThisType & other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    const volatile ThisType && operator = ( const volatile ThisType & other ) const volatile &&
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return ::std::forward< const volatile ThisType >( *this );
    }

    ThisType & operator = ( ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( volatile ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( const volatile ThisType && other ) &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
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

    ThisType & operator = ( volatile ThisType & other ) &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return *this;
    }

    ThisType & operator = ( const volatile ThisType & other ) &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return *this;
    }

    const ThisType & operator = ( ThisType && other ) const &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    const ThisType & operator = ( const ThisType && other ) const &
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    const ThisType & operator = ( volatile ThisType && other ) const &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return *this;
    }

    const ThisType & operator = ( const volatile ThisType && other ) const &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return *this;
    }

    const ThisType & operator = ( ThisType & other ) const &
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    const ThisType & operator = ( const ThisType & other ) const &
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    const ThisType & operator = ( volatile ThisType & other ) const &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return *this;
    }

    const ThisType & operator = ( const volatile ThisType & other ) const &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( ThisType && other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( const ThisType && other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( volatile ThisType && other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( const volatile ThisType && other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( ThisType & other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( const ThisType & other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( volatile ThisType & other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return *this;
    }

    volatile ThisType & operator = ( const volatile ThisType & other ) volatile &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( ThisType && other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( const ThisType && other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, const ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( volatile ThisType && other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType && >( ::std::forward< volatile ThisType >( other ) ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( const volatile ThisType && other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType && >( ::std::forward< const volatile ThisType >( other ) ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( ThisType & other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( const ThisType & other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( volatile ThisType & other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, volatile ThisType & >( other ).resolve();
        return *this;
    }

    const volatile ThisType & operator = ( const volatile ThisType & other ) const volatile &
    {
        m_holder = InstanceResolver< ThisType, const volatile ThisType & >( other ).resolve();
        return *this;
    }

    // Convertation operators
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

    ValueGuard< volatile ThisType && > operator -> () volatile &&
    {
        return ::std::forward< volatile ThisType >( *this );
    }

    ValueGuard< const volatile ThisType && > operator -> () const volatile &&
    {
        return ::std::forward< const volatile ThisType >( *this );
    }

    ValueGuard< ThisType & > operator -> () &
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator -> () const &
    {
        return *this;
    }

    ValueGuard< volatile ThisType & > operator -> () volatile &
    {
        return *this;
    }

    ValueGuard< const volatile ThisType & > operator -> () const volatile &
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

    ValueGuard< volatile ThisType && > operator & () volatile &&
    {
        return ::std::forward< const ThisType >( *this );
    }

    ValueGuard< const volatile ThisType && > operator & () const volatile &&
    {
        return ::std::forward< const volatile ThisType >( *this );
    }

    ValueGuard< ThisType & > operator & () &
    {
        return *this;
    }

    ValueGuard< const ThisType & > operator & () const &
    {
        return *this;
    }

    ValueGuard< volatile ThisType & > operator & () volatile &
    {
        return *this;
    }

    ValueGuard< const volatile ThisType & > operator & () const volatile &
    {
        return *this;
    }

    /// Проксирование оператора []
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

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) volatile &&
    {
        auto invokable = ::Operator::Private::SquareBrackets< volatile Value &&, _Argument && >();
        return Operator::invoke< volatile ThisType && >( ::std::forward< volatile ThisType >( *this ), invokable, ::std::forward< _Argument >( argument ) );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const volatile &&
    {
        auto invokable = ::Operator::Private::SquareBrackets< const volatile Value &&, _Argument && >();
        return Operator::invoke< const volatile ThisType && >( ::std::forward< const volatile ThisType >( *this ), invokable, ::std::forward< _Argument >( argument ) );
    }

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
    constexpr decltype(auto) operator [] ( _Argument && argument ) volatile &
    {
        auto invokable = ::Operator::Private::SquareBrackets< volatile Value &, _Argument && >();
        return Operator::invoke< volatile ThisType & >( *this, invokable, ::std::forward< _Argument >( argument ) );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const volatile &
    {
        auto invokable = ::Operator::Private::SquareBrackets< const volatile Value &, _Argument && >();
        return Operator::invoke< const volatile ThisType & >( *this, invokable, ::std::forward< _Argument >( argument ) );
    }

    /// Проксирование оператора ()
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

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) volatile &&
    {
        auto invokable = ::Operator::Private::RoundBrackets< volatile Value &&, _Arguments && ... >();
        return Operator::invoke< volatile ThisType && >( ::std::forward< volatile ThisType >( *this ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const volatile &&
    {
        auto invokable = ::Operator::Private::RoundBrackets< const volatile Value &&, _Arguments && ... >();
        return Operator::invoke< const volatile ThisType && >( ::std::forward< const volatile ThisType >( *this ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

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
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) volatile &
    {
        auto invokable = ::Operator::Private::RoundBrackets< volatile Value &, _Arguments && ... >();
        return Operator::invoke< volatile ThisType & >( *this, invokable, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const volatile &
    {
        auto invokable = ::Operator::Private::RoundBrackets< const volatile Value &, _Arguments && ... >();
        return Operator::invoke< const volatile ThisType & >( *this, invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

#endif
