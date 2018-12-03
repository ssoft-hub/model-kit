#pragma once

#include <ModelKit.h>
#include <string>

struct MyType
{
    Instance< ::std::string > m_first_name;
    Instance< ::std::string > m_last_name;
    Instance< int > m_age;
    Instance< double > m_stature;
};
