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
struct InstanceFeatureGuard
{
    using ThisType = InstanceFeatureGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = ReferType;

    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = typename ::std::remove_reference< ReferType >::type;
    using InstanceTool = typename InstanceType::ValueTool;

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

    constexpr ReferType operator * () const
    {
        return ::std::forward< ReferType >( *m_pointer );
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
