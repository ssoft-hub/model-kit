#pragma once
#ifndef OPERATOR_RESOLVER_H
#define OPERATOR_RESOLVER_H

#include <ModelKit/Featuring/Tool/Inplace/DefaultTool.h>
#include <ModelKit/Featuring/Tool/Member/FunctionTool.h>
#include <ModelKit/Featuring/Traits.h>

#include <functional>
#include <type_traits>
#include <utility>

template < typename _Value, typename _Tool >
class Instance;

namespace Operator
{
    namespace Private
    {
        template < typename _Refer, typename ... _Arguments >
        struct SquareBrackets
        {
            decltype(auto) operator () ( _Refer refer, _Arguments && ... arguments )
            {
                return refer.operator[] ( ::std::forward< _Arguments >( arguments ) ... );
            }
        };
    }

    namespace Private
    {
        template < typename _Refer, typename ... _Arguments >
        struct RoundBrackets
        {
            decltype(auto) operator () ( _Refer refer, _Arguments && ... arguments )
            {
                return refer.operator() ( ::std::forward< _Arguments >( arguments ) ... );
            }
        };
    }
}

namespace Operator
{
    namespace Private
    {
        template < bool _use_featured >
        struct InvokeHelper;

        template <>
        struct InvokeHelper< true >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer featured, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Invokable::FunctionTool< _InstanceRefer, _Invokable, _Arguments ... >;

                return Instance< Result, Tool >( ::std::forward< _InstanceRefer >( featured ), invokable, ::std::forward< _Arguments >( arguments ) ...  );
            }
        };

        template <>
        struct InvokeHelper< false >
        {
            template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _InstanceRefer featured, _Invokable invokable, _Arguments && ... arguments )
            {
                using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
                using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Inplace::DefaultTool;

                return Instance< Result, Tool >( invokable( ::std::forward< ValueRefer >( InstanceGuard< _InstanceRefer >( featured ).value() ), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };
    }

    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    static decltype(auto) invoke ( _InstanceRefer featured, _Invokable invokable, _Arguments && ... arguments )
    {
        using ValueRefer = SimilarRefer< typename ::std::remove_reference_t< _InstanceRefer >::Value, _InstanceRefer >;
        using Returned = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;

        // Возвращаем результат в виде Fetured< Result, Inplace::DefaultTool >, если оператор возвращает значение.
        // Возвращаем результат в виде Fetured< Result, Member::FunctionTool >, если оператор возвращает ссылку.
        return Private::InvokeHelper< ::std::is_reference< Returned >::value >:: template invoke< _InstanceRefer >(
            ::std::forward< _InstanceRefer >( featured ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

#endif
