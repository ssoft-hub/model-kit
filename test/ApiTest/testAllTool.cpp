#include <memory.h>
#include <iostream>
#include "MyType.h"

template < typename _TestType >
void testTool ()
{
    Variable< _TestType > value;
    Variable< _TestType > other;

    other = _TestType();
    value = ::std::move( other );
    other = value;
    wGet( value ) = wGet( _TestType() );

    wGet( value ).m_first_name = "first name";
    wGet( value ).m_last_name = "last name";
    wGet( value ).m_age = 50;
    wGet( value ).m_stature = 178.5;

    // Явное использование FeatureGuard гарантирует применение только того
    // свойства, за которое он отвечает.

    // Доступ к значению дожно осуществляться строго через метод access(),
    // так как фактическое размещение экземляра значения может измениться
    // (например, в случае implicit shared значения).
    //
    // Например, использование кода: auto x = cvalueFeatureGuard( value )->access();
    // или эквивалентного: auto x = cGet( value ); не гарантирует корректный
    // доступ по значению x.

    // Иногда, имеет смысл применять гаранты, если необходимо снизить
    // накладные расходы связанные с их конкретным инструментом _ValueTool
    // (например, системные блокировки и т.п.)

    // Гарант первоочередного свойства для применения константного значения value
    auto cguard = cGuard( value );

    // value
    ::std::cout
        << "Test tool:" << ::std::endl
        << ( cGet( gGet( cguard ).m_first_name ) ) << ::std::endl
        << ( cGet( gGet( cguard ).m_last_name ) ) << ::std::endl
        << ( cGet( gGet( cguard ).m_age ) ) << ::std::endl
        << ( cGet( gGet( cguard ).m_stature ) ) << ::std::endl;
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
