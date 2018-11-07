//#pragma once

//#include <Relation/Tool/Std/Shared/RelationTool.h>
//#include <Relation/Tool/Std/Shared/ReferEnd.h>
//#include <Relation/Tool/Std/Shared/SharedEnd.h>
//#include <Relation/Tool/Std/Shared/UniqueEnd.h>

//namespace Std
//{
//    namespace Shared
//    {
//        template < typename _Type >
//        using ReferEnd = ::ReferEnd< _Type, ::Std::Shared::RelationTool >;

//        template < typename _Type >
//        using SharedEnd = ::SharedEnd< _Type, ::Std::Shared::RelationTool >;

//        template < typename _Type >
//        using UniqueEnd = ::UniqueEnd< _Type, ::Std::Shared::RelationTool >;

////        template < typename _Type, typename _Other >
////        struct AssociationMaker< _Type, ReferEnd< _Other > >
////        {
////            using ResultEnd = ReferEnd< _Other >;
////            using AnyEnd = typename Tool::AnyEnd< _Type >;

////            static ResultEnd make ( const AnyEnd & variant )
////            {
//////                if ( ::std::get< Tool::NoneIndex >( variant ) )
//////                {
//////                    return ResultEnd( *::std::get< Tool::NoneIndex >( variant ) );
//////                }
//////                else if ( ::std::get< Tool::SharedIndex >( variant ) )
//////                {
//////                    return ResultEnd( *::std::get< Tool::SharedIndex >( variant ) );
//////                }
//////                else if ( ::std::get< Tool::UniqueIndex >( variant ) )
//////                {
//////                    return ResultEnd( *::std::get< Tool::UniqueIndex >( variant ) );
//////                }

////                // ERROR
//////                return ResultEnd( *::std::get< Tool::NoneIndex >( variant ) );
////                return ResultEnd();
////            }
////        };

////        template < typename _Type, typename _Other >
////        struct AssociationMaker< _Type, SharedEnd< _Other > >
////        {
////            using ResultEnd = SharedEnd< _Other >;
////            using AnyEnd = typename Tool::AnyEnd< _Type >;

////            static ResultEnd make ( const AnyEnd & variant )
////            {
////                if ( ::std::get< Tool::SharedIndex >( variant ) )
////                {
////                    const Tool::SharedHolder< _Type > & reference
////                        = *::std::get< Tool::SharedIndex >( variant );
////                    return ResultEnd::make( reference );
////                }

////                // ERROR
////                return ResultEnd( *::std::get< Tool::NoneIndex >( variant ) );
////            }
////        };

////        template < typename _Type, typename _Other >
////        struct AssociationMaker< _Type, UniqueEnd< _Other > >
////        {
////            using ResultEnd = UniqueEnd< _Other >;
////            using AnyEnd = typename Tool::AnyEnd< _Type >;

////            static ResultEnd make ( const AnyEnd & variant )
////            {
////                // ERROR
////                return ResultEnd( *::std::get< Tool::NoneIndex >( variant ) );
////            }
////        };
//    }
//}
