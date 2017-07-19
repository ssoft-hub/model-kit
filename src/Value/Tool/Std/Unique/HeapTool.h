#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>
#include <memory>

namespace Std
{
    namespace Unique
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * умного указателя ::std::unique_ptr.
         */
        struct HeapTool
        {
            template < typename _Type >
            using HolderType = ::std::unique_ptr< _Type >;

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( new _Type( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return makeHolder< _Type >( *holder.get() );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return makeHolder< _Type >( ::std::forward< _Type >( *holder.get() ) );
            }

            template < typename _Type >
            static constexpr void destroyHolder ( HolderType< _Type > & holder )
            {
                holder.reset();
            }

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
                return *holder.get();
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return *holder.get();
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( *holder.get() );
            }
        };
    }
}

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeap< Instance< _Value, ::Std::Unique::HeapTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация проверки свойства опциональности.
 */
template < typename _Value >
struct IsOptional< Instance< _Value, ::Std::Unique::HeapTool > >
    : public ::std::true_type
{
};
