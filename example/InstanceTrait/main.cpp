#include <ModelKit/Instance.h>
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
        << "is_compatible< Featured< int >, Featured< int > >" << ::std::endl
        << true << " = "
        << is_compatible< Featured< int >, Featured< int > > << ::std::endl;
    ::std::cout
        << "is_compatible< Featured< int >, Featured< double > >" << ::std::endl
        << false << " = "
        << is_compatible< Featured< int >, Featured< double > > << ::std::endl;
    ::std::cout
        << "is_compatible< Featured< BaseType >, Featured< DerivedType > >" << ::std::endl
        << true << " = "
        << is_compatible< Featured< BaseType >, Featured< DerivedType > > << ::std::endl;
    ::std::cout
        << "is_compatible< Featured< DerivedType >, Featured< BaseType > >" << ::std::endl
        << false << " = "
        << is_compatible< Featured< DerivedType >, Featured< BaseType > > << ::std::endl;

    using FirstType = Featured< Featured< Featured< BaseType
        , ::Implicit::SharedTool >
        , ::Heap::UniqueTool >
        , ::ThreadSafe::RecursiveMutexTool >;

    using SecondType = Featured< Featured< Featured< DerivedType
        , ::Implicit::SharedTool >
        , ::Heap::UniqueTool >
        , ::ThreadSafe::RecursiveMutexTool >;

    ::std::cout
        << "is_compatible< FirstType, SecondType >" << ::std::endl
        << true << " = "
        << is_compatible< FirstType, SecondType > << ::std::endl;
    ::std::cout
        << "is_compatible< SecondType, FirstType >" << ::std::endl
        << false << " = "
        << is_compatible< SecondType, FirstType > << ::std::endl;

    using FirstPartType = Featured< BaseType, ::Implicit::SharedTool >;
    using SecondPartType = Featured< DerivedType, ::Implicit::SharedTool >;

    ::std::cout
        << "is_this_part_of_other< FirstPartType, FirstType >" << ::std::endl
        << true << " = "
        << is_this_part_of_other< FirstPartType, FirstType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< SecondPartType, FirstType >" << ::std::endl
        << false << " = "
        << is_this_part_of_other< SecondPartType, FirstType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< FirstPartType, SecondType >" << ::std::endl
        << true << " = "
        << is_this_part_of_other< FirstPartType, SecondType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< SecondPartType, SecondType >" << ::std::endl
        << true << " = "
        << is_this_part_of_other< SecondPartType, SecondType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< FirstType, FirstType >" << ::std::endl
        << false << " = "
        << is_this_part_of_other< FirstType, FirstType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< FirstType, SecondType >" << ::std::endl
        << false << " = "
        << is_this_part_of_other< FirstType, SecondType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< SecondType, FirstType >" << ::std::endl
        << false << " = "
        << is_this_part_of_other< SecondType, FirstType > << ::std::endl;
    ::std::cout
        << "is_this_part_of_other< FirstPartType, SecondPartType >" << ::std::endl
        << false << " = "
        << is_this_part_of_other< FirstPartType, SecondPartType > << ::std::endl;

    FirstType first_value;
    FirstPartType first_part_value;

    FirstType check_value( first_part_value );
    FirstPartType check_part_value( first_value );

    return 0;
}
