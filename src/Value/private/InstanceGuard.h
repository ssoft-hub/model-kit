#pragma once

#include <Helper/ValueGuard.h>

template < typename _ValueType, typename _ValueTool >
class Instance;

// TODO: заменить бы одним классом все три специализации

///////////////////////////////////////////////////////////////////////////////

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using ReferType = Instance< _ValueType, _ValueTool > &;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    InstancePointer m_pointer;

public:
    constexpr FeatureGuard ()
    : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    FeatureGuard ( ReferType refer )
    : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardHolder( m_pointer->m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_pointer( ::std::forward< InstancePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~FeatureGuard ()
    {
        if ( !!m_pointer )
            ValueTool::unguardHolder( m_pointer->m_holder );
    }

    bool operator ! () const
    {
        return !m_pointer;
    }

    ReferType access () const
    {
        assert( !!m_pointer );
        return ::std::forward< ReferType >( *m_pointer );
    }
};

/*!
 * Специализация FeatureGuard для const Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< const Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< const Instance< _ValueType, _ValueTool > & >;

public:
    using ReferType = const Instance< _ValueType, _ValueTool > &;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = const typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    InstancePointer m_pointer;

public:
    constexpr FeatureGuard ()
    : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    FeatureGuard ( ReferType refer )
    : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardHolder( m_pointer->m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_pointer( ::std::forward< InstancePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~FeatureGuard ()
    {
        if ( !!m_pointer )
            ValueTool::unguardHolder( m_pointer->m_holder );
    }

    bool operator ! () const
    {
        return !m_pointer;
    }

    ReferType access () const
    {
        assert( !!m_pointer );
        return ::std::forward< ReferType >( *m_pointer );
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
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    InstancePointer m_pointer;

public:
    constexpr FeatureGuard ()
    : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    FeatureGuard ( ReferType refer )
    : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
    }

    FeatureGuard ( ThisType && other )
    : m_pointer( ::std::forward< InstancePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~FeatureGuard ()
    {
        if ( !!m_pointer )
            ValueTool::unguardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
    }

    bool operator ! () const
    {
        return !m_pointer;
    }

    ReferType access () const
    {
        assert( !!m_pointer );
        return ::std::forward< ReferType >( *m_pointer );
    }
};

////////////////////////////////////////////////////////////////////////////////

/*!
 * Специализация ValueGuard для Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct ValueGuard< Instance< _ValueType, _ValueTool > & >
{
    using ThisType = ValueGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using ReferType = Instance< _ValueType, _ValueTool > &;
    using GuardType = FeatureGuard< ReferType >;
    using NextGuardType = ValueGuard< _ValueType & >;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_guard( ::std::forward< GuardType >( other.m_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~ValueGuard ()
    {
        if ( !!m_guard )
            ValueTool::featureGuard( m_guard.access().m_holder );
    }

    constexpr bool operator ! () const
    {
        return !m_guard;
    }

    constexpr NextGuardType & operator -> ()
    {
        return m_next_guard;
    }
};

/*!
 * Специализация ValueGuard для const Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct ValueGuard< const Instance< _ValueType, _ValueTool > & >
{
    using ThisType = ValueGuard< const Instance< _ValueType, _ValueTool > & >;

public:
    using ReferType = const Instance< _ValueType, _ValueTool > &;
    using GuardType = FeatureGuard< ReferType >;
    using NextGuardType = ValueGuard< const _ValueType & >;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = const typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_guard( ::std::forward< GuardType >( other.m_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~ValueGuard ()
    {
        if ( !!m_guard )
            ValueTool::featureGuard( m_guard.access().m_holder );
    }

    constexpr bool operator ! () const
    {
        return !m_guard;
    }

    constexpr NextGuardType & operator -> ()
    {
        return m_next_guard;
    }
};

/*!
 * Специализация ValueGuard для Instance< _ValueType, _ValueTool > &&.
 */
template < typename _ValueType, typename _ValueTool >
struct ValueGuard< Instance< _ValueType, _ValueTool > && >
{
    using ThisType = ValueGuard< Instance< _ValueType, _ValueTool > && >;

public:
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using GuardType = FeatureGuard< ReferType >;
    using NextGuardType = ValueGuard< _ValueType && >;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ValueType = typename InstanceType::ValueType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( ValueTool::featureGuard( m_guard.access().m_holder ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_guard( ::std::forward< GuardType >( other.m_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ~ValueGuard ()
    {
        if ( !!m_guard )
            ValueTool::featureGuard( m_guard.access().m_holder );
    }

    constexpr bool operator ! () const
    {
        return !m_guard;
    }

    constexpr NextGuardType & operator -> ()
    {
        return m_next_guard;
    }
};
