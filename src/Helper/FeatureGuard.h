#pragma once

#include <Helper/ReferPointer.h>
#include <utility>

/*!
 * Класс - защитник свойства значения.
 * Предоставляет доступ к занчению посредством метода access().
 * Специализация данного шаблона определеяет каким образом защищается значение.
 */
template < typename _ReferType >
class FeatureGuard
{
    using ThisType = FeatureGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = _ReferType;
    using ValuePointer = ReferPointer< ReferType >;

private:
    ValuePointer m_pointer;

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
    }

    FeatureGuard ( ThisType && other )
    : m_pointer( ::std::forward< ValuePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType access () const
    {
        return ::std::forward< AccessType >( *m_pointer );
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( access() );
    }

    constexpr const ValuePointer & operator -> () const
    {
        return m_pointer;
    }
};

/*!
 * Методы для формирования защитников текущего свойства значения.
 */
template < typename _WrapperType >
inline constexpr FeatureGuard< _WrapperType & > featureGuard ( _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< const _WrapperType & > featureGuard ( const _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< _WrapperType && > featureGuard ( _WrapperType && wrapper ) noexcept
{
    return FeatureGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< const _WrapperType & > cfeatureGuard ( const _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< const _WrapperType & >( wrapper );
}
