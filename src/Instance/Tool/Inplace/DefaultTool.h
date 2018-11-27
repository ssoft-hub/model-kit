#pragma once
#ifndef INSTANCE_TOOL_INPLACE_DEFAULT_H
#define INSTANCE_TOOL_INPLACE_DEFAULT_H

#include <utility>

namespace Inplace
{
    /*!
     * Инструмент для формирования значения "по месту". Определение "по месту" означает,
     * что для значения не используется специальное размещение в куче и оно является
     * неотъемлемой частью пространства имен, в котором это значение определено.
     */
    struct DefaultTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using Value = _Type;

            Value m_value;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
                : m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }

            Holder ( ThisType && other )
                : Holder( ::std::forward< Value >( other.m_value ) )
            {
            }

            Holder ( const ThisType && other )
                : Holder( ::std::forward< const Value >( other.m_value ) )
            {
            }

            Holder ( ThisType & other )
                : Holder( other.m_value )
            {
            }

            Holder ( const ThisType & other )
                : Holder( other.m_value )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
                : Holder( ::std::forward< typename Holder< _OtherType >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > && other )
                : Holder( ::std::forward< typename Holder< const _OtherType >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > & other )
                : Holder( other.m_value )
            {
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
                : Holder( other.m_value )
            {
            }

            ThisType & operator = ( ThisType && other )
            {
                return *this = ::std::forward< Value >( other.m_value );
            }

            ThisType & operator = ( const ThisType && other )
            {
                return *this = ::std::forward< const Value >( other.m_value );
            }

            ThisType & operator = ( ThisType & other )
            {
                return *this = other.m_value;
            }

            ThisType & operator = ( const ThisType & other )
            {
                return *this = other.m_value;
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                m_value = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType && other )
            {
                m_value = ::std::forward< const _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType & other )
            {
                m_value = other;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                m_value = other;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                return *this = ::std::forward< typename Holder< _OtherType >::Value >( other.m_value );
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > && other )
            {
                return *this = ::std::forward< typename Holder< const _OtherType >::Value >( other.m_value );
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > & other )
            {
                return *this = other.m_value;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                return *this = other.m_value;
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
            return holder.m_value;
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return holder.m_value;
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( holder.m_value );
        }

        template < typename _Type >
        static constexpr const _Type && value ( const Holder< _Type > && holder )
        {
            return ::std::forward< const _Type >( holder.m_value );
        }
    };
}

#endif
