#pragma once

#include <ModelKit/Common/HolderPointer.h>
#include <ModelKit/Common/InitializeType.h>
#include <ModelKit/Value/InstanceTrait.h>
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
            struct HolderType
            {
                using ThisType = HolderType< _Type >;
                using ValueTool = ::Std::Mutex::AtomicTool;

                using LockType = _LockType;
                using ValueType = _Type;
                using LockPointerType = ::std::lock_guard< LockType >;
                using WritableHolderPointer = ::HolderPointer< ThisType &, ValueTool >;

                mutable LockType m_lock;
                ValueType m_value;

                constexpr HolderType ( InitializeType )
                {
                }

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                    : m_lock()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                }

                HolderType ( ThisType && other )
                    : m_lock()
                {
                    using OtherMovableHolderPointer = ::HolderPointer< ThisType &&, ValueTool >;
                    OtherMovableHolderPointer guard( other );
                    m_value = ::std::forward< ValueType >( other.m_value );
                }

                HolderType ( const ThisType & other )
                    : m_lock()
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const ThisType &, ValueTool >;
                    OtherReadableHolderPointer guard( other );
                    m_value =  other.m_value;
                }

                template < typename _OtherType >
                HolderType ( HolderType< _OtherType > && other )
                    : m_lock()
                {
                    using OtherMovableHolderPointer = ::HolderPointer< HolderType< _OtherType > &&, ValueTool >;
                    OtherMovableHolderPointer guard( other );
                    m_value = ::std::forward< ValueType >( other.m_value );
                }

                template < typename _OtherType >
                HolderType ( const HolderType< _OtherType > & other )
                    : m_lock()
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const HolderType< _OtherType > &, ValueTool >;
                    OtherReadableHolderPointer guard( other );
                    m_value =  other.m_value;
                }

                ~HolderType ()
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
                    using OtherMovableHolderPointer = ::HolderPointer< ThisType &&, ValueTool >;
                    OtherMovableHolderPointer other_guard( ::std::forward< ThisType >( other ) );
                    WritableHolderPointer guard( *this );
                    m_value = ::std::forward< ValueType >( other.m_value );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    using OtherReadableHolderPointer = ::HolderPointer< const ThisType &, ValueTool >;
                    OtherReadableHolderPointer other_guard( other );
                    WritableHolderPointer guard( *this );
                    m_value = other.m_value;
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( HolderType< _OtherType > && other )
                {
                    using OtherType = HolderType< _OtherType >;
                    using OtherMovableHolderPointer = ::HolderPointer< OtherType &&, ValueTool >;
                    OtherMovableHolderPointer other_guard( ::std::forward< OtherType >( other ) );
                    WritableHolderPointer guard( *this );
                    m_value = ::std::forward< typename OtherType::ValueType >( other.m_value );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const HolderType< _OtherType > & other )
                {
                    using OtherType = HolderType< _OtherType >;
                    using OtherReadableHolderPointer = ::HolderPointer< const OtherType &, ValueTool >;
                    OtherReadableHolderPointer other_guard( other );
                    WritableHolderPointer guard( *this );
                    m_value = other.m_value;
                    return *this;
                }
            };

            template < typename _Type >
            //static constexpr void guardHolder ( HolderType< _Type > & )
            //static constexpr void guardHolder ( HolderType< _Type > && )
            static constexpr void guardHolder ( const HolderType< _Type > & holder )
            {
                holder.m_lock.lock();
            }

            template < typename _Type >
            //static constexpr void unguardHolder ( HolderType< _Type > & )
            //static constexpr void unguardHolder ( HolderType< _Type > && )
            static constexpr void unguardHolder ( const HolderType< _Type > & holder )
            {
                holder.m_lock.unlock();
            }

            template < typename _Type >
            static constexpr _Type & value ( HolderType< _Type > & holder )
            {
                return holder.m_value;
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return holder.m_value;
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( holder.m_value );
            }
        };
    }
}
