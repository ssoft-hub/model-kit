#pragma once

#include <assert.h>
#include <utility>

/*!
 * Указатель на ссылку экземпляра значения, который ведет себя как "сырой" указатель
 * (raw pointer), независимо от того переопределен ли оператор & у типа значения или нет.
 * По умолчанию значение указателя нулевое. В случае перемещения указателя,
 * производится операция swap.
 */
template < typename _ReferType >
class ReferPointer
{
    using ThisType = ReferPointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using Value = ::std::remove_reference_t< ReferType >;
    using RawPointer = Value *;

private:
    RawPointer m_pointer;

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
        : m_pointer()
    {
    }

    ReferPointer ( ReferType refer )
        : m_pointer( addressOf( refer ) )
    {
    }

    ReferPointer ( ThisType && other )
        : m_pointer( other.m_pointer )
    {
        other.m_pointer = RawPointer();
    }

    ThisType & operator = ( ThisType && other )
    {
        m_pointer = other.m_pointer;
        other.m_pointer = RawPointer();
        return *this;
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr ReferType operator * () const
    {
        assert( m_pointer );
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr RawPointer operator -> () const
    {
        assert( m_pointer );
        return m_pointer;
    }
};
