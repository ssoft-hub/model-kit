#include <iostream>
#include <typeinfo>
#include <ModelKit/Meta.h>

#if defined( __GNUC__ )
#   include <cxxabi.h>
#endif

template < typename _Type >
void printTypeOf ()
{
#if defined( __GNUC__ )
    int status = 0;
    char * realname = abi::__cxa_demangle( typeid( _Type ).name(), nullptr, nullptr, &status );
#elif defined( _MSC_VER )
    const char * realname = typeid( _Type ).name();
#endif

    ::std::cout
        << realname;

    if ( ::std::is_const< _Type >::value )
        ::std::cout
            << " const";

    if ( ::std::is_lvalue_reference< _Type >::value )
        ::std::cout
            << " &";

    if ( ::std::is_rvalue_reference< _Type >::value )
        ::std::cout
            << " &&";

    ::std::cout
        << ::std::endl << ::std::flush;

#if defined( __GNUC__ )
    if ( status )
        free( realname );
#endif
}

template < typename _Type >
void printTypeOf ( const _Type & )
{
    printTypeOf< _Type >();
}

// Global meta counter test
struct CounterTag {};
constexpr auto a = SCL_META_COUNTER_VALUE( CounterTag );
SCL_META_COUNTER_NEXT( CounterTag )
constexpr auto b = SCL_META_COUNTER_VALUE( CounterTag );
SCL_META_COUNTER_NEXT( CounterTag )
constexpr auto c = SCL_META_COUNTER_VALUE( CounterTag );

// Types registration test
struct TypeTag {};
SCL_META_SEQUENCE_APPEND( TypeTag, int )
SCL_META_SEQUENCE_APPEND( TypeTag, float )
SCL_META_SEQUENCE_APPEND( TypeTag, double )
static_assert( ::std::is_same< SCL_META_SEQUENCE( TypeTag ), ::ScL::Meta::Sequence< int, float, double > >::value, "");

void metaSequenceTest ()
{
    using namespace ::std;
    using namespace ::ScL::Meta;

    struct MyType { MyType() = delete; MyType( const MyType & ) = delete; };

    {
        constexpr auto value_type = Identity< MyType >{};
        using Type = TypeOf< decltype( value_type ) >;
        static_assert( is_same< Type, MyType >::value, "" );
    }

    {
        using EmptySequence = Sequence<>;
        using TestSequence = Sequence< int, double, MyType >;

        static_assert( isEmpty( EmptySequence{} ), "" );
        static_assert( !isEmpty( TestSequence{} ), "" );

        static_assert( reverse( TestSequence{} ) == Sequence< MyType, double, int >{}, "" );

        static_assert( first( TestSequence{} ) == Sequence< int >{}, "" );
        static_assert( last( TestSequence{} ) == Sequence< MyType >{}, "" );

        static_assert( removeFirst( TestSequence{} ) == Sequence< double, MyType >{}, "" );
        static_assert( removeLast( TestSequence{} ) == Sequence< int, double >{}, "" );
        //static_assert( removeIf( TestSequence{}, Condition{} ) == Sequence< int, MyType >{}, "" );
        //static_assert( removeAt( TestSequence{}, 1 ) == Sequence< int, MyType >{}, "" );

        static_assert( append( TestSequence{}, Identity< float >{} ) == Sequence< int, double, MyType, float >{}, "" );
        static_assert( prepend( TestSequence{}, Identity< float >{} ) == Sequence< float, int, double, MyType >{}, "" );

        static_assert ( contains( TestSequence{}, Identity< double >{} ), "" );
        static_assert ( indexOf( TestSequence{}, Identity< double >{} ) == 1, "" );
    }
}

// Meta counter test
void metaCounterTest ()
{
    using Counter = ::ScL::Meta::Counter< struct _ >;

    constexpr auto a = Counter::value();
    constexpr auto b = Counter::next();
    constexpr auto c = Counter::next();
    constexpr auto d = Counter::value();
    constexpr auto e = Counter::value();
    constexpr auto f = Counter::value();
    constexpr auto g = Counter::next();

    // run-time: OK
    ::std::cout
        << a << ' '
        << b << ' '
        << c << ' '
        << d << ' '
        << e << ' '
        << f << ' '
        << g << ' '
        << ::std::endl;

    // compile-time: CLang(FALSE)
    static_assert( a == 0, "" );
    static_assert( b == 1, "" );
//    static_assert( c == 2, "" );
//    static_assert( d == 2, "" );
//    static_assert( e == 2, "" );
//    static_assert( f == 2, "" );
//    static_assert( g == 3, "" );
}


int main ( int, char ** )
{
    return 0;
}
