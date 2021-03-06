#pragma once
#ifndef SCL_INSTANCE_TOOL_INPLACE_DEFAULT_H
#define SCL_INSTANCE_TOOL_INPLACE_DEFAULT_H

#include <ModelKit/Featuring/Access.h>
#include <ModelKit/Featuring/Traits.h>
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

            constexpr Holder ( ThisType && other )
                : m_value( ::std::forward< Value >( other.m_value ) )
            {
            }

            constexpr Holder ( const ThisType && other )
                : m_value( ::std::forward< const Value >( other.m_value ) )
            {
            }

            constexpr Holder ( volatile ThisType && other )
                : m_value( ::std::forward< volatile Value >( other.m_value ) )
            {
            }

            constexpr Holder ( const volatile ThisType && other )
                : m_value( ::std::forward< const volatile Value >( other.m_value ) )
            {
            }

            constexpr Holder ( ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( const ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( volatile ThisType & other )
                : m_value( other.m_value )
            {
            }

            constexpr Holder ( const volatile ThisType & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< const _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( volatile Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< volatile _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const volatile Holder< _OtherValue > && other )
                : m_value( ::std::forward< typename Holder< const volatile _OtherValue >::Value >( other.m_value ) )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( volatile Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            template < typename _OtherValue >
            constexpr Holder ( const volatile Holder< _OtherValue > & other )
                : m_value( other.m_value )
            {
            }

            /*!
             * Access to internal value of Holder for any king of referencies.
             */
            template < typename _HolderRefer >
            static constexpr decltype(auto) value ( _HolderRefer && holder )
            {
                using HolderRefer = _HolderRefer &&;
                using ValueRefer = ::ScL::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( holder.m_value );
            }
        };
    };
}

#endif
