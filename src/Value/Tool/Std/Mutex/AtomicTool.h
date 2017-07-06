#pragma once
#include <mutex>
#include <iostream>

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

                mutable LockType m_lock;
                ValueType m_value;

                HolderType ( ThisType && other )
                : m_lock()
                , m_value( ::std::forward< ValueType >( other.m_value ) )
                {
                    ::std::cout << "HolderType ( ThisType && other )" << ::std::endl;
                }

                ThisType & operator = ( ThisType && other )
                {
                    ::std::cout << "HolderType operator ( ThisType && other )" << ::std::endl;
                    m_value = ( ::std::forward< ValueType >( other.m_value ) );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    ::std::cout << "HolderType operator ( const ThisType & other )" << ::std::endl;
                    m_value = other.m_value;
                    return *this;
                }

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                : m_lock()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                    ::std::cout << "HolderType ( _Arguments && ... arguments )" << ::std::endl;
                }
            };

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static constexpr void destroyValueHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardWritableHolder ( HolderType< _Type > & holder )
            {
                ::std::cout << "writable lock" << ::std::endl;
                holder.m_lock.lock();
            }

            template < typename _Type >
            static constexpr void unguardWritableHolder ( HolderType< _Type > & holder )
            {
                holder.m_lock.unlock();
                ::std::cout << "writable unlock" << ::std::endl;
            }

            template < typename _Type >
            static constexpr void guardReadableHolder ( const HolderType< _Type > & holder )
            {
                ::std::cout << "readable lock" << ::std::endl;
                holder.m_lock.lock();
            }

            template < typename _Type >
            static constexpr void unguardReadableHolder ( const HolderType< _Type > & holder )
            {
                holder.m_lock.unlock();
                ::std::cout << "readable unlock" << ::std::endl;
            }

            template < typename _Type >
            static constexpr void guardMovableHolder ( HolderType< _Type > && holder )
            {
                ::std::cout << "movable lock" << ::std::endl;
                holder.m_lock.lock();
            }

            template < typename _Type >
            static constexpr void unguardMovableHolder ( const HolderType< _Type > & holder )
            {
                holder.m_lock.unlock();
                ::std::cout << "movable unlock" << ::std::endl;
            }

            template < typename _Type >
            static constexpr AccessProxy< _Type & > getWritableProxy ( HolderType< _Type > & holder )
            {
                return AccessProxy< _Type & >( holder.m_value );
            }

            template < typename _Type >
            static constexpr AccessProxy< const _Type & > getReadableProxy ( const HolderType< _Type > & holder )
            {
                return AccessProxy< const _Type & >( holder.m_value );
            }

            template < typename _Type >
            static constexpr AccessProxy< _Type && > getMovableProxy ( HolderType< _Type > && holder )
            {
                return AccessProxy< _Type && >( ::std::forward< _Type >( holder.m_value ) );
            }
        };
    }
}

#include <Helper/InstanceHelper.h>
#include <Helper/TypeHelper.h>

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsAtomicInstance< Instance< _Value, ::Std::Mutex::AtomicTool > >
{
    static constexpr bool result = true;
};

/*!
 * Специализация помошника для вычисления типа InstanceType для значений в виде
 * оберток Instance с размещением в куче с помощью инструмента ::Std::Unique::HeapTool.
 *
 * Если оборачиваемое значений уже размещается в куче, то данный вид обертки игнорируется.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Std::Mutex::AtomicTool > >
{

    using InstanceType = typename TypeHelper< IsAtomicInstance< _ValueType >::result,
        typename InstanceHelper< _ValueType >::InstanceType,
        Instance< _ValueType, ::Std::Mutex::AtomicTool > >::Type;
};
