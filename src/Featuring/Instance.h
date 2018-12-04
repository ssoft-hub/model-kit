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

namespace Operator
{
    template < typename _Instance,
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _Instance > > > >
    using AnyKingOfInstance = _Instance;

    template < typename _Argument,
        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _Argument > > > >
    using NotInstance = _Argument;
}

// Unary operators
#define INSTANCE_PREFIX_UNARY_OPERATOR( symbol, invokable ) \
    template < typename _RightInstance > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _RightInstance > && value ) \
    { \
        using RightInstance = ::std::decay_t< _RightInstance >; \
        using RightInstanceRefer = ::std::add_rvalue_reference_t< ::Operator::AnyKingOfInstance< _RightInstance > >; \
        using RightValueRefer = ::SimilarRefer< typename RightInstance::Value, RightInstanceRefer >; \
        return Operator::invoke< RightInstanceRefer >( ::std::forward< RightInstanceRefer >( value ), invokable< RightValueRefer >() ); \
    } \

INSTANCE_PREFIX_UNARY_OPERATOR( +, ::Operator::Private::UnaryPrefixPlus )
INSTANCE_PREFIX_UNARY_OPERATOR( -, ::Operator::Private::UnaryPrefixMinus )
INSTANCE_PREFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPrefixPlusPlus )
INSTANCE_PREFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPrefixMinusMinus )
INSTANCE_PREFIX_UNARY_OPERATOR( ~, ::Operator::Private::UnaryPrefixBitwiseNot )
INSTANCE_PREFIX_UNARY_OPERATOR( !, ::Operator::Private::UnaryPrefixLogicalNot )
#undef INSTANCE_PREFIX_UNARY_OPERATOR

#define INSTANCE_POSTFIX_UNARY_OPERATOR( symbol, invokable ) \
    template < typename _LeftInstance > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && value, int ) \
    { \
        using LeftInstance = ::std::decay_t< _LeftInstance >; \
        using LeftInstanceRefer = ::std::add_rvalue_reference_t< Operator::AnyKingOfInstance< _LeftInstance > >; \
        using LeftValueRefer = ::SimilarRefer< typename LeftInstance::Value, LeftInstanceRefer >; \
        return Operator::invoke< LeftInstanceRefer >( ::std::forward< LeftInstanceRefer >( value ), invokable< LeftValueRefer >() ); \
    } \

INSTANCE_POSTFIX_UNARY_OPERATOR( ++, ::Operator::Private::UnaryPostfixPlusPlus )
INSTANCE_POSTFIX_UNARY_OPERATOR( --, ::Operator::Private::UnaryPostfixMinusMinus )
#undef INSTANCE_POSTFIX_UNARY_OPERATOR

#define INSTANCE_BINARY_OPERATOR( symbol, invokable ) \
    template < typename _LeftInstance, typename _RightInstance, \
        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _LeftInstance > > && ::is_instance< ::std::decay_t< _RightInstance > > > > \
    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && /*left*/, ::Operator::AnyKingOfInstance< _RightInstance > && /*right*/ ) \
    { \
        return; \
    } \
//    \
//    template < typename _LeftInstance, typename _RightArgument, \
//        typename = ::std::enable_if_t< ::is_instance< ::std::decay_t< _LeftInstance > > && !::is_instance< ::std::decay_t< _RightArgument > > > > \
//    inline decltype(auto) operator symbol ( ::Operator::AnyKingOfInstance< _LeftInstance > && /*left*/, ::Operator::NotInstance< _RightArgument > && /*right*/ ) \
//    { \
//        return; \
//    } \
//    \
//    template < typename _LeftArgument, typename _RightInstance, \
//        typename = ::std::enable_if_t< !::is_instance< ::std::decay_t< _LeftArgument > > && ::is_instance< ::std::decay_t< _RightInstance > > > > \
//    inline decltype(auto) operator symbol ( ::Operator::NotInstance< _LeftArgument > && /*left*/, ::Operator::AnyKingOfInstance< _RightInstance > && /*right*/ ) \
//    { \
//        return; \
//    } \

// Binary operators
INSTANCE_BINARY_OPERATOR( ==, ??? )
INSTANCE_BINARY_OPERATOR( !=, ??? )
INSTANCE_BINARY_OPERATOR( <, ??? )
INSTANCE_BINARY_OPERATOR( <=, ??? )
INSTANCE_BINARY_OPERATOR( >, ??? )
INSTANCE_BINARY_OPERATOR( >=, ??? )

INSTANCE_BINARY_OPERATOR( *, ??? )
INSTANCE_BINARY_OPERATOR( /, ??? )
INSTANCE_BINARY_OPERATOR( %, ??? )
INSTANCE_BINARY_OPERATOR( +, ??? )
INSTANCE_BINARY_OPERATOR( -, ??? )

INSTANCE_BINARY_OPERATOR( <<, ??? )
INSTANCE_BINARY_OPERATOR( >>, ??? )

INSTANCE_BINARY_OPERATOR( &, ??? )
INSTANCE_BINARY_OPERATOR( ^, ??? )
INSTANCE_BINARY_OPERATOR( |, ??? )
INSTANCE_BINARY_OPERATOR( &&, ??? )
INSTANCE_BINARY_OPERATOR( ||, ??? )

INSTANCE_BINARY_OPERATOR( *=, ??? )
INSTANCE_BINARY_OPERATOR( /=, ??? )
INSTANCE_BINARY_OPERATOR( %=, ??? )
INSTANCE_BINARY_OPERATOR( +=, ??? )
INSTANCE_BINARY_OPERATOR( -=, ??? )
INSTANCE_BINARY_OPERATOR( <<=, ??? )
INSTANCE_BINARY_OPERATOR( >>=, ??? )
INSTANCE_BINARY_OPERATOR( &=, ??? )
INSTANCE_BINARY_OPERATOR( ^=, ??? )
INSTANCE_BINARY_OPERATOR( |=, ??? )


#endif
