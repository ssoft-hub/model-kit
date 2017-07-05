#pragma once
#include <Relation/AssociationEnd.h>

/*!
 * Полюс ассоциативной связи ассоциации агрегирования.
 * Ассоциативная связь агрегации реализует отношение Часть/Целое.
 */
template < typename _Type, typename _RelationTool >
class AggregationEnd
    : public AssociationEnd< _Type, _RelationTool > {};
