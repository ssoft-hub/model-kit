#include "ModelKit.h"
#include <iostream>
#include <map>
#include <string>

#define FUNC_INFO __PRETTY_FUNCTION__

#define DATA_UNARY_OPERATOR_INT_PROTOTYPE( symbol, refer ) \
    void operator symbol ( int ) refer \
    { ::std::cout << FUNC_INFO << ::std::endl; } \

#define DATA_UNARY_OPERATOR_INT( symbol ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_UNARY_OPERATOR_PROTOTYPE( symbol, refer ) \
    void operator symbol () refer \
    { ::std::cout << FUNC_INFO << ::std::endl; } \

#define DATA_UNARY_OPERATOR( symbol ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_BINARY_OPERATOR_PROTOTYPE( symbol, refer ) \
    template < typename _Right > \
    void operator symbol ( _Right && ) refer \
        { ::std::cout << FUNC_INFO << ::std::endl; } \

#define DATA_BINARY_OPERATOR( symbol ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer ) \
    void operator symbol ( ThisType other_refer ) this_refer \
        { ::std::cout << FUNC_INFO << ::std::endl; } \

#define DATA_BINARY_OPERATOR_FOR_THIS( symbol ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), volatile &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SINGLE_ARG( symbol ), const volatile &, const volatile & ) \

#define GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer ) \
    template < typename _Left, typename _Right > \
    void operator symbol ( _Left && /*left*/, Data< _Right> right_refer ) \
        { ::std::cout << FUNC_INFO << ::std::endl; } \

#define GLOBAL_DATA_BINARY_OPERATOR( symbol ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), && ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const && ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile && ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile && ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), & ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const & ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), volatile & ) \
    GLOBAL_DATA_BINARY_OPERATOR_PROTOTYPE( SINGLE_ARG( symbol ), const volatile & ) \

class Dummy {};

template < typename _Type >
class Data
{
    static_assert( !::std::is_reference< _Type >::value, "_Type must to be a not reference." );
    using ThisType = Data< _Type >;

public:
    _Type m_value;

public:
    template < typename ... _Arguments >
    Data ( _Arguments && ... arguments ) : m_value( ::std::forward< _Arguments >( arguments ) ... )
        { ::std::cout << FUNC_INFO << ::std::endl; }

