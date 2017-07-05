#include <memory.h>
#include <iostream>
#include "MyType.h"


template < typename _TestType >
void testTool ()
{
    Variable< _TestType > value;

    get< MyType >( value ).m_first_name = "first name";
    get< MyType >( value ).m_last_name = "last name";
    get< MyType >( value ).m_age = 50;
    get< MyType >( value ).m_stature = 178.5;

    ::std::cout
        << "Test tool:" << ::std::endl
        << cget< ::std::string >( cget< MyType >( value ).m_first_name ) << ::std::endl
        << cget< ::std::string >( cget< MyType >( value ).m_last_name )<< ::std::endl
        << cget< int >( cget< MyType >( value ).m_age ) << ::std::endl
        << cget< double >( cget< MyType >( value ).m_stature ) << ::std::endl;
}

//template < typename _TestType >
//void testToolWithOperator ()
//{
//    Variable< _TestType > value;

//    get_ptr ( value )->m_first_name = "first name";
//    get_ptr( value )->m_last_name = "last name";
//    get_ptr( value )->m_age = 50;
//    get_ptr( value )->m_stature = 178.5;

//    ::std::cout
//        << "Test tool:" << ::std::endl
//        << cget_ptr( value )->m_first_name << ::std::endl
//        << cget_ptr( value )->m_last_name << ::std::endl
//        << cget_ptr( value )-> m_age << ::std::endl
//        << cget_ptr( value )->m_stature << ::std::endl;
//}

void testAllTool ()
{
    testTool< Instance< MyType, ::Cpp::Inplace::InplaceTool > >();
    testTool< Instance< MyType, ::Cpp::Raw::HeapTool > >();
    testTool< Instance< MyType, ::Std::Mutex::AtomicTool > >();
    testTool< Instance< MyType, ::Std::Shared::ImplicitTool > >();
    testTool< Instance< MyType, ::Std::Shared::HeapTool > >();
    testTool< Instance< MyType, ::Std::Unique::HeapTool > >();

//    testToolWithOperator< Instance< MyType, ::Cpp::Inplace::InplaceTool > >();
}
