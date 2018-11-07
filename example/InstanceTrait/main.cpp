#include <ModelKit/Featured.h>
#include <ModelKit/Featured/Tool.h>
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
        << "IsCompatibleHelper< Featured< int >, Featured< int > >::value" << ::std::endl
        << true << " = "
        << IsCompatibleHelper< Featured< int >, Featured< int > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Featured< int >, Featured< double > >::value" << ::std::endl
        << false << " = "
        << IsCompatibleHelper< Featured< int >, Featured< double > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Featured< BaseType >, Featured< DerivedType > >::value" << ::std::endl
        << true << " = "
        << IsCompatibleHelper< Featured< BaseType >, Featured< DerivedType > >::value << ::std::endl;
    ::std::cout
        << "IsCompatibleHelper< Featured< DerivedType >, Featured< BaseType > >::value" << ::std::endl
        << false << " = "
        << IsCompatibleHelper< Featured< DerivedType >, Featured< BaseType > >::value << ::std::endl;

    using FirstType = Featured< Featured< Featured< BaseType
        , ::Std::Shared::ImplicitTool >
        , ::Std::Unique::HeapTool >
        , ::Std::Mutex::AtomicTool >;

    using SecondType = Featured< Featured< Featured< DerivedType
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

    using FirstPartType = Featured< BaseType, ::Std::Shared::ImplicitTool >;
    using SecondPartType = Featured< DerivedType, ::Std::Shared::ImplicitTool >;

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
