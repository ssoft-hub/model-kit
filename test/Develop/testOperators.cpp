#include "ModelKit.h"
#include <iostream>

template < typename _Type >
class Data
{
    static_assert( !::std::is_reference< _Type >::value, "_Type must be a not reference." );
    using ThisType = Data< _Type >;

public:
    _Type m_value;

public:
    Data () : m_value() { ::std::cout << "Data()" << ::std::endl; }

    template < typename ... _Arguments >
    Data ( _Arguments && ... arguments ) : m_value( ::std::forward< _Arguments >( arguments ) ... )
    {
        auto out_lambda = [] ( auto && param ) { ::std::cout << ::std::forward< decltype( param ) >( param ) << " "; };

        ::std::cout << "Data( ";
        out_lambda( ::std::forward< _Arguments >( arguments ) ... );
        ::std::cout << ")" << ::std::endl;
    }

    template < typename _Other >
    Data ( Data< _Other > && other ) : m_value( ::std::forward< _Other >( other.m_value ) ) { ::std::cout << "Data(Data<> &&)" << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > && other ) : m_value( ::std::forward< const _Other >( other.m_value ) ) { ::std::cout << "Data(const Data<> &&)" << ::std::endl; }
    template < typename _Other >
    Data ( Data< _Other > & other ) : m_value( other.m_value ) { ::std::cout << "Data(Data<> &)" << ::std::endl; }
    template < typename _Other >
    Data ( const Data< _Other > & other ) : m_value( other.m_value ) { ::std::cout << "Data(const Data<> &)" << ::std::endl; }

    Data ( ThisType & other ) : m_value( other.m_value ) { ::std::cout << "Data(Data &)" << ::std::endl; }
    Data ( ThisType && other ) : m_value( ::std::forward< _Type >( other.m_value ) ) { ::std::cout << "Data(Data &&)" << ::std::endl; }
    Data ( const ThisType & other ) : m_value( other.m_value ) { ::std::cout << "Data(const Data &)" << ::std::endl; }
    Data ( const ThisType && other ) : m_value( ::std::forward< const _Type >( other.m_value ) ) { ::std::cout << "Data(const Data &&)" << ::std::endl; }
    ~Data () { ::std::cout << "~Data()" << ::std::endl; }

    void valueMethod () { ::std::cout << "valueMethod()" << ::std::endl; }
    void valueConstMethod () const { ::std::cout << "valueConstMethod()" << ::std::endl; }
    void rvalueMethod () && { ::std::cout << "rvalueMethod()" << ::std::endl; }
    void rvalueConstMethod () const && { ::std::cout << "rvalueConstMethod()" << ::std::endl; }
    void lvalueMethod () & { ::std::cout << "lvalueMethod()" << ::std::endl; }
    void lvalueConstMethod () const & { ::std::cout << "lvalueConstMethod()" << ::std::endl; }

