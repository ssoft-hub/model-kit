#include "ModelKit.h"
#include <iostream>
#include <map>
#include <string>

using namespace ::Scl;
using ::std::move;

#if defined( __GNUC__ )
#   define DATA_FUNC_INFO __PRETTY_FUNCTION__
#elif defined( _MSC_VER )
#   define DATA_FUNC_INFO __FUNCSIG__
#endif

#define DATA_UNARY_OPERATOR_INT_PROTOTYPE( symbol, refer ) \
    void operator symbol ( int ) refer \
    { ::std::cout << DATA_FUNC_INFO << ::std::endl; } \

#define DATA_UNARY_OPERATOR_INT( symbol ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    DATA_UNARY_OPERATOR_INT_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_UNARY_OPERATOR_PROTOTYPE( symbol, refer ) \
    void operator symbol () refer \
    { ::std::cout << DATA_FUNC_INFO << ::std::endl; } \

#define DATA_UNARY_OPERATOR( symbol ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    DATA_UNARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_BINARY_OPERATOR_PROTOTYPE( symbol, refer ) \
    template < typename _Right > \
    void operator symbol ( _Right && ) refer \
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; } \

#define DATA_BINARY_OPERATOR( symbol ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

#define DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( symbol, this_refer, other_refer ) \
    void operator symbol ( ThisType other_refer ) this_refer \
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; } \

#define DATA_BINARY_OPERATOR_FOR_THIS( symbol ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &&, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), volatile &, const volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const volatile && ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, volatile & ) \
    DATA_BINARY_OPERATOR_PROTOTYPE_FOR_THIS( SCL_SINGLE_ARG( symbol ), const volatile &, const volatile & ) \

#define DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( symbol, right_refer ) \
    template < typename _Left, typename _Right > \
    void operator symbol ( _Left && /*left*/, Data< _Right> right_refer ) \
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; } \

