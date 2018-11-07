#pragma once
#include <memory>

namespace Std
{
    namespace Shared
    {
        template < typename _Type, typename _Association >
        struct AssociationMaker;

        struct RelationTool
        {
            template < typename _Type >
            using ReferHolder = ::std::weak_ptr< _Type >;

            template < typename _Type >
            using SharedHolder = ::std::shared_ptr< _Type >;

            template < typename _Type >
            using UniqueHolder = ::std::shared_ptr< _Type >;

            template < typename _Type >
            using ReferHolderPtr = const ReferHolder< _Type > *;

            template < typename _Type >
            using SharedHolderPtr = const SharedHolder< _Type > *;

            template < typename _Type >
            using UniqueHolderPtr = const UniqueHolder< _Type > *;

            enum
            {
                NoneIndex,
                SharedIndex,
                UniqueIndex
            };

            template < typename _Type >
            using AnyEnd = ::std::tuple<
                ReferHolderPtr< _Type >,
                SharedHolderPtr< _Type >,
                UniqueHolderPtr< _Type > >;

            template < typename _Type >
            static inline AnyEnd< _Type > makeForNone ( const ReferHolder< _Type > & holder )
            {
                return AnyEnd< _Type >( &holder, nullptr, nullptr );
            }

            template < typename _Type >
            static inline AnyEnd< _Type > makeForShared ( const SharedHolder< _Type > & holder )
            {
                return AnyEnd< _Type >( nullptr, &holder, nullptr );
            }

            template < typename _Type >
            static inline AnyEnd< _Type > makeForUnique ( const UniqueHolder< _Type > & holder )
            {
                return AnyEnd< _Type >( nullptr, nullptr, &holder );
            }

            template < typename _Type, typename _Association >
            static _Association makeAssociation ( const AnyEnd< _Type > & variant )
            {
                return AssociationMaker< _Type, _Association >::make( variant );
            }
        };
    }
}
