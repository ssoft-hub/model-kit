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
            using _LockType = ::std::mutex;

            template < typename _Type >
            using Value = _Type;

            template < typename _Type >
            struct ValueHolder
            {
                using ThisType = ValueHolder< _Type >;
                using LockType = _LockType;
                using ValueType = Value< _Type >;

                mutable LockType m_lock;
                ValueType m_value;

                ValueHolder ( ThisType && other )
                : m_lock()
                , m_value( ::std::forward< ValueType >( other.m_value ) )
                {
                }

                ValueHolder ( const ThisType & other )
                : m_lock()
                , m_value( other.m_value )
                {
                }

                template < typename ... _Arguments >
                ValueHolder ( _Arguments && ... arguments )
                : m_lock()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                }
            };

            template < typename _Type >
            class AccessValue
            {
                using ValueType = Value< _Type >;
                using LockType = _LockType;
                using LockerType = ::std::unique_lock< LockType >;

                LockerType m_locker;
                ValueType * const m_value;

            public:
                AccessValue ( LockType & lock, ValueType & value )
                : m_locker( lock )
                , m_value( &value )
                {
                    std::cout << "locked" << ::std::endl;
                }

                AccessValue ( AccessValue && other )
                : m_locker( ::std::forward< LockerType >( other.m_locker ) )
                , m_value( other.m_value )
                {
                }

                ~AccessValue ()
                {
                    std::cout << "unlocked" << ::std::endl;
                }

                ValueType & access () const
                {
                    return *m_value;
                }


            };

            template < typename _Type >
            using WritableProxy = AccessValue< _Type >;

            template < typename _Type >
            using ReadableProxy = AccessValue< const _Type >;

            template < typename _Type >
            using WritableAccessValue = AccessValue< _Type >;

            template < typename _Type >
            using ReadableAccessValue = AccessValue< const _Type >;

            template < typename _Type >
            using MovableAccessValue = AccessValue< _Type >;


            template < typename _Type, typename ... _Arguments >
            static ValueHolder< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return ValueHolder< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static void destroyValueHolder ( ValueHolder< _Type > & )
            {
            }

            template < typename _Type >
            static WritableAccessValue< _Type > writable ( ValueHolder< _Type > & holder )
            {
                return WritableAccessValue< _Type >( holder.m_lock, holder.m_value );
            }

            template < typename _Type >
            static ReadableAccessValue< _Type > readable ( const ValueHolder< _Type > & holder )
            {
                return ReadableAccessValue< _Type >( holder.m_lock, holder.m_value );
            }

            template < typename _Type >
            static MovableAccessValue< _Type > movable ( ValueHolder< _Type > && holder )
            {
                return MovableAccessValue< _Type >( holder.m_lock, holder.m_value );
            }
        };
    }
}

template < typename _RequaredType, typename _ValueType >
struct AccessHelper< _RequaredType, ::Std::Mutex::AtomicTool::AccessValue< _ValueType > >
{
    using WrapperType = typename ::Std::Mutex::AtomicTool::AccessValue< _ValueType >;

    static constexpr _RequaredType & writable ( WrapperType & wrapper )
    {
        return getWritableValue< _RequaredType >( wrapper.access()  );
    }

    static constexpr const _RequaredType & readable ( const WrapperType & wrapper )
    {
        return getReadableValue< _RequaredType >( wrapper.access() );
    }

    static constexpr _RequaredType & movable ( WrapperType && wrapper )
    {
        return getWritableValue< _RequaredType >( wrapper.access() );
    }
};


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
