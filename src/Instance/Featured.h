#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Instance/Operator.h>
#include <ModelKit/Instance/Resolver.h>
#include <ModelKit/Utility/AsConst.h>

namespace Inplace { struct DefaultTool; }

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = ::Inplace::DefaultTool >
class Featured
{
    template < typename >
    friend class Private::SpecialValueGuard;

    template < typename >
    friend class Private::SpecialFeaturedGuard;

    template < typename, typename >
    friend class Private::FeaturedCompatibleResolver;

    using ThisType = Featured< _Value, _Tool >;

public:
    using Value = _Value;
    using Tool = _Tool;
    using Holder = typename Tool:: template Holder< _Value >;
    template < typename _Type, typename = ::std::enable_if_t< is_featured< _Type > > >
    using AnyFeatured = _Type;

private:
    /*
     * NOTE: В операциях присвоения (конструкторы и операторы) гарантия свойств
     * хранимого значения должна осуществляться посредством реализации Holder.
     */
    Holder m_holder;

public:
    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr Featured ( _Arguments && ... arguments )
        : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {
    }

    //! Конструкторы ThisType
    Featured ( ThisType && other )
        : m_holder( FeaturedResolver< ThisType, ThisType && >(
            ::std::forward< ThisType >( other ) ).resolve() )
    {
    }

    Featured ( const ThisType && other )
        : m_holder( FeaturedResolver< ThisType, const ThisType && >(
            ::std::forward< const ThisType >( other ) ).resolve() )
    {
    }

    Featured ( ThisType & other )
        : m_holder( FeaturedResolver< ThisType, ThisType & >( other ).resolve() )
    {
    }

    Featured ( const ThisType & other )
        : m_holder( FeaturedResolver< ThisType, const ThisType & >( other ).resolve() )
    {
    }

    //! Конструкторы Featured< _OtherType, _OtherTool >
    template < typename _OtherType, typename _OtherTool >
    Featured ( Featured< _OtherType, _OtherTool > && other )
        : m_holder( FeaturedResolver< ThisType, Featured< _OtherType, _OtherTool > && >(
            ::std::forward< Featured< _OtherType, _OtherTool > >( other ) ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Featured ( const Featured< _OtherType, _OtherTool > && other )
        : m_holder( FeaturedResolver< ThisType, const Featured< _OtherType, _OtherTool > && >(
            ::std::forward< const Featured< _OtherType, _OtherTool > >( other ) ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Featured ( const Featured< _OtherType, _OtherTool > & other )
        : m_holder( FeaturedResolver< ThisType, const Featured< _OtherType, _OtherTool > & >( other ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Featured ( Featured< _OtherType, _OtherTool > & other )
        : m_holder( FeaturedResolver< ThisType, Featured< _OtherType, _OtherTool > & >( other ).resolve() )
    {
    }

//    //! Конструкторы
//    template < typename _OtherType >
//    Featured ( AnyFeatured< _OtherType > && other )
//        : m_holder( FeaturedResolver< ThisType, AnyFeatured< _OtherType > && >(
//            ::std::forward< AnyFeatured< _OtherType > && >( other ) ).resolve() )
//    {
//    }

//    template < typename _OtherType >
//    Featured ( AnyFeatured< _OtherType > & other )
//        : m_holder( FeaturedResolver< ThisType, AnyFeatured< _OtherType > & >(
//            ::std::forward< AnyFeatured< _OtherType > & >( other ) ).resolve() )
//    {
//    }

//    template < typename _OtherType >
//    Featured ( const AnyFeatured< _OtherType > && other )
//        : m_holder( FeaturedResolver< ThisType, const AnyFeatured< _OtherType > && >(
//            ::std::forward< const AnyFeatured< _OtherType > && >( other ) ).resolve() )
//    {
//    }

//    template < typename _OtherType >
//    Featured ( const AnyFeatured< _OtherType > & other )
//        : m_holder( FeaturedResolver< ThisType, const AnyFeatured< _OtherType > & >(
//            ::std::forward< const AnyFeatured< _OtherType > & >( other ) ).resolve() )
//    {
//    }

//    //! Оператор присвоения
//    template < typename _OtherType >
//    ThisType & operator = ( _OtherType && other ) &
//    {
//        m_holder = FeaturedResolver< ThisType, _OtherType && >( ::std::forward< _OtherType && >( other ) ).resolve();
//        return *this;
//    }

//    template < typename _OtherType >
//    ThisType && operator = ( _OtherType && other ) &&
//    {
//        m_holder = FeaturedResolver< ThisType, _OtherType && >( ::std::forward< _OtherType && >( other ) ).resolve();
//        return ::std::forward< ThisType && >( *this );
//    }

    //! Оператор присвоения rvalue значений
    template < typename _OtherType >
    ThisType & operator = ( _OtherType && other ) &
    {
        m_holder = FeaturedResolver< ThisType, _OtherType && >( ::std::forward< _OtherType >( other ) ).resolve();
        return *this;
    }

    template < typename _OtherType >
    ThisType && operator = ( _OtherType && other ) &&
    {
        m_holder = FeaturedResolver< ThisType, _OtherType && >( ::std::forward< _OtherType >( other ) ).resolve();
        return ::std::forward< ThisType >( *this );
    }

    ThisType & operator = ( ThisType && other ) &
    {
        m_holder = FeaturedResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType && other ) &
    {
        m_holder = FeaturedResolver< ThisType, ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( ThisType & other ) &
    {
        m_holder = FeaturedResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType & other ) &
    {
        m_holder = FeaturedResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    ThisType && operator = ( ThisType && other ) &&
    {
        m_holder = FeaturedResolver< ThisType, ThisType && >( ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType && operator = ( const ThisType && other ) &&
    {
        m_holder = FeaturedResolver< ThisType, ThisType && >( ::std::forward< const ThisType >( other ) ).resolve();
        return *this;
    }

    ThisType && operator = ( ThisType & other ) &&
    {
        m_holder = FeaturedResolver< ThisType, ThisType & >( other ).resolve();
        return *this;
    }

    ThisType && operator = ( const ThisType & other ) &&
    {
        m_holder = FeaturedResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }


    /// Операторы преобразования к типу
    operator Featured< const _Value, _Tool > & ()
    {
        return reinterpret_cast< Featured< const _Value, _Tool > & >( *this );
    }

    operator Featured< const _Value, _Tool > && () &&
    {
        return reinterpret_cast< Featured< const _Value, _Tool > && >( *this );
    }

    operator const Featured< const _Value, _Tool > & () const
    {
        return reinterpret_cast< const Featured< const _Value, _Tool > & >( *this );
    }

    operator const Featured< const _Value, _Tool > && () const &&
    {
        return reinterpret_cast< const Featured< const _Value, _Tool > && >( *this );
    }

    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
    ValueGuard< ThisType & > operator -> () &
    {
        return *this;
    }

    ValueGuard< ThisType && > operator -> () &&
    {
        return ::std::forward< ThisType >( *this );
    }

    ValueGuard< const ThisType & > operator -> () const &
    {
        return *this;
    }

    ValueGuard< const ThisType && > operator -> () const &&
    {
        return ::std::forward< const ThisType >( *this );
    }

    ValueGuard< ThisType & > operator & () &
    {
        return *this;
    }

    ValueGuard< ThisType && > operator & () &&
    {
        return ::std::forward< ThisType >( *this );
    }

    ValueGuard< const ThisType & > operator & () const &
    {
        return *this;
    }

    ValueGuard< const ThisType && > operator & () const &&
    {
        return ::std::forward< const ThisType >( *this );
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
