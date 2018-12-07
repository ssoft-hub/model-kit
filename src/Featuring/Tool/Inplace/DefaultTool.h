#pragma once
#ifndef INSTANCE_TOOL_INPLACE_DEFAULT_H
#define INSTANCE_TOOL_INPLACE_DEFAULT_H

#include <utility>

namespace Inplace
{
    /*!
     * Инструмент для формирования значения "по месту". Определение "по месту" означает,
     * что для значения не используется специальное размещение в куче и оно является
     * неотъемлемой частью пространства имен, в котором это значение определено.
     */
    struct DefaultTool
    {
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            Value m_value;

            template < typename ... _Arguments >
            constexpr Holder ( _Arguments && ... arguments )
                : m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }

            constexpr Holder ( ThisType && other )
                : m_value( ::std::forward< Value >( other.m_value ) )
            {
            }

            constexpr Holder ( const ThisType && other )
                : m_value( ::std::forward< const Value >( other.m_value ) )
            {
            }

            constexpr Holder ( volatile ThisType && other )
                : m_value( ::std::forward< volatile Value >( other.m_value ) )
            {
            }

            constexpr Holder ( const volatile ThisType && other )
                : m_value( ::std::forward< const volatile Value >( other.m_value ) )
            {
            }

            constexpr Holder ( ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( const ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( volatile ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( const volatile ThisType & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< const _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( volatile Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< volatile _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const volatile Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< const volatile _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( volatile Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const volatile Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _Argument >
            constexpr void operator = ( _Argument && argument )
            {
                m_value = ::std::forward< _Argument >( argument );
            }

            constexpr void operator = ( ThisType && other )
            {
                m_value = ::std::forward< Value >( other.m_value );
            }

            constexpr void operator = ( const ThisType && other )
            {
                m_value = ::std::forward< const Value >( other.m_value );
            }

            constexpr void operator = ( volatile ThisType && other )
            {
                m_value = ::std::forward< volatile Value >( other.m_value );
            }

            constexpr void operator = ( const volatile ThisType && other )
            {
                m_value = ::std::forward< const volatile Value >( other.m_value );
            }

            constexpr void operator = ( ThisType && other ) volatile
            {
                m_value = ::std::forward< Value >( other.m_value );
            }

            constexpr void operator = ( const ThisType && other ) volatile
            {
                m_value = ::std::forward< const Value >( other.m_value );
            }

            constexpr void operator = ( volatile ThisType && other ) volatile
            {
                m_value = ::std::forward< volatile Value >( other.m_value );
            }

            constexpr void operator = ( const volatile ThisType && other ) volatile
            {
                m_value = ::std::forward< const volatile Value >( other.m_value );
            }

            constexpr void operator = ( ThisType && other ) const volatile
            {
                m_value = ::std::forward< Value >( other.m_value );
            }

            constexpr void operator = ( const ThisType && other ) const volatile
            {
                m_value = ::std::forward< const Value >( other.m_value );
            }

            constexpr void operator = ( volatile ThisType && other ) const volatile
            {
                m_value = ::std::forward< volatile Value >( other.m_value );
            }

            constexpr void operator = ( const volatile ThisType && other ) const volatile
            {
                m_value = ::std::forward< const volatile Value >( other.m_value );
            }

            constexpr void operator = ( ThisType && other ) const
            {
                m_value = ::std::forward< Value >( other.m_value );
            }

            constexpr void operator = ( const ThisType && other ) const
            {
                m_value = ::std::forward< const Value >( other.m_value );
            }

            constexpr void operator = ( volatile ThisType && other ) const
            {
                m_value = ::std::forward< volatile Value >( other.m_value );
            }

            constexpr void operator = ( const volatile ThisType && other ) const
            {
                m_value = ::std::forward< const volatile Value >( other.m_value );
            }

            constexpr void operator = ( ThisType & other )
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const ThisType & other )
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( volatile ThisType & other )
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const volatile ThisType & other )
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( ThisType & other ) const
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const ThisType & other ) const
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( volatile ThisType & other ) const
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const volatile ThisType & other ) const
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( ThisType & other ) volatile
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const ThisType & other ) volatile
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( volatile ThisType & other ) volatile
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const volatile ThisType & other ) volatile
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( ThisType & other ) const volatile
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const ThisType & other ) const volatile
            {
                return *this = other.m_value;
            }

            constexpr void operator = ( volatile ThisType & other ) const volatile
            {
                *this = other.m_value;
            }

            constexpr void operator = ( const volatile ThisType & other ) const volatile
            {
                return *this = other.m_value;
            }

            template < typename _OtherValue >
            constexpr void operator = ( Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            constexpr void operator = ( const Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< const _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            constexpr void operator = ( volatile Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< volatile _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            constexpr void operator = ( const volatile Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< const volatile _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            constexpr void operator = ( Holder< _OtherValue > & other )
            {
                *this = other.m_value;
            }

            template < typename _OtherValue >
            constexpr void operator = ( const Holder< _OtherValue > & other )
            {
                *this = other.m_value;
            }

            template < typename _OtherValue >
            constexpr void operator = ( volatile Holder< _OtherValue > & other )
            {
                *this = other.m_value;
            }

            template < typename _OtherValue >
            constexpr void operator = ( const volatile Holder< _OtherValue > & other )
            {
                *this = other.m_value;
            }

            // nothing to do
            static constexpr void guard ( ThisType && ) {}
            static constexpr void guard ( const ThisType && ) {}
            static constexpr void guard ( volatile ThisType && ) {}
            static constexpr void guard ( const volatile ThisType && ) {}
            static constexpr void guard ( ThisType & ) {}
            static constexpr void guard ( const ThisType & ) {}
            static constexpr void guard ( volatile ThisType & ) {}
            static constexpr void guard ( const volatile ThisType & ) {}

            // nothing to do
            static constexpr void unguard ( ThisType && ) {}
            static constexpr void unguard ( const ThisType && ) {}
            static constexpr void unguard ( volatile ThisType && ) {}
            static constexpr void unguard ( const volatile ThisType && ) {}
            static constexpr void unguard ( ThisType & ) {}
            static constexpr void unguard ( const ThisType & ) {}
            static constexpr void unguard ( volatile ThisType & ) {}
            static constexpr void unguard ( const volatile ThisType & ) {}

            static constexpr Value && value ( ThisType && holder )
            {
                return ::std::forward< Value >( holder.m_value );
            }

            static constexpr const Value && value ( const ThisType && holder )
            {
                return ::std::forward< const Value >( holder.m_value );
            }

            static constexpr volatile Value && value ( volatile ThisType && holder )
            {
                return ::std::forward< volatile Value >( holder.m_value );
            }

            static constexpr const volatile Value && value ( const volatile ThisType && holder )
            {
                return ::std::forward< const volatile Value >( holder.m_value );
            }

            static constexpr Value & value ( ThisType & holder )
            {
                return holder.m_value;
            }

            static constexpr const Value & value ( const ThisType & holder )
            {
                return holder.m_value;
            }

            static constexpr volatile Value & value ( volatile ThisType & holder )
            {
                return holder.m_value;
            }

            static constexpr const volatile Value & value ( const volatile ThisType & holder )
            {
                return holder.m_value;
            }
        };
    };
}

#endif
