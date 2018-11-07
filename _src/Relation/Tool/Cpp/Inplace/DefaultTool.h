#pragma once

#include <utility>

namespace Relation
{

    namespace Cpp
    {
        namespace Inplace
        {
            /*!
             * Инструмент для формирования значения "по месту". Определение "по месту" означает,
             * что для значения не используется специальное размещение в куче и оно является
             * неотъемлемой частью пространства имен, в котором это значение определено:
             */
            struct DefaultTool
            {
                template < typename _Type >
                struct HolderType
                {
                    using ThisType = HolderType< _Type >;
                    using ValueType = _Type;

                    ValueType m_value;

                    template < typename ... _Arguments >
                    constexpr HolderType ( _Arguments && ... arguments )
                        : m_value( ::std::forward< _Arguments >( arguments ) ... )
                    {
                    }

                    HolderType ( ThisType && other )
                    : HolderType( ::std::forward< ValueType >( other.m_value ) )
                    {
                    }

                    HolderType ( const ThisType & other )
                    : HolderType( other.m_value )
                    {
                    }

                    template < typename _OtherType >
                    HolderType ( HolderType< _OtherType > && other )
                    : HolderType( ::std::forward< typename HolderType< _OtherType >::ValueType >( other.m_value ) )
                    {
                    }

                    template < typename _OtherType >
                    HolderType ( const HolderType< _OtherType > & other )
                    : HolderType( other.m_value )
                    {
                    }

                    template < typename _OtherType >
                    ThisType & operator = ( _OtherType && other )
                    {
                        m_value = ::std::forward< _OtherType >( other );
                        return *this;
                    }

                    template < typename _OtherType >
                    ThisType & operator = ( const _OtherType & other )
                    {
                        m_value = other;
                        return *this;
                    }

                    ThisType & operator = ( ThisType && other )
                    {
                        return *this = ::std::forward< ValueType >( other.m_value );
                    }

                    ThisType & operator = ( const ThisType & other )
                    {
                        return *this = other.m_value;
                    }

                    template < typename _OtherType >
                    ThisType & operator = ( HolderType< _OtherType > && other )
                    {
                        return *this = ::std::forward< typename HolderType< _OtherType >::ValueType >( other.m_value );
                    }

                    template < typename _OtherType >
                    ThisType & operator = ( const HolderType< _OtherType > & other )
                    {
                        return *this = other.m_value;
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
                    return holder.m_value;
                }

                template < typename _Type >
                static constexpr const _Type & value ( const HolderType< _Type > & holder )
                {
                    return holder.m_value;
                }

                template < typename _Type >
                static constexpr _Type && value ( HolderType< _Type > && holder )
                {
                    return ::std::forward< _Type >( holder.m_value );
                }
            };
        }
    }
}
