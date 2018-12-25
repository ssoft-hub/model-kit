#pragma once
#ifndef OPERATOR_RESOLVER_H
#define OPERATOR_RESOLVER_H

#include <ModelKit/Featuring/Tool/Inplace/DefaultTool.h>
#include "../Tool/Member/FunctionTool.h"
#include <ModelKit/Featuring/Traits.h>
#include <type_traits>
#include <utility>

template < typename _Value, typename _Tool >
class Instance;
namespace Inplace { struct DefaultTool; }

namespace Operator
{
    namespace Private
    {
        /*
         * Если результат воздействия оператора возвращает
         *      - значение не оборачиваемого типа (фундаментальный или перечисляемый), тогда возвращается само значение не оборачиваемого типа;
         *      - значение оборачиваемого типа Value, тогда возвращается Instance< Value, DefaultTool >;
         *      - ссылку на значение любого типа Value, тогда
         *          - если Instance является и правым, и левым операндом, Instance< Value, BothOperandTool >
         *          - если Instance является только правым операндом, Instance< Value, RightOperandTool >
         *          - если Instance является только левым операндом, Instance< Value, LeftOperandTool >
         */

        template < typename _Kind >
        struct InvokeHelper;

        struct FundamentalKind {};
        struct DefaultInstanceKind {};
        struct RightBlockedInstanceKind {};
        struct LeftBlockedInstanceKind {};
        struct BothBlockedInstanceKind {};

        template < typename _Returned, typename _Right, typename _Left >
        struct OperatorKindHelper
        {
            using Returned = _Returned;
            using Right = ::std::decay_t< _Right >;
            using Left = ::std::decay_t< _Left >;

            static constexpr bool returned_is_not_wrappable = ::std::is_fundamental< Returned >::value || ::std::is_enum< Returned >::value;
            static constexpr bool returned_is_reference = ::std::is_reference< Returned >::value;
            static constexpr bool right_is_instance = ::is_instance< Right >;
            static constexpr bool left_is_instance = ::is_instance< Left >;

            using Type = ::std::conditional_t< returned_is_not_wrappable,
                FundamentalKind,
                ::std::conditional_t< !returned_is_reference || ( !right_is_instance && !left_is_instance ),
                    DefaultInstanceKind,
                    ::std::conditional_t< right_is_instance && left_is_instance,
                        BothBlockedInstanceKind,
                        ::std::conditional_t< right_is_instance,
                            RightBlockedInstanceKind,
                            LeftBlockedInstanceKind > > > >;
        };

        template <>
        struct InvokeHelper< FundamentalKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                return Invokable( ::std::forward< ValueRefer >( InstanceGuard< _InstanceRefer >( ::std::forward< _InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... );
            }
        };

        template <>
        struct InvokeHelper< RightBlockedInstanceKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::_Invokable::FunctionTool< _InstanceRefer, _Invokable, _Arguments ... >;
                return Instance< Result, Tool >( ::std::forward< _InstanceRefer >( instance ), invokable, ::std::forward< _Arguments >( arguments ) ...  );
            }
        };

        template <>
        struct InvokeHelper< DefaultInstanceKind >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = ::SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Inplace::DefaultTool;
                return Instance< Result, Tool >( invokable( ::std::forward< ValueRefer >( InstanceGuard< _InstanceRefer >( ::std::forward< _InstanceRefer >( instance ) ).value() ), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };
    }

    template < typename _Returned, typename _Right, typename _Left >
    using OperatorKind = typename Private::OperatorKindHelper< _Returned, _Right, _Left >::Type;

    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    static decltype(auto) invoke ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
    {
        using ValueRefer = ::SimilarRefer< typename ::std::remove_reference_t< _InstanceRefer >::Value, _InstanceRefer >;
        using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
        //using Returned = decltype( ::std::declval< _Invokable >().operator () ( ::std::declval< ValueRefer >(), ::std::declval< _Arguments >() ... ) );
        return Private::InvokeHelper< OperatorKind< Returned, ValueRefer, void > >:: template invoke< _InstanceRefer >(
            ::std::forward< _InstanceRefer >( instance ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

#endif
