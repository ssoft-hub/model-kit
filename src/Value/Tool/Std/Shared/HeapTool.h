#pragma once
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
            static constexpr HolderType< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static constexpr void destroyValueHolder ( HolderType< _Type > & holder )
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
            static constexpr AccessProxy< _Type & > getWritableProxy ( HolderType< _Type > & holder )
            {
                return AccessProxy< _Type & >( *holder.get() );
            }

            template < typename _Type >
            static constexpr AccessProxy< const _Type & > getReadableProxy ( const HolderType< _Type > & holder )
            {
                return AccessProxy< const _Type & >( *holder.get() );
            }

            template < typename _Type >
            static constexpr AccessProxy< _Type && > getMovableProxy ( HolderType< _Type > && holder )
            {
                return AccessProxy< _Type && >( ::std::forward< _Type >( *holder.get() ) );
            }
        };
    }
}

#include <Helper/InstanceHelper.h>
#include <Helper/TypeHelper.h>

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeapInstance< Instance< _Value, ::Std::Shared::HeapTool > >
{
    static constexpr bool result = true;
};

/*!
 * Специализация помошника для вычисления типа InstanceType для значений в виде
 * оберток Instance с размещением в куче с помощью инструмента ::Std::Shared::HeapTool.
 *
 * Если оборачиваемое значений уже размещается в куче, то данный вид обертки игнорируется.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Std::Shared::HeapTool > >
{

    using InstanceType = typename TypeHelper< IsHeapInstance< _ValueType >::result,
        typename InstanceHelper< _ValueType >::InstanceType,
        Instance< _ValueType, ::Std::Shared::HeapTool > >::Type;
};
