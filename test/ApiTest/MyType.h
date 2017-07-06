#pragma once

#include "memory.h"

struct MyType
{
    Property< ::std::string > m_first_name;
    Property< ::std::string > m_last_name;
    Property< int > m_age;
    Property< double > m_stature;


    MyType () = default;
    MyType ( MyType && ) = default;
    MyType ( const MyType & ) = default;

    MyType & operator = ( MyType && other )
    {
        m_age = other.m_age;
        return *this;
    }

    MyType & operator = ( const MyType & other )
    {
        m_age = other.m_age;
        return *this;
    }
};
