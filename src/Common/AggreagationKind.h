#pragma once

enum class AggregationKind
{
    None,       //!< Indicates that the Instance has no aggregation semantics.
    Shared,     //!< Indicates that the Instance has shared aggregation semantics.
                ///  Precise semantics of shared aggregation varies by application area and modeler.
    Composite   //!< Indicates that the Instance is aggregated compositely, i.e., the composite object
                ///  has responsibility for the existence and storage of the composed objects.
};
