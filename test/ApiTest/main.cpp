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
void foo ( const _Type & ) {}

void testConstructor ()
{
    using TestType = ::std::string;

    // Конструктор без инициализации
    {
        Variable< TestType > value( NotInitialized );
        foo( value );
    }

    // Конструктор по умолчанию
    {
        Variable< TestType > value;
        foo( value );
    }

    // Конструктор инициализации по значению и соответсвующий оператор равенства
    {
        Variable< TestType > value( "Hello!" );
        value = "Hello Memory!";
    }

    // Конструктор инициализации по копии значения и соответсвующий оператор равенства
    {
        const char first_text[] = "Hello!";
        const char second_text[] = "Hello Memory!";
        Variable< TestType > value( first_text );
        value = second_text;
    }

    // Конструктор перемещения и соответсвующий оператор равенства
    {
        Variable< TestType > first;
        Variable< TestType > second( ::std::move( first ) );
        first = ::std::move( second );
        foo( second );
    }

    // Конструктор копирования и соответсвующий оператор равенства
    {
        Variable< TestType > first;
        Variable< TestType > second( first );
        second = first;
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

    vGet( int_value ) += 12345;
    cGet( int_value );

    Variable< ::std::string > name;
    vGet( name ) = "Hello";

    Variable< Instance< Item, ValueTool >, ImplicitTool > item;
    vGet( item ); // ОК
    vGet( vGet( item ).m_int ) = 67890;
    vGet( vGet( item ).m_string ) = "Item";

    ::std::cout
        << cGet( int_value ) << ::std::endl
        << cGet( name ) << ::std::endl
        << cGet( cGet( item ).m_int ) << ::std::endl
        << cGet( cGet( item ).m_string ) << ::std::endl
    ;

    Variable< Instance< Item, ValueTool >, ImplicitTool > other_item;// = item;
//    vGet( other_item ).m_refer = cGet( item ).m_string;

    vGet( vGet( other_item ).m_int ) = 1;
    vGet( vGet( other_item ).m_string ) = "Word";

    ::std::cout
        << cGet( vGet( other_item ).m_int ) << ::std::endl
        << cGet( cGet( other_item ).m_string ) << ::std::endl
//        << cGet( cGet( other_item ).m_string_refer ) << ::std::endl
    ;

    return 0;
}
