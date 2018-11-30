#include <iostream>
#include <map>
#include <ModelKit.h>
#include <string>
#include <thread>
#include <vector>

using Map = ::std::map< ::std::string, ::std::pair< ::std::string, int > >;

using RecursiveMutexMap = Featured< Map, ThreadSafe::RecursiveMutexTool >;
using SharedMutexMap = Featured< Map, ThreadSafe::SharedMutexTool >;

using DefaultMap = Featured< Map, Inplace::DefaultTool >;
using ImplicitMap = Featured< Map, Implicit::RawTool >;

template < typename _MapType >
void func ()
{
    static _MapType test_map;

    test_map[ "apple" ]->first = "fruit";
    test_map[ "potato" ]->first = "vegetable";

    for ( size_t i = 0; i < 100000; ++i )
    {
        test_map->at( "apple" ).second++;
        test_map->find( "potato" )->second.second++;
    }

    auto read_ptr = &asConst( test_map );
    ::std::cout
        << "potato is " << read_ptr->at( "potato" ).first
        << " " << read_ptr->at( "potato" ).second
        << ", apple is " << read_ptr->at( "apple" ).first
        << " " << read_ptr->at( "apple" ).second
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
void example ()
{
    ::std::vector< ::std::thread > threads( 10 );
    for ( auto & thread : threads )
        thread = ::std::thread( func< _MapType > );
    for ( auto & thread : threads )
        thread.join();

    ::std::cout << "end" << ::std::endl;
}


int main ( int, char ** )
{
    // Паралельно, но не атомарно.
    //example< Map >();
    // Паралельно, но не атомарно.
    example< DefaultMap >();
    // Паралельно, но не атомарно.
    example< ImplicitMap >();
    // Псевдопаралельно с блокировками, атомарно.
    example< RecursiveMutexMap >();
    // Псевдопаралельно с блокировками, атомарно.
    example< SharedMutexMap >();
    return 0;
}
