#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>
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

                using LockType = _LockType;
                using ValueType = _Type;
                using LockGuardType = ::std::lock_guard< LockType >;

                struct HolderGuard
                {
                    ThisType & m_holder;
                    HolderGuard ( ThisType & holder ) : m_holder( holder ) { guardHolder( m_holder ); }
                    ~HolderGuard () { unguardHolder( m_holder ); }
                };

                mutable LockType m_lock;
                ValueType m_value;

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                : m_lock()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                }

                HolderType ( ThisType && other )
                : m_lock()
                , m_value( ::std::forward< ValueType >( other.m_value ) )
                {
                }

                ~HolderType ()
                {
                    HolderGuard guard( *this );
                }

                ThisType & operator = ( ThisType && other )
                {
                    HolderGuard guard( *this );
                    m_value = ::std::forward< ValueType >( other.m_value );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    HolderGuard guard( *this );
                    m_value = other.m_value;
                    return *this;
                }
            };

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return makeHolder< _Type >( holder.m_value );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return makeHolder< _Type >( ::std::forward< _Type >( holder.m_value ) );
            }

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
