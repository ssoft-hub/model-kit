#pragma once
#ifndef ACCESS_HELPER_H
#define ACCESS_HELPER_H

#include <memory>

/*!
 * Шаблоный класс, специализация которого определяет правила доступа к внутреннему
 * значению обертки. В реализации "по умолчанию" принимается, что тип входящего
 * параметра In соответствует типу возвращаемого Return.
 *
 * Доступ к внутреннему значению должен быть специализирован для двух случаев
 * (чтения и записи), которые определяют возможность изменения значения
 * в зависимости от возможности изменения обертки.
 */
template < typename _RequaredType, typename _WrapperType >
struct AccessHelper
{
    using RequaredType = typename ::std::decay< _RequaredType >::type;
    using WrapperType = typename ::std::decay< _WrapperType >::type;

    static constexpr RequaredType * writableProxy (
        WrapperType & wrapper ) noexcept
    {
        static_assert( ::std::is_same< RequaredType, WrapperType >::value
                || ::std::is_base_of< RequaredType, WrapperType >::value,
            "The wrapper type is not compatible with requared." );
        return &wrapper;
    }

    static constexpr const RequaredType * readableProxy (
        const WrapperType & wrapper ) noexcept
    {
        static_assert( ::std::is_same< RequaredType, WrapperType >::value
                || ::std::is_base_of< RequaredType, WrapperType >::value,
            "The wrapper type is not compatible with requared." );
        return &wrapper;
    }

    static constexpr RequaredType &
        writable ( WrapperType & wrapper ) noexcept
    {
        static_assert( ::std::is_same< RequaredType, WrapperType >::value
                || ::std::is_base_of< RequaredType, WrapperType >::value,
            "The types Value and Wrapper are not compatible" );
        return wrapper;
    }

    static constexpr const RequaredType &
        readable ( const WrapperType & wrapper ) noexcept
    {
        static_assert( ::std::is_same< RequaredType, WrapperType >::value
                || ::std::is_base_of< RequaredType, WrapperType >::value,
            "The types Value and Wrapper are not compatible" );
        return wrapper;
    }

    static constexpr RequaredType &&
        movable ( WrapperType && wrapper ) noexcept
    {
        static_assert( ::std::is_same< RequaredType, WrapperType >::value
                || ::std::is_base_of< RequaredType, WrapperType >::value,
            "The types Value and Wrapper are not compatible" );
        return ::std::forward< RequaredType >( wrapper );
    }
};

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType * get_ptr ( _WrapperType & wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::writableProxy( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType * get_ptr ( const _WrapperType & wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::readableProxy( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType * get_ptr ( _WrapperType && wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::movable(
        ::std::forward< _WrapperType >( wrapper ) );
}




/*!
 * Метод обеспечивает доступ к любому представлению внутреннего значения,
 * последовательно раскрывая суперпозицию оберток.
 */
template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType & get ( _WrapperType & wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::writable( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType & get ( const _WrapperType & wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::readable( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType && get ( _WrapperType && wrapper ) noexcept
{
    using Value = typename ::std::decay< _RequaredType >::type;
    using Wrapper = typename ::std::decay< _WrapperType >::type;
    return AccessHelper< Value, Wrapper >::movable(
        ::std::forward< _WrapperType >( wrapper ) );
}


/*!
 * Метод эквиваленетен методу get< const _RequaredType >( const _WrapperType & wrapper ).
 * Применяется для сокращения записи вызова метода get.
 *
 * Метод не изменяет значение оберток во время доступа к их внутреннему значению.
 */
template < typename _RequaredType, typename _WrapperType >
constexpr const _RequaredType & cget ( const _WrapperType & wrapper )  noexcept
{
    return get< const _RequaredType, _WrapperType >( wrapper );
}

/*!
 *
 */
template < typename _RequaredType, typename _WrapperType >
_RequaredType && getMovableValue ( _WrapperType & ) = delete;

template < typename _RequaredType, typename _WrapperType >
_RequaredType && getMovableValue ( const _WrapperType & ) = delete;

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType && getMovableValue ( _WrapperType && wrapper ) noexcept
{
    return get< _RequaredType, _WrapperType >( ::std::forward< _WrapperType >( wrapper ) );
}

/*!
 *
 */
template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType & getWritableValue ( _WrapperType & wrapper )  noexcept
{
    return get< _RequaredType, _WrapperType >( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType & getWritableValue ( const _WrapperType & wrapper )  noexcept
{
    return get< _RequaredType, _WrapperType >( wrapper );
}

template < typename _RequaredType, typename _WrapperType >
constexpr _RequaredType & getWritableValue ( _WrapperType && wrapper )  noexcept
{
    return get< _RequaredType, _WrapperType >( static_cast< _WrapperType & >( wrapper ) );
}

/*!
 *
 */
template < typename _RequaredType, typename _WrapperType >
constexpr const _RequaredType & getReadableValue ( const _WrapperType & wrapper )
{
    return cget< _RequaredType, _WrapperType >( wrapper );
}

#endif

