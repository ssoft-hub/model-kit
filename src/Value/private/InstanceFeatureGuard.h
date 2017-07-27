#pragma once

#include <ModelKit/Common/FeatureGuard.h>

template < typename _ValueType, typename _ValueTool >
class Instance;

/*!
 *
 */
template < typename _ReferType >
struct InstanceFeatureGuard
{
    using ThisType = InstanceFeatureGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = ReferType;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = typename ::std::remove_reference< ReferType >::type;
    using InstanceTool = typename InstanceType::ValueTool;
    using HolderType = typename ::std::conditional<
        ::std::is_const< InstanceType >::value,
        const typename InstanceType::HolderType &,
        typename InstanceType::HolderType >::type;

private:
    InstancePointer m_pointer;

public:
    constexpr InstanceFeatureGuard ()
    : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceFeatureGuard ( ReferType refer )
    : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        InstanceTool::guardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
    }

    InstanceFeatureGuard ( ThisType && other )
    : m_pointer( ::std::forward< InstancePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~InstanceFeatureGuard ()
    {
        if ( !!m_pointer )
            InstanceTool::unguardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType access () const
    {
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr ReferType operator * () const
    {
        return access();
    }

    constexpr const InstancePointer & operator -> () const
    {
        return m_pointer;
    }
};


template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceFeatureGuard< Instance< _ValueType, _ValueTool > & >;
};

template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< Instance< _ValueType, _ValueTool > && >
{
    using type = InstanceFeatureGuard< Instance< _ValueType, _ValueTool > && >;
};

template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< const Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceFeatureGuard< const Instance< _ValueType, _ValueTool > & >;
};
