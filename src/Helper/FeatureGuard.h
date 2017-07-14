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
 * Методы для формирования защитников текущего свойства значения.
 */
template < typename _WrapperType >
inline constexpr FeatureGuard< _WrapperType & > valueFeatureGuard ( _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< const _WrapperType & > valueFeatureGuard ( const _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< _WrapperType && > valueFeatureGuard ( _WrapperType && wrapper ) noexcept
{
    return FeatureGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< const _WrapperType & > cvalueFeatureGuard ( const _WrapperType & wrapper ) noexcept
{
    return FeatureGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr FeatureGuard< _WrapperType && > mvalueFeatureGuard ( _WrapperType && wrapper ) noexcept
{
    return FeatureGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

/*!
 * Макросы, сокращающие запись при использовании защитников текущего свойства значения.
 */
#define vFGuard( value )  valueFeatureGuard( value )
#define cFGuard( value ) cvalueFeatureGuard( value )
#define mFGuard( value ) mvalueFeatureGuard( ::std::forward< decltype(value) >( value ) )

#define vFGet( value ) vFGuard( value ).access()
#define cFGet( value ) cFGuard( value ).access()
#define mFGet( value ) mFGuard( value ).access()
