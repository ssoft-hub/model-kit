//#ifdef _MSC_VER
//#pragma warning( disable : 4552 4553 )
//#endif

#include <ModelKit.h>
#include <type_traits>
#include <vector>
#include <iostream>
#include <cxxabi.h>
#include <memory>

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
void printTypeOf ( const _Type & )
{
    printTypeOf< _Type >();
}


template < typename _Type, typename _Other >
constexpr bool isSimilar ( _Type &&, _Other && )
{
    static_assert( is_similar< _Type, _Other >, "Error" );
    return is_similar< _Type, _Other >;
}


struct My
{
    void rvalueMethod () && {}
    void rvalueConstMethod () const && {}
    void lvalueMethod () & {}
    void lvalueConstMethod () const & {}
};

void compileTestInstanceTrancpatancy ()
{
    Instance< My > instance;
    My value;

    My().rvalueMethod();
    My().rvalueConstMethod();
    asConst( My() ).rvalueConstMethod();

    ::std::make_shared< My >()->lvalueMethod();

    // не работает (taking address of temporary)
    // так как значение не обязательно должно быть размещено в памяти
    // оно может быть размещено в регистрах ЦП, и как следствие,
    // не иметь адреса.
    //(*&My()).rvalueMethod();
    //(*&My()).rvalueConstMethod();
    //(*&asConst( My() ) ).rvalueConstMethod();

    // по похожим причинам нельзя через operator -> вызвать rvalue методы
    //ReferPointer< My && >( My() )->rvalueMethod();
    //ReferPointer< My && >( My() )->rvalueConstMethod();
    //asConst( ReferPointer< My && >( My() ) )->rvalueConstMethod();

    // как следствие, не работает
    //Instance< My >()->rvalueMethod();
    //Instance< My >()->rvalueConstMethod();
    //asConst( Instance< My >() )->rvalueConstMethod();

    //My().lvalueMethod(); // почему-то T&& не преобразуется к T&
    My().lvalueConstMethod();
    asConst( My() ).lvalueConstMethod();

    Instance< My >()->lvalueMethod();
    Instance< My >()->lvalueConstMethod();
    asConst( Instance< My >() )->lvalueConstMethod();

    instance->lvalueMethod();
    instance->lvalueConstMethod();
    value.lvalueMethod();
    value.lvalueConstMethod();

    isSimilar( Instance< My >(), My() );
    isSimilar( Instance< My >(), *&Instance< My >() );
    isSimilar( instance, value );
    isSimilar( instance, *&instance );
}

extern void testResultOf();
extern void testInstanceValue ();
extern void testInstanceContainer ();

extern void testConstructors ();
extern void testSameToolConstructors ();
extern void testDiffToolConstructors ();
extern void testBinaryOperators ();

extern void testFeaturing ();

