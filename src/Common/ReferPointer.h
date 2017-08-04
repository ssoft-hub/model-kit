#pragma once

#include <assert.h>
#include <utility>

/*!
 * Указатель на ссылку с проверкой на корректность. По-умолчанию значение
 * указателя нулевое. В случае перемещения указателя, производится операция swap.
 */
template < typename _ReferType >
class ReferPointer
{
    using ThisType = ReferPointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using ValueType = typename ::std::remove_reference< ReferType >::type;

private:
    ValueType * m_pointer;

private:
    //! Формирует указатель для любого типа ValueType, независимо от того
    /// переопределён оператор & или нет.
    static constexpr ValueType * addressOf ( ValueType & refer )
    {
        struct Dummy {};
        using DummyType = typename ::std::conditional<
            ::std::is_const< ValueType >::value, const Dummy, Dummy >::type;
        return reinterpret_cast< ValueType * >( &reinterpret_cast< DummyType & >( refer ) );
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
        other.m_pointer = nullptr;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_pointer = other.m_pointer;
        other.m_pointer = nullptr;
        return *this;
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr ReferType operator * () const
    {
        assert( !!m_pointer );
        return ::std::forward< ReferType >( *m_pointer );
    }

    constexpr ValueType * operator -> () const
    {
        assert( !!m_pointer );
        return m_pointer;
    }
};
