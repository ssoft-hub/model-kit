#pragma once
#include <memory>

namespace Std
{
    namespace Shared
    {
        /*!
         * Инструмент для формирования неявно обобщенного значения на основе
         * умного указателя ::std::shared_ptr.
         * Инструмент реализует технику ленивых вычислений, когда копирование
         * экземпляра значения происходит только в момент доступа к неконстантному
         * экземпляру.
         */
        struct ImplicitTool
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
                if ( !holder.unique() )
                    holder = makeValueHolder< _Type >( *holder.get() );
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
struct IsHeapInstance< Instance< _Value, ::Std::Shared::ImplicitTool > >
{
    static constexpr bool result = true;
};

/*!
 * Специализация проверки свойства неявного обобщения значения.
 */
template < typename _Value >
struct IsImplicitInstance< Instance< _Value, ::Std::Shared::ImplicitTool > >
{
    static constexpr bool result = true;
};

/*!
 * Специализация помошника для вычисления типа InstanceType для неявно обобщенных значений в виде
 * оберток Instance с размещением в куче с помощью инструмента ::Std::Shared::ImplicitTool.
 *
 * Если оборачиваемое значений уже размещается в куче, то данный вид обертки игнорируется.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Std::Shared::ImplicitTool > >
{
    using InstanceType = typename TypeHelper< IsImplicitInstance< _ValueType >::result,
        typename InstanceHelper< _ValueType >::InstanceType,
        Instance< _ValueType, ::Std::Shared::ImplicitTool > >::Type;
};
