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
        << *cguard( cguard( base_value )->m_int ) << " "
        << *cguard( cguard( base_value )->m_string ) << ::std::endl;
}

void testBase ()
{
    Variable< BaseType > default_base_value;
    Variable< BaseType > contruct_base_value( 10, "Constructed base type" );
    Variable< BaseType > move_base_value = Variable< BaseType >( 20, "Moved base type" );

    default_base_value = contruct_base_value;

    printBase( *cguard( default_base_value ) );
    printBase( *cguard( contruct_base_value ) );
    printBase( *cguard( move_base_value ) );
}

void testBaseSetting ()
{
    Variable< BaseType > base_value;

    guard( base_value )->m_int = 1;
    guard( base_value )->m_string = "one";
    printBase( *cguard( base_value ) );

    guard( base_value )->m_int = BaseInt( 2 );
    guard( base_value )->m_string = BaseString( "two" );
    printBase( *cguard( base_value ) );

    guard( base_value )->m_int = DerivedInt( 3 );
    guard( base_value )->m_string = DerivedString( "three" );
    printBase( *cguard( base_value ) );

    guard( base_value )->m_int = HeapInt( 4 );
    guard( base_value )->m_string = HeapString( "four" );
    printBase( *cguard( base_value ) );
}

void printDerived ( const DerivedType & derived_value )
{
    printBase( derived_value );
    ::std::cout
        << "Derived: "
        << *cguard( cguard( derived_value )->m_int ) << " "
        << *cguard( cguard( derived_value )->m_string ) << ::std::endl;
}

void testDerived ()
{
    Variable< DerivedType > default_derived_value; //
    Variable< DerivedType > contruct_derived_value( 10, "Constructed derived type" ); //
    Variable< DerivedType > move_derived_value = Variable< DerivedType >( 20, "Moved derived type" ); //

    default_derived_value = contruct_derived_value; //

    printDerived( *cguard( default_derived_value ) );
    printDerived( *cguard( contruct_derived_value ) );
    printDerived( *cguard( move_derived_value ) );
}

void testBaseDerivedSetting ()
{
    using BaseTestType = Instance< BaseType, ::Std::Shared::ImplicitTool >;
    using DerivedTestType = Instance< DerivedType, ::Std::Shared::ImplicitTool >;

    Variable< DerivedTestType > derived_value;

    guard( derived_value )->BaseType::m_int = 1;
    guard( derived_value )->BaseType::m_string = "one";
    guard( derived_value )->m_int = -1;
    guard( derived_value )->m_string = "negative one";
    printDerived( *cguard( derived_value ) );

    guard( derived_value )->BaseType::m_int = BaseInt( 2 );
    guard( derived_value )->BaseType::m_string = BaseString( "two" );
    guard( derived_value )->m_int = BaseInt( -2 );
    guard( derived_value )->m_string = BaseString( "negative two" );
    printDerived( *cguard( derived_value ) );

    guard( derived_value )->BaseType::m_int = DerivedInt( 3 );
    guard( derived_value )->BaseType::m_string = DerivedString( "three" );
    guard( derived_value )->m_int = DerivedInt( -3 );
    guard( derived_value )->m_string = DerivedString( "negative three" );
    printDerived( *cguard( derived_value ) );

    guard( derived_value )->BaseType::m_int = HeapInt( 4 );
    guard( derived_value )->BaseType::m_string = HeapString( "four" );
    guard( derived_value )->m_int = HeapInt( -4 );
    guard( derived_value )->m_string = HeapString( "negative four" );
    printDerived( *cguard( derived_value ) );

    Variable< BaseTestType > base_value;
    base_value = derived_value; // OK
    printBase( *cguard( base_value ) );

//    Variable< DerivedType > other_derived_value = base_value; //ERROR
}

void testBaseDerived ()
{
    testBase();
    testBaseSetting();
    testDerived();
    testBaseDerivedSetting();
}
