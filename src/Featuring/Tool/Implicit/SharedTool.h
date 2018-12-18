#pragma once
#ifndef INSTANCE_TOOL_IMPLICIT_SHARED_H
#define INSTANCE_TOOL_IMPLICIT_SHARED_H

#include <cassert>
#include <memory>
#include <ModelKit/Featuring/Access/HolderGuard.h>
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

            static constexpr void guard ( ThisType && ) {}
            static constexpr void guard ( const ThisType && ) {}
            static constexpr void guard ( volatile ThisType && ) {}
            static constexpr void guard ( const volatile ThisType && ) {}
            static constexpr void guard ( ThisType & holder )
            {
//                if ( !!holder.m_pointer && !holder.m_pointer.unique() )
//                    holder = Holder< Value >( *holder.m_pointer.get() );
            }
            static constexpr void guard ( const ThisType & ) {}
            static constexpr void guard ( volatile ThisType & holder ) {}
            static constexpr void guard ( const volatile ThisType & ) {}

            static constexpr void unguard ( ThisType && ) {}
            static constexpr void unguard ( const ThisType && ) {}
            static constexpr void unguard ( volatile ThisType && ) {}
            static constexpr void unguard ( const volatile ThisType && ) {}
            static constexpr void unguard ( ThisType & holder ) {}
            static constexpr void unguard ( const ThisType & ) {}
            static constexpr void unguard ( volatile ThisType & holder ) {}
            static constexpr void unguard ( const volatile ThisType & ) {}


            static constexpr Value && value ( ThisType && holder )
            {
                return ::std::forward< Value >( *holder.m_pointer.get() );
            }

            static constexpr const Value && value ( const ThisType && holder )
            {
                return ::std::forward< const Value >( *holder.m_pointer.get() );
            }

            static constexpr volatile Value && value ( volatile ThisType && holder )
            {
                return ::std::forward< volatile Value >( *const_cast< Pointer & >( holder.m_pointer ).get() );
            }

            static constexpr const volatile Value && value ( const volatile ThisType && holder )
            {
                return ::std::forward< const Value >( *const_cast< const Pointer & >( holder.m_pointer ).get() );
            }

            static constexpr Value & value ( ThisType & holder )
            {
                return *holder.m_pointer.get();
            }

            static constexpr const Value & value ( const ThisType & holder )
            {
                return *holder.m_pointer.get();
            }

            static constexpr volatile Value & value ( volatile ThisType & holder )
            {
                return *const_cast< Pointer & >( holder.m_pointer ).get();
            }

            static constexpr const volatile Value & value ( const volatile ThisType & holder )
            {
                return reinterpret_cast< const volatile Value & >( *(holder.m_pointer.get()) );
            }
        };
    };
}

#endif
