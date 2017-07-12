#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>
#include <memory>

namespace Std
{
    namespace Shared
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * умного указателя ::std::shared_ptr.
         */
        struct HeapTool
        {
            template < typename _Type >
            using HolderType = ::std::shared_ptr< _Type >;

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return makeHolder< _Type >( *holder.get() );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return makeHolder< _Type >( *holder.get() );
            }

            template < typename _Type >
            static constexpr void destroyHolder ( HolderType< _Type > & holder )
            {
                holder.reset();
            }

            template < typename _Type >
            static constexpr void guardWritableHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardWritableHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardReadableHolder ( const HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardReadableHolder ( const HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void guardMovableHolder ( HolderType< _Type > && /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr void unguardMovableHolder ( HolderType< _Type > && /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static constexpr FeatureGuard< _Type & > writableValueGuard ( HolderType< _Type > & holder )
            {
                return FeatureGuard< _Type & >( *holder.get() );
            }

            template < typename _Type >
            static constexpr FeatureGuard< const _Type & > readableValueGuard ( const HolderType< _Type > & holder )
            {
                return FeatureGuard< const _Type & >( *holder.get() );
            }

            template < typename _Type >
            static constexpr FeatureGuard< _Type && > movableValueGuard ( HolderType< _Type > && holder )
            {
                return FeatureGuard< _Type && >( ::std::forward< _Type >( *holder.get() ) );
            }
        };
    }
}

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeap< Instance< _Value, ::Std::Shared::HeapTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация проверки свойства опциональности.
 */
template < typename _Value >
struct IsOptional< Instance< _Value, ::Std::Shared::HeapTool > >
    : public ::std::true_type
{
};
