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
        << cGet( cGet( base_value ).m_int ) << " "
        << cGet( cGet( base_value ).m_string ) << ::std::endl;
}

void testBase ()
{
    Variable< BaseType > default_base_value;
    Variable< BaseType > contruct_base_value( 10, "Constructed base type" );
    Variable< BaseType > move_base_value = Variable< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( cGet( default_base_value ) );
    printBase( cGet( contruct_base_value ) );
    printBase( cGet( move_base_value ) );
}

void testBaseSetting ()
{
    Variable< BaseType > base_value;

    vGet( base_value ).m_int = 1;
    vGet( base_value ).m_string = "one";
    printBase( cGet( base_value ) );

    vGet( base_value ).m_int = BaseInt( 2 );
    vGet( base_value ).m_string = BaseString( "two" );
    printBase( cGet( base_value ) );

    vGet( base_value ).m_int = DerivedInt( 3 );
    vGet( base_value ).m_string = DerivedString( "three" );
    printBase( cGet( base_value ) );

    vGet( base_value ).m_int = HeapInt( 4 );
    vGet( base_value ).m_string = HeapString( "four" );
    printBase( cGet( base_value ) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    ::std::cout
        << "Derived: "
        << cGet( cGet( derived_value ).m_int ) << " "
        << cGet( cGet( derived_value ).m_string ) << ::std::endl;
}

void testDerived ()
{
    Variable< DerivedType > default_derived_value; //
    Variable< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Variable< DerivedType > move_derived_value = Variable< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( cGet( default_derived_value ) );
    printDerived( cGet( contruct_derived_value ) );
    printDerived( cGet( move_derived_value ) );
}

void testBaseDerivedSetting ()
{
    using BaseTestType = Instance< BaseType, ::Std::Shared::ImplicitTool >;
    using DerivedTestType = Instance< DerivedType, ::Std::Shared::ImplicitTool >;

    Variable< DerivedTestType > derived_value;

    vGet( derived_value ).BaseType::m_int = 1;
    vGet( derived_value ).BaseType::m_string = "one";
    vGet( derived_value ).m_int = -1;
    vGet( derived_value ).m_string = "negative one";
    printDerived( cGet( derived_value ) );

    vGet( derived_value ).BaseType::m_int = BaseInt( 2 );
    vGet( derived_value ).BaseType::m_string = BaseString( "two" );
    vGet( derived_value ).m_int = BaseInt( -2 );
    vGet( derived_value ).m_string = BaseString( "negative two" );
    printDerived( cGet( derived_value ) );

    vGet( derived_value ).BaseType::m_int = DerivedInt( 3 );
    vGet( derived_value ).BaseType::m_string = DerivedString( "three" );
    vGet( derived_value ).m_int = DerivedInt( -3 );
    vGet( derived_value ).m_string = DerivedString( "negative three" );
    printDerived( cGet( derived_value ) );

    vGet( derived_value ).BaseType::m_int = HeapInt( 4 );
    vGet( derived_value ).BaseType::m_string = HeapString( "four" );
    vGet( derived_value ).m_int = HeapInt( -4 );
    vGet( derived_value ).m_string = HeapString( "negative four" );
    printDerived( cGet( derived_value ) );

    Variable< BaseTestType > base_value;
    base_value = derived_value; // OK
    printBase( cGet( base_value ) );

    //Variable< DerivedType > other_derived_value = base_value; //ERROR
}

int main ( int, char** )
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
    return 0;
}
