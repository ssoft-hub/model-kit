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

    Data & operator = ( const Data & other ) { m_value = other.m_value; return *this; }

    void valueMethod () { ::std::cout << "valueMethod()" << ::std::endl; }
    void valueConstMethod () const { ::std::cout << "valueConstMethod()" << ::std::endl; }
    void rvalueMethod () && { ::std::cout << "rvalueMethod()" << ::std::endl; }
    void rvalueConstMethod () const && { ::std::cout << "rvalueConstMethod()" << ::std::endl; }
    void lvalueMethod () & { ::std::cout << "lvalueMethod()" << ::std::endl; }
    void lvalueConstMethod () const & { ::std::cout << "lvalueConstMethod()" << ::std::endl; }

    template < typename _Index > _Type & operator [] ( _Index && ) & { ::std::cout << "operator [] &" << ::std::endl; return m_value; }
    template < typename _Index > const _Type & operator [] ( _Index && ) const & { ::std::cout << "operator [] const &" << ::std::endl; return m_value; }
    template < typename _Index > _Type && operator [] ( _Index && ) && { ::std::cout << "operator [] &&" << ::std::endl; return ::std::forward< _Type >( m_value ); }
    template < typename _Index > const _Type && operator [] ( _Index && ) const && { ::std::cout << "operator [] const &&" << ::std::endl; return ::std::forward< const _Type >( m_value ); }

    template < typename ... _Arguments > _Type & operator () ( _Arguments && ... ) & { ::std::cout << "operator () &" << ::std::endl; return m_value; }
    template < typename ... _Arguments > const _Type & operator () ( _Arguments && ... ) const & { ::std::cout << "operator () const &" << ::std::endl; return m_value; }
    template < typename ... _Arguments > _Type && operator () ( _Arguments && ... ) && { ::std::cout << "operator () &&" << ::std::endl; return ::std::forward< _Type >( m_value ); }
    template < typename ... _Arguments > const _Type && operator () ( _Arguments && ... ) const && { ::std::cout << "operator () const &&" << ::std::endl; return ::std::forward< const _Type >( m_value ); }

    void operator + () && { ::std::cout << "operator + &&" << ::std::endl; }
    void operator + () const && { ::std::cout << "operator + const &&" << ::std::endl; }
    void operator + () & { ::std::cout << "operator + &" << ::std::endl; }
    void operator + () const & { ::std::cout << "operator + const &" << ::std::endl; }
    void operator + () volatile && { ::std::cout << "operator + volatile &&" << ::std::endl; }
    void operator + () volatile const && { ::std::cout << "operator + const volatile &&" << ::std::endl; }
    void operator + () volatile & { ::std::cout << "operator + volatile &" << ::std::endl; }
    void operator + () volatile const & { ::std::cout << "operator + const volatile &" << ::std::endl; }

    void operator - () && { ::std::cout << "operator - &&" << ::std::endl; }
    void operator - () const && { ::std::cout << "operator - const &&" << ::std::endl; }
    void operator - () & { ::std::cout << "operator - &" << ::std::endl; }
    void operator - () const & { ::std::cout << "operator - const &" << ::std::endl; }
    void operator - () volatile && { ::std::cout << "operator - volatile &&" << ::std::endl; }
    void operator - () volatile const && { ::std::cout << "operator - const volatile &&" << ::std::endl; }
    void operator - () volatile & { ::std::cout << "operator - volatile &" << ::std::endl; }
    void operator - () volatile const & { ::std::cout << "operator - const volatile &" << ::std::endl; }

    void operator ++ () && { ::std::cout << "operator ++ &&" << ::std::endl; }
    void operator ++ () const && { ::std::cout << "operator ++ const &&" << ::std::endl; }
    void operator ++ () & { ::std::cout << "operator ++ &" << ::std::endl; }
    void operator ++ () const & { ::std::cout << "operator ++ const &" << ::std::endl; }
    void operator ++ () volatile && { ::std::cout << "operator ++ volatile &&" << ::std::endl; }
    void operator ++ () volatile const && { ::std::cout << "operator ++ const volatile &&" << ::std::endl; }
    void operator ++ () volatile & { ::std::cout << "operator ++ volatile &" << ::std::endl; }
    void operator ++ () volatile const & { ::std::cout << "operator ++ const volatile &" << ::std::endl; }

    void operator -- () && { ::std::cout << "operator -- &&" << ::std::endl; }
    void operator -- () const && { ::std::cout << "operator -- const &&" << ::std::endl; }
    void operator -- () & { ::std::cout << "operator -- &" << ::std::endl; }
    void operator -- () const & { ::std::cout << "operator -- const &" << ::std::endl; }
    void operator -- () volatile && { ::std::cout << "operator -- volatile &&" << ::std::endl; }
    void operator -- () volatile const && { ::std::cout << "operator -- const volatile &&" << ::std::endl; }
    void operator -- () volatile & { ::std::cout << "operator -- volatile &" << ::std::endl; }
    void operator -- () volatile const & { ::std::cout << "operator -- const volatile &" << ::std::endl; }

    void operator ++ ( int ) && { ::std::cout << "operator _++ &&" << ::std::endl; }
    void operator ++ ( int ) const && { ::std::cout << "operator _++ const &&" << ::std::endl; }
    void operator ++ ( int ) & { ::std::cout << "operator _++ &" << ::std::endl; }
    void operator ++ ( int ) const & { ::std::cout << "operator _++ const &" << ::std::endl; }
    void operator ++ ( int ) volatile && { ::std::cout << "operator ++ volatile &&" << ::std::endl; }
    void operator ++ ( int ) volatile const && { ::std::cout << "operator ++ const volatile &&" << ::std::endl; }
    void operator ++ ( int ) volatile & { ::std::cout << "operator ++ volatile &" << ::std::endl; }
    void operator ++ ( int ) volatile const & { ::std::cout << "operator ++ const volatile &" << ::std::endl; }

    void operator -- ( int ) && { ::std::cout << "operator _-- &&" << ::std::endl; }
    void operator -- ( int ) const && { ::std::cout << "operator _-- const &&" << ::std::endl; }
    void operator -- ( int ) & { ::std::cout << "operator _-- &" << ::std::endl; }
    void operator -- ( int ) const & { ::std::cout << "operator _-- const &" << ::std::endl; }
    void operator -- ( int ) volatile && { ::std::cout << "operator -- volatile &&" << ::std::endl; }
    void operator -- ( int ) volatile const && { ::std::cout << "operator -- const volatile &&" << ::std::endl; }
    void operator -- ( int ) volatile & { ::std::cout << "operator -- volatile &" << ::std::endl; }
    void operator -- ( int ) volatile const & { ::std::cout << "operator -- const volatile &" << ::std::endl; }

    void operator ~ () && { ::std::cout << "operator ~ &&" << ::std::endl; }
    void operator ~ () const && { ::std::cout << "operator ~ const &&" << ::std::endl; }
    void operator ~ () & { ::std::cout << "operator ~ &" << ::std::endl; }
    void operator ~ () const & { ::std::cout << "operator ~ const &" << ::std::endl; }
    void operator ~ () volatile && { ::std::cout << "operator ~ volatile &&" << ::std::endl; }
    void operator ~ () volatile const && { ::std::cout << "operator ~ const volatile &&" << ::std::endl; }
    void operator ~ () volatile & { ::std::cout << "operator ~ volatile &" << ::std::endl; }
    void operator ~ () volatile const & { ::std::cout << "operator ~ const volatile &" << ::std::endl; }

    void operator ! () && { ::std::cout << "operator ! &&" << ::std::endl; }
    void operator ! () const && { ::std::cout << "operator ! const &&" << ::std::endl; }
    void operator ! () & { ::std::cout << "operator ! &" << ::std::endl; }
    void operator ! () const & { ::std::cout << "operator ! const &" << ::std::endl; }
    void operator ! () volatile && { ::std::cout << "operator ! volatile &&" << ::std::endl; }
    void operator ! () volatile const && { ::std::cout << "operator ! const volatile &&" << ::std::endl; }
    void operator ! () volatile & { ::std::cout << "operator ! volatile &" << ::std::endl; }
    void operator ! () volatile const & { ::std::cout << "operator ! const volatile &" << ::std::endl; }

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

