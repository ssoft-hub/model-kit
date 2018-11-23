#pragma once

#include <assert.h>
#include <utility>
#include <ModelKit/Instance/Traits.h>

/*!
 * Указатель на ссылку экземпляра значения, который ведет себя как "сырой" указатель
 * (raw pointer), независимо от того переопределен ли оператор & у типа значения или нет.
 * По умолчанию значение указателя нулевое. В случае перемещения указателя,
 * производится операция swap.
 */
template < typename _Refer >
class ReferPointer
{
    using ThisType = ReferPointer< _Refer >;

public:
    using Refer = _Refer;
    using Value = ::std::remove_reference_t< Refer >;
    using RawPointer = Value *;

    static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must be a reference!" );

private:
    Refer m_refer;

private:
    //! Формирует указатель для любого типа Value, независимо от того
    /// переопределён оператор & или нет.
    struct Dummy {};
    static constexpr RawPointer addressOf ( Value & refer )
    {
        using DummyType = ::std::conditional_t<
            ::std::is_const< Value >::value, const Dummy, Dummy >;
        return reinterpret_cast< RawPointer >( &reinterpret_cast< DummyType & >( refer ) );
    }

public:
    constexpr ReferPointer ()
        : m_refer()
    {
    }

    ReferPointer ( Refer refer )
        : m_refer( ::std::forward< Refer >( refer ) )
    {
    }

    ReferPointer ( ThisType && other )
        : m_refer( ::std::forward< Refer >( other.m_refer ) )
    {
    }

    constexpr bool operator ! () const
    {
        return !addressOf( m_refer );
    }

    constexpr Refer operator * () const
    {
        return ::std::forward< Refer >( m_refer );
    }

    constexpr RawPointer operator -> () const
    {
        return addressOf( m_refer );
    }
};
