//#include <vector>
//#include "MyType.h"

//using TenMyTypes = MyType[10];

//struct OtherType
//{
//    Featured< TenMyTypes > m_my_datas;
//};

//using OtherTypes = std::vector< OtherType >;

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

//void testUnaryOperators ()
//{
//    using IntImpl = Featured< int, ::Std::Shared::ImplicitTool >;

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
//}

