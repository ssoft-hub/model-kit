#pragma once
#ifndef OPERATOR_ROUND_BACKETS_H

#include <type_traits>
#include <ModelKit/Instance/Traits.h>
#include <ModelKit/Instance/Access/HolderPointer.h>

//template< typename _Value, typename _Index >
//using SquareBracketsReturnType = ::std::result_of_t< decltype( &_Value::operator [] )( const _Index & ) >;

//template< typename _Value, typename _Index >
//using SquareBracketsView = Featured< SquareBracketsReturnType< _Value, _Index >, ::Cpp::Inplace::DefaultTool >;

//template< typename _Value, typename _Index >
//using SquareBracketsView = Featured< _Value, ::Cpp::Inplace::DefaultTool >;

//template < typename _Type, typename _Index >
//struct SquareBackets
//{
//    _Type & m_value;
//    _Index m_index;

//    SquareBackets ( _Type & value, const _Index & index )
//            : m_value( value )
//        , m_index( m_index )
//    {
//    }
//};


namespace Operator
{
    struct SquareBacketsTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder< _Type >;
            //using PointerType = ::std::shared_ptr< _Type >;
            using WritableHolderPointer = ::HolderPointer< ThisType &, ::Operator::SquareBacketsTool >;

            //PointerType m_pointer;

            template < typename ... _Arguments >
            Holder ( _Arguments && ... arguments )
            {
            }

            Holder ( ThisType && other )
            {
            }

            Holder ( const ThisType & other )
            {
            }

            template < typename _OtherType >
            Holder ( Holder< _OtherType > && other )
            {
            }

            template < typename _OtherType >
            Holder ( const Holder< _OtherType > & other )
            {
            }

            ~Holder ()
            {
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                return *this;
            }

            ThisType & operator = ( ThisType && other )
            {
                return *this;
            }

            ThisType & operator = ( const ThisType & other )
            {
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( Holder< _OtherType > && other )
            {
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const Holder< _OtherType > & other )
            {
                return *this;
            }
        };

        template < typename _Type >
        static constexpr void guardHolder ( Holder< _Type > & holder )
        {
            if ( !!holder.m_pointer && !holder.m_pointer.unique() )
                holder = Holder< _Type >( *holder.m_pointer.get() );
        }

        template < typename _Type >
        //static constexpr void guardHolder ( Holder< _Type > && )
        static constexpr void guardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        //static constexpr void unguardHolder ( Holder< _Type > & )
        //static constexpr void unguardHolder ( Holder< _Type > && )
        static constexpr void unguardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        static constexpr _Type & value ( Holder< _Type > & holder )
        {
            return *holder.m_pointer.get();
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return *holder.m_pointer.get();
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( *holder.m_pointer.get() );
        }
    };
}

struct SBHelper
{

};

#endif
