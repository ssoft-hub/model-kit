#pragma once
#ifndef SCL_META_SEQUENCE_H

#include <cstddef>
#include "Counter.h"
#include "TypeOf.h"

namespace ScL { namespace Meta
{
    template < typename ... > struct Sequence {};
        //!< Sequence of types (heterogeneous ordered collection of types)

    namespace Detail
        //!< Specilizing TypeOf for Sequence
    {
        template < typename _Type >
        struct TypeOf< Sequence< _Type > > { using Type = _Type; };
        template < typename _Type >
        struct TypeOf< const Sequence< _Type > > { using Type = _Type; };
    }

    template < typename... _Types >
    constexpr bool operator == ( Sequence< _Types ... >, Sequence< _Types ... > ) { return true; }

    template < typename... _Types >
    constexpr bool operator != ( Sequence< _Types ... >, Sequence< _Types ... > ) { return false; }

    template < typename ... _LeftTypes, typename ... _RightTypes >
    constexpr bool operator == ( Sequence< _LeftTypes ... >, Sequence< _RightTypes ... > ) { return false; }

    template < typename ... _LeftTypes, typename ... _RightTypes >
    constexpr bool operator != ( Sequence< _LeftTypes ... >, Sequence< _RightTypes ... > ) { return true; }

    template < typename ... _Types >
    constexpr auto count ( Sequence< _Types ... > ) { return sizeof...( _Types ); }
        //!< Count of types in Sequence.

    template < typename ... _Types >
    constexpr bool isEmpty ( Sequence< _Types ... > ) { return count( Sequence< _Types ... >{} ) == 0; }
        //!< Return true if sequence is empty.

    namespace Detail
    {
        template < typename ... _Types >
        constexpr Sequence< _Types ... > reverse ( Sequence<>, Sequence< _Types ... > ) { return {}; }

        template < typename _Type, typename ... _LeftTypes, typename ... _RightTypes >
        constexpr auto reverse ( Sequence< _Type, _LeftTypes ... >, Sequence< _RightTypes ... > )
        { return reverse( Sequence< _LeftTypes ... >{}, Sequence< _Type, _RightTypes ... >{} ); }
    }

    template <typename ... _Types >
    constexpr auto reverse ( Sequence< _Types ... > sequence ) { return Detail::reverse( sequence, {} ); }

    template < typename _Type, typename ... _Types >
    constexpr auto first ( Sequence< _Type, _Types ... > ) { return Sequence< _Type >{}; }
        //!< Return sequence with first element only.

    template < typename ... _Types >
    constexpr auto last ( Sequence< _Types ... > sequence ) { return first( reverse( sequence ) ); }
    //!< Return sequence with last element only.

    template < typename _Type, typename ... _Types >
    constexpr auto removeFirst ( Sequence< _Type, _Types ... > ) { return Sequence< _Types ... >{}; }
        //!< Return tail of sequence without first element.

    template < typename ... _Types >
    constexpr auto removeLast ( Sequence< _Types ... > sequence ) { return reverse( removeFirst( reverse( sequence ) ) ); }
        //!< Return head of sequence without last element.

    template < typename ... _Types, typename ... _OtherTypes >
    constexpr auto append ( Sequence< _Types ... >, Sequence< _OtherTypes ... > ) { return Sequence< _Types ... , _OtherTypes ... >{}; }
        //!< Return ...

    template < typename ... _Types, typename ... _OtherTypes >
    constexpr auto prepend ( Sequence< _Types ... >, Sequence< _OtherTypes ... > ) { return Sequence< _OtherTypes ..., _Types ... >{}; };
        //!< Return ...

    template < typename ... _Types, typename _OtherTypes /*...*/ >
    constexpr bool contains ( Sequence< _Types ... >, Sequence< _OtherTypes /*...*/ > )
    {
#if __cplusplus >= 201703L
        return ( ... || Sequence< _OtherTypes >{} == Sequence< _Types >{} );
#else
        bool results[] = { Sequence< _OtherTypes /*...*/ >{} == Sequence< _Types >{} ... };
        bool result = false;
        for ( bool value : results ) { result |= value; }
        return result;
#endif
    }

    template < typename ... _Types, typename _OtherTypes /*...*/ >
    constexpr auto indexOf ( Sequence< _Types ... >, Sequence< _OtherTypes /*...*/ > )
    {
        bool results[] = { Sequence< _OtherTypes /*...*/ >{} == Sequence< _Types >{} ... };
#if __cplusplus >= 201703L
        return ::std::find( results, results + sizeof... ( _Types ), true ) - results;
#else
        for ( ::std::size_t i = 0; i < sizeof...( _Types ); ++i )
            if ( results[i] )
                return i;
        return sizeof...( _Types );
#endif
    }

    template < typename ... _LeftTypes, typename ... _RightTypes >
    constexpr Sequence< _LeftTypes ... , _RightTypes ... > operator + ( Sequence< _LeftTypes ... >, Sequence< _RightTypes ... > ) { return {}; };
}}

namespace ScL { namespace Meta
{
    namespace Detail
    {
        template < typename _Type >
        class Inherited;

        template < typename _Type >
        class Inherited< Sequence< _Type > >
            : public _Type
        {};

        template < typename _Type, typename ... _Types >
        class Inherited< Sequence< _Type, _Types ... > >
            : public _Type
            , public Inherited< Sequence< _Types ... > >
        {};
    }
    template < typename _Type >
    using Inherited = Detail::Inherited< _Type >;
}}

namespace ScL { namespace Meta {
    template < typename _Tag, int _N >
    struct Types { static constexpr auto sequence () { return ::ScL::Meta::Sequence<>{}; } };
}}

#define SCL_META_SEQUENCE_APPEND( Tag, Type ) \
    SCL_META_COUNTER_NEXT( Tag ) \
    namespace ScL { namespace Meta { \
        template <> \
        struct Types< Tag, SCL_META_COUNTER_VALUE( Tag ) > \
        { \
            static constexpr auto sequence () { return append( Types< Tag, SCL_META_COUNTER_VALUE( Tag ) - 1 >::sequence(), Sequence< Type >{} ); } \
        }; \
    }} \

#define SCL_META_SEQUENCE( Tag ) \
    decltype( ::ScL::Meta::Types< Tag, SCL_META_COUNTER_VALUE( Tag ) >::sequence() )

#endif
