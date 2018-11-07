#pragma once

#include <assert.h>
#include <ModelKit/Common/InitializeType.h>
#include <ModelKit/Value/InstanceTrait.h>

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

                PointerType m_pointer;

                constexpr HolderType ( InitializeType )
                    : m_pointer()
                {
                }

                template < typename ... _Arguments >
                HolderType ( _Arguments && ... arguments )
                    : m_pointer( new _Type( ::std::forward< _Arguments >( arguments ) ... ) )
                {
                }

                HolderType ( ThisType && other )
                    : m_pointer( ::std::forward< PointerType >( other.m_pointer ) )
                {
                    other.m_pointer = nullptr;
                }

                HolderType ( const ThisType & other )
                : HolderType( *other.m_pointer )
                {
                }

                template < typename _OtherType >
                HolderType ( HolderType< _OtherType > && other )
                    : m_pointer( ::std::forward< typename HolderType< _OtherType >::PointerType >( other.m_pointer ) )
                {
                    other.m_pointer = nullptr;
                }

                template < typename _OtherType >
                HolderType ( const HolderType< _OtherType > & other )
                : HolderType( *other.m_pointer )
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
                    *m_pointer = ::std::forward< _OtherType >( other );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const _OtherType & other )
                {
                    assert( m_pointer );
                    *m_pointer = other;
                    return *this;
                }

                ThisType & operator = ( ThisType && other )
                {
                    ::std::swap( m_pointer, other.m_pointer );
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
                    ::std::swap< PointerType >( m_pointer, other.m_pointer );
                    return *this;
                }

                template < typename _OtherType >
                ThisType & operator = ( const HolderType< _OtherType > & other )
                {
                    assert( other.m_pointer );
                    return *this = *other.m_pointer;
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
