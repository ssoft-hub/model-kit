#pragma once
#ifndef INSTANCE_TOOL_INPLACE_UNINITIALIZED_H
#define INSTANCE_TOOL_INPLACE_UNINITIALIZED_H

#include <ModelKit/Featuring/Access.h>
#include <ModelKit/Featuring/Traits.h>
#include <utility>

namespace Inplace
{
    /*!
     * Инструмент для формирования значения "по месту", которое в случае POD типов
     * по умолчанию не инициализировано (содержит мусор).
     */
    struct UninitializedTool
    {
        template < typename _Value >
        struct Holder
        {
            using ThisType = Holder< _Value >;
            using Value = _Value;

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

            /*!
             * Access to internal value of Holder for any king of referencies.
             */
            template < typename _Refer >
            static constexpr decltype(auto) value ( _Refer && holder )
            {
                using HolderRefer = _Refer &&;
                using ValueRefer = ::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( holder.m_value );
            }
        };
    };
}

#endif
