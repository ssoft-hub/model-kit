//#ifdef _MSC_VER
//#pragma warning( disable : 4552 4553 )
//#endif

#include <ModelKit.h>
#include <vector>
#include <iostream>
#include <cxxabi.h>

template < typename _Type >
void printTypeOf ()
{
    int status = 0;
    char * realname = 0;
    realname = abi::__cxa_demangle( typeid( _Type ).name(), 0, 0, &status );

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

    free( realname );
}

template < typename _Type >
void printTypeOf ( _Type )
{
    printTypeOf< _Type >();
}


extern void testResultOf();
extern void testFeaturedValue ();
extern void testFeaturedContainer ();

int main ( int /*argc*/, char ** /*argv*/ )
{
    //testResultOf();
    testFeaturedContainer();

//    testMemberOperators();
//    testConstructor();
//    testAllTool();
//    testRelation();


////    int int_value; // OK
////    const int int_value = 0; // ERROR

//    Featured< int > int_value; // OK
////    const Featured< int > int_value = 0; // ERROR
////    Featured< const int > int_value = 0; // ERROR

////    Featured< Featured< int, ImplicitTool > > int_value; // OK
////    const Featured< Featured< int, ImplicitTool > > int_value; // ERROR
////    Featured< const Featured< int, ImplicitTool > > int_value; // ERROR
////    Featured< Featured< const int, ImplicitTool > > int_value; // ERROR

////    Featured< UniqueEnd< int, ToolType > > int_value( UniqueEnd< int, ToolType >::make( 10 ) ); // OK
////    const Featured< UniqueEnd< int, ToolType > > int_value; // OK
////    Featured< const UniqueEnd< int, ToolType > > int_value; // OK
////    Featured< UniqueEnd< const int, ToolType > > int_value; // ERROR

////    Featured< SharedEnd< int, ToolType >  > int_value; // OK
////    const Featured< SharedEnd< int, ToolType >  > int_value; // OK
////    Featured< const SharedEnd< int, ToolType >  > int_value; // OK
////    Featured< SharedEnd< const int, ToolType > > int_value; // ERROR

//    (*&int_value) += 12345;
//    *&asConst(int_value);

//    Featured< ::std::string > name;
//    name = "Hello";

//    Featured< Featured< Item, Tool >, ImplicitTool > item;
//    item->m_int = 67890;
//    item->m_string = "Item";
//    item->m_unique_string = "Changed unique string";
//    item->m_shared_string = "Change shared string";
//    item->m_refer_string = "Change refer string";

//    ::std::cout
//        << asConst(int_value) << ::std::endl
//        << asConst(name) << ::std::endl
//        << asConst(item)->m_int << ::std::endl
//        << asConst(item)->m_string << ::std::endl
//        << asConst(item)->m_unique_string << ::std::endl
//        << asConst(item)->m_shared_string << ::std::endl
//        << asConst(item)->m_refer_string << ::std::endl
//    ;

//    Featured< Featured< Item, Tool >, ImplicitTool > other_item;// = item;
////    (*&other_item ).m_refer = (*&asConst(item ).m_string;
//    other_item = item;

//    other_item->m_int = 1;
//    other_item->m_string = "Word";

//    ::std::cout
//        << asConst(other_item)->m_int << ::std::endl
//        << asConst(other_item)->m_string << ::std::endl
////        << asConst(other_item)->m_string_refer << ::std::endl
//    ;

    return 0;
}


//// Альтернативный синтаксис
//void syntaxExample ()
//{
//    struct ExampleType
//    {
//        int m_member;
//    };

//    Featured< ExampleType > value;

//    // Синтаксис подобный значению:
//    // распределяет значение в соответствии с типом value.
//    value = ExampleType();

//    // Доступ к неконстантному внутреннему значению
//    (*&value);  // совместим с типом отличным от Featured

//    // Доступ к константному внутреннему значению
//    (*&asConst(value)); // совместим с типом отличным от Featured

//    // Доступ к неконстантному члену класса
//    value->m_member;    // не совместим с типом отличным от Featured
//    (&value)->m_member; // совместим с типом отличным от Featured
//    (*&value).m_member; // совместим с типом отличным от Featured

//    // Доступ к константному члену класса
//    asConst(value)->m_member;      // не совместим с типом отличным от Featured
//    (&asConst(value))->m_member;   // совместим с типом отличным от Featured
//    (*&asConst(value)).m_member;   // совместим с типом отличным от Featured
//}

