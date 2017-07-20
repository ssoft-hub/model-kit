#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>
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
            struct HolderType
            {
                using ThisType = HolderType< _Type >;
                using PointerType = ::std::shared_ptr< _Type >;

                PointerType m_pointer;

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                : m_pointer( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) )
                {
                }

                HolderType ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
                {
                }

                HolderType ( const ThisType & other )
                : HolderType( *other.m_pointer.get() )
                {
                }

                template < typename _OtherType >
                HolderType ( HolderType< _OtherType > && other )
                : m_pointer( ::std::forward< typename HolderType< _OtherType >::PointerType >( other.m_pointer ) )
                {
                }

                template < typename _OtherType >
                HolderType ( const HolderType< _OtherType > & other )
                : HolderType( *other.m_pointer.get() )
                {
                }

                ~HolderType ()
                {
                    m_pointer.reset();
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    assert( m_pointer );
                    *m_pointer.get() = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    assert( m_pointer );
                    *m_pointer.get() = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    m_pointer = ::std::forward< PointerType >( other.m_pointer );
                    return *this;
                }

                ThisType & operator = ( const ThisType & other )
                {
                    assert( other.m_pointer );
                    return *this = *other.m_pointer.get();
                }

                template < typename _OtherType >
                ThisType & operator = ( HolderType< _OtherType > && other )
                {
                    using OtherPointerType = typename HolderType< _OtherType >::PointerType;
                    m_pointer = ::std::forward< OtherPointerType >( other.m_pointer );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const HolderType< _OtherType > & other )
                {
                    assert( other.m_pointer );
                    return *this = *other.m_pointer.get();
                }
            };

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
                return *holder.m_pointer.get();
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return *holder.m_pointer.get();
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( *holder.m_pointer.get() );
            }
        };
    }
}

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeap< Instance< _Value, ::Std::Shared::HeapTool > >
    : public ::std::true_type
{
};

/*!
 * Специализация проверки свойства опциональности.
 */
template < typename _Value >
struct IsOptional< Instance< _Value, ::Std::Shared::HeapTool > >
    : public ::std::true_type
{
};
