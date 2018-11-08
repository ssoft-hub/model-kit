#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_MUTEX_H
#define INSTANCE_TOOL_THREAD_SAFE_MUTEX_H

#include <ModelKit/Instance/Access/HolderPointer.h>
#include <ModelKit/Instance/Traits.h>
#include <mutex>

namespace ThreadSafe
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * умного указателя ::std::unique_ptr.
     */
    struct MutexTool
    {
        using _Lock = ::std::recursive_mutex;

        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using Tool = ThreadSafe::MutexTool;

            using Lock = _Lock;
            using Value = _Type;
            using LockGuard = ::std::lock_guard< Lock >;
            using WritableGuard = ::HolderPointer< ThisType &, Tool >;

            mutable Lock m_lock;
            Value m_value;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_lock()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }

            Holder ( ThisType && other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderPointer< ThisType &&, Tool >;
                OtherMovableGuard guard( other );
                m_value = ::std::forward< Value >( other.m_value );
            }

            Holder ( const ThisType & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderPointer< const ThisType &, Tool >;
                OtherReadableGuard guard( other );
                m_value =  other.m_value;
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderPointer< Holder< _OtherType > &&, Tool >;
                OtherMovableGuard guard( other );
                m_value = ::std::forward< Value >( other.m_value );
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderPointer< const Holder< _OtherType > &, Tool >;
                OtherReadableGuard guard( other );
                m_value =  other.m_value;
            }

            ~Holder ()
            {
                WritableGuard guard( *this );
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                WritableGuard guard( *this );
                m_value = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                WritableGuard guard( *this );
                m_value = other;
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                using OtherMovableGuard = ::HolderPointer< ThisType &&, Tool >;
                OtherMovableGuard other_guard( ::std::forward< ThisType >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< Value >( other.m_value );
                return *this;
            }

            ThisType & operator = ( const ThisType & other )
            {
                using OtherReadableGuard = ::HolderPointer< const ThisType &, Tool >;
                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                using OtherType = Holder< _OtherType >;
                using OtherMovableGuard = ::HolderPointer< OtherType &&, Tool >;
                OtherMovableGuard other_guard( ::std::forward< OtherType >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< typename OtherType::Value >( other.m_value );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                using OtherType = Holder< _OtherType >;
                using OtherReadableGuard = ::HolderPointer< const OtherType &, Tool >;
                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
                return *this;
            }
        };

        template < typename _Type >
        //static constexpr void guardHolder ( Holder< _Type > & )
        //static constexpr void guardHolder ( Holder< _Type > && )
        static constexpr void guardHolder ( const Holder< _Type > & holder )
        {
            holder.m_lock.lock();
        }

        template < typename _Type >
        //static constexpr void unguardHolder ( Holder< _Type > & )
        //static constexpr void unguardHolder ( Holder< _Type > && )
        static constexpr void unguardHolder ( const Holder< _Type > & holder )
        {
            holder.m_lock.unlock();
        }

        template < typename _Type >
        static constexpr _Type & value ( Holder< _Type > & holder )
        {
            return holder.m_value;
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return holder.m_value;
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( holder.m_value );
        }
    };
}

#endif
