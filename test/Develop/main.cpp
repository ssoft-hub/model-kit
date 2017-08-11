#ifdef _MSC_VER
#pragma warning( disable : 4552 4553 )
#endif

#include <ModelKit.h>
#include <iostream>
#include <string>

using ImplicitTool = ::Std::Shared::ImplicitTool;
//using RelationTool = ::Std::Shared::RelationTool;
using ValueTool = ::Cpp::Inplace::DefaultTool;

struct Item
{
    using Int = int;
    using String = ::std::string;
    using UniqueString = CompositeEnd< String >;
    using SharedString = SharedEnd< String >;
    using ReferString = NoneEnd< String >;

    Property< Int > m_int;
    Property< String > m_string;
    Property< UniqueString > m_unique_string;
    Property< SharedString > m_shared_string;
    Property< ReferString > m_refer_string;

    Item ()
    : m_int()
    , m_string()
    , m_unique_string( UniqueString::make( "Unique string" ) )
    , m_shared_string( SharedString::make( "Shared string" ) )
    , m_refer_string( ReferString::refer( m_unique_string ) )
    {
    }

    Item ( const Item & other )
    : m_int( other.m_int )
    , m_string( other.m_string )
    , m_unique_string( UniqueString::copy( other.m_unique_string ) )
    , m_shared_string( SharedString::share( other.m_shared_string ) )
    , m_refer_string( ReferString::refer( other.m_refer_string ) )
    {
    }

    Item ( Item && other ) = default;

    Item & operator = ( const Item & other )
    {
        m_int = other.m_int;
        m_string = other.m_string;
        m_unique_string = other.m_unique_string;
        m_shared_string = other.m_shared_string;
        m_refer_string = other.m_refer_string;
        return *this;
    }

//    ReturnUpdate< StringRef > stringRefer ();
//    ReturnRead< StringRef > stringRefer () const;
};

//using ItemRawRefer = ReferEnd< Item, ::Cpp::Raw::RelationTool >;

//using BaseItemRefer = ReferEnd< BaseItem, RelationTool >;
//using ItemRefer = ReferEnd< Item, RelationTool >;
//using ItemUnique = UniqueEnd< Item, RelationTool >;
//using ItemShared = SharedEnd< Item, RelationTool >;

//using ItemAggregation = AggregationEnd< Item, RelationTool >;
//using ItemAssociation = AssociationEnd< Item, RelationTool >;

//using BaseItemAggregation = AggregationEnd< BaseItem, RelationTool >;
//using BaseItemAssociation = AssociationEnd< BaseItem, RelationTool >;


//void testItem ()
//{
//    ItemShared shared_item = ItemShared::make(); // make new shared end and new value
//    ItemShared shared_other_item = ItemShared::share( shared_item ); // copy shared end | share value
//    ItemUnique unique_item = ItemUnique::make(); // make new unique end and new value

////    ItemRefer unique_refer_item = ItemRefer::refer( unique_item );
////    ItemRefer shared_refer_item = ItemRefer::refer( shared_item );
////    ItemRefer refer_refer_item = ItemRefer::refer( unique_refer_item );
////    BaseItemRefer base_refer_item = BaseItemRefer::refer( shared_other_item );

//    ::std::cout
//        << "OK" << ::std::endl;
//}

extern void testAllTool ();
extern void testRelation ();

template < typename _Type >
void foo ( const _Type & ) { ::std::cout << std::endl; }

template < typename _Type >
void overloaded( _Type const & ) { ::std::cout << "by lvalue" << std::endl; }

template < typename _Type >
void overloaded( _Type && ) { ::std::cout << "by rvalue" << std::endl; }

struct TestType
{
    ::std::string m_string;


    TestType () : m_string() { ::std::cout << "construct default" << ::std::endl; }
    TestType ( ::std::string && other ) : m_string( other ) { ::std::cout << "construct from string && "; overloaded( other ); }
    TestType ( const ::std::string & other ) : m_string( other ) { ::std::cout << "construct from string & "; overloaded( other ); }

    TestType & operator = ( ::std::string && other ) { ::std::cout << "operate from string && "; overloaded( other ); return *this; }
    TestType & operator = ( const ::std::string & other ) { ::std::cout << "operate from string & "; overloaded( other ); return *this; }

