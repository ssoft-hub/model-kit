#pragma once

#include <assert.h>

namespace Cpp
{
    namespace Raw
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * "голого" raw указателя.
         */
        struct HeapTool
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

                template < typename _OtherType >
                Holder ( const Holder< _OtherType > & other )
                : Holder( *other.m_pointer )
                {
                }

                ~Holder ()
                {
                    delete m_pointer;
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    assert( m_pointer );
                    *m_pointer = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    assert( m_pointer );
                    *m_pointer = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    ::std::swap( m_pointer, other.m_pointer );
                    return *this;
                }

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
            };

            template < typename _Type >
            //static constexpr void guardHolder ( Holder< _Type > & )
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
                return *holder.m_pointer;
            }

            template < typename _Type >
            static constexpr const _Type & value ( const Holder< _Type > & holder )
            {
                return *holder.m_pointer;
            }

            template < typename _Type >
            static constexpr _Type && value ( Holder< _Type > && holder )
            {
                return ::std::forward< _Type >( *holder.m_pointer );
            }
        };
    }
}
