#pragma once

namespace Cpp
{
    namespace Raw
    {
        struct RelationTool
        {
            template < typename _Type >
            using ReferHolder = _Type *;

            template < typename _Type >
            using ReferHolderPtr = const ReferHolder< _Type > *;
        };
    }
}
