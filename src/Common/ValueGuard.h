#pragma once

#include <Common/FeatureGuard.h>

/*!
 * Класс - защитник всех свойств текущего значения. Предоставляет доступ к
 * занчению посредством последовательного раскрытия метода access().
 * Специализация данного шаблона определеяет каким образом защищается значение.
 */
template < typename _ReferType >
class DefaultValueGuard
{
    using ThisType = DefaultValueGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using GuardType = FeatureGuard< _ReferType >;
    using AccessType = ReferType;

private:
    GuardType m_feature_guard;

public:
    constexpr DefaultValueGuard ()
    : m_feature_guard()
    {
    }

    DefaultValueGuard ( ReferType refer )
    : m_feature_guard( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultValueGuard ( GuardType && other )
    : m_feature_guard( ::std::forward< GuardType >( other ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultValueGuard ( ThisType && other )
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
 * Специализация данного шаблона определеяет каким образом защищается значение.
 * По умолчанию для всех видов ссылок используется DefaultValueGuard.
 */
template < typename _ReferType >
struct ValueGuardHelper
{
    using type = DefaultValueGuard< _ReferType >;
};

/*!
 * Класс - защитник свойства значения. Введен для удобства использования,
 * переопределяет typename ValueGuardHelper< _ReferType >::type.
 */
template < typename _ReferType >
using ValueGuard = typename ValueGuardHelper< _ReferType >::type;

/*!
 * Методы формирования ValueGuard для применения заданного набора свойств
 * при осущесвлении доступа к внутреннему экземпляру значения.
 */
template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType & > guard ( _WrapperType & wrapper ) noexcept
{
    return wrapper;
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > guard ( const _WrapperType & wrapper ) noexcept
{
    return wrapper;
}

template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType && > guard ( _WrapperType && wrapper ) noexcept
{
    return ::std::forward< _WrapperType >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > cguard ( const _WrapperType & wrapper ) noexcept
{
    return wrapper;
}

#define vGet( value ) guard( value ).access()
#define cGet( value ) cguard( value ).access()
