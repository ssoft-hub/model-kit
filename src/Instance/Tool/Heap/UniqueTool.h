#pragma once
#ifndef INSTANCE_TOOL_HEAP_UNIQUE_H
#define INSTANCE_TOOL_HEAP_UNIQUE_H

#include <cassert>
#include <memory>
#include <utility>

namespace Heap
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * умного указателя ::std::unique_ptr.
     */
    struct UniqueTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using PointerType = ::std::unique_ptr< _Type >;

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
            : Holder( *other.m_pointer.get() )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::PointerType >( other.m_pointer ) )
            {
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : Holder( *other.m_pointer.get() )
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
                *m_pointer.get() = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                assert( m_pointer );
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
                assert( other.m_pointer );
                return *this = *other.m_pointer.get();
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
                assert( other.m_pointer );
                return *this = *other.m_pointer.get();
            }

            //static constexpr void guard ( ThisType && )
            //static constexpr void guard ( const ThisType && )
            //static constexpr void guard ( ThisType & )
            static constexpr void guard ( const ThisType & )
            {
                // nothing to do
            }

            //static constexpr void unguard ( ThisType && )
            //static constexpr void unguard ( const ThisType && )
            //static constexpr void unguard ( ThisType & )
            static constexpr void unguard ( const ThisType & )
            {
                // nothing to do
            }

            static constexpr _Type && value ( ThisType && holder )
            {
                return ::std::forward< _Type && >( *holder.m_pointer.get() );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type && >( *holder.m_pointer.get() );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return ::std::forward< _Type & >( *holder.m_pointer.get() );
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return ::std::forward< const _Type & >( *holder.m_pointer.get() );
            }
        };
    };
}

#endif
