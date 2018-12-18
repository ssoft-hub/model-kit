#pragma once
#ifndef INSTANCE_TOOL_HEAP_RAW_H
#define INSTANCE_TOOL_HEAP_RAW_H

#include <ModelKit/Featuring/Access/Accessing.h>
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
                : Holder( ::std::forward< const Pointer >( *other.m_pointer ) )
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
            Holder ( Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : Holder( *other.m_pointer )
            {
            }

            ~Holder ()
            {
                delete m_pointer;
            }

//            template < typename _Argument >
//            void operator = ( _Argument && argument )
//            {
//                assert( m_pointer );
//                *m_pointer = ::std::forward< _Argument >( argument );
//            }

//            void operator = ( ThisType && other )
//            {
//                ::std::swap( m_pointer, other.m_pointer );
//                return *this;
//            }

//            void operator = ( const ThisType && other )
//            {
//                assert( other.m_pointer );
//                return *this = *other.m_pointer;
//            }

//            void operator = ( ThisType & other )
//            {
//                assert( other.m_pointer );
//                return *this = *other.m_pointer;
//            }

//            void operator = ( const ThisType & other )
//            {
//                assert( other.m_pointer );
//                return *this = *other.m_pointer;
//            }

//            template < typename _OtherValue >
//            void operator = ( Holder< _OtherValue > && other )
//            {
//                ::std::swap< Pointer >( m_pointer, other.m_pointer );
//            }

//            template < typename _OtherValue >
//            void operator = ( const Holder< _OtherValue > && other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer;
//            }

//            template < typename _OtherValue >
//            void operator = ( Holder< _OtherValue > & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer;
//            }

//            template < typename _OtherValue >
//            void operator = ( const Holder< _OtherValue > & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer;
//            }

            /*!
             * Access to internal value of Holder for any king of referencies.
             */
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
