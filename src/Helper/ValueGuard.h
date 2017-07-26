#pragma once

#include <Helper/FeatureGuard.h>

/*!
 * Класс - защитник всех свойств текущего значения. Предоставляет доступ к
 * занчению посредством последовательного раскрытия метода access().
 * Специализация данного шаблона определеяет каким образом защищается значение.
 */
template < typename _ReferType >
class ValueGuard
{
    using ThisType = ValueGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using GuardType = FeatureGuard< _ReferType >;
    using AccessType = ReferType;

private:
    GuardType m_feature_guard;

public:
    constexpr ValueGuard ()
    : m_feature_guard()
    {
    }

    ValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_feature_guard( ::std::forward< GuardType >( other ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_feature_guard( ::std::forward< GuardType >( other.m_feature_guard ) )
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
        return ::std::forward< AccessType >( m_feature_guard.access() );
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( access() );
    }

    constexpr const GuardType & operator -> () const
    {
        return m_feature_guard;
    }
};

/*!
 * Методы формирования ValueGuard для применения заданного набора свойств
 * при осущесвлении доступа к внутреннему экземпляру значения.
 */
template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType & > guard ( _WrapperType & wrapper ) noexcept
{
    return ValueGuard< _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > guard ( const _WrapperType & wrapper ) noexcept
{
    return ValueGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType && > guard ( _WrapperType && wrapper ) noexcept
{
    return ValueGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > cguard ( const _WrapperType & wrapper ) noexcept
{
    return ValueGuard< const _WrapperType & >( wrapper );
}

#define vGet( value ) guard( value ).access()
#define cGet( value ) cguard( value ).access()