//void operatorExample ()
//{
//    Featured< int > test_value;
//    Featured< int > left_value;
//    Featured< int > right_value;
//    int cpp_value = 1;

//    left_value + right_value;
//    cpp_value + right_value;
//    left_value + cpp_value;

//    left_value - right_value;
//    cpp_value - right_value;
//    left_value - cpp_value;

//    +test_value;
//    -test_value;

//    left_value * right_value;
//    cpp_value * right_value;
//    left_value * cpp_value;

//    left_value / right_value;
//    cpp_value / right_value;
//    left_value / cpp_value;

//    left_value % right_value;
//    cpp_value % right_value;
//    left_value % cpp_value;

//    ++test_value;
//    --test_value;
//    test_value++;
//    test_value--;

//    left_value == right_value;
//    cpp_value == right_value;
//    left_value == cpp_value;

//    left_value != right_value;
//    cpp_value != right_value;
//    left_value != cpp_value;

//    left_value > right_value;
//    cpp_value > right_value;
//    left_value > cpp_value;

//    left_value < right_value;
//    cpp_value < right_value;
//    left_value < cpp_value;

//    left_value >= right_value;
//    cpp_value >= right_value;
//    left_value >= cpp_value;

//    left_value <= right_value;
//    cpp_value <= right_value;
//    left_value <= cpp_value;

//    !test_value;

//    left_value && right_value;
//    cpp_value && right_value;
//    left_value && cpp_value;

//    left_value || right_value;
//    cpp_value || right_value;
//    left_value || cpp_value;

//    ~test_value;

//    left_value & right_value;
//    cpp_value & right_value;
//    left_value & cpp_value;

//    left_value | right_value;
//    cpp_value | right_value;
//    left_value | cpp_value;

//    left_value ^ right_value;
//    cpp_value ^ right_value;
//    left_value ^ cpp_value;

//    left_value << right_value;
//    cpp_value << right_value;
//    left_value << cpp_value;

//    left_value >> right_value;
//    cpp_value >> right_value;
//    left_value >> cpp_value;

//    left_value += right_value;
//    cpp_value += right_value;
//    left_value += cpp_value;

//    left_value -= right_value;
//    cpp_value -= right_value;
//    left_value -= cpp_value;

//    left_value *= right_value;
//    cpp_value *= right_value;
//    left_value *= cpp_value;

//    left_value /= right_value;
//    cpp_value /= right_value;
//    left_value /= cpp_value;

//    left_value %= right_value;
//    cpp_value %= right_value;
//    left_value %= cpp_value;

//    left_value &= right_value;
//    cpp_value &= right_value;
//    left_value &= cpp_value;

//    left_value |= right_value;
//    cpp_value |= right_value;
//    left_value |= cpp_value;

//    left_value ^= right_value;
//    cpp_value ^= right_value;
//    left_value ^= cpp_value;

//    left_value <<= right_value;
//    cpp_value <<= right_value;
//    left_value <<= cpp_value;

//    left_value >>= right_value;
//    cpp_value >>= right_value;
//    left_value >>= cpp_value;
//}

struct Foo {
    void Bar() { // do something
    }
};

//template <typename TOwner, void(TOwner::*func)()>
template <typename _Refer, typename _Function >
void Call( _Refer p, _Function func ) {
    (p.*func)();
}
int testFoo() {
    Foo a;
    Call<Foo &>(a, &Foo::Bar);
    return 0;
}

//using Res = ::std::result_of_t< double & (std::vector<double>&, int&)>;

template < typename _Refer, typename ... _Arguments >
decltype(auto) squareBrackets ( _Refer refer, _Arguments && ... arguments )
{
    return refer.operator[] ( ::std::forward< _Arguments >( arguments ) ... );
}

//template < typename _Refer, typename ... _Arguments >
//int squareBrackets1 ( _Refer refer, _Arguments && ... arguments )
//{
//    return 1;
//}

//using Container = ::std::vector< double >;

//template < typename _Refer, typename _Function, typename ... _Arguments >
//decltype(auto) execute ( _Refer refer, _Function function, _Arguments && ... arguments )
//{
//    printTypeOf< _Refer >();
//    printTypeOf< Container & >();
//    printTypeOf< _Function >();
//    printTypeOf( function );
//    using R = ::std::result_of_t< decltype( function )( _Refer, _Arguments && ... ) >;
//    using F = ::std::remove_reference_t< R >;
//    printTypeOf< R >();
//    printTypeOf< F >();

