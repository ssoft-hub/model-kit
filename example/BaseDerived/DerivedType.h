#include <ModelKit/Value/Tool/Std/Shared/ImplicitTool.h>
#include "BaseType.h"

struct DerivedType
    : public BaseType
{
    using ValueTool = ::Std::Shared::ImplicitTool;
    using Int = Instance< int, ValueTool >;
    using String = Instance< ::std::string, ValueTool >;

    Property< Int > m_int;
    Property< String > m_string;

    DerivedType (
        Int int_value = Int(),
        String string_value = String() )
    : BaseType( 1, "Base string" )
    , m_int( int_value )
    , m_string( string_value )
    {
    }

    DerivedType ( DerivedType && ) = default;
    DerivedType ( const DerivedType & ) = default;

    DerivedType & operator = ( DerivedType && ) = default;
    DerivedType & operator = ( const DerivedType & ) = default;
};
