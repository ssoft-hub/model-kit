#pragma once
#ifndef INSTANCE_TOOL_HEAP_RAW_H
#define INSTANCE_TOOL_HEAP_RAW_H

#include <cassert>
#include <memory>
#include <utility>

namespace Heap
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * "голого" raw указателя.
     */
    struct RawTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using PointerType = _Type *;

            PointerType m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new _Type( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            //Holder ( const ThisType && other );
            //Holder ( ThisType & other );
            Holder ( const ThisType & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherType >::PointerType >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            //template < typename _OtherType >
            //Holder ( const Holder< _OtherType > && other )
            //template < typename _OtherType >
            //Holder ( Holder< _OtherType > & other )
            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : Holder( *other.m_pointer )
            {
            }

            ~Holder ()
            {
                delete m_pointer;
            }

            template < typename _Argument >
            ThisType & operator = ( _Argument && argument )
            {
                assert( m_pointer );
                *m_pointer = ::std::forward< _Argument >( argument );
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                ::std::swap( m_pointer, other.m_pointer );
                return *this;
            }

            //ThisType & operator = ( const ThisType && other )
            //ThisType & operator = ( ThisType & other )
            ThisType & operator = ( const ThisType & other )
            {
                assert( other.m_pointer );
                return *this = *other.m_pointer;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                ::std::swap< PointerType >( m_pointer, other.m_pointer );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                assert( other.m_pointer );
                return *this = *other.m_pointer;
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
                return ::std::forward< _Type >( *holder.m_pointer );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type >( *holder.m_pointer );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return *holder.m_pointer;
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return *holder.m_pointer;
            }
        };
    };
}

#endif
