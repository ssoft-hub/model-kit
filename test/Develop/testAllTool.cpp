//#include <memory.h>
//#include <iostream>
//#include "MyType.h"
//#include <ModelKit.h>

//template < typename _TestType >
//void testTool ()
//{
//    Featured< _TestType > value;
//    Featured< _TestType > other;

//    other = _TestType();
//    value = ::std::move( other ); // other not valid!!!
//    other = Featured< _TestType >();
//    other = value;

//    value->m_first_name = "first name";
//    value->m_last_name = "last name";
//    value->m_age = 50;
//    value->m_stature = 178.5;

//    // Имеет смысл применять гаранты (указатели), если необходимо снизить
//    // накладные расходы связанные с их конкретным инструментом _Tool
//    // (например, системные блокировки и т.п.)

//    // Гарант свойств для применения константного значения value
//    auto cvalue_ptr = &asConst( value );

//    // value
//    ::std::cout
//        << "Test tool:" << ::std::endl
//        << cvalue_ptr->m_first_name << ::std::endl
//        << cvalue_ptr->m_last_name << ::std::endl
//        << cvalue_ptr->m_age << ::std::endl
//        << cvalue_ptr->m_stature << ::std::endl;
//}

//template < typename _Type >
//void testToolByTool ()
//{
//    testTool< _Type >();
//    testTool< Featured< _Type, ::Cpp::Inplace::DefaultTool > >();
//    testTool< Featured< _Type, ::Cpp::Raw::HeapTool > >();
//    testTool< Featured< _Type, ::Implicit::RawTool > >();
//    testTool< Featured< _Type, ::Std::Mutex::AtomicTool > >();
//    testTool< Featured< _Type, ::Std::Shared::ImplicitTool > >();
//    testTool< Featured< _Type, ::Std::Shared::HeapTool > >();
//    testTool< Featured< _Type, ::Std::Unique::HeapTool > >();
//}

//void testAllTool ()
//{
//    testToolByTool< MyType >();
//    testToolByTool< Featured< MyType, ::Cpp::Inplace::DefaultTool > >();
//    testToolByTool< Featured< MyType, ::Cpp::Raw::HeapTool > >();
//    testToolByTool< Featured< MyType, ::Implicit::RawTool > >();
//    testToolByTool< Featured< MyType, ::Std::Mutex::AtomicTool > >();
//    testToolByTool< Featured< MyType, ::Std::Shared::ImplicitTool > >();
//    testToolByTool< Featured< MyType, ::Std::Shared::HeapTool > >();
//    testToolByTool< Featured< MyType, ::Std::Unique::HeapTool > >();
//}
