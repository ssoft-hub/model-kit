#include <iostream>
#include <map>
#include <ModelKit.h>
#include <string>
#include <thread>
#include <vector>

using Map = ::std::map< ::std::string, ::std::pair< ::std::string, int > >;

using AtomicMap = Instance< Map, ::Std::Mutex::AtomicTool >;
using DefaultMap = Instance< Map, ::Cpp::Inplace::DefaultTool >;
using ImplicitMap = Instance< Map, ::Cpp::Raw::ImplicitTool >;

template < typename _MapType >
void func ( _MapType * map_strings_ptr )
{
    auto & map_strings = *map_strings_ptr;

    (**map_strings)[ "apple" ].first = "fruit";
    (**map_strings)[ "potato" ].first = "vegetable";

    for ( size_t i = 0; i < 100000; ++i )
    {
        map_strings->at( "apple" ).second++;
        map_strings->find( "potato" )->second.second++;
    }

    auto const & readonly_safe_map_string = map_strings;
    ::std::cout
        << "potato is " << readonly_safe_map_string->at( "potato" ).first
        << " " << readonly_safe_map_string->at( "potato" ).second
        << ", apple is " << readonly_safe_map_string->at( "apple" ).first
        << " " << readonly_safe_map_string->at( "apple" ).second
        << ::std::endl;
}

/*
 * Пример взят и модифицирован из
 * https://habrahabr.ru/post/328348/
 * http://coliru.stacked-crooked.com/a/5def728917274b22
 *
 * Итоговые значения равные 100000 означают, что каждое сложение в каждом
 * из 10 потоков было выполнено потоко-безопасно.
 * Промежуточные значения не кратные 10000 говорят о том, что потоки исполнялись
 * параллельно или псевдопараллельно, т.е. прерывались посреди любой из операции
 * и в это время исполнялся другой поток.
 */
template < typename _MapType >
void example ( _MapType * map_strings_ptr )
{
    ::std::vector< ::std::thread > threads( 10 );
    for ( auto & thread : threads )
        thread = ::std::thread( func< _MapType >, map_strings_ptr );
    for ( auto & thread : threads )
        thread.join();

    std::cout << "end" << ::std::endl;
}


int main ( int, char ** )
{
    // Паралельно, но не атомарно.
    DefaultMap default_map;
    example( &default_map );

    // Паралельно, но не атомарно.
    ImplicitMap implicit_map;
    example( &implicit_map );

    // Псевдопаралельно с блокировками, атомарно.
    AtomicMap atomic_map;
    example( &atomic_map );

    return 0;
}
