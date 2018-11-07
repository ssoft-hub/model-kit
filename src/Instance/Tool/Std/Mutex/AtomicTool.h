#pragma once

#include <ModelKit/Instance/Access/HolderPointer.h>
#include <ModelKit/Instance/Traits.h>
#include <mutex>

namespace Std
{
    namespace Mutex
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * умного указателя ::std::unique_ptr.
         */
        struct AtomicTool
        {
            using _LockType = ::std::recursive_mutex;

            template < typename _Type >
            struct Holder
            {
                using ThisType = Holder< _Type >;
                using Tool = ::Std::Mutex::AtomicTool;

                using LockType = _LockType;
                using Value = _Type;
                using LockPointerType = ::std::lock_guard< LockType >;
                using WritableHolderPointer = ::HolderPointer< ThisType &, Tool >;

                mutable LockType m_lock;
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
                    using OtherMovableHolderPointer = ::HolderPointer< ThisType &&, Tool >;
                    OtherMovableHolderPointer guard( other );
                    m_value = ::std::forward< Value >( other.m_value );
                }

                Holder ( const ThisType & other )
                    : m_lock()
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const ThisType &, Tool >;
                    OtherReadableHolderPointer guard( other );
                    m_value =  other.m_value;
                }

                template < typename _OtherType >
                Holder ( Holder< _OtherType > && other )
                    : m_lock()
                {
                    using OtherMovableHolderPointer = ::HolderPointer< Holder< _OtherType > &&, Tool >;
                    OtherMovableHolderPointer guard( other );
                    m_value = ::std::forward< Value >( other.m_value );
                }

                template < typename _OtherType >
                Holder ( const Holder< _OtherType > & other )
                    : m_lock()
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const Holder< _OtherType > &, Tool >;
                    OtherReadableHolderPointer guard( other );
                    m_value =  other.m_value;
                }

                ~Holder ()
                {
                    WritableHolderPointer guard( *this );
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    WritableHolderPointer guard( *this );
                    m_value = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    WritableHolderPointer guard( *this );
                    m_value = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    using OtherMovableHolderPointer = ::HolderPointer< ThisType &&, Tool >;
                    OtherMovableHolderPointer other_guard( ::std::forward< ThisType >( other ) );
                    WritableHolderPointer guard( *this );
                    m_value = ::std::forward< Value >( other.m_value );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const ThisType &, Tool >;
                    OtherReadableHolderPointer other_guard( other );
                    WritableHolderPointer guard( *this );
                    m_value = other.m_value;
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( Holder< _OtherType > && other )
                {
                    using OtherType = Holder< _OtherType >;
                    using OtherMovableHolderPointer = ::HolderPointer< OtherType &&, Tool >;
                    OtherMovableHolderPointer other_guard( ::std::forward< OtherType >( other ) );
                    WritableHolderPointer guard( *this );
                    m_value = ::std::forward< typename OtherType::Value >( other.m_value );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const Holder< _OtherType > & other )
                {
                    using OtherType = Holder< _OtherType >;
                    using OtherReadableHolderPointer = ::HolderPointer< const OtherType &, Tool >;
                    OtherReadableHolderPointer other_guard( other );
                    WritableHolderPointer guard( *this );
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
}