int main ( int /*argc*/, char ** /*argv*/ )
{
    testConstructors ();
    testSameToolConstructors ();
    testDiffToolConstructors ();

    compileTestInstanceTrancpatancy();
    //testResultOf();
    testInstanceContainer();
    testBinaryOperators();
    testFeaturing();
//    testMemberOperators();
//    testConstructor();
//    testAllTool();
//    testRelation();


////    int int_value; // OK
////    const int int_value = 0; // ERROR

//    Instance< int > int_value; // OK
////    const Instance< int > int_value = 0; // ERROR
////    Instance< const int > int_value = 0; // ERROR

////    Instance< Instance< int, ImplicitTool > > int_value; // OK
////    const Instance< Instance< int, ImplicitTool > > int_value; // ERROR
////    Instance< const Instance< int, ImplicitTool > > int_value; // ERROR
////    Instance< Instance< const int, ImplicitTool > > int_value; // ERROR

////    Instance< UniqueEnd< int, ToolType > > int_value( UniqueEnd< int, ToolType >::make( 10 ) ); // OK
////    const Instance< UniqueEnd< int, ToolType > > int_value; // OK
////    Instance< const UniqueEnd< int, ToolType > > int_value; // OK
////    Instance< UniqueEnd< const int, ToolType > > int_value; // ERROR

////    Instance< SharedEnd< int, ToolType >  > int_value; // OK
////    const Instance< SharedEnd< int, ToolType >  > int_value; // OK
////    Instance< const SharedEnd< int, ToolType >  > int_value; // OK
////    Instance< SharedEnd< const int, ToolType > > int_value; // ERROR

//    (*&int_value) += 12345;
//    *&asConst(int_value);

//    Instance< ::std::string > name;
//    name = "Hello";

//    Instance< Instance< Item, Tool >, ImplicitTool > item;
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

//    Instance< Instance< Item, Tool >, ImplicitTool > other_item;// = item;
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

//    Instance< ExampleType > value;

//    // Синтаксис подобный значению:
//    // распределяет значение в соответствии с типом value.
//    value = ExampleType();

//    // Доступ к неконстантному внутреннему значению
//    (*&value);  // совместим с типом отличным от Instance

//    // Доступ к константному внутреннему значению
//    (*&asConst(value)); // совместим с типом отличным от Instance

//    // Доступ к неконстантному члену класса
//    value->m_member;    // не совместим с типом отличным от Instance
//    (&value)->m_member; // совместим с типом отличным от Instance
//    (*&value).m_member; // совместим с типом отличным от Instance

//    // Доступ к константному члену класса
//    asConst(value)->m_member;      // не совместим с типом отличным от Instance
//    (&asConst(value))->m_member;   // совместим с типом отличным от Instance
//    (*&asConst(value)).m_member;   // совместим с типом отличным от Instance
//}

//void operatorExample ()
//{
//    Instance< int > test_value;
//    Instance< int > left_value;
//    Instance< int > right_value;
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

void testInstanceValue ()
{
    { Instance< double > value; }
    { Instance< double > value( double() ); }
    { Instance< double > value = double(); (void)value; }
    { Instance< double > value( Instance< double >() ); }
    { Instance< double > value = Instance< double >(); (void)value; }
    { Instance< double > value( Instance< int >() ); }
    { Instance< double > value = Instance< int >(); (void)value; }
}

void testInstanceContainer ()
{
    using Container = ::std::vector< double >;
    //using Container = Instance< ::std::vector< double >, Implicit::SharedTool >;

    { Instance< Container > value; }
    { Instance< Container > value( Container() ); }
    { Instance< Container > value = Container(); (void)value; }
    {
        Instance< Container > container;
        for ( int i = 0; i < 10; ++i )
            container->push_back( i );

        auto v0 = container[5];
        auto v1 = asConst( container )[5];
        auto v2 = &(asConst( container )[5]);
        auto v3 = *v2;

        printTypeOf( v0 );
        printTypeOf( v1 );
        printTypeOf( v2 );
        printTypeOf( v3 );

        for ( int i = 0; i < 10; ++i )
            container[ i ] = asConst( container )[ 9 - i ];

        Instance< Container >()[0];
        asConst( Instance< Container >() )[0];
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

//    Instance< Int > m_int;
//    Instance< String > m_string;
//    Instance< UniqueString > m_unique_string;
//    Instance< SharedString > m_shared_string;
//    Instance< ReferString > m_refer_string;

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
////    using TestInstance = Instance< TestType >;
//    using TestInstance = Instance< Optimal< TestType > >;

//    // Конструктор без инициализации
//    {
//        TestInstance value( InitializeType::NotInitialized );
//        foo( value );
//    }

//    // Конструктор по умолчанию
//    {
//        TestInstance value;
//        foo( value );
//    }

//    // Конструктор инициализации по значению и соответсвующий оператор равенства
//    {
//        TestInstance value( "Hello!" );
//        value = ::std::string( "Hello Memory!" );
//        foo( value );
//    }

//    // Конструктор инициализации по копии значения и соответсвующий оператор равенства
//    {
//        const TestType first_text = ::std::string( "Hello!" );
//        const TestType second_text = ::std::string( "Hello Memory!" );
//        TestInstance value( first_text );
//        value = second_text;
//        foo( value );
//    }

//    // Конструктор перемещения и соответсвующий оператор равенства
//    {
//        TestInstance first( "Move test" );
//        TestInstance second( ::std::move( first ) );
//        first = ::std::move( second );
//        foo( first );
//    }

//    // Конструктор копирования и соответсвующий оператор равенства
//    {
//        TestInstance first( "Copy test" );
//        TestInstance second( first );
//        second = first;
//        foo( first );
//    }
//}


//void testRelation ()
//{
//    // Значения определенного типа
////    using UniqueInt = UniqueEnd< int, ::Cpp::Inplace::DefaultTool >;
////    using SharedInt = SharedEnd< int, ::Cpp::Inplace::DefaultTool >;
//    using UniqueInt = Instance< int, ::Cpp::Inplace::DefaultTool >;
//    using SharedInt = Instance< int, ::Cpp::Inplace::DefaultTool >;

//    Instance< UniqueInt > unique_int;
//    Instance< SharedInt > shared_int;

//    shared_int = unique_int; // равенство на уровне внутренних значений.
//}

//extern void testMemberOperators ();

