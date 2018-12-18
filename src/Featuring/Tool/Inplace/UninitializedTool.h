#pragma once
#ifndef INSTANCE_TOOL_INPLACE_UNINITIALIZED_H
#define INSTANCE_TOOL_INPLACE_UNINITIALIZED_H

#include <utility>

namespace Inplace
{
    /*!
     * Инструмент для формирования значения "по месту", которое в случае POD типов
     * по умолчанию не инициализировано (содержит мусор).
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

            // nothing to do
            template < typename _Refer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Refer > >::value > >
            static constexpr void guard ( _Refer && ) {}

            // nothing to do
            template < typename _Refer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Refer > >::value > >
            static constexpr void unguard ( _Refer && ) {}

            // value proxying
            template < typename _Refer,
                typename = ::std::enable_if_t< ::std::is_same< ThisType, ::std::decay_t< _Refer > >::value > >
            static constexpr decltype(auto) value ( _Refer && holder )
            {
                using HolderRefer = _Refer &&;
                using ValueRefer = ::SimilarRefer< Value, HolderRefer >;
                return ::std::forward< ValueRefer >( ::std::forward< HolderRefer >( holder ).m_value );
            }
        };
    };
}

#endif
