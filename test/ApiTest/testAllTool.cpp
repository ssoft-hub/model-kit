#include <memory.h>
#include <iostream>
#include "MyType.h"

template < typename _TestType >
void testTool ()
{
    Variable< _TestType > value;
    Variable< _TestType > other;

    other = _TestType();
    value = ::std::move( other ); // other not valid!!!
    other = Variable< _TestType >(); // initialize other
    other = value;
    value = _TestType();

    value->m_first_name = "first name";
    value->m_last_name = "last name";
    value->m_age = 50;
    value->m_stature = 178.5;

    // Явное использование guard гарантирует применение только того
    // свойства, за которое он отвечает.

    // Доступ к значению дожно осуществляться строго через метод access(),
    // так как фактическое размещение экземляра значения может измениться
    // (например, в случае implicit shared значения).

    // Например, использование кода: auto x = cguard( value )->access();
    // или эквивалентного: auto x = cGet( value ); не гарантирует корректный
    // доступ по значению x.

    // Иногда, имеет смысл применять гаранты, если необходимо снизить
    // накладные расходы связанные с их конкретным инструментом _ValueTool
    // (например, системные блокировки и т.п.)

    // Гарант свойств для применения константного значения value
    auto guarder = cguard( value );

    // value
    ::std::cout
        << "Test tool:" << ::std::endl
        << ( **( (*guarder).m_first_name ) ) << ::std::endl
        << ( **( (*guarder).m_last_name ) ) << ::std::endl
        << ( **( guarder->m_age ) ) << ::std::endl
        << ( **( guarder->m_stature ) ) << ::std::endl;
}

template < typename _Type >
void testToolByTool ()
{
    testTool< _Type >();
    testTool< Instance< _Type, ::Cpp::Inplace::DefaultTool > >();
    testTool< Instance< _Type, ::Cpp::Raw::HeapTool > >();
    testTool< Instance< _Type, ::Std::Mutex::AtomicTool > >();
    testTool< Instance< _Type, ::Std::Shared::ImplicitTool > >();
    testTool< Instance< _Type, ::Std::Shared::HeapTool > >();
    testTool< Instance< _Type, ::Std::Unique::HeapTool > >();
}

void testAllTool ()
{
    testToolByTool< MyType >();
    testToolByTool< Instance< MyType, ::Cpp::Inplace::DefaultTool > >();
    testToolByTool< Instance< MyType, ::Cpp::Raw::HeapTool > >();
    testToolByTool< Instance< MyType, ::Std::Mutex::AtomicTool > >();
    testToolByTool< Instance< MyType, ::Std::Shared::ImplicitTool > >();
    testToolByTool< Instance< MyType, ::Std::Shared::HeapTool > >();
    testToolByTool< Instance< MyType, ::Std::Unique::HeapTool > >();
}
