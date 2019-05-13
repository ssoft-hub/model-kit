#pragma once
#ifndef SCL_META_COUNTER_H
#define SCL_META_COUNTER_H

#include <type_traits>

// https://stackoverflow.com/a/6174263/1190123

#define SCL_META_COUNTER_CRUMB( Tag, rank, acc ) \
    ::ScL::Meta::crumb( Tag{}, ::ScL::Meta::CounterValue< rank >(), ::ScL::Meta::CounterValue< acc >() ) \

#define SCL_META_COUNTER_VALUE( Tag ) \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 0, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 1, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 2, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 3, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 4, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 5, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 6, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 7, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 8, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 9, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 10, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 11, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 12, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 13, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 14, \
    SCL_META_COUNTER_CRUMB( Tag, 1 << 15, \
    0 ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) \

#define SCL_META_COUNTER_NEXT( Tag ) \
    namespace ScL { namespace Meta \
    { \
        constexpr ::ScL::Meta::CounterValue< SCL_META_COUNTER_VALUE( Tag ) + 1 > \
        crumb( Tag, \
            ::ScL::Meta::CounterValue< ( SCL_META_COUNTER_VALUE( Tag ) + 1 ) & ~SCL_META_COUNTER_VALUE( Tag ) >, \
            ::ScL::Meta::CounterValue< ( SCL_META_COUNTER_VALUE( Tag ) + 1 ) & SCL_META_COUNTER_VALUE( Tag ) > ) \
        { return {}; } \
    }} \

namespace ScL { namespace Meta \
{
    template< ::std::size_t _n >
    struct CounterValue : ::std::integral_constant< ::std::size_t, _n > {};
}}

namespace ScL { namespace Meta
{
    template< typename _Tag, ::std::size_t _rank, ::std::size_t _acc >
    constexpr ::ScL::Meta::CounterValue< _acc >
    crumb( _Tag,
        ::ScL::Meta::CounterValue< _rank >,
        ::ScL::Meta::CounterValue< _acc > )
    { return {}; }
}}

// http://b.atch.se/posts/non-constant-constant-expressions/
// http://b.atch.se/posts/constexpr-counter/#prereq-adl
// https://stackoverflow.com/questions/44267673/is-stateful-metaprogramming-ill-formed-yet
// http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2118

namespace ScL { namespace Meta
{
    template < typename _Tag >
    class Counter
    {
        using Value = int;

        template < Value _n >
        struct Index
        {
            friend constexpr Value adlValue ( Index< _n > );
        };

        template < Value _n >
        struct Writer
        {
            friend constexpr Value adlValue ( Index< _n > ) { return _n; }
            static constexpr Value value = _n;
        };

        template < Value _n >
        static constexpr Value increment ( float, Index< _n > )
        { return _n; }

        template < Value _n, Value = adlValue( Index< _n >{} ) >
        static constexpr Value increment ( Value, Index< _n >, Value v = increment( 0, Index< _n + 1 >{} ) )
        { return v; }

    public:
        template < Value _n = 0 >
        static constexpr Value next ( Value v = Writer< increment( 0, Index< 0 >{} ) + _n >::value )
        { return v + 1; }

        template < Value _n = 0 >
        static constexpr Value value ( Value v = Writer< increment( 0, Index< 0 >{} ) + _n - 1 >::value )
        { return v + 1; }
    };
}}

#endif
