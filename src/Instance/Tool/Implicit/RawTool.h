#pragma once
#ifndef INSTANCE_TOOL_IMPLICIT_RAW_H
#define INSTANCE_TOOL_IMPLICIT_RAW_H

#include <atomic>
#include <cassert>
#include <memory>
#include <ModelKit/Instance/Access/HolderGuard.h>
#include <utility>

namespace Implicit
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * "голого" raw указателя.
     */
    template < typename _Counter >
    struct CountedRawTool
    {
        struct BaseCounted
        {
            using Counter = _Counter;
            Counter m_counter;
            BaseCounted () : m_counter() {}
            virtual ~BaseCounted () {}
        };

        template < typename _Value >
        struct Counted
            : public BaseCounted
        {
            using ThisType = Counted< _Value >;
            using Value = _Value;

            Value m_value;

            template < typename ... _Arguments >
            Counted ( _Arguments && ... arguments )
                : BaseCounted()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }
        };

        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            using CountedValue = Counted< Value >;
            using CountedPointer = BaseCounted *;
            using Access = Value *;
            using WritableGuard = ::HolderGuard< ThisType & >;

            CountedPointer m_pointer;
            Access m_access; // to resolve multiple inheritance

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new CountedValue( ::std::forward< _Arguments >( arguments ) ... ) )
                , m_access( ::std::addressof( static_cast< CountedValue * >( m_pointer )->m_value ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const ThisType && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherValue >::CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< typename Holder< _OtherValue >::Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
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
                    ++m_pointer->m_counter;
            }

            void decrement ()
            {
                if ( m_pointer && !(--m_pointer->m_counter) )
                {
                    delete m_pointer;
                    m_pointer = nullptr;
                }
            }

            template < typename _Argument >
            void operator = ( _Argument && argument )
            {
                assert( m_pointer );
                WritableGuard guard( *this );
                *m_access = ::std::forward< _Argument >( argument );
            }

            void operator = ( ThisType && other )
            {
                decrement();
                m_pointer = other.m_pointer;
                m_access = other.m_access;
                other.m_pointer = nullptr;
            }

            void operator = ( const ThisType && other )
            {
                if ( m_pointer != other.m_pointer )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
            }

            void operator = ( ThisType & other )
            {
                if ( m_pointer != other.m_pointer )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
            }

            void operator = ( const ThisType & other )
            {
                if ( m_pointer != other.m_pointer )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > && other )
            {
                static_assert( ::std::is_base_of< _Value, _OtherValue >::value,
                    "_Value must be base of _OtherValue" );

                decrement();
                m_pointer = other.m_pointer;
                m_access = other.m_access;
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > && other )
            {
                static_assert( ::std::is_base_of< _Value, _OtherValue >::value,
                    "_Value must be base of _OtherValue" );

                if ( m_pointer != reinterpret_cast< CountedPointer >( other.m_pointer ) )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
                return *this;
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > & other )
            {
                static_assert( ::std::is_base_of< _Value, _OtherValue >::value,
                    "_Value must be base of _OtherValue" );

                if ( m_pointer != reinterpret_cast< CountedPointer >( other.m_pointer ) )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > & other )
            {
                static_assert( ::std::is_base_of< _Value, _OtherValue >::value,
                    "_Value must be base of _OtherValue" );

                if ( m_pointer != reinterpret_cast< CountedPointer >( other.m_pointer ) )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    m_access = other.m_access;
                    increment();
                }
            }

            static constexpr void guard ( ThisType & holder )
            {
                if ( !!holder.m_pointer && !!holder.m_pointer->m_counter )
                    holder = Holder< Value >( *holder.m_access );
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

            static constexpr Value && value ( ThisType && holder )
            {
                return ::std::forward< Value >( *holder.m_access );
            }

            static constexpr const Value && value ( const ThisType && holder )
            {
                return ::std::forward< const Value >( *holder.m_access );
            }

            static constexpr Value & value ( ThisType & holder )
            {
                return *holder.m_access;
            }

            static constexpr const Value & value ( const ThisType & holder )
            {
                return *holder.m_access;
            }
        };
    };
}

namespace Implicit
{
    using RawTool = struct CountedRawTool< ::std::atomic< int32_t > >;
}

#endif
