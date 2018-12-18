#pragma once
#ifndef INSTANCE_TOOL_THREAD_SAFE_SHARED_LOCK_H
#define INSTANCE_TOOL_THREAD_SAFE_SHARED_LOCK_H

#include <ModelKit/Featuring/Access/HolderGuard.h>
#include <ModelKit/Featuring/Traits.h>

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

            //! Guard internal value of Holder for any king of mutable referencies.
            template < typename _Holder,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Holder > >::value > >
            static constexpr void guard ( _Holder && holder )
            {
                using HolderRefer = _Holder &&;
                ::std::forward< HolderRefer >( holder ).m_lock.lock();
            }

            //! Guard internal value of Holder for any king of constant referencies.
            template < typename _Holder,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Holder > >::value > >
            static constexpr void guard ( const _Holder && holder )
            {
                using HolderRefer = _Holder &&;
                ::std::forward< HolderRefer >( holder ).m_lock.lock_shared();
            }

            //! Unguard internal value of Holder for any king of mutable referencies.
            template < typename _Holder,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Holder > >::value > >
            static constexpr void unguard ( _Holder && holder )
            {
                using HolderRefer = _Holder &&;
                ::std::forward< HolderRefer >( holder ).m_lock.unlock();
            }

            //! Unguard internal value of Holder for any king of constant referencies.
            template < typename _Holder,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Holder > >::value > >
            static constexpr void unguard ( const _Holder && holder )
            {
                using HolderRefer = _Holder &&;
                ::std::forward< HolderRefer >( holder ).m_lock.unlock_shared();
            }

            //! Access to internal value of Holder for any king of referencies.
            template < typename _Refer >
            static constexpr decltype(auto) value ( _Refer && holder )
            {
                using HolderRefer = _Refer &&;
                using ValueRefer = ::SimilarRefer< _Value, HolderRefer >;
                return ::std::forward< ValueRefer >( holder.m_value );
            }
        };
    };
}

#endif
