#pragma once
#ifndef ACCESS_HELPER_H
#define ACCESS_HELPER_H

#include <memory>

template < typename _ReferType >
class ValueProxy
{
    using ThisType = ValueProxy< _ReferType >;

public:
    using ReferType = _ReferType;

private:
    ReferType m_refer;

public:
    ValueProxy ( ReferType refer )
    : m_refer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    ValueProxy ( ThisType && other ) = default;

    ReferType refer () const
    {
        return ::std::forward< ReferType >( m_refer );
    }
};

/*!
 * Шаблоный класс, специализация которого определяет правила доступа
 * к внутреннему значению обертки.
 */
template < typename _ReferType >
class AccessProxy
{
    using ThisType = AccessProxy< _ReferType >;

public:
    using ReferType = _ReferType;
    using ValueType = typename ::std::remove_reference< ReferType >::type;
    using ProxyType = ValueProxy< _ReferType >;

private:
    ProxyType m_proxy;

public:
    AccessProxy ( ReferType refer )
    : m_proxy( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    AccessProxy ( ThisType && other ) = default;

    constexpr ProxyType * operator -> ()
    {
        return &m_proxy;
    }
};

/*!
 * Метод доступа к внутреннему содержимому
 */
template < typename _WrapperType >
inline constexpr AccessProxy< _WrapperType & > accessValue ( _WrapperType & wrapper ) noexcept
{
    return AccessProxy< _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr AccessProxy< const _WrapperType & > accessValue ( const _WrapperType & wrapper ) noexcept
{
    return AccessProxy< const _WrapperType & >( wrapper );
}

template < typename _WrapperType >
inline constexpr AccessProxy< _WrapperType && > accessValue ( _WrapperType && wrapper ) noexcept
{
    return AccessProxy< _WrapperType && >( ::std::forward< _WrapperType >( wrapper ) );
}

template < typename _WrapperType >
inline constexpr AccessProxy< const _WrapperType & > accessConstValue ( const _WrapperType & wrapper ) noexcept
{
    return AccessProxy< const _WrapperType & >( wrapper );
}

#define g_get( value ) accessValue( value )->refer()
#define c_get( value ) accessConstValue( value )->refer()

#endif
