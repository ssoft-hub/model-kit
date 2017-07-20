#pragma once

#include <Helper/FeatureGuard.h>

namespace Cpp
{
    namespace Inplace
    {
        /*!
         * Инструмент для формирования значения "по месту". Определение "по месту" означает,
         * что для значения не используется специальное размещение в куче и оно является
         * неотъемлемой частью пространства имен, в котором это значение определено:
         */
        struct InplaceTool
        {
            template < typename _Type >
            struct HolderType
            {
                using ThisType = HolderType< _Type >;
                using ValueType = _Type;

                ValueType m_value;

                template < typename ... _Arguments >
                constexpr HolderType ( _Arguments && ... arguments )
                : m_value( ::std::forward< _Arguments >( arguments ) ... )
                {
                }
            };

            template < typename _Type >
            //static constexpr void guardHolder ( HolderType< _Type > & )
            //static constexpr void guardHolder ( HolderType< _Type > && )
            static constexpr void guardHolder ( const HolderType< _Type > & )
            {
                // nothing to do
            }

            template < typename _Type >
            //static constexpr void unguardHolder ( HolderType< _Type > & )
            //static constexpr void unguardHolder ( HolderType< _Type > && )
            static constexpr void unguardHolder ( const HolderType< _Type > & )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr _Type & value ( HolderType< _Type > & holder )
            {
                return holder.m_value;
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return holder.m_value;
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( holder.m_value );
            }
        };
    }
}
