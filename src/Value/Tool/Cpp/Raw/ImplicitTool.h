#pragma once

#include <assert.h>
#include <atomic>
#include <ModelKit/Common/HolderGuard.h>
#include <ModelKit/Common/InitializeType.h>
#include <ModelKit/Value/InstanceTrait.h>
namespace Cpp
{
    namespace Raw
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * "голого" raw указателя.
         */
        struct ImplicitTool
        {
            template < typename _Type,
                typename _CounterType = ::std::atomic< int32_t > >
            struct CountedType
            {
                using ThisType = CountedType< _Type, _CounterType >;

                using ValueType = _Type;
                using CounterType = _CounterType;

                CounterType m_counter;
                ValueType m_value;

                template < typename ... _Arguments >
                CountedType ( _Arguments && ... arguments )
                : m_counter()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                }
            };

            template < typename _Type,
                typename _CounterType = ::std::atomic< int32_t > >
            struct HolderType
            {
                using ThisType = HolderType< _Type, _CounterType >;

                using ValueType = CountedType< _Type >;
                using PointerType = ValueType *;
                using WritableHolderGuard = ::HolderGuard< ThisType &, ::Cpp::Raw::ImplicitTool >;

                PointerType m_pointer;

                constexpr HolderType ( InitializeType )
                : m_pointer()
                {
                }

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                : m_pointer( new ValueType( ::std::forward< _Arguments >( arguments ) ... ) )
                {
                }

                HolderType ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
                {
                    other.m_pointer = nullptr;
                }

                HolderType ( const ThisType & other )
                : m_pointer( other.m_pointer )
                {
                    increment();
                }

                template < typename _OtherType >
                HolderType ( HolderType< _OtherType > && other )
                : m_pointer( ::std::forward< typename HolderType< _OtherType >::PointerType >( other.m_pointer ) )
                {
                    other.m_pointer = nullptr;
                }

                template < typename _OtherType >
                HolderType ( const HolderType< _OtherType > & other )
                : HolderType( static_cast< const ThisType & >( other.m_pointer ) )
                {
                    increment();
                }

                ~HolderType ()
                {
                    decrement();
                }

                void increment ()
                {
                    if ( m_pointer )
                        m_pointer->m_counter.fetch_add( 1 );
                }

                void decrement ()
                {
                    if ( m_pointer && !m_pointer->m_counter.fetch_sub( 1 ) )
                    {
                        delete m_pointer;
                        m_pointer = nullptr;
                    }
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    assert( m_pointer );
                    WritableHolderGuard guard( *this );
                    m_pointer->m_value = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    assert( m_pointer );
                    WritableHolderGuard guard( *this );
                    m_pointer->m_value = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    decrement();
                    m_pointer = other.m_pointer;
                    other.m_pointer = nullptr;
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    if ( m_pointer != other.m_pointer )
                    {
                        decrement();
                        m_pointer = other.m_pointer;
                        increment();
                    }
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( HolderType< _OtherType > && other )
                {
                    static_assert( ::std::is_base_of< _Type, _OtherType >::value,
                        "_Type must be base of _OtherType" );

                    decrement();
                    // TODO: проверить вариант с множественным наследованием
                    m_pointer = reinterpret_cast< PointerType >( other.m_pointer );
                    other.m_pointer = nullptr;
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const HolderType< _OtherType > & other )
                {
                    static_assert( ::std::is_base_of< _Type, _OtherType >::value,
                        "_Type must be base of _OtherType" );

                    if ( m_pointer != reinterpret_cast< PointerType >( other.m_pointer ) )
                    {
                        decrement();
                        // TODO: проверить вариант с множественным наследованием
                        m_pointer = reinterpret_cast< PointerType >( other.m_pointer );
                        increment();
                    }
                    return *this;
                }
            };

            template < typename _Type >
            static constexpr void guardHolder ( HolderType< _Type > & holder )
            {
                if ( !!holder.m_pointer && !!holder.m_pointer->m_counter )
                    holder = HolderType< _Type >( holder.m_pointer->m_value );
            }

            template < typename _Type >
            //static constexpr void guardHolder ( HolderType< _Type > && )
            static constexpr void guardHolder ( const HolderType< _Type > & )
            {
                // nothing to do
            }

            template < typename _Type >
            //static constexpr void unguardHolder ( HolderType< _Type > & )
            //static constexpr void unguardHolder ( HolderType< _Type > && )
            static constexpr void unguardHolder ( const HolderType< _Type > & )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr _Type & value ( HolderType< _Type > & holder )
            {
                return holder.m_pointer->m_value;
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return holder.m_pointer->m_value;
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( holder.m_pointer->m_value );
            }
        };
    }
}
