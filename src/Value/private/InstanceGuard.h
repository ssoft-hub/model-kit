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
    using AccessType = ReferType;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using InstanceTool = typename InstanceType::ValueTool;

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
        InstanceTool::guardHolder( m_pointer->m_holder );
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
            InstanceTool::unguardHolder( m_pointer->m_holder );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType access () const
    {
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr AccessType operator * () const
    {
        return access();
    }

    constexpr const InstancePointer & operator -> () const
    {
        return m_pointer;
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
    using AccessType = ReferType;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using InstanceTool = typename InstanceType::ValueTool;

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
        InstanceTool::guardHolder( m_pointer->m_holder );
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
            InstanceTool::unguardHolder( m_pointer->m_holder );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType access () const
    {
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr AccessType operator * () const
    {
        return access();
    }

    constexpr const InstancePointer & operator -> () const
    {
        return m_pointer;
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
    using AccessType = ReferType;
    using InstancePointer = ReferPointer< ReferType >;
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using InstanceTool = typename InstanceType::ValueTool;
    using HolderType = typename InstanceType::HolderType;

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
        InstanceTool::guardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
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
            InstanceTool::unguardHolder( ::std::forward< HolderType >( m_pointer->m_holder ) );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr ReferType access () const
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
    using AccessType = typename NextGuardType::AccessType;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_feature_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_feature_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( featureGuard( ValueTool::value( m_feature_guard.access().m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_feature_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( featureGuard( ValueTool::value( m_feature_guard.access().m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< GuardType >( other.m_feature_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_feature_guard;
    }

    constexpr AccessType access () const
    {
        return m_next_guard.access();
    }

    constexpr AccessType operator * () const
    {
        return access();
    }

    constexpr const NextGuardType & operator -> () const
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
    using AccessType = typename NextGuardType::AccessType;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_feature_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_feature_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( featureGuard( ValueTool::value( m_feature_guard.access().m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_feature_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( featureGuard( ValueTool::value( m_feature_guard.access().m_holder ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< GuardType >( other.m_feature_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_feature_guard;
    }

    constexpr AccessType access () const
    {
        return m_next_guard.access();
    }

    constexpr AccessType operator * () const
    {
        return access();
    }

    constexpr const NextGuardType & operator -> () const
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
    using AccessType = typename NextGuardType::AccessType;

    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ValueTool = typename InstanceType::ValueTool;

private:
    GuardType m_feature_guard;
    NextGuardType m_next_guard;

public:
    constexpr ValueGuard ()
    : m_feature_guard()
    , m_next_guard()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    , m_next_guard( featureGuard( ValueTool::value(
        ::std::forward< HolderType >( m_feature_guard.access().m_holder ) ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_feature_guard( ::std::forward< GuardType >( other ) )
    , m_next_guard( featureGuard( ValueTool::value(
        ::std::forward< HolderType >( m_feature_guard.access().m_holder ) ) ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< GuardType >( other.m_feature_guard ) )
    , m_next_guard( ::std::forward< NextGuardType >( other.m_next_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_feature_guard;
    }

    constexpr AccessType access () const
    {
        return ::std::forward< AccessType >( m_next_guard.access() );
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( access() );
    }

    constexpr const NextGuardType & operator -> () const
    {
        return m_next_guard;
    }
};