//    //using F = ::std::remove_reference_t< R >;
//    auto f = function( refer, ::std::forward< _Arguments >( arguments ) ... );
//    return f;
//}

//void testResultOf ()
//{
//    using R = ::std::result_of_t< decltype( squareBrackets< Container &, size_t > )&( Container &, size_t ) >;
//    using F = ::std::remove_reference_t< R >;
//    auto func = &squareBrackets< Container &, size_t >;

//    using R1 = ::std::result_of_t< decltype( func )( Container &, size_t ) >;
//    using F1 = ::std::remove_reference_t< R1 >;

//    printTypeOf< const double & >();
//    printTypeOf< R >();
//    printTypeOf< F >();
//    printTypeOf< R1 >();
//    printTypeOf< F1 >();
//    printTypeOf< decltype( func ) >();

//    Container container = { 1, 2, 3 };
//    F f = squareBrackets< Container &, size_t >( container, size_t() );
//    F1 f1 = execute< Container & >( container, func, size_t() );


//    //using R1 = ::std::result_of_t< decltype( squareBrackets1< Container &, int > )&( Container &, int ) >;
//}

void testFeaturedValue ()
{
    { Featured< double > value; }
    { Featured< double > value( double() ); }
    { Featured< double > value = double(); (void)value; }
    { Featured< double > value( Featured< double >() ); }
    { Featured< double > value = Featured< double >(); (void)value; }
    { Featured< double > value( Featured< int >() ); }
    { Featured< double > value = Featured< int >(); (void)value; }
}

void testFeaturedContainer ()
{
    using Container = ::std::vector< double >;
    //using Container = Featured< ::std::vector< double >, Implicit::SharedTool >;

    { Featured< Container > value; }
    { Featured< Container > value( Container() ); }
    { Featured< Container > value = Container(); (void)value; }
    {
        Featured< Container > value;
        for ( int i = 0; i < 10; ++i )
            value->push_back( i );

        for ( int i = 0; i < 10; ++i )
            value[ i ] = i;//asConst( value )[ 9 - i ];

        Featured< Container >()[0];
        asConst( Featured< Container >() )[0];
    }
}

//#include <iostream>
//#include <string>

//using ImplicitTool = Shared::ImplicitTool;
////using RelationTool = Shared::RelationTool;
//using Tool = ::Cpp::Inplace::DefaultTool;

//struct Item
//{
//    using Int = int;
//    using String = ::std::string;
//    using UniqueString = CompositeEnd< String >;
//    using SharedString = SharedEnd< String >;
//    using ReferString = NoneEnd< String >;

//    Featured< Int > m_int;
//    Featured< String > m_string;
//    Featured< UniqueString > m_unique_string;
//    Featured< SharedString > m_shared_string;
//    Featured< ReferString > m_refer_string;

//    Item ()
//        : m_int()
//    , m_string()
//    , m_unique_string( UniqueString::make( "Unique string" ) )
//    , m_shared_string( SharedString::make( "Shared string" ) )
//    , m_refer_string( ReferString::refer( m_unique_string ) )
//    {
//    }

//    Item ( const Item & other )
//        : m_int( other.m_int )
//    , m_string( other.m_string )
//    , m_unique_string( UniqueString::copy( other.m_unique_string ) )
//    , m_shared_string( SharedString::share( other.m_shared_string ) )
//    , m_refer_string( ReferString::refer( other.m_refer_string ) )
//    {
//    }

//    Item ( Item && other ) = default;

//    Item & operator = ( const Item & other )
//    {
//        m_int = other.m_int;
//        m_string = other.m_string;
//        m_unique_string = other.m_unique_string;
//        m_shared_string = other.m_shared_string;
//        m_refer_string = other.m_refer_string;
//        return *this;
//    }

////    ReturnUpdate< StringRef > stringRefer ();
////    ReturnRead< StringRef > stringRefer () const;
//};

////using ItemRawRefer = ReferEnd< Item, ::Cpp::Raw::RelationTool >;

////using BaseItemRefer = ReferEnd< BaseItem, RelationTool >;
////using ItemRefer = ReferEnd< Item, RelationTool >;
////using ItemUnique = UniqueEnd< Item, RelationTool >;
////using ItemShared = SharedEnd< Item, RelationTool >;

////using ItemAggregation = AggregationEnd< Item, RelationTool >;
////using ItemAssociation = AssociationEnd< Item, RelationTool >;

