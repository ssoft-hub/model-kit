#pragma once

#include <Element/Property.h>
#include <iostream>
#include <string>

struct BaseType
{
    using Int = int;
    using String = ::std::string;

    Property< Int > m_int;
    Property< String > m_string;

    BaseType (
        Int int_value = Int(),
        String string_value = String() )
    : m_int( int_value )
    , m_string( string_value )
    {
    }
};
