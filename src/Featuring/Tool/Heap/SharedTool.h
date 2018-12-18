#pragma once
#ifndef INSTANCE_TOOL_HEAP_SHARED_H
#define INSTANCE_TOOL_HEAP_SHARED_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Traits.h>
#include <cassert>
#include <memory>
#include <utility>

namespace Heap
{
    /*!
     * Инструмент для формирования значения в "куче" на основе
     * умного указателя ::std::shared_ptr.
     */
    struct SharedTool
    {
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;
            using Pointer = ::std::shared_ptr< _Value >;

            Pointer m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_pointer( ::std::make_shared< Value >( ::std::forward< _Arguments >( arguments ) ... ) )
            {
            }

            Holder ( ThisType && other )
                : m_pointer( ::std::forward< Pointer >( other.m_pointer ) )
            {
            }

            Holder ( const ThisType && other )
                : m_pointer( ::std::forward< const Pointer >( other.m_pointer ) )
            {
            }

            Holder ( ThisType & other )
                : Holder( *other.m_pointer.get() )
            {
            }

            Holder ( const ThisType & other )
                : Holder( *other.m_pointer.get() )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherValue >::Pointer >( other.m_pointer ) )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : Holder( *other.m_pointer.get() )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : Holder( *other.m_pointer.get() )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : Holder( *other.m_pointer.get() )
            {
            }

            ~Holder ()
            {
                m_pointer.reset();
            }

//            template < typename _Argument >
//            void operator = ( _Argument && argument )
//            {
//                assert( m_pointer );
//                *m_pointer.get() = ::std::forward< _Argument >( argument );
//            }

//            template < typename _OtherValue >
//            void operator = ( const _OtherValue & other )
//            {
//                assert( m_pointer );
//                *m_pointer.get() = other;
//            }

//            void operator = ( ThisType && other )
//            {
//                m_pointer = ::std::forward< Pointer >( other.m_pointer );
//            }

//            void operator = ( const ThisType && other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

//            void operator = ( ThisType & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

//            void operator = ( const ThisType & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

//            template < typename _OtherValue >
//            void operator = ( Holder< _OtherValue > && other )
//            {
//                using OtherPointer = typename Holder< _OtherValue >::Pointer;
//                m_pointer = ::std::forward< OtherPointer >( other.m_pointer );
//            }

//            template < typename _OtherValue >
//            void operator = ( const Holder< _OtherValue > && other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

//            template < typename _OtherValue >
//            void operator = ( Holder< _OtherValue > & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

//            template < typename _OtherValue >
//            void operator = ( const Holder< _OtherValue > & other )
//            {
//                assert( other.m_pointer );
//                *this = *other.m_pointer.get();
//            }

            //! Access to internal value of Holder for any king of referencies.
            template < typename _Refer >
            static constexpr decltype(auto) value ( _Refer && holder )
            {
                using HolderRefer = _Refer &&;
                using ValueRefer = ::SimilarRefer< _Value, HolderRefer >;
                // NOTE: Functionality ::std::shared_ptr has a limitation for volatile case.
                return ::std::forward< ValueRefer >( *holder.m_pointer.get() );
            }
        };
    };
}

#endif
