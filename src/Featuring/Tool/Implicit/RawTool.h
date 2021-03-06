#pragma once
#ifndef SCL_INSTANCE_TOOL_IMPLICIT_RAW_H
#define SCL_INSTANCE_TOOL_IMPLICIT_RAW_H

#include <atomic>
#include <cassert>
#include <memory>
#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Access/HolderGuard.h>
#include <ModelKit/Featuring/Traits.h>
#include <utility>

namespace Implicit
{
    /*!
     * Инструмент для формирования значения в "куче" на основе raw указателя.
     * Реализует технику ленивых вычислений, когда копирование
     * экземпляра значения происходит только в момент доступа к неконстантному
     * экземпляру.
     */
    template < typename _Counter >
    struct CountedRawTool
    {
        struct BaseCounted
        {
            using Counter = _Counter;
            Counter m_counter;
            BaseCounted () : m_counter() {}
            virtual ~BaseCounted () {}
        };

        template < typename _Value >
        struct Counted
            : public BaseCounted
        {
            using ThisType = Counted< _Value >;
            using Value = _Value;

            Value m_value;

            template < typename ... _Arguments >
            Counted ( _Arguments && ... arguments )
                : BaseCounted()
                , m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }
        };

        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            using CountedValue = Counted< Value >;
            using CountedPointer = BaseCounted *;
            using Access = Value *;
            using WritableGuard = ::ScL::HolderGuard< ThisType & >;

            CountedPointer m_pointer;
            Access m_access; // to resolve multiple inheritance

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new CountedValue( ::std::forward< _Arguments >( arguments ) ... ) )
                , m_access( ::std::addressof( static_cast< CountedValue * >( m_pointer )->m_value ) )
            {
                increment();
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const ThisType && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( volatile ThisType && other )
                : m_pointer( ::std::forward< volatile CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< volatile Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const volatile ThisType && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( volatile ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            Holder ( const volatile ThisType & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherValue >::CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< typename Holder< _OtherValue >::Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( volatile Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< volatile typename Holder< _OtherValue >::CountedPointer >( other.m_pointer ) )
                , m_access( ::std::forward< volatile typename Holder< _OtherValue >::Access >( other.m_access ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            Holder ( const volatile Holder< _OtherValue > && other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( volatile Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            template < typename _OtherValue >
            Holder ( const volatile Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
                , m_access( other.m_access )
            {
                increment();
            }

            ~Holder ()
            {
                decrement();
            }

            void increment ()
            {
                if ( m_pointer )
                    ++m_pointer->m_counter;
            }

            void decrement ()
            {
                if ( m_pointer && !(--m_pointer->m_counter) )
                {
                    delete m_pointer;
                    m_pointer = nullptr;
                }
            }

            /*!
             * Assignment operation between compatible Holders. Specialization
             * of operation enabled if left is not constant reference and any
             * kind of right.
             */
            template < typename _LeftHolderRefer, typename _RightHolderRefer,
                typename = ::std::enable_if_t<
                        !::std::is_const< ::std::remove_reference_t< _LeftHolderRefer > >::value > >
            static void operatorAssignment ( _LeftHolderRefer && left, _RightHolderRefer && right )
            {
                if ( left.m_pointer != right.m_pointer )
                {
                    left.decrement();
                    left.m_pointer = right.m_pointer;
                    left.m_access = right.m_access;
                    left.increment();
                }
            }

            /*!
             * Guard internal value of Holder for any not constant referencies.
             */
            template < typename _HolderRefer,
                typename = ::std::enable_if_t<
                        !::std::is_const< ::std::remove_reference_t< _HolderRefer > >::value > >
            static constexpr void guard ( _HolderRefer && holder )
            {
                if ( !!holder.m_pointer && holder.m_pointer->m_counter > 1 )
                {
                    CountedPointer pointer = new CountedValue( *holder.m_access );
                    holder.decrement();
                    holder.m_pointer = pointer;
                    holder.m_access = ::std::addressof( static_cast< CountedValue * >( holder.m_pointer )->m_value );
                }
            }

            /*!
             * Access to internal value of Holder for any king of referencies.
             */
            template < typename _HolderRefer >
            static constexpr decltype(auto) value ( _HolderRefer && holder )
            {
                using HolderRefer = _HolderRefer &&;
                using ValueRefer = ::ScL::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( *holder.m_access );
            }
        };
    };
}

namespace Implicit
{
    using RawTool = CountedRawTool< ::std::atomic< int32_t > >;
}

#endif