void testUnaryOperators ()
{
    using TestData = Instance< Data< int > >;

    TestData impl_data = 10;
    impl_data[0];
    asConst(impl_data)[0];
    TestData()[0];
    asConst(TestData())[0];

    impl_data(0, 1);
    asConst(impl_data)(0, 1);
    TestData()(0, 1);
    asConst(TestData())(0, 1);

    // lvalue
    +impl_data;
    +asConst(impl_data);
    -impl_data;
    -asConst(impl_data);
    ++impl_data;
    ++asConst(impl_data);
    --impl_data;
    --asConst(impl_data);
    impl_data++;
    asConst(impl_data)++;
    impl_data--;
    asConst(impl_data)--;
    !impl_data;
    !asConst(impl_data);
    ~impl_data;
    ~asConst(impl_data);

    // rvalue
    +TestData();
    +asConst(TestData());
    -TestData();
    -asConst(TestData());
    ++TestData();
    ++asConst(TestData());
    --TestData();
    --asConst(TestData());
    TestData()++;
    asConst(TestData())++;
    TestData()--;
    asConst(TestData())--;
    !TestData();
    !asConst(TestData());
    ~TestData();
    ~asConst(TestData());

    using VTestData = volatile TestData;
    VTestData vol_data = 10;
    +vol_data;
    //+asConst(vol_data);
    -vol_data;
    -asConst(vol_data);
    ++vol_data;
    ++asConst(vol_data);
    --vol_data;
    --asConst(vol_data);
    vol_data++;
    asConst(vol_data)++;
    vol_data--;
    asConst(vol_data)--;
    !vol_data;
    !asConst(vol_data);
    ~vol_data;
    ~asConst(vol_data);

//    +VTestData();
//    +asConst(VTestData());
//    -VTestData();
//    -asConst(VTestData());
//    ++VTestData();
//    ++asConst(VTestData());
//    --VTestData();
//    --asConst(VTestData());
//    VTestData()++;
//    asConst(VTestData())++;
//    VTestData()--;
//    asConst(VTestData())--;
//    !VTestData();
//    !asConst(VTestData());
//    ~VTestData();
//    ~asConst(VTestData());
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
