#pragma once

#include <memory>
#include <ModelKit/Instance/Traits.h>

namespace Std
{
    namespace Shared
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * умного указателя ::std::shared_ptr.
         */
        struct HeapTool
        {
            template < typename _Type >
            struct Holder
            {
                using ThisType = Holder< _Type >;
                using PointerType = ::std::shared_ptr< _Type >;

                PointerType m_pointer;

                template < typename ... _Arguments >
                Holder ( _Arguments && ... arguments )
                    : m_pointer( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) )
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
}
