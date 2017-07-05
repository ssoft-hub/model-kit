#pragma once
#include <Helper/AccessHelper.h>
#include <memory>

/*!
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename _RequaredType, typename _WrapperType >
    friend struct AccessHelper;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueTool = _ValueTool;
    using Value = typename ValueTool:: template Value< _ValueType >;
    using Holder = typename ValueTool:: template ValueHolder< _ValueType >;

    using WritableProxy = typename ValueTool:: template WritableProxy< _ValueType >;
    using ReadableProxy = typename ValueTool:: template ReadableProxy< _ValueType >;

    using WritableAccessValue = typename ValueTool:: template WritableAccessValue< _ValueType >;
    using ReadableAccessValue = typename ValueTool:: template ReadableAccessValue< _ValueType >;
    using MovableAccessValue = typename ValueTool:: template MovableAccessValue< _ValueType >;

    enum InitializeFlag { NotInitialized };

private:
    Holder m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать
    /// \sa isNull()
    constexpr Instance ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    Instance ( _Arguments && ... arguments )
    : m_holder( ValueTool:: template makeValueHolder< Value >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
    }

    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        m_holder = ValueTool:: template makeValueHolder< Value >(
            ::std::forward< _Type >( other ) );
        return *this;
    }

    //! Конструктор перемещения.
    Instance ( ThisType && other )
    : m_holder( ::std::forward< Holder >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< Holder >( other.m_holder );
        return *this;
    }

    //! Конструктор копирования (необходим для правильной компиляции).
    Instance ( ThisType & other )
    : m_holder( other.m_holder )
    {
    }

    ThisType & operator = ( ThisType & other )
    {
        m_holder = other.m_holder;
        return *this;
    }

    //! Конструктор копирования.
    Instance ( const ThisType & other )
    : m_holder( other.m_holder )
    {
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = other.m_holder;
        return *this;
    }

    //! Конструктор перемещения из подобного объекта.
    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > && other )
    : Instance( getMovableValue< _OtherType >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && other )
    {
        *this = getMovableValue< _OtherType >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
        return *this;
    }

    //! Конструктор копирования из подобного объекта (необходим для правильной компиляции).
    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > & other )
    : Instance( cget< _OtherType >( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & other )
    {
        *this = cget< _OtherType >( other );
        return *this;
    }

    //! Конструктор копирования из подобного объекта.
    template < typename _OtherType, typename _OtherTool >
    Instance ( const Instance< _OtherType, _OtherTool > & other )
    : Instance( cget< _OtherType >( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & other )
    {
        *this = cget< _OtherType >( other );
        return *this;
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyValueHolder( m_holder );
    }

    WritableProxy operator -> ()
    {
        return ValueTool:: template writableProxy< _ValueType >( m_holder );
    }

    ReadableProxy operator -> () const
    {
        return ValueTool:: template readableProxy< _ValueType >( m_holder );
    }

private:
    WritableAccessValue writable ()
    {
        return ValueTool:: template writable< _ValueType >( m_holder );
    }

    ReadableAccessValue readable () const
    {
        return ValueTool:: template readable< _ValueType >( m_holder );
    }

    MovableAccessValue movable ()
    {
        return ValueTool:: template movable< _ValueType >(
            ::std::forward< Holder >( m_holder ) );
    }
};

// TODO: Имеет смысл дополнить понятие Instance значения
// набором операторов сравнения/присвоения и т.д.

/*!
 * Специализация шаблоного класса для доступа к внутреннему значению Instance.
 */
template < typename _RequaredType, typename _ValueType, typename _ValueTool >
struct AccessHelper< _RequaredType, Instance< _ValueType, _ValueTool > >
{
    using RequaredType = _RequaredType;
    using WrapperType = Instance< _ValueType, _ValueTool >;

    using WritableAccessValue = typename WrapperType::WritableAccessValue;
    using ReadableAccessValue = typename WrapperType::ReadableAccessValue;
    using MovableAccessValue = typename WrapperType::MovableAccessValue;

    static constexpr RequaredType & writable ( WrapperType & wrapper )
    {
        return getWritableValue< _RequaredType >( wrapper.writable() );
    }

    static constexpr const RequaredType & readable ( const WrapperType & wrapper )
    {
        return getReadableValue< _RequaredType >( wrapper.readable() );
    }

    static constexpr RequaredType && movable ( WrapperType && wrapper )
    {
        return getMovableValue< _RequaredType >( ::std::forward< MovableAccessValue >( wrapper.movable() ) );
    }
};

/*!
 * Специализация шаблоного класса для доступа к самому Instance.
 */
template < typename _ValueType, typename _ValueTool >
struct AccessHelper< Instance< _ValueType, _ValueTool >, Instance< _ValueType, _ValueTool > >
{
    using RequaredType = Instance< _ValueType, _ValueTool >;
    using WrapperType = Instance< _ValueType, _ValueTool >;

    static constexpr RequaredType & writable ( WrapperType & wrapper )
    {
        return wrapper;
    }

    static constexpr const RequaredType & readable ( const WrapperType & wrapper )
    {
        return wrapper;
    }

    static constexpr RequaredType && movable ( WrapperType && wrapper )
    {
        return ::std::forward< WrapperType >( wrapper );
    }
};
