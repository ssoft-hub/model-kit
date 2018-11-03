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
        << "IsCompatibleHelper< Variable< int >, Variable< int > >::value" << ::std::endl
        << true << " = "
        << IsCompatibleHelper< Variable< int >, Variable< int > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Variable< int >, Variable< double > >::value" << ::std::endl
        << false << " = "
        << IsCompatibleHelper< Variable< int >, Variable< double > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Variable< BaseType >, Variable< DerivedType > >::value" << ::std::endl
        << true << " = "
        << IsCompatibleHelper< Variable< BaseType >, Variable< DerivedType > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Variable< DerivedType >, Variable< BaseType > >::value" << ::std::endl
        << false << " = "
        << IsCompatibleHelper< Variable< DerivedType >, Variable< BaseType > >::value << ::std::endl;

    using FirstType = Instance< Instance< Instance< BaseType
        , ::Std::Shared::ImplicitTool >
        , ::Std::Unique::HeapTool >
        , ::Std::Mutex::AtomicTool >;

    using SecondType = Instance< Instance< Instance< DerivedType
        , ::Std::Shared::ImplicitTool >
        , ::Std::Unique::HeapTool >
        , ::Std::Mutex::AtomicTool >;

    ::std::cout
        << "IsCompatibleHelper< FirstType, SecondType >::value" << ::std::endl
        << true << " = "
        << IsCompatibleHelper< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< SecondType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsCompatibleHelper< SecondType, FirstType >::value << ::std::endl;

    using FirstPartType = Instance< BaseType, ::Std::Shared::ImplicitTool >;
    using SecondPartType = Instance< DerivedType, ::Std::Shared::ImplicitTool >;

    ::std::cout
        << "IsThisPartOfOtherHelper< FirstPartType, FirstType >::value" << ::std::endl
        << true << " = "
        << IsThisPartOfOtherHelper< FirstPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< SecondPartType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsThisPartOfOtherHelper< SecondPartType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< FirstPartType, SecondType >::value" << ::std::endl
        << true << " = "
        << IsThisPartOfOtherHelper< FirstPartType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< SecondPartType, SecondType >::value" << ::std::endl
        << true << " = "
        << IsThisPartOfOtherHelper< SecondPartType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< FirstType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsThisPartOfOtherHelper< FirstType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< FirstType, SecondType >::value" << ::std::endl
        << false << " = "
        << IsThisPartOfOtherHelper< FirstType, SecondType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< SecondType, FirstType >::value" << ::std::endl
        << false << " = "
        << IsThisPartOfOtherHelper< SecondType, FirstType >::value << ::std::endl;
    ::std::cout
        << "IsThisPartOfOtherHelper< FirstPartType, SecondPartType >::value" << ::std::endl
        << false << " = "
        << IsThisPartOfOtherHelper< FirstPartType, SecondPartType >::value << ::std::endl;

    FirstType first_value;
    FirstPartType first_part_value;

    FirstType check_value( first_part_value );
    FirstPartType check_part_value( first_value );

    return 0;
}
