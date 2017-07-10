#include <iostream>
#include <string>
#include "memory.h"
#include "BaseType.h"
#include "DerivedType.h"

//using ImplicitTool = ::Std::Shared::ImplicitTool;
//using RelationTool = ::Std::Shared::RelationTool;

struct Item
{

////    using Int = int;
////    using String = ::std::string;
////    using StringRef = ::std::reference_wrapper< String >;
//    using Int = Implicit< int, ValueTool >;
//    using String = Implicit< ::std::string, ValueTool >;
//    using StringRef = String *;
////    using Int = UniqueEnd< int, RelationTool >;
////    using String = UniqueEnd< ::std::string, RelationTool  >;
////    using StringRef = ReferEnd< ::std::string, RelationTool  >;

//    Property< Int > m_int;
//    Property< String > m_string;
//    Property< StringRef > m_string_refer;

//    Item ()
//    : m_int( Int() )
//    , m_string( "" )
//    , m_string_refer()
//    {
//    }

//    Item ( const Item & other )
//    : m_int( other.m_int )
//    , m_string( other.m_string )
//    , m_string_refer()
//    {
//    }

////    ReturnUpdate< StringRef > stringRefer ();
////    ReturnRead< StringRef > stringRefer () const;
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

extern void testBaseDerived ();
extern void testAllTool ();

int main ( int /*argc*/, char ** /*argv*/ )
{
//    testBaseDerived();
//    testAllTool ();


    ::std::cout
        << "IsCompatible< Variable< int >, Variable< int > >::value" << ::std::endl
        << IsCompatible< Variable< int >, Variable< int > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< int >, Variable< double > >::value" << ::std::endl
        << IsCompatible< Variable< int >, Variable< double > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< BaseType >, Variable< DerivedType > >::value" << ::std::endl
        << IsCompatible< Variable< BaseType >, Variable< DerivedType > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< DerivedType >, Variable< BaseType > >::value" << ::std::endl
        << IsCompatible< Variable< DerivedType >, Variable< BaseType > >::value << ::std::endl;


    using FirstType = Instance< Instance< Instance< BaseType
        , ::Std::Unique::HeapTool >
        , ::Std::Shared::ImplicitTool >
        , ::Std::Mutex::AtomicTool >;

    using SecondType = Instance< Instance< Instance< DerivedType
        , ::Std::Unique::HeapTool >
        , ::Std::Shared::ImplicitTool >
        , ::Std::Mutex::AtomicTool >;

    ::std::cout
        << "IsCompatible< FirstType, SecondType >::value" << ::std::endl
        << IsCompatible< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< SecondType, FirstType >::value" << ::std::endl
        << IsCompatible< SecondType, FirstType >::value << ::std::endl;

    using FirstPartType = Instance< BaseType, ::Std::Unique::HeapTool >;
    using SecondPartType = Instance< DerivedType, ::Std::Unique::HeapTool >;

    ::std::cout
        << "IsPartOf< FirstPartType, FirstType >::value" << ::std::endl
        << IsPartOf< FirstPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondPartType, FirstType >::value" << ::std::endl
        << IsPartOf< SecondPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstPartType, SecondType >::value" << ::std::endl
        << IsPartOf< FirstPartType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondPartType, SecondType >::value" << ::std::endl
        << IsPartOf< SecondPartType, SecondType >::value << ::std::endl;


//    int int_value; // OK
//    const int int_value; // ERROR

//    Variable< int > int_value; // OK
//    const Variable< int > int_value; // ERROR
//    Variable< const int > int_value; // ERROR

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


//    get< int >( int_value ) += 12345;
//    cget< int >( int_value );

//    Variable< ::std::string > name;
//    get< ::std::string >( name ) = "Hello";

//    Variable< Implicit< Item, ValueTool > > item;
//    get< BaseItem >( item ); // ОК
//    get< int >( get< Item >( item ).m_int ) = 67890;
//    get< ::std::string >( get< Item >( item ).m_string ) = "Item";

//    ::std::cout
//        << cget< int >( int_value ) << ::std::endl
//        << cget< ::std::string >( name ) << ::std::endl
//        << cget< int >( cget< Item >( item ).m_int ) << ::std::endl
//        << cget< ::std::string >( cget< Item >( item ).m_string ) << ::std::endl
//    ;

//    Variable< Implicit< Item, ValueTool > > other_item;// = item;
////    get< Item >( other_item ).m_refer = cget< Item >( item ).m_string;

//    get< int >( get< Item >( other_item ).m_int ) = 1;
//    get< ::std::string >( get< Item >( other_item ).m_string ) = "Word";

//    ::std::cout
//        << get< const int >( get< Item >( other_item ).m_int ) << ::std::endl
//        << cget< ::std::string >( cget< Item >( other_item ).m_string ) << ::std::endl
////        << cget< ::std::string >( cget< Item >( other_item ).m_string_refer ) << ::std::endl
//    ;

    return 0;
}
