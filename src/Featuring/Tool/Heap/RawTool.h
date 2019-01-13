#pragma once
#ifndef INSTANCE_TOOL_HEAP_RAW_H
#define INSTANCE_TOOL_HEAP_RAW_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Traits.h>
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
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;
            using Pointer = _Value *;

            Pointer m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( new Value( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const ThisType && other )
                : Holder( ::std::forward< const Value >( *other.m_pointer ) )
            {
            }

            Holder ( volatile ThisType && other )
                : m_pointer( ::std::forward< volatile Pointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            Holder ( const volatile ThisType && other )
                : Holder( ::std::forward< const volatile Value >( *other.m_pointer ) )
            {
            }

            Holder ( ThisType & other )
                : Holder( *other.m_pointer )
            {
            }

            Holder ( const ThisType & other )
                : Holder( *other.m_pointer )
            {
            }

            Holder ( volatile ThisType & other )
                : Holder( *other.m_pointer )
            {
            }

            Holder ( const volatile ThisType & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherValue >::Pointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( volatile Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< volatile typename Holder< _OtherValue >::Pointer >( other.m_pointer ) )
            {
                other.m_pointer = nullptr;
            }

            template < typename _OtherValue >
            Holder ( const volatile Holder< _OtherValue > && other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( volatile Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( const volatile Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            ~Holder ()
            {
                if ( m_pointer != nullptr )
                    delete m_pointer;
            }

            template < typename _Refer >
            static void operatorAssignment ( _Refer && holder, ThisType && other )
            {
                ::std::swap( holder.m_pointer, other.m_pointer );
            }

            template < typename _Refer >
            static void operatorAssignment ( _Refer && holder, volatile ThisType && other )
            {
                delete holder.m_pointer;
                holder.m_pointer = other.m_pointer;
                other.m_pointer = nullptr;
            }

            template < typename _Refer, typename _OtherValue >
            static void operatorAssignment ( _Refer && holder, Holder< _OtherValue > && other )
            {
                delete holder.m_pointer;
                holder.m_pointer = other.m_pointer;
                other.m_pointer = nullptr;
            }

            //! Access to internal value of Holder for any king of referencies.
            template < typename _Refer, typename _OtherValue >
            static void operatorAssignment ( _Refer && holder, volatile Holder< _OtherValue > && other )
            {
                delete holder.m_pointer;
                holder.m_pointer = other.m_pointer;
                other.m_pointer = nullptr;
            }

            template < typename _Refer >
            static constexpr decltype(auto) value ( _Refer && holder )
            {
                using HolderRefer = _Refer &&;
                using ValueRefer = ::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( *holder.m_pointer );
            }
        };
    };
}

#endif