    template < typename _Other >
    Data ( Data< _Other > && other ) : m_value( ::std::forward< _Other >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > && other ) : m_value( ::std::forward< const _Other >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( volatile Data< _Other > && other ) : m_value( ::std::forward< _Other >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const volatile Data< _Other > && other ) : m_value( ::std::forward< const _Other >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( volatile Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const volatile Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }

    Data ( ThisType && other ) : m_value( ::std::forward< _Type >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( const ThisType && other ) : m_value( ::std::forward< const _Type >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( volatile ThisType && other ) : m_value( ::std::forward< volatile _Type >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( const volatile ThisType && other ) : m_value( ::std::forward< const volatile _Type >( other.m_value ) )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( ThisType & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( const ThisType & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( volatile ThisType & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }
    Data ( const volatile ThisType & other ) : m_value( other.m_value )
        { ::std::cout << FUNC_INFO << ::std::endl; }

    ~Data ()
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void valueMethod ()
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void valueConstMethod () const
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void valueVolatileMethod () volatile
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void valueConstVolatileMethod () const volatile
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void rvalueMethod () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void rvalueConstMethod () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void rvalueVolatileMethod () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void rvalueConstVolatileMethod () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void lvalueMethod () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void lvalueConstMethod () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void lvalueVolatileMethod () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void lvalueConstVolatileMethod () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    template < typename _Index > void operator [] ( _Index && ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    template < typename ... _Arguments > void operator () ( _Arguments && ... ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    DATA_BINARY_OPERATOR( = )
    DATA_BINARY_OPERATOR_FOR_THIS( = )

    DATA_UNARY_OPERATOR( * )
    DATA_UNARY_OPERATOR( & )
    DATA_UNARY_OPERATOR( -> )
    DATA_BINARY_OPERATOR( ->* )
    DATA_BINARY_OPERATOR( SINGLE_ARG( , ) )

    DATA_UNARY_OPERATOR( + )
    DATA_UNARY_OPERATOR( - )
    DATA_UNARY_OPERATOR( ++ )
    DATA_UNARY_OPERATOR( -- )
    DATA_UNARY_OPERATOR_INT( ++ )
    DATA_UNARY_OPERATOR_INT( -- )
    DATA_UNARY_OPERATOR( ~ )
    DATA_UNARY_OPERATOR( ! )

    DATA_BINARY_OPERATOR( * )
    DATA_BINARY_OPERATOR( / )
    DATA_BINARY_OPERATOR( % )
    DATA_BINARY_OPERATOR( + )
    DATA_BINARY_OPERATOR( - )
    /* Compound assignment */
    DATA_BINARY_OPERATOR( *= )
    DATA_BINARY_OPERATOR( /= )
    DATA_BINARY_OPERATOR( %= )
    DATA_BINARY_OPERATOR( +=)
    DATA_BINARY_OPERATOR( -= )
    DATA_BINARY_OPERATOR( <<= )
    DATA_BINARY_OPERATOR( >>= )
    DATA_BINARY_OPERATOR( &= )
    DATA_BINARY_OPERATOR( ^= )
    DATA_BINARY_OPERATOR( |= )
    /* Increment and decrement */
    /* Relational and comparison operators */
    DATA_BINARY_OPERATOR( == )
    DATA_BINARY_OPERATOR( != )
    DATA_BINARY_OPERATOR( < )
    DATA_BINARY_OPERATOR( <= )
    DATA_BINARY_OPERATOR( > )
    DATA_BINARY_OPERATOR( >= )
    /* Logical operators */
    DATA_BINARY_OPERATOR( && )
    DATA_BINARY_OPERATOR( || )
    /* Bitwise operators */
    DATA_BINARY_OPERATOR( & )
    DATA_BINARY_OPERATOR( ^ )
    DATA_BINARY_OPERATOR( | )
    DATA_BINARY_OPERATOR( << )
    DATA_BINARY_OPERATOR( >> )

    template < typename ... _Arguments >
    static ThisType make ( _Arguments && ... arguments ) { return Data( ::std::forward< _Arguments >( arguments ) ... ); }
};

/* RIGHT-SIDE INSTANCE OPERATORS */

/* Arithmetic operators */
GLOBAL_DATA_BINARY_OPERATOR( * )
GLOBAL_DATA_BINARY_OPERATOR( / )
GLOBAL_DATA_BINARY_OPERATOR( % )
GLOBAL_DATA_BINARY_OPERATOR( + )
GLOBAL_DATA_BINARY_OPERATOR( - )
/* Compound assignment */
GLOBAL_DATA_BINARY_OPERATOR( *= )
GLOBAL_DATA_BINARY_OPERATOR( /= )
GLOBAL_DATA_BINARY_OPERATOR( %= )
GLOBAL_DATA_BINARY_OPERATOR( += )
GLOBAL_DATA_BINARY_OPERATOR( -= )
GLOBAL_DATA_BINARY_OPERATOR( <<= )
GLOBAL_DATA_BINARY_OPERATOR( >>= )
GLOBAL_DATA_BINARY_OPERATOR( &= )
GLOBAL_DATA_BINARY_OPERATOR( ^= )
GLOBAL_DATA_BINARY_OPERATOR( |= )
/* Relational and comparison operators */
GLOBAL_DATA_BINARY_OPERATOR( == )
GLOBAL_DATA_BINARY_OPERATOR( != )
GLOBAL_DATA_BINARY_OPERATOR( < )
GLOBAL_DATA_BINARY_OPERATOR( <= )
GLOBAL_DATA_BINARY_OPERATOR( > )
GLOBAL_DATA_BINARY_OPERATOR( >= )
/* Logical operators */
GLOBAL_DATA_BINARY_OPERATOR( && )
GLOBAL_DATA_BINARY_OPERATOR( || )
/* Bitwise operators */
GLOBAL_DATA_BINARY_OPERATOR( & )
GLOBAL_DATA_BINARY_OPERATOR( ^ )
GLOBAL_DATA_BINARY_OPERATOR( | )
GLOBAL_DATA_BINARY_OPERATOR( << )
GLOBAL_DATA_BINARY_OPERATOR( >> )

void testConstructors ()
{
    using TestData = Instance< Data< double > >;

    TestData one;
    TestData two( 1 );
    TestData three( 0.1 );
    TestData four( one );
    TestData five( asConst( one ) );
    TestData six( TestData::Value::make() );
    TestData seven( asConst( TestData::Value::make() ) );

    TestData() = 10;
    one = 10;

    (void) one;
    (void) two;
    (void) three;
    (void) four;
    (void) five;
    (void) six;
    (void) seven;
}

void testSameToolConstructors ()
{
    using TestData = Instance< Data< double > >;
    using SameData = Instance< Data< int > >;

    SameData one;
    TestData four( one );
    TestData five( asConst( one ) );
    TestData six( SameData::Value::make() );
    TestData seven( asConst( SameData::Value::make() ) );

    (void) one;
    (void) four;
    (void) five;
    (void) six;
    (void) seven;
}

void testDiffToolConstructors ()
{
    using TestData = Instance< Data< double > >;
    using SameData = Instance< Data< int > >;//, ::Implicit::SharedTool >;

    SameData one;
    TestData four( one );
    TestData five( asConst( one ) );
    TestData six( SameData::Value::make() );
    TestData seven( asConst( SameData::Value::make() ) );

    (void) one;
    (void) four;
    (void) five;
    (void) six;
    (void) seven;
}

void testAccess ()
{
//    using Key = int;//Instance< int >;
//    using Value = Instance< Data< int > >;
//    using Map = ::std::map< Key, Value >;

//    Instance< Map > value;
//    *value;
}

template < typename _Data >
void testConstructors ()
{
    using TestData = _Data;
    using CTestData = const TestData;
    using VTestData = volatile TestData;
    using CVTestData = const volatile TestData;

    TestData lvalue;
    CTestData lvalue_c;
    VTestData lvalue_v;
    CVTestData lvalue_cv;

    { TestData data; }
    { TestData data = lvalue; }
    { TestData data = lvalue_c; }
    { TestData data = lvalue_v; }
    { TestData data = lvalue_cv; }
    { TestData data = ::std::move( lvalue ); }
    { TestData data = ::std::move( lvalue_c ); }
    { TestData data = ::std::move( lvalue_v ); }
    { TestData data = ::std::move( lvalue_cv ); }
}

/* Remove warning like this:
 * implicit dereference will not access object of type 'volatile ...' in statement */
template < typename ... _Arguments >
void warnOff ( _Arguments && ... ) {}

template < typename _Data, typename _Other >
void testAssignmentOperator ()
{
    using Data = _Data;
    using Other = _Other;

    Data data;
    Other other;

    data = other;
    data = asConst( other );
    data = asVolatile( other );
    data = asConstVolatile( other );
    data = ::std::move( other );
    data = asConst( ::std::move( other ) );
    data = asVolatile( ::std::move( other ) );
    data = asConstVolatile( ::std::move( other ) );

    ::std::move( data ) = other;
    ::std::move( data ) = asConst( other );
    ::std::move( data ) = asVolatile( other );
    ::std::move( data ) = asConstVolatile( other );
    ::std::move( data ) = ::std::move( other );
    ::std::move( data ) = asConst( ::std::move( other ) );
    ::std::move( data ) = asVolatile( ::std::move( other ) );
    ::std::move( data ) = asConstVolatile( ::std::move( other ) );
}

template < typename _Data >
void testInstanceUnaryOperators ()
{
    using TestData = _Data;

    TestData data;
    TestData & lvalue = data;

    // lvalue
    lvalue[0];
    asConst(lvalue)[0];
    asVolatile(lvalue)[0];
    asConstVolatile(lvalue)[0];

    lvalue(0, 1);
    asConst(lvalue)(0, 1);
    asVolatile(lvalue)(0, 1);
    asConstVolatile(lvalue)(0, 1);

    +lvalue;
    +asConst(lvalue);
    +asVolatile(lvalue);
    +asConstVolatile(lvalue);

    -lvalue;
    -asConst(lvalue);
    -asVolatile(lvalue);
    -asConstVolatile(lvalue);

    ++lvalue;
    ++asConst(lvalue);
    ++asVolatile(lvalue);
    ++asConstVolatile(lvalue);

    --lvalue;
    --asConst(lvalue);
    --asVolatile(lvalue);
    --asConstVolatile(lvalue);

    lvalue++;
    asConst(lvalue)++;
    asVolatile(lvalue)++;
    asConstVolatile(lvalue)++;

    lvalue--;
    asConst(lvalue)--;
    asVolatile(lvalue)--;
    asConstVolatile(lvalue)--;

    !lvalue;
    !asConst(lvalue);
    !asVolatile(lvalue);
    !asConstVolatile(lvalue);

    ~lvalue;
    ~asConst(lvalue);
    ~asVolatile(lvalue);
    ~asConstVolatile(lvalue);

    // rvalue
    ::std::move( lvalue )[0];
    asConst( ::std::move( lvalue ) )[0];
    asVolatile( ::std::move( lvalue ) )[0];
    asConstVolatile( ::std::move( lvalue ) )[0];

    ::std::move( lvalue )(0, 1);
    asConst( ::std::move( lvalue ) )(0, 1);
    asVolatile( ::std::move( lvalue ) )(0, 1);
    asConstVolatile( ::std::move( lvalue ) )(0, 1);

    +::std::move( lvalue );
    +asConst( ::std::move( lvalue ) );
    +asVolatile( ::std::move( lvalue ) );
    +asConstVolatile( ::std::move( lvalue ) );

    -::std::move( lvalue );
    -asConst( ::std::move( lvalue ) );
    -asVolatile( ::std::move( lvalue ) );
    -asConstVolatile( ::std::move( lvalue ) );

    ++::std::move( lvalue );
    ++asConst( ::std::move( lvalue ) );
    ++asVolatile( ::std::move( lvalue ) );
    ++asConstVolatile( ::std::move( lvalue ) );

    --::std::move( lvalue );
    --asConst( ::std::move( lvalue ) );
    --asVolatile( ::std::move( lvalue ) );
    --asConstVolatile( ::std::move( lvalue ) );

    ::std::move( lvalue )++;
    asConst( ::std::move( lvalue ) )++;
    asVolatile( ::std::move( lvalue ) )++;
    asConstVolatile( ::std::move( lvalue ) )++;

    ::std::move( lvalue )--;
    asConst( ::std::move( lvalue ) )--;
    asVolatile( ::std::move( lvalue ) )--;
    asConstVolatile( ::std::move( lvalue ) )--;

    !::std::move( lvalue );
    !asConst( ::std::move( lvalue ) );
    !asVolatile( ::std::move( lvalue ) );
    !asConstVolatile( ::std::move( lvalue ) );

    ~::std::move( lvalue );
    ~asConst( ::std::move( lvalue ) );
    ~asVolatile( ::std::move( lvalue ) );
    ~asConstVolatile( ::std::move( lvalue ) );
}

template < typename _Left, typename _Right >
void testInstanceBinaryOperatorsSpec ()
{
    _Left left;
    _Right right;

    // lvalue / lvalue
    left + right;
    left - right;
    left * right;
    left / right;
    left % right;

    left == right;
    left != right;
    left > right;
    left < right;
    left >= right;
    left <= right;
    left && right;
    left || right;
    left & right;
    left | right;
    left ^ right;
    left >> right;
    left << right;

    left += right;
    left -= right;
    left *= right;
    left /= right;
    left %= right;
    left &= right;
    left |= right;
    left ^= right;
    left >>= right;
    left <<= right;

    // lvalue / rvalue
    left + ::std::move( right );
    left - ::std::move( right );
    left * ::std::move( right );
    left / ::std::move( right );
    left % ::std::move( right );

    left == ::std::move( right );
    left != ::std::move( right );
    left > ::std::move( right );
    left < ::std::move( right );
    left >= ::std::move( right );
    left <= ::std::move( right );
    left && ::std::move( right );
    left || ::std::move( right );
    left & ::std::move( right );
    left | ::std::move( right );
    left ^ ::std::move( right );
    left >> ::std::move( right );
    left << ::std::move( right );

    left += ::std::move( right );
    left -= ::std::move( right );
    left *= ::std::move( right );
    left /= ::std::move( right );
    left %= ::std::move( right );
    left &= ::std::move( right );
    left |= ::std::move( right );
    left ^= ::std::move( right );
    left >>= ::std::move( right );
    left <<= ::std::move( right );

    // rvalue / lvalue
    ::std::move( left ) + right;
    ::std::move( left ) - right;
    ::std::move( left ) * right;
    ::std::move( left ) / right;
    ::std::move( left ) % right;

    ::std::move( left ) == right;
    ::std::move( left ) != right;
    ::std::move( left ) > right;
    ::std::move( left ) < right;
    ::std::move( left ) >= right;
    ::std::move( left ) <= right;
    ::std::move( left ) && right;
    ::std::move( left ) || right;
    ::std::move( left ) & right;
    ::std::move( left ) | right;
    ::std::move( left ) ^ right;
    ::std::move( left ) >> right;
    ::std::move( left ) << right;

    ::std::move( left ) += right;
    ::std::move( left ) -= right;
    ::std::move( left ) *= right;
    ::std::move( left ) /= right;
    ::std::move( left ) %= right;
    ::std::move( left ) &= right;
    ::std::move( left ) |= right;
    ::std::move( left ) ^= right;
    ::std::move( left ) >>= right;
    ::std::move( left ) <<= right;

    // rvalue / rvalue
    ::std::move( left ) + ::std::move( right );
    ::std::move( left ) - ::std::move( right );
    ::std::move( left ) * ::std::move( right );
    ::std::move( left ) / ::std::move( right );
    ::std::move( left ) % ::std::move( right );

    ::std::move( left ) == ::std::move( right );
    ::std::move( left ) != ::std::move( right );
    ::std::move( left ) > ::std::move( right );
    ::std::move( left ) < ::std::move( right );
    ::std::move( left ) >= ::std::move( right );
    ::std::move( left ) <= ::std::move( right );
    ::std::move( left ) && ::std::move( right );
    ::std::move( left ) || ::std::move( right );
    ::std::move( left ) & ::std::move( right );
    ::std::move( left ) | ::std::move( right );
    ::std::move( left ) ^ ::std::move( right );
    ::std::move( left ) >> ::std::move( right );
    ::std::move( left ) << ::std::move( right );

    ::std::move( left ) += ::std::move( right );
    ::std::move( left ) -= ::std::move( right );
    ::std::move( left ) *= ::std::move( right );
    ::std::move( left ) /= ::std::move( right );
    ::std::move( left ) %= ::std::move( right );
    ::std::move( left ) &= ::std::move( right );
    ::std::move( left ) |= ::std::move( right );
    ::std::move( left ) ^= ::std::move( right );
    ::std::move( left ) >>= ::std::move( right );
    ::std::move( left ) <<= ::std::move( right );
}

template < typename _Left, typename _Right >
void testInstanceBinaryOperators ()
{
    testInstanceBinaryOperatorsSpec< _Left, _Right >();
    testInstanceBinaryOperatorsSpec< _Left, const _Right >();
    testInstanceBinaryOperatorsSpec< _Left, volatile _Right >();
    testInstanceBinaryOperatorsSpec< _Left, const volatile _Right >();
    //testInstanceBinaryOperatorsSpec< const _Left, _Right >();
    //testInstanceBinaryOperatorsSpec< const _Left, const _Right >();
    //testInstanceBinaryOperatorsSpec< const _Left, volatile _Right >();
    //testInstanceBinaryOperatorsSpec< const _Left, const volatile _Right >();
    //testInstanceBinaryOperatorsSpec< volatile _Left, _Right >();
    //testInstanceBinaryOperatorsSpec< volatile _Left, const _Right >();
    //testInstanceBinaryOperatorsSpec< volatile _Left, volatile _Right >();
    //testInstanceBinaryOperatorsSpec< volatile _Left, const volatile _Right >();
    //testInstanceBinaryOperatorsSpec< const volatile _Left, _Right >();
    //testInstanceBinaryOperatorsSpec< const volatile _Left, const _Right >();
    //testInstanceBinaryOperatorsSpec< const volatile _Left, volatile _Right >();
    //testInstanceBinaryOperatorsSpec< const volatile _Left, const volatile _Right >();

    //testInstanceBinaryOperatorsSpec< _Right, _Left >();
    //testInstanceBinaryOperatorsSpec< _Right, const _Left >();
    //testInstanceBinaryOperatorsSpec< _Right, volatile _Left >();
    //testInstanceBinaryOperatorsSpec< _Right, const volatile _Left >();
    //testInstanceBinaryOperatorsSpec< const _Right, _Left >();
    //testInstanceBinaryOperatorsSpec< const _Right, const _Left >();
    //testInstanceBinaryOperatorsSpec< const _Right, volatile _Left >();
    //testInstanceBinaryOperatorsSpec< const _Right, const volatile _Left >();
    //testInstanceBinaryOperatorsSpec< volatile _Right, _Left >();
    //testInstanceBinaryOperatorsSpec< volatile _Right, const _Left >();
    //testInstanceBinaryOperatorsSpec< volatile _Right, volatile _Left >();
    //testInstanceBinaryOperatorsSpec< volatile _Right, const volatile _Left >();
    //testInstanceBinaryOperatorsSpec< const volatile _Right, _Left >();
    //testInstanceBinaryOperatorsSpec< const volatile _Right, const _Left >();
    //testInstanceBinaryOperatorsSpec< const volatile _Right, volatile _Left >();
    //testInstanceBinaryOperatorsSpec< const volatile _Right, const volatile _Left >();
}



template < typename _Data >
void testAll ()
{
    using DData = Dummy;//Data< _Data >;
    testConstructors< _Data >();
    testAssignmentOperator< _Data, _Data >();
    testInstanceUnaryOperators< _Data >();
    testInstanceBinaryOperators< _Data, DData >();
    testAccess();
}

void testFeaturing ()
{
    using TestData = Instance< Data< int > >;
//    using CTestData = const TestData;
//    using VTestData = volatile TestData;
    using CVTestData = const volatile TestData;

//    testAll< TestData >();
//    testAll< CTestData >();
//    testAll< VTestData >();
    testAll< CVTestData  >();
}

void testBinaryOperators ()
{

}

//#include <vector>
//#include "MyType.h"

//using TenMyTypes = MyType[10];

//struct OtherType
//{
//    Instance< TenMyTypes > m_my_datas;
//};

//using OtherTypes = ::std::vector< OtherType >;

//void testMemberOperators ()
//{
//    Instance< MyType > my_data;
//    Instance< OtherType > other_data;
//    Instance< OtherTypes > other_datas;

//    other_datas->push_back( *&other_data );
//    other_datas->push_back( *&other_data );
//    other_datas->push_back( *&other_data );

//    my_data->m_age = 10;

//    Instance< OtherTypes >()[0];

//    for ( size_t i = 0; i < other_datas->size(); ++i )
//    {
//        other_datas[ i ] = other_data;
//        //for ( size_t j = 0; j < 10; ++j )
//        //{
//        //    other_datas[ i ][ j ]->m_age = 10;
//        //}
//    }
//}
