#pragma once
#include <memory>
#include <Helper/AccessHelper.h>

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
            using HolderType = _Type;

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeValueHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static constexpr void destroyValueHolder ( HolderType< _Type > & /*holder*/ )
            {
                // nothing to do
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
                return AccessProxy< _Type & >( holder );
            }

            template < typename _Type >
            static constexpr AccessProxy< const _Type & > getReadableProxy ( const HolderType< _Type > & holder )
            {
                return AccessProxy< const _Type & >( holder );
            }

            template < typename _Type >
            static constexpr AccessProxy< _Type && > getMovableProxy ( HolderType< _Type > && holder )
            {
                return AccessProxy< _Type && >( ::std::forward< _Type >( holder ) );
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

