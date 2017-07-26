#include <iostream>
#include <string>
#include "memory.h"
#include "BaseType.h"
#include "DerivedType.h"

using ImplicitTool = ::Std::Shared::ImplicitTool;
//using RelationTool = ::Std::Shared::RelationTool;
using ValueTool = ::Cpp::Inplace::InplaceTool;

struct Item
{
    using Int = int;
    using String = ::std::string;
//    using StringRef = ::std::reference_wrapper< String >;
//    using Int = SharedEnd< int, RelationTool >;
//    using String = UniqueEnd< ::std::string, RelationTool  >;
//    using StringRef = ReferEnd< ::std::string, RelationTool  >;

    Property< Int > m_int;
    Property< String > m_string;
//    Property< StringRef > m_string_refer;

//    Item ()
//    : m_int() // создано значение
//    , m_string() // создано значение
//    , m_string_refer() // ничего не создано
//    {
//    }

//    Item ( const Item & other )
//    : m_int( other.m_int ) // копирование
//    , m_string( other.m_string ) // копирование
//    , m_string_refer() // копирование
//    {
//    }

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
extern void testBaseDerived ();
extern void testTrait ();
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
    //using TestVariable = Instance< TestType, ::Cpp::Inplace::InplaceTool >;
    using TestVariable = Variable< TestType >;

    // Конструктор без инициализации
    {
        TestVariable value( NotInitialized );
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
        value = "Hello Memory!";
        foo( value );
    }

    // Конструктор инициализации по копии значения и соответсвующий оператор равенства
    {
        const TestType first_text = ::std::string( "Hello!" );
        const TestType second_text = ::std::string( "Hello Memory!" );
        TestVariable value( first_text );
        // TODO: операция присвоения не должна приводить к созданию нового экземпляра значения
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
//    using UniqueInt = UniqueEnd< int, ::Cpp::Inplace::InplaceTool >;
//    using SharedInt = SharedEnd< int, ::Cpp::Inplace::InplaceTool >;
    using UniqueInt = Instance< int, ::Cpp::Inplace::InplaceTool >;
    using SharedInt = Instance< int, ::Cpp::Inplace::InplaceTool >;

    Variable< UniqueInt > unique_int;
    Variable< SharedInt > shared_int;

    shared_int = unique_int; // равенство на уровне внутренних значений.
}

int main ( int /*argc*/, char ** /*argv*/ )
{
    testConstructor();
    testBaseDerived();
    testAllTool();
    testTrait();
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

    ( **int_value ) += 12345;
    **cnst( int_value );

    Variable< ::std::string > name;
    name = "Hello";

    Variable< Instance< Item, ValueTool >, ImplicitTool > item;
    guard( item ); // ОК
    item->m_int = 67890;
    item->m_string = "Item";

    ::std::cout
        << **cnst( int_value ) << ::std::endl
        << **cnst( name ) << ::std::endl
        << **( cnst( item )->m_int ) << ::std::endl
        << **( cnst( item )->m_string ) << ::std::endl
    ;

    Variable< Instance< Item, ValueTool >, ImplicitTool > other_item;// = item;
//    vGet( other_item ).m_refer = cGet( item ).m_string;

    other_item->m_int = 1;
    other_item->m_string = "Word";

    ::std::cout
        << **cguard( other_item )->m_int << ::std::endl
        << **cguard( other_item )->m_string << ::std::endl
//        << **cguard( other_item )->m_string_refer << ::std::endl
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
    // распределяет значение в соответсвии с типом value.
    value = ExampleType();

    // Доступ к неконстантному внутреннему значению
    value.guard().access();
    *value.guard();
    guard(value).access();      // совместим с любым типом (*)
    *guard(value);              // совместим с любым типом
    (**value);

    vGet(value);                // реализуется через макрос (!)

    // Доступ к константному вутреннему значению
    value.cguard().access();
    *value.cguard();
    cguard(value).access();     // совместим с любым типом (*)
    cGet(value);                // то же, но реализуется через макрос (*!)
    *cguard(value);             // совместим с любым типом

    value.cnst().guard().access();
    *value.cnst().guard();
    guard(value.cnst()).access();
    *guard(value.cnst());
    (**value.cnst());

    cnst(value).guard().access();
    *cnst(value).guard();
    guard(cnst(value)).access();    // совместим с любым типом
    *guard(cnst(value));            // совместим с любым типом
    (**cnst(value));

    // Доступ к неконстантному члену класса
    value->m_member;

    value.guard().access().m_member;
    value.guard()->m_member;
    (*value.guard()).m_member;

    guard(value).access().m_member; // совместим с любым типом (*)
    vGet(value).m_member;           // то же, но реализуется через макрос (*!)
    guard(value)->m_member;         // совместим с любым типом
    (*guard(value)).m_member;       // совместим с любым типом

    (*value).access().m_member;
    (*value)->m_member;
    (**value).m_member;

    // Доступ к константному члену класса
    value.cnst()->m_member;
    cnst(value)->m_member;

    value.cguard().access().m_member;
    value.cguard()->m_member;
    (*value.cguard()).m_member;

    cguard(value).access().m_member;    // совместим с любым типом (*)
    cGet(value).m_member;               // то же, но реализуется через макрос (*!)
    cguard(value)->m_member;            // совместим с любым типом
    (*cguard(value)).m_member;          // совместим с любым типом

    value.cnst().guard().access().m_member;
    value.cnst().guard()->m_member;
    (*value.cnst().guard()).m_member;

    guard(value.cnst()).access().m_member;
    guard(value.cnst())->m_member;
    (*guard(value.cnst())).m_member;

    (*value.cnst()).access().m_member;
    (*value.cnst())->m_member;
    (**value.cnst()).m_member;

    cnst(value).guard().access().m_member;
    cnst(value).guard()->m_member;
    (*cnst(value).guard()).m_member;

    guard(cnst(value)).access().m_member;   // совместим с любым типом
    guard(cnst(value))->m_member;           // совместим с любым типом
    (*guard(cnst(value))).m_member;         // совместим с любым типом

    (*cnst(value)).access().m_member;
    (*cnst(value))->m_member;
    (**cnst(value)).m_member;
}
