#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_SHARED_LOCK_H
#define INSTANCE_TOOL_THREAD_SAFE_SHARED_LOCK_H

#include <ModelKit/Instance/Access/HolderGuard.h>
#include <ModelKit/Instance/Traits.h>

namespace ThreadSafe
{
    /*!
     * Инструмент для формирования значения "по месту" с добавлением
     * объекта блокировки _Lock типа.
     */
    template <typename _Lock >
    struct SharedLockTool
    {
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            using Lock = _Lock;

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

            Holder ( const ThisType && other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const ThisType &&  >;
                OtherReadableGuard guard( other );
                m_value = ::std::forward< const Value >( other.m_value );
            }

            Holder ( ThisType & other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderGuard< ThisType &  >;
                OtherMovableGuard guard( other );
                m_value = other.m_value;
            }

            Holder ( const ThisType & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const ThisType &  >;
                OtherReadableGuard guard( other );
                m_value = other.m_value;
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderGuard< Holder< _OtherValue > && >;
                OtherMovableGuard guard( other );
                m_value = ::std::forward< Value >( other.m_value );
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const Holder< _OtherValue > && >;
                OtherReadableGuard guard( other );
                m_value = ::std::forward< const Value >( other.m_value );
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : m_lock()
            {
                using OtherMovableGuard = ::HolderGuard< Holder< _OtherValue > & >;
                OtherMovableGuard guard( other );
                m_value = other.m_value;
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : m_lock()
            {
                using OtherReadableGuard = ::HolderGuard< const Holder< _OtherValue > & >;
                OtherReadableGuard guard( other );
                m_value = other.m_value;
            }

            ~Holder ()
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                WritableGuard guard( *this );
            }

            template < typename _Argument >
            void operator = ( _Argument && other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                WritableGuard guard( *this );
                m_value = ::std::forward< _Argument >( other );
            }

            void operator = ( ThisType && other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherMovableGuard = ::HolderGuard< ThisType && >;
                OtherMovableGuard other_guard( ::std::forward< ThisType >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< Value >( other.m_value );
            }

            void operator = ( const ThisType && other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherReadableGuard = ::HolderGuard< const ThisType && >;

                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = ::std::forward< const Value >( other.m_value );
            }

            void operator = ( ThisType & other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherMovableGuard = ::HolderGuard< ThisType & >;

                OtherMovableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
            }

            void operator = ( const ThisType & other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherReadableGuard = ::HolderGuard< const ThisType & >;

                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > && other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherValue = Holder< _OtherValue >;
                using OtherMovableGuard = ::HolderGuard< OtherValue && >;

                OtherMovableGuard other_guard( ::std::forward< OtherValue >( other ) );
                WritableGuard guard( *this );
                m_value = ::std::forward< typename OtherValue::Value >( other.m_value );
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > && other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherValue = Holder< _OtherValue >;
                using OtherReadableGuard = ::HolderGuard< const OtherValue && >;

                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = ::std::forward< const OtherValue >( other.m_value );
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > & other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherType = Holder< _OtherValue >;
                using OtherMovableGuard = ::HolderGuard< OtherType & >;

                OtherMovableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > & other )
            {
                using WritableGuard = ::HolderGuard< ThisType & >;
                using OtherType = Holder< _OtherValue >;
                using OtherReadableGuard = ::HolderGuard< const OtherType & >;

                OtherReadableGuard other_guard( other );
                WritableGuard guard( *this );
                m_value = other.m_value;
            }

            static constexpr void guard ( ThisType && holder )
            {
                holder.m_lock.lock();
            }

            static constexpr void guard ( const ThisType && holder )
            {
                holder.m_lock.lock_shared();
            }

            static constexpr void guard ( ThisType & holder )
            {
                holder.m_lock.lock();
            }

            static constexpr void guard ( const ThisType & holder )
            {
                holder.m_lock.lock_shared();
            }

            static constexpr void unguard ( ThisType && holder )
            {
                holder.m_lock.unlock();
            }

            static constexpr void unguard ( const ThisType && holder )
            {
                holder.m_lock.unlock_shared();
            }

            static constexpr void unguard ( ThisType & holder )
            {
                holder.m_lock.unlock();
            }

            static constexpr void unguard ( const ThisType & holder )
            {
                holder.m_lock.unlock_shared();
            }

            static constexpr Value && value ( ThisType && holder )
            {
                return ::std::forward< Value >( holder.m_value );
            }

            static constexpr const Value && value ( const ThisType && holder )
            {
                return ::std::forward< const Value >( holder.m_value );
            }

            static constexpr Value & value ( ThisType & holder )
            {
                return holder.m_value;
            }

            static constexpr const Value & value ( const ThisType & holder )
            {
                return holder.m_value;
            }
        };
    };
}

#endif
