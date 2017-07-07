#pragma once
#include <Helper/AccessHelper.h>
#include <memory>

/*!
 * Класс для формирования экземпляра значения, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством заданного
 * _ValueTool.
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename _WrapperType >
    friend struct FeatureGuard;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using HolderType = typename ValueTool:: template HolderType< _ValueType >;

    enum InitializeFlag { NotInitialized };

private:
    HolderType m_holder;

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
    : m_holder( ValueTool:: template makeHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
        // в этот конструктор нельзя передать Instance, поэтому
        // нет необходимости защищать входящие аргументы
    }

    template < typename _Type >
    ThisType & operator = ( _Type & other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        wGuard( *this ).holder()
            = ValueTool:: template makeHolder< ValueType >( other );
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        wGuard( *this ).holder()
            = ValueTool:: template makeHolder< ValueType >(
                ::std::forward< _Type >( other ) );
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        wGuard( *this ).holder()
            = ValueTool:: template makeHolder< ValueType >( other );
        return *this;
    }

    Instance ( ThisType & other )
    : m_holder( ValueTool::copyHolder( cGuard( other ).holder() ) )
    {
        // наделяем свойствами other
    }

    ThisType & operator = ( ThisType & other )
    {
        // наделяем свойствами *this и other
        wGuard( *this ).holder()
            = ValueTool::copyHolder( cGuard( other ).holder() );
        return *this;
    }

    Instance ( ThisType && other )
    : m_holder( ValueTool::moveHolder( ::std::forward< HolderType >( mGuard( other ).holder() ) ) )
    {
        // наделяем свойствами other
    }

    ThisType & operator = ( ThisType && other )
    {
        // наделяем свойствами *this и other
        wGuard( *this ).holder()
            = ValueTool::moveHolder( ::std::forward< HolderType >( mGuard( other ).holder() ) );
        return *this;
    }

    Instance ( const ThisType & other )
    : m_holder( ValueTool::copyHolder( cGuard( other ).holder() ) )
    {
        // наделяем свойствами other
    }

    ThisType & operator = ( const ThisType & other )
    {
        // наделяем свойствами *this и other
        wGuard( *this ).holder()
            = ValueTool::copyHolder( cGuard( other ).holder() );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > & other )
    : Instance( cGet( other ) )
    {
        // наделяем свойствами other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & other )
    {
        // наделяем свойствами *this и other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.

        return *this = cGet( other );
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > && other )
    : Instance( mGet( other ) )
    {
        // наделяем свойствами other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && other )
    {
        // наделяем свойствами *this и other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.

        return *this = mGet( other );
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( const Instance< _OtherType, _OtherTool > & other )
    : Instance( cGet( other ) )
    {
        // наделяем свойствами other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & other )
    {
        // наделяем свойствами *this и other

        // TODO: реализовать оптимальный вариант
        // * если внутри ThisInstance может находится значение OtherInstance,
        //   то необходимо выполнить операцию присвоения внутреннему
        //   значению *this и other;
        // * если внутри OtherInstance может находится значение ThisInstance,
        //   то необходимо выполнить операцию присвоения *this внутреннему
        //   значению other.

        return *this = cGet( other );
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyHolder( wGuard( *this ).holder() );
    }
};

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardWritableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardWritableHolder( m_refer.m_holder );
    }

    constexpr HolderType & holder ()
    {
        return m_refer.m_holder;
    }

    constexpr FeatureGuard< ValueType & > operator -> ()
    {
        return ValueTool::writableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для const Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< const Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = const Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardReadableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardReadableHolder( m_refer.m_holder );
    }

    constexpr const HolderType & holder ()
    {
        return m_refer.m_holder;
    }

    constexpr FeatureGuard< const ValueType & > operator -> ()
    {
        return ValueTool::readableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &&.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > && >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > && >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    constexpr HolderType && holder ()
    {
        return ::std::forward< HolderType >( m_refer.m_holder );
    }

    constexpr FeatureGuard< ValueType && > operator -> ()
    {
        return ValueTool::movableValueGuard(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }
};
