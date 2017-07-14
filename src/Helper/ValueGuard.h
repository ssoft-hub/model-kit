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

private:
    GuardType m_guard;

public:
    constexpr ValueGuard ()
    : m_guard()
    {
    }

    ValueGuard ( ReferType refer )
    : m_guard( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( GuardType && other )
    : m_guard( ::std::forward< GuardType >( other ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueGuard ( ThisType && other )
    : m_guard( ::std::forward< GuardType >( other.m_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    bool operator ! () const
    {
        return !m_guard;
    }

    constexpr GuardType * operator -> ()
    {
        return &m_guard;
    }

    constexpr GuardType const * operator -> () const
    {
        return &m_guard;
    }
};

/*!
 * Методы формирования ValueGuard для применения заданного набора свойств
 * при осущесвлении доступа к внутреннему экземпляру значения.
 */
template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType & > valueValueGuard ( _WrapperType & wrapper ) noexcept
{
    return ValueGuard< _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > valueValueGuard ( const _WrapperType & wrapper ) noexcept
{
    return ValueGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType && > valueValueGuard ( _WrapperType && wrapper ) noexcept
{
    return ValueGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

template < typename _WrapperType >
inline constexpr ValueGuard< const _WrapperType & > cvalueValueGuard ( const _WrapperType & wrapper ) noexcept
{
    return ValueGuard< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr ValueGuard< _WrapperType && > mvalueValueGuard ( _WrapperType && wrapper ) noexcept
{
    return ValueGuard< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

/*!
 * Макросы, сокращающие запись при использовании защитников для доступа к внутреннему
 * экземпляру значения.
 */
#define vGuard( value )  valueValueGuard( value )
#define cGuard( value ) cvalueValueGuard( value )
#define mGuard( value ) mvalueValueGuard( ::std::forward< decltype(value) >( value ) )

#define gGet( value ) value->access()

#define vGet( value ) gGet( vGuard( value ) )
#define cGet( value ) gGet( cGuard( value ) )
#define mGet( value ) gGet( mGuard( value ) )
