#pragma once

#include <ModelKit/Common/HolderGuard.h>
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
                using LockGuardType = ::std::lock_guard< LockType >;
                using WritableHolderGuard = ::HolderGuard< ThisType &, ValueTool >;

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
                    using OtherMovableHolderGuard = ::HolderGuard< ThisType &&, ValueTool >;
                    OtherMovableHolderGuard guard( other );
                    m_value = ::std::forward< ValueType >( other.m_value );
                }

                HolderType ( const ThisType & other )
                : m_lock()
                {
                    using OtherReadableHolderGuard = ::HolderGuard< const ThisType &, ValueTool >;
                    OtherReadableHolderGuard guard( other );
                    m_value =  other.m_value;
                }

                template < typename _OtherType >
                HolderType ( HolderType< _OtherType > && other )
                : m_lock()
                {
                    using OtherMovableHolderGuard = ::HolderGuard< HolderType< _OtherType > &&, ValueTool >;
                    OtherMovableHolderGuard guard( other );
                    m_value = ::std::forward< ValueType >( other.m_value );
                }

                template < typename _OtherType >
                HolderType ( const HolderType< _OtherType > & other )
                : m_lock()
                {
                    using OtherReadableHolderGuard = ::HolderGuard< const HolderType< _OtherType > &, ValueTool >;
                    OtherReadableHolderGuard guard( other );
                    m_value =  other.m_value;
                }

                ~HolderType ()
                {
                    WritableHolderGuard guard( *this );
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    WritableHolderGuard guard( *this );
                    m_value = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    WritableHolderGuard guard( *this );
                    m_value = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    using OtherMovableHolderGuard = ::HolderGuard< ThisType &&, ValueTool >;
                    OtherMovableHolderGuard other_guard( ::std::forward< ThisType >( other ) );
                    WritableHolderGuard guard( *this );
                    m_value = ::std::forward< ValueType >( other.m_value );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    using OtherReadableHolderGuard = ::HolderGuard< const ThisType &, ValueTool >;
                    OtherReadableHolderGuard other_guard( other );
                    WritableHolderGuard guard( *this );
                    m_value = other.m_value;
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( HolderType< _OtherType > && other )
                {
                    using OtherType = HolderType< _OtherType >;
                    using OtherMovableHolderGuard = ::HolderGuard< OtherType &&, ValueTool >;
                    OtherMovableHolderGuard other_guard( ::std::forward< OtherType >( other ) );
                    WritableHolderGuard guard( *this );
                    m_value = ::std::forward< typename OtherType::ValueType >( other.m_value );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const HolderType< _OtherType > & other )
                {
                    using OtherType = HolderType< _OtherType >;
                    using OtherReadableHolderGuard = ::HolderGuard< const OtherType &, ValueTool >;
                    OtherReadableHolderGuard other_guard( other );
                    WritableHolderGuard guard( *this );
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

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsAtomic< Instance< _Value, ::Std::Mutex::AtomicTool > >
    : public ::std::true_type
{
};
