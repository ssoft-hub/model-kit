#pragma once

#include <ModelKit.h>
#include <string>

struct MyType
{
    Property< ::std::string > m_first_name;
    Property< ::std::string > m_last_name;
    Property< int > m_age;
    Property< double > m_stature;
};
