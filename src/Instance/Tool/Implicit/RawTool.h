#pragma once
#ifndef INSTANCE_TOOL_IMPLICIT_RAW_H
#define INSTANCE_TOOL_IMPLICIT_RAW_H

#include <atomic>
#include <cassert>
#include <ModelKit/Instance/Access/HolderGuard.h>
#include <utility>

namespace Implicit
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * "голого" raw указателя.
     */
    struct RawTool
    {
        template < typename _Type,
            typename _CounterType = ::std::atomic< int32_t > >
        struct CountedType
        {
            using ThisType = CountedType< _Type, _CounterType >;

            using Value = _Type;
            using CounterType = _CounterType;

            CounterType m_counter;
            Value m_value;

            template < typename ... _Arguments >
            CountedType ( _Arguments && ... arguments )
                : m_counter()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }
        };

        template < typename _Type,
            typename _CounterType = ::std::atomic< int32_t > >
        struct Holder
        {
            using ThisType = Holder< _Type, _CounterType >;

            using Value = CountedType< _Type >;
            using RawPointer = Value *;
            using ConstRawPointer = const Value *;
            using WritableGuard = ::HolderGuard< ThisType & >;

            RawPointer m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new Value( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< RawPointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const ThisType && other )
                : m_pointer( ::std::forward< ConstRawPointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( ThisType & other )
                : m_pointer( other.m_pointer )
            {
                increment();
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
            {
                increment();
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::RawPointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::ConstRawPointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherType >
            Holder (  Holder< _OtherType > & other )
                : m_pointer( other.m_pointer )
            {
                increment();
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : m_pointer( other.m_pointer )
            {
                increment();
            }

            ~Holder ()
            {
                decrement();
            }

            void increment ()
            {
                if ( m_pointer )
                    m_pointer->m_counter.fetch_add( 1 );
            }

            void decrement ()
            {
                if ( m_pointer && !m_pointer->m_counter.fetch_sub( 1 ) )
                {
                    delete m_pointer;
                    m_pointer = nullptr;
                }
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                assert( m_pointer );
                WritableGuard guard( *this );
                m_pointer->m_value = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                assert( m_pointer );
                WritableGuard guard( *this );
                m_pointer->m_value = other;
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                decrement();
                m_pointer = other.m_pointer;
                other.m_pointer = nullptr;
                return *this;
            }

            ThisType & operator = ( const ThisType & other )
            {
                if ( m_pointer != other.m_pointer )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    increment();
                }
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                static_assert( ::std::is_base_of< _Type, _OtherType >::value,
                    "_Type must be base of _OtherType" );

                decrement();
                // TODO: проверить вариант с множественным наследованием
                m_pointer = reinterpret_cast< RawPointer >( other.m_pointer );
                other.m_pointer = nullptr;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                static_assert( ::std::is_base_of< _Type, _OtherType >::value,
                    "_Type must be base of _OtherType" );

                if ( m_pointer != reinterpret_cast< RawPointer >( other.m_pointer ) )
                {
                    decrement();
                    // TODO: проверить вариант с множественным наследованием
                    m_pointer = reinterpret_cast< RawPointer >( other.m_pointer );
                    increment();
                }
                return *this;
            }

            static constexpr void guard ( ThisType & holder )
            {
                if ( !!holder.m_pointer && !!holder.m_pointer->m_counter )
                    holder = Holder< _Type >( holder.m_pointer->m_value );
            }

            //static constexpr void guard ( ThisType && )
            //static constexpr void guard ( const ThisType && )
            static constexpr void guard ( const ThisType & )
            {
                // nothing to do
            }

            //static constexpr void unguard ( ThisType && )
            //static constexpr void unguard ( const ThisType && )
            //static constexpr void unguard ( ThisType & )
            static constexpr void unguard ( const ThisType & )
            {
                // nothing to do
            }

            static constexpr _Type && value ( ThisType && holder )
            {
                return ::std::forward< _Type && >( holder.m_pointer->m_value );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type && >( holder.m_pointer->m_value );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return ::std::forward< _Type & >( holder.m_pointer->m_value );
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return ::std::forward< const _Type & >( holder.m_pointer->m_value );
            }
        };
    };
}

#endif