#define DATA_GLOBAL_BINARY_OPERATOR( symbol ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), && ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const && ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile && ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile && ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), & ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const & ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), volatile & ) \
    DATA_GLOBAL_BINARY_OPERATOR_PROTOTYPE( SCL_SINGLE_ARG( symbol ), const volatile & ) \

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
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    template < typename _Other >
    Data ( Data< _Other > && other ) : m_value( ::std::forward< _Other >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > && other ) : m_value( ::std::forward< const _Other >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( volatile Data< _Other > && other ) : m_value( ::std::forward< _Other >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const volatile Data< _Other > && other ) : m_value( ::std::forward< const _Other >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( volatile Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Other >
    Data ( const volatile Data< _Other > & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    Data ( ThisType && other ) : m_value( ::std::forward< _Type >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( const ThisType && other ) : m_value( ::std::forward< const _Type >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( volatile ThisType && other ) : m_value( ::std::forward< volatile _Type >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( const volatile ThisType && other ) : m_value( ::std::forward< const volatile _Type >( other.m_value ) )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( ThisType & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( const ThisType & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( volatile ThisType & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    Data ( const volatile ThisType & other ) : m_value( other.m_value )
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    ~Data ()
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    void valueMethod ()
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void valueConstMethod () const
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void valueVolatileMethod () volatile
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void valueConstVolatileMethod () const volatile
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    void rvalueMethod () &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void rvalueConstMethod () const &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void rvalueVolatileMethod () volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void rvalueConstVolatileMethod () const volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    void lvalueMethod () &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void lvalueConstMethod () const &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void lvalueVolatileMethod () volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    void lvalueConstVolatileMethod () const volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    template < typename _Index > void operator [] ( _Index && ) &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    template < typename ... _Arguments > void operator () ( _Arguments && ... ) &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const volatile &&
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const volatile &
        { ::std::cout << DATA_FUNC_INFO << ::std::endl; }

    DATA_BINARY_OPERATOR( = )
    DATA_BINARY_OPERATOR_FOR_THIS( = )

    DATA_UNARY_OPERATOR( * )
    DATA_UNARY_OPERATOR( & )
    DATA_UNARY_OPERATOR( -> )
    DATA_BINARY_OPERATOR( ->* )
    DATA_BINARY_OPERATOR( SCL_SINGLE_ARG( , ) )

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
DATA_GLOBAL_BINARY_OPERATOR( * )
DATA_GLOBAL_BINARY_OPERATOR( / )
DATA_GLOBAL_BINARY_OPERATOR( % )
DATA_GLOBAL_BINARY_OPERATOR( + )
DATA_GLOBAL_BINARY_OPERATOR( - )
/* Compound assignment */
DATA_GLOBAL_BINARY_OPERATOR( *= )
DATA_GLOBAL_BINARY_OPERATOR( /= )
DATA_GLOBAL_BINARY_OPERATOR( %= )
DATA_GLOBAL_BINARY_OPERATOR( += )
DATA_GLOBAL_BINARY_OPERATOR( -= )
DATA_GLOBAL_BINARY_OPERATOR( <<= )
DATA_GLOBAL_BINARY_OPERATOR( >>= )
DATA_GLOBAL_BINARY_OPERATOR( &= )
DATA_GLOBAL_BINARY_OPERATOR( ^= )
DATA_GLOBAL_BINARY_OPERATOR( |= )
/* Relational and comparison operators */
DATA_GLOBAL_BINARY_OPERATOR( == )
DATA_GLOBAL_BINARY_OPERATOR( != )
DATA_GLOBAL_BINARY_OPERATOR( < )
DATA_GLOBAL_BINARY_OPERATOR( <= )
DATA_GLOBAL_BINARY_OPERATOR( > )
DATA_GLOBAL_BINARY_OPERATOR( >= )
/* Logical operators */
DATA_GLOBAL_BINARY_OPERATOR( && )
DATA_GLOBAL_BINARY_OPERATOR( || )
/* Bitwise operators */
DATA_GLOBAL_BINARY_OPERATOR( & )
DATA_GLOBAL_BINARY_OPERATOR( ^ )
DATA_GLOBAL_BINARY_OPERATOR( | )
DATA_GLOBAL_BINARY_OPERATOR( << )
DATA_GLOBAL_BINARY_OPERATOR( >> )

void testConstructors ()
{
    using TestData = Instance< Data< double > >;

    TestData one;
    TestData two( 1 );
    TestData three( 0.1 );
    TestData four( one );
    TestData five( castConst( one ) );
    TestData six( TestData::Value::make() );
    TestData seven( castConst( TestData::Value::make() ) );

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
    TestData five( castConst( one ) );
    TestData six( SameData::Value::make() );
    TestData seven( castConst( SameData::Value::make() ) );

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
    TestData five( castConst( one ) );
    TestData six( SameData::Value::make() );
    TestData seven( castConst( SameData::Value::make() ) );

    (void) one;
    (void) four;
    (void) five;
    (void) six;
    (void) seven;
}

void testAccess ()
{
    //using Key = int;//Instance< int >;
    using Key = Instance< ::std::string, ::Implicit::RawTool >;
    using Value = Instance< Data< int > >;
    using Map = ::std::map< Key, Value >;

    Instance< Map > map;
    //Instance< Key > key;
    Key key;
    (*&map)[ *&key ];
}

template < typename _Data >
void testConstructors ()
{
    using TestData = _Data;
    using CTestData = const TestData;
    using VTestData = volatile TestData;
    using CVTestData = const volatile TestData;
    using ::std::move;

    TestData lvalue;
    CTestData lvalue_c;
    VTestData lvalue_v;
    CVTestData lvalue_cv;

    { TestData data; }
    { TestData data = lvalue; }
    { TestData data = lvalue_c; }
    { TestData data = lvalue_v; }
    { TestData data = lvalue_cv; }
    { TestData data = move( lvalue ); }
    { TestData data = move( lvalue_c ); }
    { TestData data = move( lvalue_v ); }
    { TestData data = move( lvalue_cv ); }
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
    data = castConst( other );
    data = castVolatile( other );
    data = castConstVolatile( other );
    data = move( other );
    data = castConst( move( other ) );
    data = castVolatile( move( other ) );
    data = castConstVolatile( move( other ) );

    move( data ) = other;
    move( data ) = castConst( other );
    move( data ) = castVolatile( other );
    move( data ) = castConstVolatile( other );
    move( data ) = move( other );
    move( data ) = castConst( move( other ) );
    move( data ) = castVolatile( move( other ) );
    move( data ) = castConstVolatile( move( other ) );
}

template < typename _Data >
void testInstanceUnaryOperators ()
{
    using TestData = _Data;

    TestData data;
    TestData & lvalue = data;


    // lvalue
    lvalue[0];
    castConst(lvalue)[0];
    castVolatile(lvalue)[0];
    castConstVolatile(lvalue)[0];

    lvalue(0, 1);
    castConst(lvalue)(0, 1);
    castVolatile(lvalue)(0, 1);
    castConstVolatile(lvalue)(0, 1);

    +lvalue;
    +castConst(lvalue);
    +castVolatile(lvalue);
    +castConstVolatile(lvalue);

    -lvalue;
    -castConst(lvalue);
    -castVolatile(lvalue);
    -castConstVolatile(lvalue);

    ++lvalue;
    ++castConst(lvalue);
    ++castVolatile(lvalue);
    ++castConstVolatile(lvalue);

    --lvalue;
    --castConst(lvalue);
    --castVolatile(lvalue);
    --castConstVolatile(lvalue);

    lvalue++;
    castConst(lvalue)++;
    castVolatile(lvalue)++;
    castConstVolatile(lvalue)++;

    lvalue--;
    castConst(lvalue)--;
    castVolatile(lvalue)--;
    castConstVolatile(lvalue)--;

    !lvalue;
    !castConst(lvalue);
    !castVolatile(lvalue);
    !castConstVolatile(lvalue);

    ~lvalue;
    ~castConst(lvalue);
    ~castVolatile(lvalue);
    ~castConstVolatile(lvalue);

    // rvalue
    move( lvalue )[0];
    castConst( move( lvalue ) )[0];
    castVolatile( move( lvalue ) )[0];
    castConstVolatile( move( lvalue ) )[0];

    move( lvalue )(0, 1);
    castConst( move( lvalue ) )(0, 1);
    castVolatile( move( lvalue ) )(0, 1);
    castConstVolatile( move( lvalue ) )(0, 1);

    +move( lvalue );
    +castConst( move( lvalue ) );
    +castVolatile( move( lvalue ) );
    +castConstVolatile( move( lvalue ) );

    -move( lvalue );
    -castConst( move( lvalue ) );
    -castVolatile( move( lvalue ) );
    -castConstVolatile( move( lvalue ) );

    ++move( lvalue );
    ++castConst( move( lvalue ) );
    ++castVolatile( move( lvalue ) );
    ++castConstVolatile( move( lvalue ) );

    --move( lvalue );
    --castConst( move( lvalue ) );
    --castVolatile( move( lvalue ) );
    --castConstVolatile( move( lvalue ) );

    move( lvalue )++;
    castConst( move( lvalue ) )++;
    castVolatile( move( lvalue ) )++;
    castConstVolatile( move( lvalue ) )++;

    move( lvalue )--;
    castConst( move( lvalue ) )--;
    castVolatile( move( lvalue ) )--;
    castConstVolatile( move( lvalue ) )--;

    !move( lvalue );
    !castConst( move( lvalue ) );
    !castVolatile( move( lvalue ) );
    !castConstVolatile( move( lvalue ) );

    ~move( lvalue );
    ~castConst( move( lvalue ) );
    ~castVolatile( move( lvalue ) );
    ~castConstVolatile( move( lvalue ) );
}

template < typename _Left, typename _Right >
void testInstanceBinaryOperatorsSpec ()
{
    using ::std::move;

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
    left + move( right );
    left - move( right );
    left * move( right );
    left / move( right );
    left % move( right );

    left == move( right );
    left != move( right );
    left > move( right );
    left < move( right );
    left >= move( right );
    left <= move( right );
    left && move( right );
    left || move( right );
    left & move( right );
    left | move( right );
    left ^ move( right );
    left >> move( right );
    left << move( right );

    left += move( right );
    left -= move( right );
    left *= move( right );
    left /= move( right );
    left %= move( right );
    left &= move( right );
    left |= move( right );
    left ^= move( right );
    left >>= move( right );
    left <<= move( right );

    // rvalue / lvalue
    move( left ) + right;
    move( left ) - right;
    move( left ) * right;
    move( left ) / right;
    move( left ) % right;

    move( left ) == right;
    move( left ) != right;
    move( left ) > right;
    move( left ) < right;
    move( left ) >= right;
    move( left ) <= right;
    move( left ) && right;
    move( left ) || right;
    move( left ) & right;
    move( left ) | right;
    move( left ) ^ right;
    move( left ) >> right;
    move( left ) << right;

    move( left ) += right;
    move( left ) -= right;
    move( left ) *= right;
    move( left ) /= right;
    move( left ) %= right;
    move( left ) &= right;
    move( left ) |= right;
    move( left ) ^= right;
    move( left ) >>= right;
    move( left ) <<= right;

    // rvalue / rvalue
    move( left ) + move( right );
    move( left ) - move( right );
    move( left ) * move( right );
    move( left ) / move( right );
    move( left ) % move( right );

    move( left ) == move( right );
    move( left ) != move( right );
    move( left ) > move( right );
    move( left ) < move( right );
    move( left ) >= move( right );
    move( left ) <= move( right );
    move( left ) && move( right );
    move( left ) || move( right );
    move( left ) & move( right );
    move( left ) | move( right );
    move( left ) ^ move( right );
    move( left ) >> move( right );
    move( left ) << move( right );

    move( left ) += move( right );
    move( left ) -= move( right );
    move( left ) *= move( right );
    move( left ) /= move( right );
    move( left ) %= move( right );
    move( left ) &= move( right );
    move( left ) |= move( right );
    move( left ) ^= move( right );
    move( left ) >>= move( right );
    move( left ) <<= move( right );
}

template < typename _Left, typename _Right >
void testInstanceBinaryOperators ()
{
    testInstanceBinaryOperatorsSpec< _Left, _Right >();
//    testInstanceBinaryOperatorsSpec< _Left, const _Right >();
//    testInstanceBinaryOperatorsSpec< _Left, volatile _Right >();
//    testInstanceBinaryOperatorsSpec< _Left, const volatile _Right >();
//    testInstanceBinaryOperatorsSpec< const _Left, _Right >();
//    testInstanceBinaryOperatorsSpec< const _Left, const _Right >();
//    testInstanceBinaryOperatorsSpec< const _Left, volatile _Right >();
//    testInstanceBinaryOperatorsSpec< const _Left, const volatile _Right >();
//    testInstanceBinaryOperatorsSpec< volatile _Left, _Right >();
//    testInstanceBinaryOperatorsSpec< volatile _Left, const _Right >();
//    testInstanceBinaryOperatorsSpec< volatile _Left, volatile _Right >();
//    testInstanceBinaryOperatorsSpec< volatile _Left, const volatile _Right >();
//    testInstanceBinaryOperatorsSpec< const volatile _Left, _Right >();
//    testInstanceBinaryOperatorsSpec< const volatile _Left, const _Right >();
//    testInstanceBinaryOperatorsSpec< const volatile _Left, volatile _Right >();
//    testInstanceBinaryOperatorsSpec< const volatile _Left, const volatile _Right >();

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
    testInstanceBinaryOperators< DData, _Data >();
    testInstanceBinaryOperators< _Data, DData >();
    //testInstanceBinaryOperators< _Data, _Data >();
    testAccess();
}

void testFeaturing ()
{
    using TestData = Instance< Data< int >, ::Heap::RawTool >;
//    using CTestData = const TestData;
//    using VTestData = volatile TestData;
//    using CVTestData = const volatile TestData;

    testAll< TestData >();
//    testAll< CTestData >();
//    testAll< VTestData >();
//    testAll< CVTestData  >();
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
