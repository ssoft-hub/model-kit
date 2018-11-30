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
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

            Value m_value;

            template < typename ... _Arguments >
            constexpr Holder ( _Arguments && ... arguments )
                : m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }

            Holder ( ThisType && other )
                : m_value( ::std::forward< Value >( other.m_value ) )
            {
            }

            Holder ( const ThisType && other )
                : m_value( ::std::forward< const Value >( other.m_value ) )
            {
            }

            Holder ( ThisType & other )
                : m_value( other.m_value )
            {
            }

            Holder ( const ThisType & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< const _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            Holder ( Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            Holder ( const Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _Argument >
            void operator = ( _Argument && argument )
            {
                m_value = ::std::forward< _Argument >( argument );
            }

            void operator = ( ThisType && other )
            {
                m_value = ::std::forward< Value >( other.m_value );
            }

            void operator = ( const ThisType && other )
            {
                m_value = ::std::forward< const Value >( other.m_value );
            }

            void operator = ( ThisType & other )
            {
                *this = other.m_value;
            }

            void operator = ( const ThisType & other )
            {
                return *this = other.m_value;
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< const _OtherValue >::Value >( other.m_value );
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > & other )
            {
                *this = other.m_value;
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > & other )
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

            static constexpr Value && value ( ThisType && holder )
            {
                return ::std::forward< Value >( holder.m_value );
            }

            static constexpr const Value && value ( const ThisType && holder )
            {
                return ::std::forward< const Value >( holder.m_value );
            }

            static constexpr Value & value ( ThisType & holder )
            {
                return holder.m_value;
            }

            static constexpr const Value & value ( const ThisType & holder )
            {
                return holder.m_value;
            }
        };
    };
}

#endif
