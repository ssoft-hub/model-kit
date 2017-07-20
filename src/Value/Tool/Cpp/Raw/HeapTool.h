#pragma once

#include <Helper/FeatureGuard.h>
#include <Helper/ValueTrait.h>

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
            struct HolderType
            {
                using ThisType = HolderType< _Type >;
                using PointerType = _Type *;

                struct HolderGuard
                {
                    ThisType & m_holder;
                    HolderGuard ( ThisType & holder ) : m_holder( holder ) { guardHolder( m_holder ); }
                    ~HolderGuard () { unguardHolder( m_holder ); }
                };

                PointerType m_pointer;

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                : m_pointer( new _Type( ::std::forward< _Arguments >( arguments ) ... ) )
                {
                }

                HolderType ( ThisType && other )
                : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
                {
                }

                ~HolderType ()
                {
                    delete m_pointer;
                }

                template < typename _OtherType >
                ThisType & operator = ( _OtherType && other )
                {
                    assert( m_pointer );
                    HolderGuard holder_guard( *this );
                    *m_pointer = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    assert( m_pointer );
                    HolderGuard holder_guard( *this );
                    *m_pointer = other;
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
                    return *this = *other.m_pointer;
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
                    return *this = *other.m_pointer;
                }
            };

            template < typename _Type, typename ... _Arguments >
            static constexpr HolderType< _Type > makeHolder ( _Arguments && ... arguments )
            {
                return HolderType< _Type >( ::std::forward< _Arguments >( arguments ) ... );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > copyHolder ( const HolderType< _Type > & holder )
            {
                return makeHolder< _Type >( *holder.m_pointer );
            }

            template < typename _Type >
            static constexpr HolderType< _Type > moveHolder ( HolderType< _Type > && holder )
            {
                return makeHolder< _Type >( ::std::forward< _Type >( *holder.m_pointer ) );
            }

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
                return *holder.m_pointer;
            }

            template < typename _Type >
            static constexpr const _Type & value ( const HolderType< _Type > & holder )
            {
                return *holder.m_pointer;
            }

            template < typename _Type >
            static constexpr _Type && value ( HolderType< _Type > && holder )
            {
                return ::std::forward< _Type >( *holder.m_pointer );
            }
        };
    }
}

/*!
 * Специализация проверки свойства размещения значения в куче.
 */
template < typename _Value >
struct IsHeap< Instance< _Value, ::Cpp::Raw::HeapTool > >
    : public ::std::true_type
{
};
