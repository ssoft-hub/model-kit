#pragma once
#ifndef INSTANCE_TOOL_IMPLICIT_SHARED_H
#define INSTANCE_TOOL_IMPLICIT_SHARED_H

#include <cassert>
#include <memory>
#include <ModelKit/Instance/Access/HolderGuard.h>
#include <utility>

namespace Implicit
{
    /*!
     * Инструмент для формирования неявно обобщенного значения на основе
     * умного указателя ::std::shared_ptr.
     * Инструмент реализует технику ленивых вычислений, когда копирование
     * экземпляра значения происходит только в момент доступа к неконстантному
     * экземпляру.
     */
    struct SharedTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using PointerType = ::std::shared_ptr< _Type >;
            using WritableGuard = ::HolderGuard< ThisType &, ::Implicit::SharedTool >;

            PointerType m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( ::std::make_unique< _Type >( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
            {
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::PointerType >( other.m_pointer ) )
            {
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : m_pointer( other.m_pointer )
            {
            }

            ~Holder ()
            {
                m_pointer.reset();
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                assert( m_pointer );
                WritableGuard guard( *this );
                *m_pointer.get() = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                assert( m_pointer );
                WritableGuard guard( *this );
                *m_pointer.get() = other;
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                m_pointer = ::std::forward< PointerType >( other.m_pointer );
                return *this;
            }

            ThisType & operator = ( const ThisType & other )
            {
                m_pointer = other.m_pointer;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                using OtherPointerType = typename Holder< _OtherType >::PointerType;
                m_pointer = ::std::forward< OtherPointerType >( other.m_pointer );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                m_pointer = other.m_pointer;
                return *this;
            }
        };

        template < typename _Type >
        static constexpr void guardHolder ( Holder< _Type > & holder )
        {
            if ( !!holder.m_pointer && !holder.m_pointer.unique() )
                holder = Holder< _Type >( *holder.m_pointer.get() );
        }

        template < typename _Type >
        //static constexpr void guardHolder ( Holder< _Type > && )
        static constexpr void guardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        //static constexpr void unguardHolder ( Holder< _Type > & )
        //static constexpr void unguardHolder ( Holder< _Type > && )
        static constexpr void unguardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        static constexpr _Type & value ( Holder< _Type > & holder )
        {
            return *holder.m_pointer.get();
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return *holder.m_pointer.get();
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( *holder.m_pointer.get() );
        }
    };
}

#endif