////using BaseItemAggregation = AggregationEnd< BaseItem, RelationTool >;
////using BaseItemAssociation = AssociationEnd< BaseItem, RelationTool >;


////void testItem ()
////{
////    ItemShared shared_item = ItemShared::make(); // make new shared end and new value
////    ItemShared shared_other_item = ItemShared::share( shared_item ); // copy shared end | share value
////    ItemUnique unique_item = ItemUnique::make(); // make new unique end and new value

//////    ItemRefer unique_refer_item = ItemRefer::refer( unique_item );
//////    ItemRefer shared_refer_item = ItemRefer::refer( shared_item );
//////    ItemRefer refer_refer_item = ItemRefer::refer( unique_refer_item );
//////    BaseItemRefer base_refer_item = BaseItemRefer::refer( shared_other_item );

////    ::std::cout
////        << "OK" << ::std::endl;
////}

//extern void testAllTool ();
//extern void testRelation ();

//template < typename _Type >
//void foo ( const _Type & ) { ::std::cout << ::std::endl; }

//template < typename _Type >
//void overloaded( _Type const & ) { ::std::cout << "by lvalue" << ::std::endl; }

//template < typename _Type >
//void overloaded( _Type && ) { ::std::cout << "by rvalue" << ::std::endl; }

//struct TestType
//{
//    ::std::string m_string;


//    TestType () : m_string() { ::std::cout << "construct default" << ::std::endl; }
//    TestType ( ::std::string && other ) : m_string( other ) { ::std::cout << "construct from string && "; overloaded( other ); }
//    TestType ( const ::std::string & other ) : m_string( other ) { ::std::cout << "construct from string & "; overloaded( other ); }

//    TestType & operator = ( ::std::string && other ) { ::std::cout << "operate from string && "; overloaded( other ); return *this; }
//    TestType & operator = ( const ::std::string & other ) { ::std::cout << "operate from string & "; overloaded( other ); return *this; }

//    TestType & operator = ( TestType && other ) { ::std::cout << "operate from TestType && "; overloaded( other ); return *this; }
//    TestType & operator = ( const TestType & other ) { ::std::cout << "operate from TestType & "; overloaded( other ); return *this; }

//    TestType ( TestType && other ) : m_string( ::std::forward< ::std::string >( other.m_string ) ) { ::std::cout << "construct from TestType && "; overloaded( other ); }
//    TestType ( const TestType & other ) : m_string( ::std::forward< const ::std::string & >( other.m_string ) ) { ::std::cout << "construct from TestType & "; overloaded( other ); }
//};

//void testConstructor ()
//{
////    using TestFeatured = Featured< TestType >;
//    using TestFeatured = Featured< Optimal< TestType > >;

//    // Конструктор без инициализации
//    {
//        TestFeatured value( InitializeType::NotInitialized );
//        foo( value );
//    }

//    // Конструктор по умолчанию
//    {
//        TestFeatured value;
//        foo( value );
//    }

//    // Конструктор инициализации по значению и соответсвующий оператор равенства
//    {
//        TestFeatured value( "Hello!" );
//        value = ::std::string( "Hello Memory!" );
//        foo( value );
//    }

//    // Конструктор инициализации по копии значения и соответсвующий оператор равенства
//    {
//        const TestType first_text = ::std::string( "Hello!" );
//        const TestType second_text = ::std::string( "Hello Memory!" );
//        TestFeatured value( first_text );
//        value = second_text;
//        foo( value );
//    }

//    // Конструктор перемещения и соответсвующий оператор равенства
//    {
//        TestFeatured first( "Move test" );
//        TestFeatured second( ::std::move( first ) );
//        first = ::std::move( second );
//        foo( first );
//    }

//    // Конструктор копирования и соответсвующий оператор равенства
//    {
//        TestFeatured first( "Copy test" );
//        TestFeatured second( first );
//        second = first;
//        foo( first );
//    }
//}


//void testRelation ()
//{
//    // Значения определенного типа
////    using UniqueInt = UniqueEnd< int, ::Cpp::Inplace::DefaultTool >;
////    using SharedInt = SharedEnd< int, ::Cpp::Inplace::DefaultTool >;
//    using UniqueInt = Featured< int, ::Cpp::Inplace::DefaultTool >;
//    using SharedInt = Featured< int, ::Cpp::Inplace::DefaultTool >;

//    Featured< UniqueInt > unique_int;
//    Featured< SharedInt > shared_int;

//    shared_int = unique_int; // равенство на уровне внутренних значений.
//}

//extern void testMemberOperators ();

