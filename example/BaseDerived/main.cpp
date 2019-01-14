#include <ModelKit.h>
#include "BaseType.h"
#include "DerivedType.h"

using namespace ::Mdk;

using Int = int;
using String = ::std::string;

using BaseInt = BaseType::Int;
using BaseString = BaseType::String;

using DerivedInt = DerivedType::Int;
using DerivedString = DerivedType::String;

using Tool = ::Heap::UniqueTool;
using HeapInt = Instance< int, Tool >;
using HeapString = Instance< ::std::string, Tool >;

void printBase ( const BaseType & base_value )
{
    ::std::cout
        << "Base: "
        << *&base_value.m_int << " "
        << *&base_value.m_string << ::std::endl;
}

void testBase ()
{
    Instance< BaseType > default_base_value;
    Instance< BaseType > contruct_base_value( 10, "Constructed base type" );
    Instance< BaseType > move_base_value = Instance< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( *&castConst(default_base_value) );
    printBase( *&castConst(contruct_base_value) );
    printBase( *&castConst(move_base_value) );
}

void testBaseSetting ()
{
    Instance< BaseType > base_value;

    base_value->m_int = 1;
    base_value->m_string = "one";
    printBase( *&castConst(base_value) );

    base_value->m_int = BaseInt( 2 );
    base_value->m_string = BaseString( "two" );
    printBase( *&castConst(base_value) );

    base_value->m_int = DerivedInt( 3 );
    base_value->m_string = DerivedString( "three" );
    printBase( *&castConst(base_value) );

    base_value->m_int = HeapInt( 4 );
    base_value->m_string = HeapString( "four" );
    printBase( *&castConst(base_value) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    ::std::cout
        << "Derived: "
        << *&derived_value.m_int << " "
        << *&derived_value.m_string << ::std::endl;
}

void testDerived ()
{
    Instance< DerivedType > default_derived_value; //
    Instance< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Instance< DerivedType > move_derived_value = Instance< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( *&castConst(default_derived_value) );
    printDerived( *&castConst(contruct_derived_value) );
    printDerived( *&castConst(move_derived_value) );
}

void testBaseDerivedSetting ()
{
    using BaseTestType = Instance< BaseType, ::Implicit::RawTool >;
    using DerivedTestType = Instance< DerivedType, ::Implicit::RawTool >;

    Instance< DerivedTestType > derived_value;

    derived_value->BaseType::m_int = 1;
    derived_value->BaseType::m_string = "one";
    derived_value->m_int = -1;
    derived_value->m_string = "negative one";
    printDerived( *&castConst( derived_value ) );

    derived_value->BaseType::m_int = BaseInt( 2 );
    derived_value->BaseType::m_string = BaseString( "two" );
    derived_value->m_int = BaseInt( -2 );
    derived_value->m_string = BaseString( "negative two" );
    printDerived( *&castConst( derived_value ) );

    derived_value->BaseType::m_int = DerivedInt( 3 );
    derived_value->BaseType::m_string = DerivedString( "three" );
    derived_value->m_int = DerivedInt( -3 );
    derived_value->m_string = DerivedString( "negative three" );
    printDerived( *&castConst( derived_value ) );

    derived_value->BaseType::m_int = HeapInt( 4 );
    derived_value->BaseType::m_string = HeapString( "four" );
    derived_value->m_int = HeapInt( -4 );
    derived_value->m_string = HeapString( "negative four" );
    printDerived( *&castConst( derived_value ) );

    Instance< BaseTestType > base_value;
    base_value = derived_value; // OK
    printBase( *&castConst( base_value ) );

    //Instance< DerivedType > other_derived_value = base_value; //ERROR
}

int main ( int, char ** )
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
    return 0;
}
