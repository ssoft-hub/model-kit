#pragma once

#include <ModelKit.h>
#include <string>

struct MyType
{
    Scl::Instance< ::std::string > m_first_name;
    Scl::Instance< ::std::string > m_last_name;
    Scl::Instance< int > m_age;
    Scl::Instance< double > m_stature;
};
