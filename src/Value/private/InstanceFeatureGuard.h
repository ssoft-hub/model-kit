#pragma once

#include <ModelKit/Common/FeatureGuard.h>

template < typename _ValueType, typename _ValueTool >
class Instance;

/*!
 * Класс - защитник свойства верхнего уровня для экземпляра значения.
 * Реализует специализацию для значений обернутых в Instance.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
class InstanceFeatureGuard
{
    using ThisType = InstanceFeatureGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = ReferType;

    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = ::std::remove_reference_t< ReferType >;
    using InstanceTool = typename InstanceType::ValueTool;

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
        InstanceTool::guardHolder( ::std::forward< HolderReferType >( m_pointer->m_holder ) );
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
            InstanceTool::unguardHolder( ::std::forward< HolderReferType >( m_pointer->m_holder ) );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr const InstancePointer & operator -> () const
    {
        return m_pointer;
    }

    constexpr decltype(auto) value () const
    {
        assert( m_pointer );
        return InstanceTool::value( ::std::forward< HolderReferType >( m_pointer->m_holder ) );
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

template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< const Instance< _ValueType, _ValueTool > && >
{
    using type = InstanceFeatureGuard< const Instance< _ValueType, _ValueTool > && >;
};

// disabled
template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< Instance< _ValueType, _ValueTool > > {};
template < typename _ValueType, typename _ValueTool >
struct FeatureGuardHelper< const Instance< _ValueType, _ValueTool > > {};
