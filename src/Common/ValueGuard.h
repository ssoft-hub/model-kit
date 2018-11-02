#pragma once

#include <ModelKit/Common/FeatureGuard.h>

/*!
 * Класс - защитник всех свойств для экземпляра значения.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
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

    //constexpr bool operator ! () const
    //{
    //    return !m_feature_guard;
    //}

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_feature_guard );
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
template < typename _ReferType >
using ValueGuardHelper_t = typename ValueGuardHelper< _ReferType >::type;

/*!
 * Класс - защитник свойства значения. Введен для удобства использования,
 * переопределяет typename ValueGuardHelper_t< _ReferType >.
 */
template < typename _ReferType >
using ValueGuard = ValueGuardHelper_t< _ReferType >;
