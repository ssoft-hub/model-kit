#include <ModelKit.h>
#include "BaseType.h"

struct DerivedType
    : public BaseType
{
    using Tool = ::Implicit::SharedTool;
    using Int = Featured< int, Tool >;
    using String = Featured< ::std::string, Tool >;

    Featured< Int > m_int;
    Featured< String > m_string;

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
