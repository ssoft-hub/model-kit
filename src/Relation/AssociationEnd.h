#pragma once


enum class AggregationKind
{
    None,       //!< Indicates that the Property has no aggregation semantics.
    Shared,     //!< Indicates that the Property has shared aggregation semantics.
                ///  Precise semantics of shared aggregation varies by application area and modeler.
    Composite   //!< Indicates that the Property is aggregated compositely, i.e., the composite object
                ///  has responsibility for the existence and storage of the composed objects.
};


/*!
 * Полюс ассоциативной связи со значением. Подразумевает возможность измненения
 * внутреннего значения без изменения самого полюса.
 *
 * Первый параметр определяет тип значения, второй - средства реализации полюса
 * ассоциативной связи.
 *
 * Ассоциативные связи, реализованные посредством разных инструментов не
 * являются совместимыми. Однако в некоторых случаях можно реализовать
 * преобразование из одной реализации в другую.
 */
template < typename _Type, AggregationKind _kind, typename _RelationTool >
class AssociationEnd;

template < typename _Type, typename _RelationTool >
using NoneEnd = AssociationEnd< _Type, AggregationKind::None, _RelationTool >;

template < typename _Type, typename _RelationTool >
using SharedEnd = AssociationEnd< _Type, AggregationKind::Shared, _RelationTool >;

template < typename _Type, typename _RelationTool >
using CompositeEnd = AssociationEnd< _Type, AggregationKind::Composite, _RelationTool >;
