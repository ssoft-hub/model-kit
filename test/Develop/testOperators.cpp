#include "ModelKit.h"
#include <iostream>

#define FUNC_INFO __PRETTY_FUNCTION__

template < typename _Type >
class Data
{
    static_assert( !::std::is_reference< _Type >::value, "_Type must be a not reference." );
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

    template < typename _Argument >
    decltype(auto) operator = ( _Argument && )
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator = ( ThisType && ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType && ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType && ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType && ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( ThisType & ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const ThisType & ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( volatile ThisType & ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator = ( const volatile ThisType & ) const volatile &
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

    void operator + () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator + () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator - () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator - () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator ++ () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator -- () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator ++ ( int ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ++ ( int ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator -- ( int ) &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator -- ( int ) const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator ~ () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ~ () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    void operator ! () &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () const &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () const volatile &&
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () const &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }
    void operator ! () const volatile &
        { ::std::cout << FUNC_INFO << ::std::endl; }

    template < typename ... _Arguments >
    static ThisType make ( _Arguments && ... arguments ) { return Data( ::std::forward< _Arguments >( arguments ) ... ); }
};

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
    using SameData = Instance< Data< int >, ::Implicit::SharedTool >;

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

    warnOff( data = other );
    warnOff( data = asConst( other ) );
    warnOff( data = asVolatile( other ) );
    warnOff( data = asConstVolatile( other ) );
    warnOff( data = ::std::move( other ) );
    warnOff( data = asConst( ::std::move( other ) ) );
    warnOff( data = asVolatile( ::std::move( other ) ) );
    warnOff( data = asConstVolatile( ::std::move( other ) ) );

    warnOff( ::std::move( data ) = other );
    warnOff( ::std::move( data ) = asConst( other ) );
    warnOff( ::std::move( data ) = asVolatile( other ) );
    warnOff( ::std::move( data ) = asConstVolatile( other ) );
    warnOff( ::std::move( data ) = ::std::move( other ) );
    warnOff( ::std::move( data ) = asConst( ::std::move( other ) ) );
    warnOff( ::std::move( data ) = asVolatile( ::std::move( other ) ) );
    warnOff( ::std::move( data ) = asConstVolatile( ::std::move( other ) ) );
}

template < typename _Data >
void testInstance ()
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

template < typename _Data >
void testAll ()
{
    testConstructors< _Data >();
    testAssignmentOperator< _Data, _Data >();
    testInstance< _Data >();
}

void testFeaturing ()
{
    using TestData = Instance< Data< int > >;
    using CTestData = const TestData;
    using VTestData = volatile TestData;
    using CVTestData = const volatile TestData;

    testAll< TestData >();
    testAll< CTestData >();
    testAll< VTestData >();
    testAll< CVTestData  >();
}

void testBinaryOperators ()
{
    using IntCpp = Instance< int, ::Inplace::DefaultTool >;
    using IntImpl = Instance< int, ::Implicit::SharedTool >;

    IntCpp cpp_int = 5;
    IntImpl impl_int = 10;

    // lvalue / lvalue
    cpp_int + impl_int;
    cpp_int - impl_int;
    cpp_int * impl_int;
    cpp_int / impl_int;
    cpp_int % impl_int;

    cpp_int == impl_int;
    cpp_int != impl_int;
    cpp_int > impl_int;
    cpp_int < impl_int;
    cpp_int >= impl_int;
    cpp_int <= impl_int;
    cpp_int && impl_int;
    cpp_int || impl_int;
    cpp_int & impl_int;
    cpp_int | impl_int;
    cpp_int ^ impl_int;
    cpp_int >> impl_int;
    cpp_int << impl_int;

    cpp_int += impl_int;
    cpp_int -= impl_int;
    cpp_int *= impl_int;
    cpp_int /= impl_int;
    cpp_int %= impl_int;
    cpp_int &= impl_int;
    cpp_int |= impl_int;
    cpp_int ^= impl_int;
    cpp_int >>= impl_int;
    cpp_int <<= impl_int;

    // lvalue / rvalue
    cpp_int + IntImpl();
    cpp_int - IntImpl();
    cpp_int * IntImpl();
    cpp_int / IntImpl();
    cpp_int % IntImpl();

    cpp_int == IntImpl();
    cpp_int != IntImpl();
    cpp_int > IntImpl();
    cpp_int < IntImpl();
    cpp_int >= IntImpl();
    cpp_int <= IntImpl();
    cpp_int && IntImpl();
    cpp_int || IntImpl();
    cpp_int & IntImpl();
    cpp_int | IntImpl();
    cpp_int ^ IntImpl();
    cpp_int >> IntImpl();
    cpp_int << IntImpl();

    cpp_int += IntImpl();
    cpp_int -= IntImpl();
    cpp_int *= IntImpl();
    cpp_int /= IntImpl();
    cpp_int %= IntImpl();
    cpp_int &= IntImpl();
    cpp_int |= IntImpl();
    cpp_int ^= IntImpl();
    cpp_int >>= IntImpl();
    cpp_int <<= IntImpl();

    // rvalue / lvalue
    IntCpp() + impl_int;
    IntCpp() - impl_int;
    IntCpp() * impl_int;
    IntCpp() / impl_int;
    IntCpp() % impl_int;

    IntCpp() == impl_int;
    IntCpp() != impl_int;
    IntCpp() > impl_int;
    IntCpp() < impl_int;
    IntCpp() >= impl_int;
    IntCpp() <= impl_int;
    IntCpp() && impl_int;
    IntCpp() || impl_int;
    IntCpp() & impl_int;
    IntCpp() | impl_int;
    IntCpp() ^ impl_int;
    IntCpp() >> impl_int;
    IntCpp() << impl_int;

    IntCpp() += impl_int;
    IntCpp() -= impl_int;
    IntCpp() *= impl_int;
    IntCpp() /= impl_int;
    IntCpp() %= impl_int;
    IntCpp() &= impl_int;
    IntCpp() |= impl_int;
    IntCpp() ^= impl_int;
    IntCpp() >>= impl_int;
    IntCpp() <<= impl_int;

    // rvalue / rvalue
    IntCpp() + IntImpl();
    IntCpp() - IntImpl();
    IntCpp() * IntImpl();
    IntCpp() / IntImpl();
    IntCpp() % IntImpl();

    IntCpp() == IntImpl();
    IntCpp() != IntImpl();
    IntCpp() > IntImpl();
    IntCpp() < IntImpl();
    IntCpp() >= IntImpl();
    IntCpp() <= IntImpl();
    IntCpp() && IntImpl();
    IntCpp() || IntImpl();
    IntCpp() & IntImpl();
    IntCpp() | IntImpl();
    IntCpp() ^ IntImpl();
    IntCpp() >> IntImpl();
    IntCpp() << IntImpl();

    IntCpp() += IntImpl();
    IntCpp() -= IntImpl();
    IntCpp() *= IntImpl();
    IntCpp() /= IntImpl();
    IntCpp() %= IntImpl();
    IntCpp() &= IntImpl();
    IntCpp() |= IntImpl();
    IntCpp() ^= IntImpl();
    IntCpp() >>= IntImpl();
    IntCpp() <<= IntImpl();
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