    TestType & operator = ( TestType && other ) { ::std::cout << "operate from TestType && "; overloaded( other ); return *this; }
    TestType & operator = ( const TestType & other ) { ::std::cout << "operate from TestType & "; overloaded( other ); return *this; }

    TestType ( TestType && other ) : m_string( ::std::forward< ::std::string >( other.m_string ) ) { ::std::cout << "construct from TestType && "; overloaded( other ); }
    TestType ( const TestType & other ) : m_string( ::std::forward< const ::std::string & >( other.m_string ) ) { ::std::cout << "construct from TestType & "; overloaded( other ); }
};

void testConstructor ()
{
//    using TestVariable = Variable< TestType >;
    using TestVariable = Variable< OptimalType< TestType > >;

    // Конструктор без инициализации
    {
        TestVariable value( InitializeType::NotInitialized );
        foo( value );
    }

    // Конструктор по умолчанию
    {
        TestVariable value;
        foo( value );
    }

    // Конструктор инициализации по значению и соответсвующий оператор равенства
    {
        TestVariable value( "Hello!" );
        value = ::std::string( "Hello Memory!" );
        foo( value );
    }

    // Конструктор инициализации по копии значения и соответсвующий оператор равенства
    {
        const TestType first_text = ::std::string( "Hello!" );
        const TestType second_text = ::std::string( "Hello Memory!" );
        TestVariable value( first_text );
        value = second_text;
        foo( value );
    }

    // Конструктор перемещения и соответсвующий оператор равенства
    {
        TestVariable first( "Move test" );
        TestVariable second( ::std::move( first ) );
        first = ::std::move( second );
        foo( first );
    }

    // Конструктор копирования и соответсвующий оператор равенства
    {
        TestVariable first( "Copy test" );
        TestVariable second( first );
        second = first;
        foo( first );
    }
}


void testRelation ()
{
    // Значения определенного типа
//    using UniqueInt = UniqueEnd< int, ::Cpp::Inplace::DefaultTool >;
//    using SharedInt = SharedEnd< int, ::Cpp::Inplace::DefaultTool >;
    using UniqueInt = Instance< int, ::Cpp::Inplace::DefaultTool >;
    using SharedInt = Instance< int, ::Cpp::Inplace::DefaultTool >;

    Variable< UniqueInt > unique_int;
    Variable< SharedInt > shared_int;

    shared_int = unique_int; // равенство на уровне внутренних значений.
}

int main ( int /*argc*/, char ** /*argv*/ )
{
    testConstructor();
    testAllTool();
    testRelation();


//    int int_value; // OK
//    const int int_value = 0; // ERROR

    Variable< int > int_value; // OK
//    const Variable< int > int_value = 0; // ERROR
//    Variable< const int > int_value = 0; // ERROR

//    Variable< Instance< int, ImplicitTool > > int_value; // OK
//    const Variable< Instance< int, ImplicitTool > > int_value; // ERROR
//    Variable< const Instance< int, ImplicitTool > > int_value; // ERROR
//    Variable< Instance< const int, ImplicitTool > > int_value; // ERROR

//    Variable< UniqueEnd< int, ToolType > > int_value( UniqueEnd< int, ToolType >::make( 10 ) ); // OK
//    const Variable< UniqueEnd< int, ToolType > > int_value; // OK
//    Variable< const UniqueEnd< int, ToolType > > int_value; // OK
//    Variable< UniqueEnd< const int, ToolType > > int_value; // ERROR

//    Variable< SharedEnd< int, ToolType >  > int_value; // OK
//    const Variable< SharedEnd< int, ToolType >  > int_value; // OK
//    Variable< const SharedEnd< int, ToolType >  > int_value; // OK
//    Variable< SharedEnd< const int, ToolType > > int_value; // ERROR

    (*&int_value) += 12345;
    *&cnst(int_value);

    Variable< ::std::string > name;
    name = "Hello";

    Variable< Instance< Item, ValueTool >, ImplicitTool > item;
    item->m_int = 67890;
    item->m_string = "Item";
    item->m_unique_string = "Changed unique string";
    item->m_shared_string = "Change shared string";
    item->m_refer_string = "Change refer string";

    ::std::cout
        << cnst(int_value) << ::std::endl
        << cnst(name) << ::std::endl
        << cnst(item)->m_int << ::std::endl
        << cnst(item)->m_string << ::std::endl
        << cnst(item)->m_unique_string << ::std::endl
        << cnst(item)->m_shared_string << ::std::endl
        << cnst(item)->m_refer_string << ::std::endl
    ;

    Variable< Instance< Item, ValueTool >, ImplicitTool > other_item;// = item;
//    (*&other_item ).m_refer = (*&cnst(item ).m_string;
    other_item = item;

    other_item->m_int = 1;
    other_item->m_string = "Word";

    ::std::cout
        << cnst(other_item)->m_int << ::std::endl
        << cnst(other_item)->m_string << ::std::endl
//        << cnst(other_item)->m_string_refer << ::std::endl
    ;

    return 0;
}


