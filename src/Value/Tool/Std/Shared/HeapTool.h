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
                return ValueHolder< _Type >( new _Type( ::std::forward< _Arguments >( arguments ) ... ) );
            }

            template < typename _Type >
            static void destroyValueHolder ( ValueHolder< _Type > & holder )
            {
                holder.reset();
            }

            template < typename _Type >
            static WritableAccessValue< _Type > writable ( ValueHolder< _Type > & holder )
            {
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
