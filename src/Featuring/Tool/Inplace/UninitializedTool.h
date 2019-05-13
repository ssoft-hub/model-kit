#pragma once
#ifndef SCL_INSTANCE_TOOL_INPLACE_UNINITIALIZED_H
#define SCL_INSTANCE_TOOL_INPLACE_UNINITIALIZED_H

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
