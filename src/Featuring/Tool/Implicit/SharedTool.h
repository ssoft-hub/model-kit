#pragma once
#ifndef INSTANCE_TOOL_IMPLICIT_SHARED_H
#define INSTANCE_TOOL_IMPLICIT_SHARED_H

#include <cassert>
#include <memory>
#include <ModelKit/Featuring/Access/Accessing.h>
#include <ModelKit/Featuring/Access/HolderGuard.h>
#include <ModelKit/Featuring/Traits.h>
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
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            using Pointer = ::std::shared_ptr< Value >;
            using WritableGuard = ::HolderGuard< ThisType & >;

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
                : m_pointer( other.m_pointer )
            {
            }

            Holder ( ThisType & other )
                : m_pointer( other.m_pointer )
            {
            }

            Holder ( const ThisType & other )
                : m_pointer( other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_pointer( ::std::forward< typename Holder< _OtherValue >::Pointer >( other.m_pointer ) )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : m_pointer( other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : m_pointer( other.m_pointer )
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
//                WritableGuard guard( *this );
//                *m_pointer.get() = ::std::forward< _Argument >( argument );
//            }

//            void operator = ( ThisType && other )
//            {
//                m_pointer = ::std::forward< Pointer >( other.m_pointer );
//            }

//            void operator = ( const ThisType && other )
//            {
//                m_pointer = other.m_pointer;
//            }

//            void operator = ( ThisType & other )
//            {
//                m_pointer = other.m_pointer;
//            }

//            void operator = ( const ThisType & other )
//            {
//                m_pointer = other.m_pointer;
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
//                m_pointer = other.m_pointer;
//            }

//            template < typename _OtherValue >
//            void operator = ( Holder< _OtherValue > & other )
//            {
//                m_pointer = other.m_pointer;
//            }

//            template < typename _OtherValue >
//            void operator = ( const Holder< _OtherValue > & other )
//            {
//                m_pointer = other.m_pointer;
//            }

            static constexpr void guard ( ThisType & holder )
            {
//                if ( !!holder.m_pointer && !holder.m_pointer.unique() )
//                    holder = Holder< Value >( *holder.m_pointer.get() );
            }

            /*!
             * Access to internal value of Holder for any king of referencies.
             */
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
