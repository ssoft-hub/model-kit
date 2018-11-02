#include <ModelKit/Value/Tool.h>
#include "BaseType.h"
#include "DerivedType.h"

using Int = int;
using String = ::std::string;

using BaseInt = BaseType::Int;
using BaseString = BaseType::String;

using DerivedInt = DerivedType::Int;
using DerivedString = DerivedType::String;

using ValueTool = ::Std::Unique::HeapTool;
using HeapInt = Instance< int, ValueTool >;
using HeapString = Instance< ::std::string, ValueTool >;

void printBase ( const BaseType & base_value )
{
    ::std::cout
        << "Base: "
        << base_value.m_int << " "
        << base_value.m_string << ::std::endl;
}

void testBase ()
{
    Variable< BaseType > default_base_value;
    Variable< BaseType > contruct_base_value( 10, "Constructed base type" );
    Variable< BaseType > move_base_value = Variable< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( *&asConst(default_base_value) );
    printBase( *&asConst(contruct_base_value) );
    printBase( *&asConst(move_base_value) );
}

void testBaseSetting ()
{
    Variable< BaseType > base_value;

    base_value->m_int = 1;
    base_value->m_string = "one";
    printBase( *&asConst(base_value) );

    base_value->m_int = BaseInt( 2 );
    base_value->m_string = BaseString( "two" );
    printBase( *&asConst(base_value) );

    base_value->m_int = DerivedInt( 3 );
    base_value->m_string = DerivedString( "three" );
    printBase( *&asConst(base_value) );

    base_value->m_int = HeapInt( 4 );
    base_value->m_string = HeapString( "four" );
    printBase( *&asConst(base_value) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    ::std::cout
        << "Derived: "
        << derived_value.m_int << " "
        << derived_value.m_string << ::std::endl;
}

void testDerived ()
{
    Variable< DerivedType > default_derived_value; //
    Variable< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Variable< DerivedType > move_derived_value = Variable< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( *&asConst(default_derived_value) );
    printDerived( *&asConst(contruct_derived_value) );
    printDerived( *&asConst(move_derived_value) );
}

void testBaseDerivedSetting ()
{
    using BaseTestType = Instance< BaseType, ::Cpp::Raw::ImplicitTool >;
    using DerivedTestType = Instance< DerivedType, ::Cpp::Raw::ImplicitTool >;

    Variable< DerivedTestType > derived_value;

    derived_value->BaseType::m_int = 1;
    derived_value->BaseType::m_string = "one";
    derived_value->m_int = -1;
    derived_value->m_string = "negative one";
    printDerived( *&asConst(derived_value) );

    derived_value->BaseType::m_int = BaseInt( 2 );
    derived_value->BaseType::m_string = BaseString( "two" );
    derived_value->m_int = BaseInt( -2 );
    derived_value->m_string = BaseString( "negative two" );
    printDerived( *&asConst(derived_value) );

    derived_value->BaseType::m_int = DerivedInt( 3 );
    derived_value->BaseType::m_string = DerivedString( "three" );
    derived_value->m_int = DerivedInt( -3 );
    derived_value->m_string = DerivedString( "negative three" );
    printDerived( *&asConst(derived_value) );

    derived_value->BaseType::m_int = HeapInt( 4 );
    derived_value->BaseType::m_string = HeapString( "four" );
    derived_value->m_int = HeapInt( -4 );
    derived_value->m_string = HeapString( "negative four" );
    printDerived( *&asConst(derived_value) );

    Variable< BaseTestType > base_value;
    base_value = derived_value; // OK
    printBase( *&asConst(base_value) );

    //Variable< DerivedType > other_derived_value = base_value; //ERROR
}

int main ( int, char ** )
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
    return 0;
}
