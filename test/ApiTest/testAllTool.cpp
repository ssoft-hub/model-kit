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
    v_get( value ) = v_get( _TestType() );

    v_get( value ).m_first_name = "first name";
    v_get( value ).m_last_name = "last name";
    v_get( value ).m_age = 50;
    v_get( value ).m_stature = 178.5;

    // Явное использование FeatureGuard гарантирует применение только того
    // свойства, за которое он отвечает.

    // Доступ к значению дожно осуществляться строго через метод access(),
    // так как фактическое размещение экземляра значения может измениться
    // (например, в случае implicit shared значения).
    //
    // Например, использование кода: auto x = cvalueFeatureGuard( value )->access();
    // или эквивалентного: auto x = c_get( value ); не гарантирует корректный
    // доступ по значению x.

    // Иногда, имеет смысл применять гаранты, если необходимо снизить
    // накладные расходы связанные с их конкретной реализацией
    // (например, системные блокировки и т.п.)

    // Гарант первоочередного свойства для применения константного value
    auto cguard = c_guard( value );

    // value
    ::std::cout
        << "Test tool:" << ::std::endl
        << ( c_get( cguard->access().m_first_name ) ) << ::std::endl
        << ( c_get( cguard->access().m_last_name ) ) << ::std::endl
        << ( c_get( cguard->access().m_age ) ) << ::std::endl
        << ( c_get( cguard->access().m_stature ) ) << ::std::endl;
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
