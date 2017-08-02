#include <iostream>
#include <map>
#include <ModelKit.h>
#include <thread>
#include <vector>

struct CreditCard
{
    Property< int64_t > m_id;
    Property< int32_t > m_limit;
};

struct UserCard
{
    Property< ::std::string > m_name;
    Property< CreditCard > m_card;
};

using Key = Instance< ::std::string, ::Cpp::Raw::ImplicitTool >;
using Value = Instance< UserCard, ::Cpp::Raw::ImplicitTool >;
using Map = ::std::map< Key, Value >;


Variable< Map > global_map;

// Идеальный вариант, "прозрачный" относительно Instance.
#ifdef _IDEAL_
void appendIdealVariant ()
{
    Variable< Key > key = "ideal";
    global_map[ key ].m_name = "Ideal user";
    global_map[ key ].m_card.m_id = -1L;
    global_map[ key ].m_card.m_limit = -1;
}
#endif

// Вариант с прямым последовательным вызовом методов членов класса guard/cguard и access.
// Не совместим со значениями отличными от Instance.
#ifdef _GUARD_AS_MEMBER_
void appendZeroVariant ()
{
    Variable< Key > key = "zero";
    global_map.guard().access()[ key ].guard().access().m_name = "Zero user";
    global_map.guard().access()[ key ].guard().access().m_card.guard().access().m_id = 1L;
    global_map.guard().access()[ key ].guard().access().m_card.guard().access().m_limit = 1;
}
#endif

// Вариант с вызовом глобальных методов guard/cguard с прямым вызовом метода
// члена класса access. Cовместим со значениями отличными от Instance.
void appendFirstVariant ()
{
    Variable< Key > key = "first";
    guard( guard( global_map ).access()[ key ] ).access().m_name = "First user";
    guard( guard( guard( global_map ).access()[ key ] ).access().m_card ).access().m_id = 1L;
    guard( guard( guard( global_map ).access()[ key ] ).access().m_card ).access().m_limit = 1;
}

// Вариант с вызовом глобальных методов guard/cguard и разыменованием.
// Cовместим со значениями отличными от Instance.
void appendSecondVariant ()
{
    Variable< Key > key = "second";
    (*guard( (*guard( global_map ))[ key ] )).m_name = "Second user";
    (*guard( (*guard( (*guard( global_map ))[ key ] )).m_card )).m_id = 2L;
    (*guard( (*guard( (*guard( global_map ))[ key ] )).m_card )).m_limit = 22;
}

// Вариант с двойным разыменованием.
// Не совместим со значениями отличными от Instance.
void appendThirdVariant ()
{
    Variable< Key > key = "third";
    (**(**global_map)[ key ]).m_name = "Third user";
    (**(**(**global_map)[ key ]).m_card).m_id = 3L;
    (**(**(**global_map)[ key ]).m_card).m_limit = 333;
}

// Вариант с использованием оператора ->.
// Не совместим со значениями отличными от Instance.
void appendFourthVariant ()
{
    Variable< Key > key = "fourth";
    global_map->operator[]( key )->m_name = "Fourth user";
    global_map->operator[]( key )->m_card->m_id = 4L;
    global_map->operator[]( key )->m_card->m_limit = 4444;
}

// Вариант со смешанным использованием двойного разыменования для доступа
// к значению и оператора -> для доступа к членам класса.
// Не совместим со значениями отличными от Instance.
void appendFifthVariant ()
{
    Variable< Key > key = "fifth";
    (**global_map)[ key ]->m_name = "Fifth user";
    (**global_map)[ key ]->m_card->m_id = 5L;
    (**global_map)[ key ]->m_card->m_limit = 55555;
}

// Вариант с использованием макроса vGet/cGet.
// Cовместим со значениями отличными от Instance.
void appendSixthVariant ()
{
    Variable< Key > key = "sixth";
    vGet( vGet( global_map )[ key ] ).m_name = "Sixth user";
    vGet( vGet( vGet( global_map )[ key ] ).m_card ).m_id = 5L;
    vGet( vGet( vGet( global_map )[ key ] ).m_card ).m_limit = 55555;
}

int main ( int, char ** )
{
    appendFirstVariant();
    appendSecondVariant();
    appendThirdVariant();
    appendFourthVariant();
    appendFifthVariant();
    appendSixthVariant();

    return 0;
}
