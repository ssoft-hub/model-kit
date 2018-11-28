#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_MUTEX_H
#define INSTANCE_TOOL_THREAD_SAFE_MUTEX_H

#include <ModelKit/Instance/Access/HolderGuard.h>
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
            using WritableGuard = ::HolderGuard< ThisType & >;

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
                using OtherMovableGuard = ::HolderGuard< ThisType && >;
                OtherMovableGuard guard( other );
                m_value = ::std::forward< Value >( other.m_value );
            }

            Holder ( const ThisType & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const ThisType &  >;
                OtherReadableGuard guard( other );
                m_value =  other.m_value;
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderGuard< Holder< _OtherType > && >;
                OtherMovableGuard guard( other );
                m_value = ::std::forward< Value >( other.m_value );
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const Holder< _OtherType > & >;
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
                using OtherMovableGuard = ::HolderGuard< ThisType && >;
                OtherMovableGuard other_guard( ::std::forward< ThisType >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< Value >( other.m_value );
                return *this;
            }

            ThisType & operator = ( const ThisType & other )
            {
                using OtherReadableGuard = ::HolderGuard< const ThisType & >;
                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                using OtherType = Holder< _OtherType >;
                using OtherMovableGuard = ::HolderGuard< OtherType && >;
                OtherMovableGuard other_guard( ::std::forward< OtherType >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< typename OtherType::Value >( other.m_value );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                using OtherType = Holder< _OtherType >;
                using OtherReadableGuard = ::HolderGuard< const OtherType & >;
                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
                return *this;
            }

            //static constexpr void guard ( ThisType && )
            //static constexpr void guard ( const ThisType && )
            //static constexpr void guard ( ThisType & )
            static constexpr void guard ( const ThisType & holder )
            {
                holder.m_lock.lock();
            }

            //static constexpr void unguard ( ThisType && )
            //static constexpr void unguard ( const ThisType && )
            //static constexpr void unguard ( ThisType & )
            static constexpr void unguard ( const ThisType & holder )
            {
                holder.m_lock.unlock();
            }

            static constexpr _Type && value ( ThisType && holder )
            {
                return ::std::forward< _Type && >( holder.m_value );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type && >( holder.m_value );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return ::std::forward< _Type & >( holder.m_value );
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return ::std::forward< const _Type & >( holder.m_value );
            }
        };
    };
}

#endif
