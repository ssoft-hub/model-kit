#pragma once

#include <ModelKit/Common/ValueGuard.h>

template < typename _ValueType, typename _ValueTool >
class Instance;

/*!
 * Класс - защитник всех свойств для экземпляра значения.
 * Реализует специализацию для значений обернутых в Instance.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
struct InstanceValueGuard
{
    using ThisType = InstanceValueGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using InstanceType = typename ::std::remove_reference< ReferType >::type;
    using ValueTool = typename InstanceType::ValueTool;

    using ValueReferType = typename ::std::conditional<
        ::std::is_const< InstanceType >::value,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename InstanceType::ValueType &&,
            const typename InstanceType::ValueType & >::type,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            typename InstanceType::ValueType &&,
            typename InstanceType::ValueType & >::type >::type;

    using HolderReferType = typename ::std::conditional<
        ::std::is_const< InstanceType >::value,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename InstanceType::HolderType &&,
            const typename InstanceType::HolderType & >::type,
        typename ::std::conditional<
            ::std::is_rvalue_reference< ReferType >::value,
            typename InstanceType::HolderType &&,
            typename InstanceType::HolderType & >::type >::type;

    using FeatureGuardType = FeatureGuard< ReferType >;
    using ValueGuardType = ValueGuard< ValueReferType >;
    using AccessType = typename ValueGuardType::AccessType;

private:
    FeatureGuardType m_feature_guard;
    ValueGuardType m_internal_guard;

public:
    constexpr InstanceValueGuard ()
    : m_feature_guard()
    , m_internal_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValueGuard ( FeatureGuardType && other )
    : m_feature_guard( ::std::forward< FeatureGuardType >( other ) )
    , m_internal_guard( ValueTool::value(
        ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< FeatureGuardType >( other.m_feature_guard ) )
    , m_internal_guard( ::std::forward< ValueGuardType >( other.m_internal_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_feature_guard;
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_internal_guard );
    }

    constexpr const ValueGuardType & operator -> () const
    {
        return m_internal_guard;
    }
};


template < typename _ValueType, typename _ValueTool >
struct ValueGuardHelper< Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceValueGuard< Instance< _ValueType, _ValueTool > & >;
};

template < typename _ValueType, typename _ValueTool >
struct ValueGuardHelper< Instance< _ValueType, _ValueTool > && >
{
    using type = InstanceValueGuard< Instance< _ValueType, _ValueTool > && >;
};

template < typename _ValueType, typename _ValueTool >
struct ValueGuardHelper< const Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceValueGuard< const Instance< _ValueType, _ValueTool > & >;
};
