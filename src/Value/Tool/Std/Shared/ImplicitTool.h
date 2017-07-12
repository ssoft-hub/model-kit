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
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return holder;
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return ::std::forward< HolderType< _Type > && >( holder );
            }

            template < typename _Type >
            static constexpr void destroyHolder ( HolderType< _Type > & holder )
            {
                holder.reset();
            }

            template < typename _Type >
            static constexpr void guardWritableHolder ( HolderType< _Type > & holder )
            {
                if ( !!holder && !holder.unique() )
                    holder = makeHolder< _Type >( *holder.get() );
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

#include <Helper/InstanceHelper.h>
#include <Helper/ValueTrait.h>

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeap< Instance< _Value, ::Std::Shared::ImplicitTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация проверки свойства неявного обобщения значения.
 */
template < typename _Value >
struct IsImplicit< Instance< _Value, ::Std::Shared::ImplicitTool > >
: public ::std::true_type
{
};

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsOptional< Instance< _Value, ::Std::Shared::ImplicitTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация помошника для вычисления типа type для неявно обобщенных значений в виде
 * оберток Instance с размещением в куче с помощью инструмента ::Std::Shared::ImplicitTool.
 *
 * Если оборачиваемое значений уже размещается в куче, то данный вид обертки игнорируется.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Std::Shared::ImplicitTool > >
{
    using type = typename ::std::conditional< IsImplicit< _ValueType >::value,
        typename InstanceHelper< _ValueType >::type,
        Instance< _ValueType, ::Std::Shared::ImplicitTool > >::type;
};
