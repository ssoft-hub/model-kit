#pragma once
#ifndef OPERATOR_RESOLVER_H
#define OPERATOR_RESOLVER_H

#include <ModelKit/Instance/Tool/Inplace/DefaultTool.h>
#include <ModelKit/Instance/Tool/Member/FunctionTool.h>

#include <functional>
#include <type_traits>
#include <utility>

template < typename _Value, typename _Tool >
class Featured;

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
            template < typename _FeaturedRefer, typename _ValueRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _FeaturedRefer featured, _Invokable invokable, _Arguments && ... arguments )
            {
                using Tool = ::Member::FunctionTool< _FeaturedRefer, _ValueRefer, _Invokable, _Arguments ... >;
                using Returned = ::std::result_of_t< _Invokable( _ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                return Featured< Result, Tool >( ::std::forward< _FeaturedRefer >( featured ), invokable, ::std::forward< _Arguments >( arguments ) ...  );
            }
        };

        template <>
        struct InvokeHelper< false >
        {
            template < typename _FeaturedRefer, typename _ValueRefer, typename _Invokable, typename ... _Arguments >
            static decltype(auto) invoke ( _FeaturedRefer featured, _Invokable invokable, _Arguments && ... arguments )
            {
                using Returned = ::std::result_of_t< _Invokable( _ValueRefer, _Arguments && ... ) >;
                using Result = ::std::remove_reference_t< Returned >;
                using Tool = ::Inplace::DefaultTool;
                FeaturedPointer< _FeaturedRefer > guard = featured;
                return Featured< Result, Tool >( invokable( ::std::forward< _ValueRefer >( guard.value() ), ::std::forward< _Arguments >( arguments ) ... ) );
            }
        };
    }

    //Featured< Result, FunctionTool >( featured, invokable, ::std::forward< _Arguments >( arguments ) ... );
    //Featured< Result, DefaultTool >( function( *&featured, ::std::forward< _Arguments >( arguments ) ... ) );

    template < typename _FeaturedRefer, typename _ValueRefer, typename _Invokable, typename ... _Arguments >
    static decltype(auto) invoke ( _FeaturedRefer featured, _Invokable invokable, _Arguments && ... arguments )
    {
        // Возвращаем результат в виде Fetured< Result, Inplace::DefaultTool >, если оператор возвращает значение.
        // Возвращаем результат в виде Fetured< Result, Member::FunctionTool >, если оператор возвращает ссылку.

        using Returned = ::std::result_of_t< _Invokable( _ValueRefer, _Arguments && ... ) >;
        return Private::InvokeHelper< ::std::is_reference< Returned >::value >:: template invoke< _FeaturedRefer,_ValueRefer >(
            ::std::forward< _FeaturedRefer >( featured ), invokable, ::std::forward< _Arguments >( arguments ) ... );
    }
};

#endif
