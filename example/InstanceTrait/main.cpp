#include <ModelKit/Element.h>
#include <ModelKit/Value/Tool.h>
#include <iostream>

struct BaseType
{
};

struct DerivedType
    : public BaseType
{
};

int main ( int /*argc*/, char ** /*argv*/ )
{
    ::std::cout
        << "IsCompatible< Variable< int >, Variable< int > >::value" << ::std::endl
        << true << " = "
        << IsCompatible< Variable< int >, Variable< int > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< int >, Variable< double > >::value" << ::std::endl
        << false << " = "
        << IsCompatible< Variable< int >, Variable< double > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< BaseType >, Variable< DerivedType > >::value" << ::std::endl
        << true << " = "
        << IsCompatible< Variable< BaseType >, Variable< DerivedType > >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< Variable< DerivedType >, Variable< BaseType > >::value" << ::std::endl
        << false << " = "
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
        << true << " = "
        << IsCompatible< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsCompatible< SecondType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsCompatible< SecondType, FirstType >::value << ::std::endl;

    using FirstPartType = Instance< BaseType, ::Std::Shared::ImplicitTool >;
    using SecondPartType = Instance< DerivedType, ::Std::Shared::ImplicitTool >;

    ::std::cout
        << "IsPartOf< FirstPartType, FirstType >::value" << ::std::endl
        << true << " = "
        << IsPartOf< FirstPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondPartType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsPartOf< SecondPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstPartType, SecondType >::value" << ::std::endl
        << true << " = "
        << IsPartOf< FirstPartType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondPartType, SecondType >::value" << ::std::endl
        << true << " = "
        << IsPartOf< SecondPartType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsPartOf< FirstType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstType, SecondType >::value" << ::std::endl
        << false << " = "
        << IsPartOf< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< SecondType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsPartOf< SecondType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsPartOf< FirstPartType, SecondPartType >::value" << ::std::endl
        << false << " = "
        << IsPartOf< FirstPartType, SecondPartType >::value << ::std::endl;

    FirstType first_value;
    FirstPartType first_part_value;

    FirstType check_value( first_part_value );
    FirstPartType check_part_value( first_value );

    return 0;
}
