#pragma once
#include <Helper/FeatureGuard.h>

template < typename _ValueType, typename _ValueTool >
class Instance;

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using type = Instance< _ValueType, _ValueTool >;
    using HolderType = typename type::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardWritableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardWritableHolder( m_refer.m_holder );
    }

    constexpr ReferType value ()
    {
        return m_refer;
    }

    constexpr FeatureGuard< ValueType & > operator -> ()
    {
        return ValueTool::writableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для const Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< const Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using type = Instance< _ValueType, _ValueTool >;
    using HolderType = typename type::HolderType;
    using ReferType = const Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardReadableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardReadableHolder( m_refer.m_holder );
    }

    constexpr ReferType value ()
    {
        return m_refer;
    }

    constexpr FeatureGuard< const ValueType & > operator -> ()
    {
        return ValueTool::readableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &&.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > && >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > && >;

public:
    using type = Instance< _ValueType, _ValueTool >;
    using HolderType = typename type::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    constexpr ReferType value ()
    {
        return ::std::forward< ReferType >( m_refer );
    }

    constexpr FeatureGuard< ValueType && > operator -> ()
    {
        return ValueTool::movableValueGuard(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }
};
