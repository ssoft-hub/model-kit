#include "memory.h"
#include "BaseType.h"
#include "DerivedType.h"

int testTrait ()
{
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
        , ::Std::Shared::ImplicitTool >
        , ::Std::Unique::HeapTool >
        , ::Std::Mutex::AtomicTool >;

    using SecondType = Instance< Instance< Instance< DerivedType
        , ::Std::Shared::ImplicitTool >
        , ::Std::Unique::HeapTool >
        , ::Std::Mutex::AtomicTool >;

    ::std::cout
        << "IsCompatible< FirstType, SecondType >::value" << ::std::endl
        << IsCompatible< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< SecondType, FirstType >::value" << ::std::endl
        << IsCompatible< SecondType, FirstType >::value << ::std::endl;

    using FirstPartType = Instance< BaseType, ::Std::Shared::ImplicitTool >;
    using SecondPartType = Instance< DerivedType, ::Std::Shared::ImplicitTool >;

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
    ::std::cout
        << "IsPartOf< FirstType, FirstType >::value" << ::std::endl
        << IsPartOf< FirstType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstType, SecondType >::value" << ::std::endl
        << IsPartOf< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondType, FirstType >::value" << ::std::endl
        << IsPartOf< SecondType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstPartType, SecondPartType >::value" << ::std::endl
        << IsPartOf< FirstPartType, SecondPartType >::value << ::std::endl;

    FirstType first_value;
    FirstPartType first_part_value;

    FirstType check_value( first_part_value );
    FirstPartType check_part_value( first_value );

    return 0;
}