// Альтернативный синтаксис
void syntaxExample ()
{
    struct ExampleType
    {
        int m_member;
    };

    Variable< ExampleType > value;

    // Синтаксис подобный значению:
    // распределяет значение в соответствии с типом value.
    value = ExampleType();

    // Доступ к неконстантному внутреннему значению
    (*&value);  // совместим с типом отличным от Instance

    // Доступ к константному внутреннему значению
    (*&cnst(value)); // совместим с типом отличным от Instance

    // Доступ к неконстантному члену класса
    value->m_member;    // не совместим с типом отличным от Instance
    (&value)->m_member; // совместим с типом отличным от Instance
    (*&value).m_member; // совместим с типом отличным от Instance

    // Доступ к константному члену класса
    cnst(value)->m_member;      // не совместим с типом отличным от Instance
    (&cnst(value))->m_member;   // совместим с типом отличным от Instance
    (*&cnst(value)).m_member;   // совместим с типом отличным от Instance
}

void operatorExample ()
{
    Variable< int > test_value;
    Variable< int > left_value;
    Variable< int > right_value;
    int cpp_value = 1;

    left_value + right_value;
    cpp_value + right_value;
    left_value + cpp_value;

    left_value - right_value;
    cpp_value - right_value;
    left_value - cpp_value;

    +test_value;
    -test_value;

    left_value * right_value;
    cpp_value * right_value;
    left_value * cpp_value;

    left_value / right_value;
    cpp_value / right_value;
    left_value / cpp_value;

    left_value % right_value;
    cpp_value % right_value;
    left_value % cpp_value;

    ++test_value;
    --test_value;
    test_value++;
    test_value--;

    left_value == right_value;
    cpp_value == right_value;
    left_value == cpp_value;

    left_value != right_value;
    cpp_value != right_value;
    left_value != cpp_value;

    left_value > right_value;
    cpp_value > right_value;
    left_value > cpp_value;

    left_value < right_value;
    cpp_value < right_value;
    left_value < cpp_value;

    left_value >= right_value;
    cpp_value >= right_value;
    left_value >= cpp_value;

    left_value <= right_value;
    cpp_value <= right_value;
    left_value <= cpp_value;

    !test_value;

    left_value && right_value;
    cpp_value && right_value;
    left_value && cpp_value;

    left_value || right_value;
    cpp_value || right_value;
    left_value || cpp_value;

    ~test_value;

    left_value & right_value;
    cpp_value & right_value;
    left_value & cpp_value;

    left_value | right_value;
    cpp_value | right_value;
    left_value | cpp_value;

    left_value ^ right_value;
    cpp_value ^ right_value;
    left_value ^ cpp_value;

    left_value << right_value;
    cpp_value << right_value;
    left_value << cpp_value;

    left_value >> right_value;
    cpp_value >> right_value;
    left_value >> cpp_value;

    left_value += right_value;
    cpp_value += right_value;
    left_value += cpp_value;

    left_value -= right_value;
    cpp_value -= right_value;
    left_value -= cpp_value;

    left_value *= right_value;
    cpp_value *= right_value;
    left_value *= cpp_value;

    left_value /= right_value;
    cpp_value /= right_value;
    left_value /= cpp_value;

    left_value %= right_value;
    cpp_value %= right_value;
    left_value %= cpp_value;

    left_value &= right_value;
    cpp_value &= right_value;
    left_value &= cpp_value;

    left_value |= right_value;
    cpp_value |= right_value;
    left_value |= cpp_value;

    left_value ^= right_value;
    cpp_value ^= right_value;
    left_value ^= cpp_value;

    left_value <<= right_value;
    cpp_value <<= right_value;
    left_value <<= cpp_value;

    left_value >>= right_value;
    cpp_value >>= right_value;
    left_value >>= cpp_value;
}
