#pragma once
#ifndef INSTANCE_TOOL_MEMBER_FUNCTION_H
#define INSTANCE_TOOL_MEMBER_FUNCTION_H

#include <ModelKit/Instance/Access/FeaturedPointer.h>
#include <ModelKit/Instance/Traits.h>

namespace Member
{
    template < typename _FeaturedRefer, typename _Invokable, typename ... _Arguments >
    struct FunctionTool
    {
        template < typename _Type >
        struct Holder
        {
            using ThisType = Holder;
            using FeaturedGuard = ::FeaturedPointer< _FeaturedRefer >;
            using ValueRefer = Similar< typename ::std::decay_t< _FeaturedRefer >::Value, _FeaturedRefer >;
            using ResultRefer = ::std::result_of_t< _Invokable( ValueRefer, _Arguments && ... ) >;

            FeaturedGuard m_feature_guard;
            ResultRefer m_result_refer;

            Holder ( _FeaturedRefer featured, _Invokable invokable, _Arguments && ... arguments )
                : m_feature_guard( ::std::forward< _FeaturedRefer >( featured ) )
                , m_result_refer( ::std::forward< ResultRefer >( invokable( ::std::forward< ValueRefer >( m_feature_guard.value() ), ::std::forward< _Arguments >( arguments ) ...  ) ) )
            {
            }

            Holder ( ThisType && other )
                : m_feature_guard( ::std::forward< FeaturedGuard >( other.m_feature_guard ) )
                , m_result_refer( ::std::forward< ResultRefer >( other.m_result_refer ) )
            {
            }

            Holder ( const ThisType & other ) = delete;

            template < typename _OtherType >
            ThisType & operator = ( _OtherType && other )
            {
                m_result_refer = ::std::forward< _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType && other )
            {
                m_result_refer = ::std::forward< const _OtherType >( other );
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( _OtherType & other )
            {
                m_result_refer = other;
                return *this;
            }

            template < typename _OtherType >
            ThisType & operator = ( const _OtherType & other )
            {
                m_result_refer = other;
                return *this;
            }
        };

        template < typename _Type >
        //static constexpr void guardHolder ( Holder< _Type > & )
        //static constexpr void guardHolder ( Holder< _Type > && )
        static constexpr void guardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        //static constexpr void unguardHolder ( Holder< _Type > & )
        //static constexpr void unguardHolder ( Holder< _Type > && )
        static constexpr void unguardHolder ( const Holder< _Type > & )
        {
            // nothing to do
        }

        template < typename _Type >
        static constexpr _Type & value ( Holder< _Type > & holder )
        {
            return holder.m_result_refer;
        }

        template < typename _Type >
        static constexpr const _Type & value ( const Holder< _Type > & holder )
        {
            return holder.m_result_refer;
        }

        template < typename _Type >
        static constexpr _Type && value ( Holder< _Type > && holder )
        {
            return ::std::forward< _Type >( holder.m_result_refer );
        }

        template < typename _Type >
        static constexpr const _Type && value ( const Holder< _Type > && holder )
        {
            return ::std::forward< const _Type >( holder.m_result_refer );
        }
    };
}

#endif
