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
    using ValueType = typename ::std::remove_reference< ReferType >::type;

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
    ValueProxy ( const ThisType & other ) = delete;

    ReferType access () const
    {
        return ::std::forward< ReferType >( m_refer );
    }
};

/*!
 * Шаблоный класс, специализация которого определяет правила доступа
 * к внутреннему значению обертки.
 */
template < typename _ReferType >
class FeatureGuard
{
    using ThisType = FeatureGuard< _ReferType >;

public:
    using ReferType = _ReferType;
    using ValueType = typename ::std::remove_reference< ReferType >::type;
    using ProxyType = ValueProxy< _ReferType >;

private:
    ProxyType m_proxy;

public:
    FeatureGuard ( ReferType refer )
    : m_proxy( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    FeatureGuard ( ThisType && other ) = default;
    FeatureGuard ( const ThisType & other ) = delete;

    constexpr ProxyType * operator -> ()
    {
        return &m_proxy;
    }
};

/*!
 * Методы формирования FeatureGuard для применения заданного к внутреннему
 * экземпляру значения свойства.
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

/*!
 * Методы доступа к экземпляру значения с применеием всех задекларированных свойств.
 */
#define v_guard( value ) valueFeatureGuard( value )
#define c_guard( value ) cvalueFeatureGuard( value )

#define g_get( value ) value->access()

#define v_get( value ) g_get( v_guard( value ) )
#define c_get( value ) g_get( c_guard( value ) )

#endif
