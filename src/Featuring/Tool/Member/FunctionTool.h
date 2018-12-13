#pragma once
#ifndef INSTANCE_TOOL_MEMBER_FUNCTION_H
#define INSTANCE_TOOL_MEMBER_FUNCTION_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Traits.h>

namespace _Invokable
{
    template < typename _InstanceRefer, typename _Invokable, typename ... _Arguments >
    struct FunctionTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using InstanceGuard = ::InstanceGuard< _InstanceRefer >;
            using ValueRefer = SimilarRefer< typename ::std::decay_t< _InstanceRefer >::Value, _InstanceRefer >;
            using ResultRefer = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;

            InstanceGuard m_feature_guard;
            ResultRefer m_result_refer;

            Holder ( _InstanceRefer instance, _Invokable invokable, _Arguments && ... arguments )
                : m_feature_guard( ::std::forward< _InstanceRefer >( instance ) )
                , m_result_refer( ::std::forward< ResultRefer >( invokable( ::std::forward< ValueRefer >( m_feature_guard.value() ), ::std::forward< _Arguments >( arguments ) ...  ) ) )
            {
            }

            Holder ( ThisType && other )
                : m_feature_guard( ::std::forward< InstanceGuard >( other.m_feature_guard ) )
                , m_result_refer( ::std::forward< ResultRefer >( other.m_result_refer ) )
            {
            }

            Holder ( const ThisType & other ) = delete;

            template < typename _Argument >
            void operator = ( _Argument && argument )
            {
                m_result_refer = ::std::forward< _Argument >( argument );
            }

            void operator = ( ThisType && other )
            {
                *this = ::std::forward< ResultRefer >( other.m_result_refer );
            }

            void operator = ( const ThisType && other )
            {
                *this = ::std::forward< const ResultRefer >( other.m_result_refer );
            }

            void operator = ( ThisType & other )
            {
                *this = other.m_result_refer;
            }

            void operator = ( const ThisType & other )
            {
                *this = other.m_result_refer;
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > && other )
            {
                *this = ::std::forward< typename Holder< _OtherValue >::ResultRefer >( other.m_result_refer );
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > && other )
            {
                *this = ::std::forward< const typename Holder< _OtherValue >::ResultRefer >( other.m_result_refer );
            }

            template < typename _OtherValue >
            void operator = ( Holder< _OtherValue > & other )
            {
                *this = other.m_result_refer;
            }

            template < typename _OtherValue >
            void operator = ( const Holder< _OtherValue > & other )
            {
                *this = other.m_result_refer;
            }

            //static constexpr void guard ( ThisType && )
            //static constexpr void guard ( const ThisType && )
            //static constexpr void guard ( ThisType & )
            static constexpr void guard ( const ThisType & )
            {
                // nothing to do
            }

            //static constexpr void unguard ( ThisType && )
            //static constexpr void unguard ( const ThisType && )
            //static constexpr void unguard ( ThisType & )
            static constexpr void unguard ( const ThisType & )
            {
                // nothing to do
            }

            static constexpr _Type && value ( ThisType && holder )
            {
                return ::std::forward< _Type >( holder.m_result_refer );
            }

            static constexpr const _Type && value ( const ThisType && holder )
            {
                return ::std::forward< const _Type >( holder.m_result_refer );
            }

            static constexpr _Type & value ( ThisType & holder )
            {
                return holder.m_result_refer;
            }

            static constexpr const _Type & value ( const ThisType & holder )
            {
                return holder.m_result_refer;
            }
        };
    };
}

#endif
