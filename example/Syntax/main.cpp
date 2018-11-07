#include <iostream>
#include <map>
#include <ModelKit.h>
#include <thread>
#include <vector>

struct CreditCard
{
    Featured< int64_t > m_id;
    Featured< int32_t > m_limit;
};

struct UserCard
{
    Featured< ::std::string > m_name;
    Featured< CreditCard > m_card;
};

using Key = Featured< ::std::string, ::Cpp::Raw::ImplicitTool >;
using Value = Featured< UserCard, ::Cpp::Raw::ImplicitTool >;
using Map = ::std::map< Key, Value >;


Featured< Map > global_map;

// Идеальный вариант, "прозрачный" относительно Featured.
#ifdef _IDEAL_
void appendIdealVariant ()
{
    Featured< Key > key = "ideal";
    global_map[ key ].m_name = "Ideal user";
    global_map[ key ].m_card.m_id = -1L;
    global_map[ key ].m_card.m_limit = -1;
}
#endif

// Вариант с прямым последовательным вызовом методов членов класса guard/cguard и access.
// Не совместим со значениями отличными от Featured.
#ifdef _GUARD_AS_MEMBER_
void appendZeroVariant ()
{
    Featured< Key > key = "zero";
    global_map.guard().access()[ key ].guard().access().m_name = "Zero user";
    global_map.guard().access()[ key ].guard().access().m_card.guard().access().m_id = 1L;
    global_map.guard().access()[ key ].guard().access().m_card.guard().access().m_limit = 1;
}
#endif

// Вариант с вызовом глобальных методов guard/cguard с прямым вызовом метода
// члена класса access. Cовместим со значениями отличными от Featured.
#ifdef _GUARD_AS_GLOBAL_
void appendFirstVariant ()
{
    Featured< Key > key = "first";
    guard( guard( global_map ).access()[ key ] ).access().m_name = "First user";
    guard( guard( guard( global_map ).access()[ key ] ).access().m_card ).access().m_id = 1L;
    guard( guard( guard( global_map ).access()[ key ] ).access().m_card ).access().m_limit = 1;
}
#endif

// Вариант с вызовом глобальных методов guard/cguard и разыменованием.
// Cовместим со значениями отличными от Featured.
#ifdef _GUARD_AS_GLOBAL_
void appendSecondVariant ()
{
    Featured< Key > key = "second";
    (*guard( (*guard( global_map ))[ key ] )).m_name = "Second user";
    (*guard( (*guard( (*guard( global_map ))[ key ] )).m_card )).m_id = 2L;
    (*guard( (*guard( (*guard( global_map ))[ key ] )).m_card )).m_limit = 22;
}
#endif

// Вариант с двойным разыменованием.
// Cовместим со значениями отличными от Featured.
void appendThirdVariant ()
{
    Featured< Key > key = "third";
    (*&(*&global_map)[ key ]).m_name = "Third user";
    (*&(*&(*&global_map)[ key ]).m_card).m_id = 3L;
    (*&(*&(*&global_map)[ key ]).m_card).m_limit = 333;
}

// Вариант с использованием оператора ->.
// Не совместим со значениями отличными от Featured.
void appendFourthVariant ()
{
    Featured< Key > key = "fourth";
    global_map->operator[]( key )->m_name = "Fourth user";
    global_map->operator[]( key )->m_card->m_id = 4L;
    global_map->operator[]( key )->m_card->m_limit = 4444;
}

// Вариант со смешанным использованием двойного разыменования для доступа
// к значению и оператора -> для доступа к членам класса.
// Не cовместим со значениями отличными от Featured.
void appendFifthVariant ()
{
    Featured< Key > key = "fifth";
    (*&global_map)[ key ]->m_name = "Fifth user";
    (*&global_map)[ key ]->m_card->m_id = 5L;
    (*&global_map)[ key ]->m_card->m_limit = 55555;
}

// Вариант с использованием макроса vGet/cGet.
// Cовместим со значениями отличными от Featured.
#ifdef _GET_MACROS_
void appendSixthVariant ()
{
    Featured< Key > key = "sixth";
    vGet( vGet( global_map )[ key ] ).m_name = "Sixth user";
    vGet( vGet( vGet( global_map )[ key ] ).m_card ).m_id = 5L;
    vGet( vGet( vGet( global_map )[ key ] ).m_card ).m_limit = 55555;
}
#endif

int main ( int, char ** )
{
    appendThirdVariant();
    appendFourthVariant();
    appendFifthVariant();

    return 0;
}
