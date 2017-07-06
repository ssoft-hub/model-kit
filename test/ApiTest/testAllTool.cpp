#include <memory.h>
#include <iostream>
#include "MyType.h"

template < typename _TestType >
void testTool ()
{
    Variable< _TestType > value;
    Variable< _TestType > other;

    value = _TestType();
    value = other;
    g_get( value ) = g_get( _TestType() );

    g_get( value ).m_first_name = "first name";
    g_get( value ).m_last_name = "last name";
    g_get( value ).m_age = 50;
    g_get( value ).m_stature = 178.5;

    ::std::cout
        << "Test tool:" << ::std::endl
        << ( c_get( c_get( value ).m_first_name ) ) << ::std::endl
        << ( c_get( c_get( value ).m_last_name ) ) << ::std::endl
        << ( c_get( c_get( value ).m_age ) ) << ::std::endl
        << ( c_get( c_get( value ).m_stature ) ) << ::std::endl;
}

void testAllTool ()
{
    testTool< MyType >();
    testTool< Instance< MyType, ::Cpp::Inplace::InplaceTool > >();
    testTool< Instance< MyType, ::Cpp::Raw::HeapTool > >();
    testTool< Instance< MyType, ::Std::Mutex::AtomicTool > >();
    testTool< Instance< MyType, ::Std::Shared::ImplicitTool > >();
    testTool< Instance< MyType, ::Std::Shared::HeapTool > >();
    testTool< Instance< MyType, ::Std::Unique::HeapTool > >();
}
