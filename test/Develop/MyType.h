#pragma once

#include <ModelKit.h>
#include <string>

struct MyType
{
    Featured< ::std::string > m_first_name;
    Featured< ::std::string > m_last_name;
    Featured< int > m_age;
    Featured< double > m_stature;
};
