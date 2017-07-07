#pragma once

namespace Cpp
{
    namespace Raw
    {
        /*!
         * Инструмент для формирования значения в "куче" на основе
         * "голого" raw указателя.
         */
        struct HeapTool
        {
            template < typename _Type >
            using HolderType = _Type *;

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( new _Type( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static constexpr void destroyValueHolder ( HolderType< _Type > & holder )
            {
                delete holder;
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
            static constexpr FeatureGuard< _Type & > getWritableGuard ( HolderType< _Type > & holder )
            {
                return FeatureGuard< _Type & >( *holder );
            }

            template < typename _Type >
            static constexpr FeatureGuard< const _Type & > getReadableGuard ( const HolderType< _Type > & holder )
            {
                return FeatureGuard< const _Type & >( *holder );
            }

            template < typename _Type >
            static constexpr FeatureGuard< _Type && > getMovableGuard ( HolderType< _Type > && holder )
            {
                return FeatureGuard< _Type && >( ::std::forward< _Type >( *holder ) );
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
struct IsHeapInstance< Instance< _Value, ::Cpp::Raw::HeapTool > >
{
    static constexpr bool result = true;
};

/*!
 * Специализация помошника для вычисления типа InstanceType для значений в виде
 * оберток Instance с размещением в куче с помощью инструмента ::Cpp::Raw::HeapTool.
 *
 * Если оборачиваемое значений уже размещается в куче, то данный вид обертки игнорируется.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Cpp::Raw::HeapTool > >
{
    using InstanceType = typename TypeHelper< IsHeapInstance< _ValueType >::result,
        typename InstanceHelper< _ValueType >::InstanceType,
        Instance< _ValueType, ::Cpp::Raw::HeapTool > >::Type;
};
