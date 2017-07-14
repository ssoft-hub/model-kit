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

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return makeHolder< _Type >( holder.m_value );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return makeHolder< _Type >( ::std::forward< _Type && >( holder.m_value ) );
            }

            template < typename _Type >
            static constexpr void destroyHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardHolder ( const HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardHolder ( const HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardHolder ( HolderType< _Type > && /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardHolder ( HolderType< _Type > && /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr FeatureGuard< _Type & > featureGuard ( HolderType< _Type > & holder )
            {
                return FeatureGuard< _Type & >( holder.m_value );
            }

            template < typename _Type >
            static constexpr FeatureGuard< const _Type & > featureGuard ( const HolderType< _Type > & holder )
            {
                return FeatureGuard< const _Type & >( holder.m_value );
            }

            template < typename _Type >
            static constexpr FeatureGuard< _Type && > featureGuard ( HolderType< _Type > && holder )
            {
                return FeatureGuard< _Type && >( ::std::forward< _Type >( holder.m_value ) );
            }
        };
    }
}
