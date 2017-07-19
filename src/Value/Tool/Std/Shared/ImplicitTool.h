#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>
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
            static constexpr void guardHolder ( HolderType< _Type > & holder )
            {
                if ( !!holder && !holder.unique() )
                    holder = makeHolder< _Type >( *holder.get() );
            }

            template < typename _Type >
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
 * Специализация проверки свойства опциональности.
 */
template < typename _Value >
struct IsOptional< Instance< _Value, ::Std::Shared::ImplicitTool > >
    : public ::std::true_type
{
};
