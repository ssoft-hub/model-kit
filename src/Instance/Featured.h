#pragma once
#ifndef INSTANCE_FEATURED_H
#define INSTANCE_FEATURED_H

#ifdef _MSC_VER
#pragma warning( disable : 4521 4522 )
#endif

#include <ModelKit/Instance/Resolver.h>
#include <ModelKit/Instance/Tool/Cpp/Inplace/DefaultTool.h>
#include <ModelKit/Instance/Tool/Operator/RoundBrackets.h>
#include <ModelKit/Instance/Tool/Operator/SquareBrackets.h>
#include <ModelKit/Utility/AsConst.h>

/*!
 * Класс для формирования экземпляра значения _Value, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством функциональности _Tool.
 */
template < typename _Value, typename _Tool = Cpp::Inplace::DefaultTool >
class Featured
{
    template < typename >
    friend class Private::SpecialValuePointer;

    template < typename >
    friend class Private::SpecialFeaturedPointer;

    template < typename, typename >
    friend class Private::FeaturedCompatibleResolver;

    using ThisType = Featured< _Value, _Tool >;

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
    //! Конструктор инициализации значения по заданным параметрам.
    template < typename ... _Arguments >
    constexpr Featured ( _Arguments && ... arguments )
        : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {
    }

    //! Конструкторы перемещения
    Featured ( ThisType && other )
        : m_holder( FeaturedResolver< ThisType, ThisType && >(
            ::std::forward< ThisType >( other ) ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Featured ( Featured< _OtherType, _OtherTool > && other )
        : m_holder( FeaturedResolver< ThisType, Featured< _OtherType, _OtherTool > && >(
            ::std::forward< Featured< _OtherType, _OtherTool > >( other ) ).resolve() )
    {
    }

    //! Конструкторы копирования
    Featured ( const ThisType & other )
        : m_holder( FeaturedResolver< ThisType, const ThisType & >(
            other ).resolve() )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    Featured ( const Featured< _OtherType, _OtherTool > & other )
        : m_holder( FeaturedResolver< ThisType, const Featured< _OtherType, _OtherTool > & >(
            other ).resolve() )
    {
    }

    // NOTE: Определен из-за неоднозначного разрешения
    constexpr Featured ( ThisType & other )
        : Featured( asConst( other ) )
    {
    }

    // NOTE: Определен из-за неоднозначного разрешения
    template < typename _OtherType, typename _OtherTool >
    constexpr Featured ( Featured< _OtherType, _OtherTool > & other )
        : Featured( asConst( other ) )
    {
    }

    //! Оператор присвоения rvalue значений
    template < typename _OtherType >
    ThisType & operator = ( _OtherType && other )
    {
        m_holder = FeaturedResolver< ThisType, _OtherType && >(
            ::std::forward< _OtherType >( other ) ).resolve();
        return *this;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = FeaturedResolver< ThisType, ThisType && >(
            ::std::forward< ThisType >( other ) ).resolve();
        return *this;
    }

    /// Операторы присвоения lvalue значения
    template < typename _OtherType >
    ThisType & operator = ( const _OtherType & other )
    {
        m_holder = FeaturedResolver< ThisType, const _OtherType & >( other ).resolve();
        return *this;
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = FeaturedResolver< ThisType, const ThisType & >( other ).resolve();
        return *this;
    }

    // NOTE: Определен из-за неоднозначного разрешения
    template < typename _OtherType >
    constexpr ThisType & operator = ( _OtherType & other )
    {
        return *this = asConst( other );
    }

    // NOTE: Определен из-за неоднозначного разрешения
    constexpr ThisType & operator = ( ThisType & other )
    {
        return *this = asConst( other );
    }

    /// Операторы преобразования к типу
    operator Featured< const _Value, _Tool > & ()
    {
        return reinterpret_cast< Featured< const _Value, _Tool > & >( *this );
    }

    operator const Featured< const _Value, _Tool > & () const
    {
        return reinterpret_cast< const Featured< const _Value, _Tool > & >( *this );
    }

    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
    ValuePointer< ThisType & > operator -> ()
    {
        return *this;
    }

    ValuePointer< const ThisType & > operator -> () const
    {
        return *this;
    }

    ValuePointer< ThisType & > operator & ()
    {
        return *this;
    }

    ValuePointer< const ThisType & > operator & () const
    {
        return *this;
    }

    /// Проксирование оператора []
    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) &
    {
        using RetunType = decltype( Tool::value( m_holder )[ std::forward< _Argument >( argument ) ] );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( m_holder ), argument );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const &
    {
        using RetunType = decltype( Tool::value( m_holder )[ std::forward< _Argument >( argument ) ] );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( m_holder ), argument );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) &&
    {
        using RetunType = decltype( Tool::value( ::std::forward< Holder && >( std::forward< _Argument >( argument ) ) )[ argument ] );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( ::std::forward< Holder && >( m_holder ) ), argument );
    }

    template< typename _Argument >
    constexpr decltype(auto) operator [] ( _Argument && argument ) const &&
    {
        using RetunType = decltype( Tool::value( ::std::forward< const Holder && >( std::forward< _Argument >( argument ) ) )[ argument ] );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( ::std::forward< const Holder && >( m_holder ) ), argument );
    }

    /// Проксирование оператора ()
    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) &
    {
        using RetunType = decltype( Tool::value( m_holder )( std::forward< _Arguments >( arguments ) ... ) );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( m_holder ), arguments ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const &
    {
        using RetunType = decltype( Tool::value( m_holder )( std::forward< _Arguments >( arguments ) ... ) );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( m_holder ), arguments ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) &&
    {
        using RetunType = decltype( Tool::value( m_holder )( std::forward< _Arguments >( arguments ) ... ) );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( ::std::forward< Holder && >( m_holder ) ), arguments ... );
    }

    template < typename ... _Arguments >
    constexpr decltype(auto) operator () ( _Arguments && ... arguments ) const &&
    {
        using RetunType = decltype( Tool::value( m_holder )( std::forward< _Arguments >( arguments ) ... ) );
        return Featured< RetunType, ::Operator::SquareBacketsTool >( Tool::value( ::std::forward< Holder && >( m_holder ) ), arguments ... );
    }
};

#endif