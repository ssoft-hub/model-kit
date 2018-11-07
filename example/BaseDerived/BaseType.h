#pragma once

#include <ModelKit/Featured.h>
#include <iostream>
#include <string>

struct BaseType
{
    using Int = int;
    using String = ::std::string;

    Featured< Int > m_int;
    Featured< String > m_string;

    BaseType (
        Int int_value = Int(),
        String string_value = String() )
        : m_int( int_value )
    , m_string( string_value )
    {
    }

    BaseType ( BaseType && ) = default;
    BaseType ( const BaseType & ) = default;

    BaseType & operator = ( BaseType && ) = default;
    BaseType & operator = ( const BaseType & ) = default;
};
