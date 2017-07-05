#include "BaseType.h"
#include "DerivedType.h"
#include <Element/Variable.h>
#include <Value/Tool/Std/Unique/HeapTool.h>

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
    std::cout
        << "Base: "
        << cget< Int >( base_value.m_int ) << " "
        << cget< String >( base_value.m_string ) << std::endl;
}

void testBase ()
{
    Variable< BaseType > default_base_value;
    Variable< BaseType > contruct_base_value( 10, "Constructed base type" );
    Variable< BaseType > move_base_value = Variable< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( cget< BaseType >( default_base_value ) );
    printBase( cget< BaseType >( contruct_base_value ) );
    printBase( cget< BaseType >( move_base_value ) );
}

void testBaseSetting ()
{
    Variable< BaseType > base_value;

    get< BaseType >( base_value ).m_int = 1;
    get< BaseType >( base_value ).m_string = "one";
    printBase( cget< BaseType >( base_value ) );

    get< BaseType >( base_value ).m_int = BaseInt( 2 );
    get< BaseType >( base_value ).m_string = BaseString( "two" );
    printBase( cget< BaseType >( base_value ) );

    get< BaseType >( base_value ).m_int = DerivedInt( 3 );
    get< BaseType >( base_value ).m_string = DerivedString( "three" );
    printBase( cget< BaseType >( base_value ) );

    get< BaseType >( base_value ).m_int = HeapInt( 4 );
    get< BaseType >( base_value ).m_string = HeapString( "four" );
    printBase( cget< BaseType >( base_value ) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    std::cout
        << "Derived: "
        << cget< Int >( derived_value.m_int ) << " "
        << cget< String >( derived_value.m_string ) << std::endl;
}

void testDerived ()
{
    Variable< DerivedType > default_derived_value; //
    Variable< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Variable< DerivedType > move_derived_value = Variable< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( cget< DerivedType >( default_derived_value ) );
    printDerived( cget< DerivedType >( contruct_derived_value ) );
    printDerived( cget< DerivedType >( move_derived_value ) );
}

void testBaseDerivedSetting ()
{
    Variable< DerivedType > derived_value;

    get< BaseType >( derived_value ).m_int = 1;
    get< BaseType >( derived_value ).m_string = "one";
    get< DerivedType >( derived_value ).m_int = -1;
    get< DerivedType >( derived_value ).m_string = "negative one";
    printDerived( cget< DerivedType >( derived_value ) );

    get< BaseType >( derived_value ).m_int = BaseInt( 2 );
    get< BaseType >( derived_value ).m_string = BaseString( "two" );
    get< DerivedType >( derived_value ).m_int = BaseInt( -2 );
    get< DerivedType >( derived_value ).m_string = BaseString( "negative two" );
    printDerived( cget< DerivedType >( derived_value ) );

    get< BaseType >( derived_value ).m_int = DerivedInt( 3 );
    get< BaseType >( derived_value ).m_string = DerivedString( "three" );
    get< DerivedType >( derived_value ).m_int = DerivedInt( -3 );
    get< DerivedType >( derived_value ).m_string = DerivedString( "negative three" );
    printDerived( cget< DerivedType >( derived_value ) );

    get< BaseType >( derived_value ).m_int = HeapInt( 4 );
    get< BaseType >( derived_value ).m_string = HeapString( "four" );
    get< DerivedType >( derived_value ).m_int = HeapInt( -4 );
    get< DerivedType >( derived_value ).m_string = HeapString( "negative four" );
    printDerived( cget< DerivedType >( derived_value ) );

    Variable< BaseType > base_value = derived_value; // OK
    printBase( cget< BaseType >( base_value ) );

    //Variable< DerivedType > other_derived_value = base_value; //ERROR
}

void testBaseDerived ()
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
}
