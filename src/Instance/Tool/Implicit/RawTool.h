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
            using PointerType = Value *;
            using WritableGuard = ::HolderGuard< ThisType &, Implicit::RawTool >;

            PointerType m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new Value( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
            {
                increment();
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::PointerType >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : Holder( static_cast< const ThisType & >( other.m_pointer ) )
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
                m_pointer = reinterpret_cast< PointerType >( other.m_pointer );
                other.m_pointer = nullptr;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                static_assert( ::std::is_base_of< _Type, _OtherType >::value,
                    "_Type must be base of _OtherType" );

                if ( m_pointer != reinterpret_cast< PointerType >( other.m_pointer ) )
                {
                    decrement();
                    // TODO: проверить вариант с множественным наследованием
                    m_pointer = reinterpret_cast< PointerType >( other.m_pointer );
                    increment();
                }
                return *this;
            }
        };

        template < typename _Type >
        static constexpr void guardHolder ( Holder< _Type > & holder )
        {
            if ( !!holder.m_pointer && !!holder.m_pointer->m_counter )
                holder = Holder< _Type >( holder.m_pointer->m_value );
        }

        template < typename _Type >
        //static constexpr void guardHolder ( Holder< _Type > && )
        static constexpr void guardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        //static constexpr void unguardHolder ( Holder< _Type > & )
        //static constexpr void unguardHolder ( Holder< _Type > && )
        static constexpr void unguardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        static constexpr _Type & value ( Holder< _Type > & holder )
        {
            return holder.m_pointer->m_value;
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return holder.m_pointer->m_value;
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( holder.m_pointer->m_value );
        }
    };
}

#endif
