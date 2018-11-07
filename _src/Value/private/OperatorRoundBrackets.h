#pragma once
#ifndef OPERATOR_ROUND_BACKETS_H

#include <type_traits>
#include <ModelKit/Value/InstanceTrait.h>
#include <ModelKit/Common/HolderPointer.h>

//template< typename _ValueType, typename _Index >
//using SquareBracketsReturnType = ::std::result_of_t< decltype( &_ValueType::operator [] )( const _Index & ) >;

//template< typename _ValueType, typename _Index >
//using SquareBracketsView = Instance< SquareBracketsReturnType< _ValueType, _Index >, ::Cpp::Inplace::DefaultTool >;

//template< typename _ValueType, typename _Index >
//using SquareBracketsView = Instance< _ValueType, ::Cpp::Inplace::DefaultTool >;


template < typename _Type, typename _Index >
struct SquareBacketsHelper
{
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index ) >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index & ) >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index && ) >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index & ) >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index && ) >;

    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index ) & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index & ) & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index && ) & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index & ) & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index && ) & >;

    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index ) && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index & ) && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index && ) && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index & ) && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index && ) && >;

    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index ) const & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index & ) const & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index && ) const & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index & ) const & >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index && ) const & >;

    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index ) const && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index & ) const && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( _Index && ) const && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index & ) const && >;
    //using ReturnType = ::std::result_of_t< decltype( &_Type::operator [] )( const _Index && ) const && >;
};


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
        struct HolderType
        {
            using ThisType = HolderType< _Type >;
            //using PointerType = ::std::shared_ptr< _Type >;
            using WritableHolderPointer = ::HolderPointer< ThisType &, ::Operator::SquareBacketsTool >;

            //PointerType m_pointer;

            template < typename ... _Arguments >
            HolderType ( _Arguments && ... arguments )
            {
            }

            HolderType ( ThisType && other )
            {
            }

            HolderType ( const ThisType & other )
            {
            }

            template < typename _OtherType >
            HolderType ( HolderType< _OtherType > && other )
            {
            }

            template < typename _OtherType >
            HolderType ( const HolderType< _OtherType > & other )
            {
            }

            ~HolderType ()
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
            ThisType & operator = ( HolderType< _OtherType > && other )
            {
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const HolderType< _OtherType > & other )
            {
                return *this;
            }
        };

        template < typename _Type >
        static constexpr void guardHolder ( HolderType< _Type > & holder )
        {
            if ( !!holder.m_pointer && !holder.m_pointer.unique() )
                holder = HolderType< _Type >( *holder.m_pointer.get() );
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

struct SBHelper
{

};

#endif
