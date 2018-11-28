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

            template < typename _OtherType >
            void operator = ( _OtherType && other )
            {
                m_value = ::std::forward< _OtherType >( other );
            }

            //void operator = ( ThisType && other );
            //void operator = ( const ThisType && other );
            void operator = ( ThisType & other )
            {
                *this = other.m_value;
            }

            void operator = ( const ThisType & other )
            {
                return *this = other.m_value;
            }

            template < typename _OtherType >
            void operator = ( Holder< _OtherType > && other )
            {
                *this = ::std::forward< typename Holder< _OtherType >::Value >( other.m_value );
            }

            template < typename _OtherType >
            void operator = ( const Holder< _OtherType > && other )
            {
                *this = ::std::forward< typename Holder< const _OtherType >::Value >( other.m_value );
            }

            template < typename _OtherType >
            void operator = ( Holder< _OtherType > & other )
            {
                *this = other.m_value;
            }

            template < typename _OtherType >
            void operator = ( const Holder< _OtherType > & other )
            {
                *this = other.m_value;
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
                return ::std::forward< _Type && >( holder.m_value );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type && >( holder.m_value );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return ::std::forward< _Type & >( holder.m_value );
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return ::std::forward< const _Type & >( holder.m_value );
            }
        };
    };
}

#endif
