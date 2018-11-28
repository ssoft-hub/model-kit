#pragma once
#ifndef INSTANCE_TOOL_INPLACE_UNINITIALIZED_H
#define INSTANCE_TOOL_INPLACE_UNINITIALIZED_H

#include <utility>

namespace Inplace
{
    /*!
     * Инструмент для формирования значения "по месту". Определение "по месту" означает,
     * что для значения не используется специальное размещение в куче и оно является
     * неотъемлемой частью пространства имен, в котором это значение определено:
     */
    struct UninitializedTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            using Value = _Type;

            Value m_value;

            constexpr Holder ()
            {
            }

            template < typename ... _Arguments >
            constexpr Holder ( _Arguments && ... arguments )
                : m_value( ::std::forward< _Arguments >( arguments ) ... )
            {
            }

            Holder ( ThisType && other )
                : Holder( ::std::forward< Value >( other.m_value ) )
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
            Holder ( const Holder< _OtherType > & other )
                : Holder( other.m_value )
            {
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                m_value = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                m_value = other;
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                return *this = ::std::forward< Value >( other.m_value );
            }

            ThisType & operator = ( const ThisType & other )
            {
                return *this = other.m_value;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                return *this = ::std::forward< typename Holder< _OtherType >::Value >( other.m_value );
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                return *this = other.m_value;
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
                return ::std::forward< _Type && >( holder.m_value );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return ::std::forward< _Type & >( holder.m_value );
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return ::std::forward< _Type & >( holder.m_value );
            }
        };
    };
}

#endif
