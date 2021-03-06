#pragma once

enum class AggregationKind
{
    None,       //!< Indicates that the Property has no aggregation semantics.
    Shared,     //!< Indicates that the Property has shared aggregation semantics.
                ///  Precise semantics of shared aggregation varies by application area and modeler.
    Composite   //!< Indicates that the Property is aggregated compositely, i.e., the composite object
                ///  has responsibility for the existence and storage of the composed objects.
};