    template < typename _Index > void operator [] ( _Index && ) & { ::std::cout << "operator [] &" << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const & { ::std::cout << "operator [] const &" << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) && { ::std::cout << "operator [] &&" << ::std::endl; }
    template < typename _Index > void operator [] ( _Index && ) const && { ::std::cout << "operator [] const &&" << ::std::endl; }

    template < typename ... _Arguments > void operator () ( _Arguments && ... ) & { ::std::cout << "operator () &" << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const & { ::std::cout << "operator () const &" << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) && { ::std::cout << "operator () &&" << ::std::endl; }
    template < typename ... _Arguments > void operator () ( _Arguments && ... ) const && { ::std::cout << "operator () const &&" << ::std::endl; }

    template < typename ... _Arguments >
    static ThisType make ( _Arguments && ... arguments ) { return Data( ::std::forward< _Arguments >( arguments ) ... ); }
};

void testConstructors ()
{
    using TestData = Featured< Data< double > >;

    TestData one;
    TestData two( 1 );
    TestData three( 0.1 );
    TestData four( one );
    TestData five( asConst( one ) );
    TestData six( TestData::Value::make() );
    TestData seven( asConst( TestData::Value::make() ) );

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
    using TestData = Featured< Data< double > >;
    using SameData = Featured< Data< int > >;

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
    using TestData = Featured< Data< double > >;
    using SameData = Featured< Data< int >, ::Implicit::SharedTool >;

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

void testUnaryOperators ()
{
    //using IntImpl = Featured< Data< double >, ::Implicit::RawTool >;



//    IntImpl impl_int = 10;

//    // lvalue
//    +impl_int;
//    -impl_int;
//    ++impl_int;
//    --impl_int;
//    !impl_int;
//    ~impl_int;

//    // rvalue
//    +IntImpl();
//    -IntImpl();
//    ++IntImpl();
//    --IntImpl();
//    !IntImpl();
//    ~IntImpl();
//}

//void testBinaryOperators ()
//{
//    using IntCpp = Featured< int, ::Cpp::Inplace::DefaultTool >;
//    using IntImpl = Featured< int, ::Std::Shared::ImplicitTool >;

//    IntCpp cpp_int = 5;
//    IntImpl impl_int = 10;

//    // lvalue / lvalue
//    cpp_int + impl_int;
//    cpp_int - impl_int;
//    cpp_int * impl_int;
//    cpp_int / impl_int;
//    cpp_int % impl_int;

//    cpp_int == impl_int;
//    cpp_int != impl_int;
//    cpp_int > impl_int;
//    cpp_int < impl_int;
//    cpp_int >= impl_int;
//    cpp_int <= impl_int;
//    cpp_int && impl_int;
//    cpp_int || impl_int;
//    cpp_int & impl_int;
//    cpp_int | impl_int;
//    cpp_int ^ impl_int;
//    cpp_int >> impl_int;
//    cpp_int << impl_int;

//    cpp_int += impl_int;
//    cpp_int -= impl_int;
//    cpp_int *= impl_int;
//    cpp_int /= impl_int;
//    cpp_int %= impl_int;
//    cpp_int &= impl_int;
//    cpp_int |= impl_int;
//    cpp_int ^= impl_int;
//    cpp_int >>= impl_int;
//    cpp_int <<= impl_int;

//    // lvalue / rvalue
//    cpp_int + IntImpl();
//    cpp_int - IntImpl();
//    cpp_int * IntImpl();
//    cpp_int / IntImpl();
//    cpp_int % IntImpl();

//    cpp_int == IntImpl();
//    cpp_int != IntImpl();
//    cpp_int > IntImpl();
//    cpp_int < IntImpl();
//    cpp_int >= IntImpl();
//    cpp_int <= IntImpl();
//    cpp_int && IntImpl();
//    cpp_int || IntImpl();
//    cpp_int & IntImpl();
//    cpp_int | IntImpl();
//    cpp_int ^ IntImpl();
//    cpp_int >> IntImpl();
//    cpp_int << IntImpl();

//    cpp_int += IntImpl();
//    cpp_int -= IntImpl();
//    cpp_int *= IntImpl();
//    cpp_int /= IntImpl();
//    cpp_int %= IntImpl();
//    cpp_int &= IntImpl();
//    cpp_int |= IntImpl();
//    cpp_int ^= IntImpl();
//    cpp_int >>= IntImpl();
//    cpp_int <<= IntImpl();

//    // rvalue / lvalue
//    IntCpp() + impl_int;
//    IntCpp() - impl_int;
//    IntCpp() * impl_int;
//    IntCpp() / impl_int;
//    IntCpp() % impl_int;

//    IntCpp() == impl_int;
//    IntCpp() != impl_int;
//    IntCpp() > impl_int;
//    IntCpp() < impl_int;
//    IntCpp() >= impl_int;
//    IntCpp() <= impl_int;
//    IntCpp() && impl_int;
//    IntCpp() || impl_int;
//    IntCpp() & impl_int;
//    IntCpp() | impl_int;
//    IntCpp() ^ impl_int;
//    IntCpp() >> impl_int;
//    IntCpp() << impl_int;

//    IntCpp() += impl_int;
//    IntCpp() -= impl_int;
//    IntCpp() *= impl_int;
//    IntCpp() /= impl_int;
//    IntCpp() %= impl_int;
//    IntCpp() &= impl_int;
//    IntCpp() |= impl_int;
//    IntCpp() ^= impl_int;
//    IntCpp() >>= impl_int;
//    IntCpp() <<= impl_int;

//    // rvalue / rvalue
//    IntCpp() + IntImpl();
//    IntCpp() - IntImpl();
//    IntCpp() * IntImpl();
//    IntCpp() / IntImpl();
//    IntCpp() % IntImpl();

//    IntCpp() == IntImpl();
//    IntCpp() != IntImpl();
//    IntCpp() > IntImpl();
//    IntCpp() < IntImpl();
//    IntCpp() >= IntImpl();
//    IntCpp() <= IntImpl();
//    IntCpp() && IntImpl();
//    IntCpp() || IntImpl();
//    IntCpp() & IntImpl();
//    IntCpp() | IntImpl();
//    IntCpp() ^ IntImpl();
//    IntCpp() >> IntImpl();
//    IntCpp() << IntImpl();

//    IntCpp() += IntImpl();
//    IntCpp() -= IntImpl();
//    IntCpp() *= IntImpl();
//    IntCpp() /= IntImpl();
//    IntCpp() %= IntImpl();
//    IntCpp() &= IntImpl();
//    IntCpp() |= IntImpl();
//    IntCpp() ^= IntImpl();
//    IntCpp() >>= IntImpl();
//    IntCpp() <<= IntImpl();
}

//#include <vector>
//#include "MyType.h"

//using TenMyTypes = MyType[10];

//struct OtherType
//{
//    Featured< TenMyTypes > m_my_datas;
//};

//using OtherTypes = ::std::vector< OtherType >;

//void testMemberOperators ()
//{
//    Featured< MyType > my_data;
//    Featured< OtherType > other_data;
//    Featured< OtherTypes > other_datas;

//    other_datas->push_back( *&other_data );
//    other_datas->push_back( *&other_data );
//    other_datas->push_back( *&other_data );

//    my_data->m_age = 10;

//    Featured< OtherTypes >()[0];

//    for ( size_t i = 0; i < other_datas->size(); ++i )
//    {
//        other_datas[ i ] = other_data;
//        //for ( size_t j = 0; j < 10; ++j )
//        //{
//        //    other_datas[ i ][ j ]->m_age = 10;
//        //}
//    }
//}
