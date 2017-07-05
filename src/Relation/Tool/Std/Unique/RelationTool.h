#pragma once
#include <memory>

namespace Std
{
    namespace Uniqie
    {
        struct RelationTool
        {
            template < typename _Type >
            using ReferHolder = ::std::weak_ptr< _Type >;

            template < typename _Type >
            using SharedHolder = ::std::shared_ptr< _Type >;

            template < typename _Type >
            using UniqueHolder = ::std::shared_ptr< _Type >;

            template < typename _Type >
            using AnyEnd = ::AnyEnd< ReferHolder< _Type >, SharedHolder< _Type >, UniqueHolder< _Type > >;
        };
    }
}
