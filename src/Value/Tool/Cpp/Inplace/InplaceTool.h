#pragma once
#include <memory>

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
            using Value = _Type;

            template < typename _Type >
            using ValueHolder = _Type;

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

            template < typename _Type, typename ... _Arguments >
            static /*constexpr*/ ValueHolder< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return ValueHolder< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static /*constexpr*/ void destroyValueHolder ( ValueHolder< _Type > & /*holder*/ )
            {
                // nothing to do
            }

            template < typename _Type >
            static WritableAccessValue< _Type > writable ( ValueHolder< _Type > & holder )
            {
                return holder;
            }

            template < typename _Type >
            static ReadableAccessValue< _Type > readable ( const ValueHolder< _Type > & holder )
            {
                return holder;
            }

            template < typename _Type >
            static MovableAccessValue< _Type > movable ( ValueHolder< _Type > && holder )
            {
                return ::std::forward< Value< _Type > >( holder );
            }
        };
    }
}

#include <Helper/InstanceHelper.h>

/*!
 * Специализация помошника для вычисления типа InstanceType для значений в виде
 * оберток Instance с размещением по месту с помощью инструмента ::Cpp::Inplace::InplaceTool.
 * Инструмент ::Cpp::Inplace::InplaceTool предназначен для предотвращения прямого
 * доступа к значениям.
 */
template < typename _ValueType >
struct InstanceHelper< Instance< _ValueType, ::Cpp::Inplace::InplaceTool > >
{
    using InstanceType = typename InstanceHelper< _ValueType >::InstanceType;
};

