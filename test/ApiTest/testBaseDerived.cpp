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
    std::cout
        << "Base: "
        << c_get( c_get( base_value ).m_int ) << " "
        << c_get( c_get( base_value ).m_string ) << std::endl;
}

void testBase ()
{
    Variable< BaseType > default_base_value;
    Variable< BaseType > contruct_base_value( 10, "Constructed base type" );
    Variable< BaseType > move_base_value = Variable< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( c_get( default_base_value ) );
    printBase( c_get( contruct_base_value ) );
    printBase( c_get( move_base_value ) );
}

void testBaseSetting ()
{
    Variable< BaseType > base_value;

    g_get( base_value ).m_int = 1;
    g_get( base_value ).m_string = "one";
    printBase( c_get( base_value ) );

    g_get( base_value ).m_int = BaseInt( 2 );
    g_get( base_value ).m_string = BaseString( "two" );
    printBase( c_get( base_value ) );

    g_get( base_value ).m_int = DerivedInt( 3 );
    g_get( base_value ).m_string = DerivedString( "three" );
    printBase( c_get( base_value ) );

    g_get( base_value ).m_int = HeapInt( 4 );
    g_get( base_value ).m_string = HeapString( "four" );
    printBase( c_get( base_value ) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    std::cout
        << "Derived: "
        << c_get( c_get( derived_value ).m_int ) << " "
        << c_get( c_get( derived_value ).m_string ) << std::endl;
}

void testDerived ()
{
    Variable< DerivedType > default_derived_value; //
    Variable< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Variable< DerivedType > move_derived_value = Variable< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( c_get( default_derived_value ) );
    printDerived( c_get( contruct_derived_value ) );
    printDerived( c_get( move_derived_value ) );
}

void testBaseDerivedSetting ()
{
    Variable< DerivedType > derived_value;

    g_get( derived_value ).BaseType::m_int = 1;
    g_get( derived_value ).BaseType::m_string = "one";
    g_get( derived_value ).m_int = -1;
    g_get( derived_value ).m_string = "negative one";
    printDerived( c_get( derived_value ) );

    g_get( derived_value ).BaseType::m_int = BaseInt( 2 );
    g_get( derived_value ).BaseType::m_string = BaseString( "two" );
    g_get( derived_value ).m_int = BaseInt( -2 );
    g_get( derived_value ).m_string = BaseString( "negative two" );
    printDerived( c_get( derived_value ) );

    g_get( derived_value ).BaseType::m_int = DerivedInt( 3 );
    g_get( derived_value ).BaseType::m_string = DerivedString( "three" );
    g_get( derived_value ).m_int = DerivedInt( -3 );
    g_get( derived_value ).m_string = DerivedString( "negative three" );
    printDerived( c_get( derived_value ) );

    g_get( derived_value ).BaseType::m_int = HeapInt( 4 );
    g_get( derived_value ).BaseType::m_string = HeapString( "four" );
    g_get( derived_value ).m_int = HeapInt( -4 );
    g_get( derived_value ).m_string = HeapString( "negative four" );
    printDerived( c_get( derived_value ) );

    Variable< BaseType > base_value = derived_value; // OK
    printBase( c_get( base_value ) );

    //Variable other_derived_value = base_value; //ERROR
}

void testBaseDerived ()
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
}
