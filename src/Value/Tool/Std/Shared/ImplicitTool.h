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
            using Value = _Type;

            template < typename _Type >
            using WritableProxy = _Type *;

            template < typename _Type >
            using ReadableProxy = const _Type *;

            template < typename _Type >
            using WritableAccessValue = _Type &;

            template < typename _Type >
            using ReadableAccessValue = const _Type &;

            template < typename _Type >
            using MovableAccessValue = _Type &&;

            template < typename _Type >
            using ValueHolder = ::std::shared_ptr< Value< _Type > >;

            template < typename _Type, typename ... _Arguments >
            static ValueHolder< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static void destroyValueHolder ( ValueHolder< _Type > & holder )
            {
                holder.reset();
            }

            template < typename _Type >
            static WritableAccessValue< _Type > writable ( ValueHolder< _Type > & holder )
            {
                if ( !holder.unique() )
                    holder = makeValueHolder< _Type >( *holder.get() );
                return *holder.get();
            }

            template < typename _Type >
            static ReadableAccessValue< _Type > readable ( const ValueHolder< _Type > & holder )
            {
                return *holder.get();
            }

            template < typename _Type >
            static MovableAccessValue< _Type > movable ( ValueHolder< _Type > && holder )
            {
                return ::std::forward< Value< _Type > >( *holder.get() );
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
