#pragma once

#include <ModelKit/Common/ValuePointer.h>

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
class InstanceValuePointer
{
    using ThisType = InstanceValuePointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using InstanceType = ::std::remove_reference_t< ReferType >;
    using ValueTool = typename InstanceType::ValueTool;

    using ValueReferType = ::std::conditional_t<
        ::std::is_const< InstanceType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename InstanceType::ValueType &&,
            const typename InstanceType::ValueType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            typename InstanceType::ValueType &&,
            typename InstanceType::ValueType & > >;

    using HolderReferType = ::std::conditional_t<
        ::std::is_const< InstanceType >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            const typename InstanceType::HolderType &&,
            const typename InstanceType::HolderType & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< ReferType >::value,
            typename InstanceType::HolderType &&,
            typename InstanceType::HolderType & > >;

    using FeaturePointerType = FeaturePointer< ReferType >;
    using ValuePointerType = ValuePointer< ValueReferType >;
    using AccessType = typename ValuePointerType::AccessType;

private:
    FeaturePointerType m_feature_guard;
    ValuePointerType m_internal_guard;

public:
    constexpr InstanceValuePointer ()
            : m_feature_guard()
        , m_internal_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValuePointer ( ReferType refer )
            : m_feature_guard( ::std::forward< ReferType >( refer ) )
        , m_internal_guard( ValueTool::value( ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValuePointer ( FeaturePointerType && other )
            : m_feature_guard( ::std::forward< FeaturePointerType >( other ) )
        , m_internal_guard( ValueTool::value( ::std::forward< HolderReferType >( (*m_feature_guard).m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    InstanceValuePointer ( ThisType && other )
            : m_feature_guard( ::std::forward< FeaturePointerType >( other.m_feature_guard ) )
        , m_internal_guard( ::std::forward< ValuePointerType >( other.m_internal_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    //constexpr bool operator ! () const
    //{
    //    return !m_feature_guard;
    //}

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_internal_guard );
    }

    constexpr const ValuePointerType & operator -> () const
    {
        return m_internal_guard;
    }
};

template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceValuePointer< Instance< _ValueType, _ValueTool > & >;
};

template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< Instance< _ValueType, _ValueTool > && >
{
    using type = InstanceValuePointer< Instance< _ValueType, _ValueTool > && >;
};

template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< const Instance< _ValueType, _ValueTool > & >
{
    using type = InstanceValuePointer< const Instance< _ValueType, _ValueTool > & >;
};

template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< const Instance< _ValueType, _ValueTool > && >
{
    using type = InstanceValuePointer< const Instance< _ValueType, _ValueTool > && >;
};

// disabled
template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< Instance< _ValueType, _ValueTool > > {};
template < typename _ValueType, typename _ValueTool >
struct ValuePointerHelper< const Instance< _ValueType, _ValueTool > > {};
